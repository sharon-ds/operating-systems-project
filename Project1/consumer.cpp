#include <iostream>
#include <thread>
#include <semaphore.h> // POSIX semaphores
#include <unistd.h>    // For sleep function
#include <vector>
#include <mutex>

#define TABLE_SIZE 2

std::vector<int> table; // Shared memory (table)
sem_t empty;            // Semaphore to track empty slots
sem_t full;             // Semaphore to track full slots
std::mutex table_mutex; // Mutex for critical section

void consumer()
{
    while (true)
    {
        sem_wait(&full);    // Wait if no full slots
        table_mutex.lock(); // Lock shared memory access

        // Critical section: Removing item from table
        int item = table.back();
        table.pop_back();
        std::cout << "Consumer consumed item " << item << std::endl;

        table_mutex.unlock(); // Unlock shared memory
        sem_post(&empty);     // Signal an empty slot
        sleep(1);             // Simulate consumption time
    }
}

int main()
{
    sem_init(&empty, 0, TABLE_SIZE); // Initialize empty slots semaphore
    sem_init(&full, 0, 0);           // Initialize full slots semaphore

    std::thread cons_thread(consumer); // Create consumer thread
    cons_thread.join();                // Wait for the consumer thread to finish (this loop will keep running)

    sem_destroy(&empty); // Destroy semaphores
    sem_destroy(&full);
    return 0;
}
