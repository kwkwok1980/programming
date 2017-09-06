#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <semaphore.h>
#include <memory.h>
#include <unistd.h>
#include <stdio.h>
#include <thread>
#include <chrono>
#include <mutex>

static const std::string MEM_NAME = "mem.demo";
static const std::string SEM_NAME = "sem.demo";
static const size_t PAGE_SIZE = 4096;
static const size_t BUFFER_SIZE = 10;

struct Message
{
    int seq;
    char text[20];
};

struct Queue
{
    int in = 0;
    int out = 0;
    Message messages[BUFFER_SIZE];

    bool Produce(const Message& message)
    {
        if (IsFull())
        {
            return false;
        }
        messages[in] = message;
        in = (in + 1)%BUFFER_SIZE;
        return true;
    }

    bool Consume(Message& message)
    {
        if(IsEmpty())
        {
            return false;
        }
        message = messages[out];
        out = (out + 1)%BUFFER_SIZE;
    }

    bool IsFull()
    {
        return (in+1)%BUFFER_SIZE == out;
    }

    bool IsEmpty()
    {
        return in == out;
    }
};

static Queue queue;
static std::mutex mutex;

struct Producer
{
    void operator()()
    {
        auto startTime = std::chrono::system_clock::now();
        auto endTime = startTime + std::chrono::seconds(10);
        int count = 0;
        while(true)
        {
            if (std::chrono::system_clock::now() > endTime)
            {
                return;
            }

            Message message;
            message.seq = count;
            snprintf(message.text, 20, "halo %d", count);
            std::unique_lock<std::mutex>(mutex);
            bool ret = queue.Produce(message);
            if (ret)
            {
                ++count;
                std::cout << "Producer: " << message.text << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    };
};

struct Consumer
{
    void operator()()
    {
        auto startTime = std::chrono::system_clock::now();
        auto endTime = startTime + std::chrono::seconds(12);
        int count = 0;
        while(true)
        {
            if (std::chrono::system_clock::now() > endTime)
            {
                return;
            }

            Message message;
            std::unique_lock<std::mutex>(mutex);
            bool ret = queue.Consume(message);
            if (ret)
            {
                std::cout << "Consumer: " << message.text << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    };
};


int main()
{
    Producer p;
    Consumer c;

    std::thread th1 {p};
    std::thread th2 {c};

    th1.join();
    th2.join();

}

