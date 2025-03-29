#include <iostream>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstdlib>

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
    // Open or create shared memory
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(shm_fd, sizeof(struct shared_memory));
    struct shared_memory *shm = (struct shared_memory *)mmap(NULL, sizeof(struct shared_memory), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    // Initialize shared memory
    shm->in = 0;
    shm->out = 0;

    // Open or create semaphores
    sem_t *empty = sem_open(SEM_EMPTY_NAME, O_CREAT, 0644, BUFFER_SIZE); // Table size
    sem_t *full = sem_open(SEM_FULL_NAME, O_CREAT, 0644, 0);             // Initially no items
    sem_t *mutex = sem_open(SEM_MUTEX_NAME, O_CREAT, 0644, 1);           // Mutex for mutual exclusion

    while (true)
    {
        int item = rand() % 100; // Random item to produce

        sem_wait(empty); // Wait for space to be available in buffer
        sem_wait(mutex); // Enter critical section

        // Insert item into buffer
        shm->buffer[shm->in] = item;
        std::cout << "Producer produced item " << item << std::endl;
        shm->in = (shm->in + 1) % BUFFER_SIZE;

        sem_post(mutex); // Leave critical section
        sem_post(full);  // Signal that a new item is available

        sleep(1); // Sleep for a while before producing the next item
    }

    // Cleanup (not reached in this infinite loop)
    munmap(shm, sizeof(struct shared_memory));
    close(shm_fd);
    sem_close(empty);
    sem_close(full);
    sem_close(mutex);
    return 0;
}
