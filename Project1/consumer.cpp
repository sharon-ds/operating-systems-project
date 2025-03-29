#include <iostream>
#include <thread>
#include <semaphore.h>
#include <fcntl.h>    // For O_CREAT
#include <unistd.h>   // For sleep
#include <sys/mman.h> // For shared memory

#define SHM_NAME "/shared_table"

sem_t *empty, *full; // POSIX semaphores
int *table;          // Shared memory

void consumer()
{
    while (true)
    {
        sem_wait(full); // Wait if no full slots

        int item = *table; // Consume the item
        std::cout << "Consumer consumed item " << item << std::endl;

        sem_post(empty); // Signal empty slot
        sleep(2);        // Simulate consumption delay
    }
}

int main()
{
    // Attach to shared memory
    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    table = (int *)mmap(0, sizeof(int), PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);

    // Open existing semaphores
    empty = sem_open("/empty_sem", 0);
    full = sem_open("/full_sem", 0);

    // Run consumer function in a thread
    std::thread cons_thread(consumer);
    cons_thread.join();

    // Cleanup
    sem_close(empty);
    sem_close(full);
    return 0;
}
