#include <iostream>
#include <thread>
#include <semaphore.h>
#include <fcntl.h>    // For O_CREAT
#include <unistd.h>   // For sleep
#include <sys/mman.h> // For shared memory

#define TABLE_SIZE 2
#define SHM_NAME "/shared_table"

sem_t *empty, *full; // POSIX semaphores
int *table;          // Shared memory

void producer()
{
    int item = 0;
    while (true)
    {
        item = rand() % 100; // Produce an item
        sem_wait(empty);     // Wait if no empty slots

        *table = item; // Place item on table
        std::cout << "Producer produced item " << item << std::endl;

        sem_post(full); // Signal full slot
        sleep(1);       // Simulate production delay
    }
}

int main()
{
    // Create shared memory object and truncate it to hold an int
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(int));
    table = (int *)mmap(0, sizeof(int), PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);

    // Create semaphores
    empty = sem_open("/empty_sem", O_CREAT, 0666, TABLE_SIZE);
    full = sem_open("/full_sem", O_CREAT, 0666, 0);

    // Run producer function in a thread
    std::thread prod_thread(producer);
    prod_thread.join();

    // Cleanup
    sem_close(empty);
    sem_close(full);
    sem_unlink("/empty_sem");
    sem_unlink("/full_sem");
    shm_unlink(SHM_NAME);

    return 0;
}
