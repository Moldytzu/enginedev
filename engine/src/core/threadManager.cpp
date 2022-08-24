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

    // we want to have the total threads count equal to the hardware threads so we remove 2 threads (1 for the main thread and 1 for the render thread that are already running)
    if (hardwareThreads > 2)
        hardwareThreads -= 2;

    jobsMutex.unlock();                       // unlock the mutex
    threads.resize(hardwareThreads);          // resize the vector to hold a maximum number of elements equal to the hardware threads count
    for (int i = 0; i < hardwareThreads; i++) // spawn the threads
        threads.at(i) = std::thread(&Engine::Core::ThreadManager::threadLoop, this);

    jobsCondition.notify_all(); // wake up the threads
}

Engine::Core::ThreadManager::~ThreadManager()
{
    Engine::Core::Logger::LogDebug("Waiting for threads to stop");

    Wait();

    jobsMutex.lock();
    shouldStop = true;
    jobsMutex.unlock();

    jobsCondition.notify_all(); // wake the threads

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
    jobsCondition.notify_one();
}

void Engine::Core::ThreadManager::Wait()
{
    std::unique_lock<std::mutex> lock(jobsMutex);
    doneCondition.wait(lock, [this]()
                       { return jobs.empty() && !busy; });
}

void Engine::Core::ThreadManager::threadLoop()
{
    while (true)
    {
        std::function<void()> job;

        std::unique_lock<std::mutex> mutex(jobsMutex);
        jobsCondition.wait(mutex, [this]()
                           { return shouldStop || !jobs.empty(); });

        if (shouldStop == true)
        {
            UNLOCK;
            return;
        }

        job = jobs.front(); // get the job
        jobs.pop();
        busy++; // increase the count of busy threads
        UNLOCK;

        job(); // run it

        LOCK;
        busy--; // decrease the count of busy threads
        doneCondition.notify_one();
        UNLOCK;

        std::this_thread::yield();
    }
}