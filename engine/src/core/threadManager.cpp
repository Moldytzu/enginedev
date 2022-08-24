#include <engine/core.h>
#include <atomic>

unsigned int hardwareThreads = std::thread::hardware_concurrency();
bool shouldStop = false;
unsigned int busy = 0;

Engine::Core::ThreadManager::ThreadManager()
{
    if (hardwareThreads == 0)
    {
        Engine::Core::Logger::LogWarn("Failed to detect hardware threads count. Disabling multiprocessing.");
        hardwareThreads = 0;
        return;
    }

    Engine::Core::Logger::LogDebug("Using " + std::to_string(hardwareThreads) + " hardware threads");

    jobsMutex.unlock();                       // unlock the mutex
    threads.resize(hardwareThreads);          // resize the vector to hold a maximum number of elements equal to the hardware threads count
    for (int i = 0; i < hardwareThreads; i++) // spawn the threads
        threads.at(i) = std::thread(&Engine::Core::ThreadManager::threadLoop, this);
}

Engine::Core::ThreadManager::~ThreadManager()
{
    Engine::Core::Logger::LogDebug("Waiting for threads to stop");

    Wait();

    jobsMutex.lock();
    shouldStop = true;
    jobsMutex.unlock();

    for (int i = 0; i < threads.size(); i++) // wait for the threads to stop
        threads.at(i).join();
}

void Engine::Core::ThreadManager::Queue(const std::function<void()> &job)
{
    if (hardwareThreads == 0) // no multiprocessing
    {
        job();
        return;
    }

    jobsMutex.lock();   // lock the mutex so there isn't any data race
    jobs.push(job);     // push the job
    jobsMutex.unlock(); // make the jobs vector accessible
}

void Engine::Core::ThreadManager::Wait()
{
    bool availableJobs = true;
    while (availableJobs)
    {
        jobsMutex.lock();                      // lock the mutex
        availableJobs = !jobs.empty() || busy; // wait until the queue is empty or the threads aren't busy
        jobsMutex.unlock();                    // unlock the mutex
        std::this_thread::yield();             // tell the operating system that we want to give back control
    }
    busy = 0;
}

void Engine::Core::ThreadManager::threadLoop()
{
    while (true)
    {
        std::function<void()> job;
        jobsMutex.lock();

        if (shouldStop == true)
        {
            jobsMutex.unlock();
            return;
        }

        if (jobs.empty()) // wait for jobs
        {
            jobsMutex.unlock();
            std::this_thread::yield();
            continue;
        }

        job = jobs.front(); // get the job
        jobs.pop();
        busy++; // increase the count of busy threads
        jobsMutex.unlock();

        job(); // run it

        jobsMutex.lock();
        busy--; // decrease the count of busy threads
        jobsMutex.unlock();
    }
}