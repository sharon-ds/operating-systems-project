#include <iostream>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define BUFFER_SIZE 2
#define SHM_NAME "/my_shm"
#define SEM_EMPTY_NAME "/empty"
#define SEM_FULL_NAME "/full"
#define SEM_MUTEX_NAME "/mutex"

struct shared_memory
{
    int buffer[BUFFER_SIZE]; // Shared buffer of size 2
    int in;                  // Index where the producer will insert next item
    int out;                 // Index where the consumer will take the item from
};

int main()
{
    // Open shared memory
    int shm_fd = shm_open(SHM_NAME, O_RDWR, S_IRUSR | S_IWUSR);
    struct shared_memory *shm = (struct shared_memory *)mmap(NULL, sizeof(struct shared_memory), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    // Open semaphores
    sem_t *empty = sem_open(SEM_EMPTY_NAME, 0); // Already created by producer
    sem_t *full = sem_open(SEM_FULL_NAME, 0);   // Already created by producer
    sem_t *mutex = sem_open(SEM_MUTEX_NAME, 0); // Already created by producer

    while (true)
    {
        sem_wait(full);  // Wait for an item to be available
        sem_wait(mutex); // Enter critical section

        // Consume the item
        int item = shm->buffer[shm->out];
        std::cout << "Consumer consumed item " << item << std::endl;
        shm->out = (shm->out + 1) % BUFFER_SIZE;

        sem_post(mutex); // Leave critical section
        sem_post(empty); // Signal that space is available for the producer

        sleep(2); // Sleep for a while before consuming the next item
    }

    // Cleanup (not reached in this infinite loop)
    munmap(shm, sizeof(struct shared_memory));
    close(shm_fd);
    sem_close(empty);
    sem_close(full);
    sem_close(mutex);
    return 0;
}
