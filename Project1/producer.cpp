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

void producer()
{
    while (true)
    {
        int item = rand() % 100; // Generate random item

        sem_wait(&empty);   // Wait if no empty slots
        table_mutex.lock(); // Lock shared memory access

        // Critical section: Adding item to table
        table.push_back(item);
        std::cout << "Producer produced item " << item << std::endl;

        table_mutex.unlock(); // Unlock shared memory
        sem_post(&full);      // Signal a full slot
        sleep(1);             // Simulate production time
    }
}

int main()
{
    sem_init(&empty, 0, TABLE_SIZE); // Initialize empty slots semaphore
    sem_init(&full, 0, 0);           // Initialize full slots semaphore

    std::thread prod_thread(producer); // Create producer thread
    prod_thread.join();                // Wait for the producer thread to finish (won’t actually happen in this loop)

    sem_destroy(&empty); // Destroy semaphores
    sem_destroy(&full);
    return 0;
}
