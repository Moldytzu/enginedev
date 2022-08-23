#include <engine/core.h>
#include <atomic>

unsigned int hardwareThreads = std::thread::hardware_concurrency();
bool shouldStop = false;
unsigned int busy = 0;
std::atomic<int> counter = 0;

Engine::Core::ThreadManager::ThreadManager()
{
    if (hardwareThreads == 0)
    {
        Engine::Core::Logger::LogWarn("Failed to detect hardware threads count. Disabling multiprocessing."); // todo: use some syscalls to detect the threads
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

    for (int i = 0; i < threads.size(); i++) // detach the threads
        threads.at(i).detach();
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
        jobsMutex.lock();                          // lock the mutex
        availableJobs = jobs.empty() && busy == 0; // check if there are still jobs
        jobsMutex.unlock();                        // unlock the mutex
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
            return;

        if (jobs.empty()) // wait for jobs
        {
            jobsMutex.unlock();
            std::this_thread::sleep_for(std::chrono::microseconds(10));
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