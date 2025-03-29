# Producer-Consumer Problem 
This project demonstrates the **Producer-Consumer problem** using **shared memory** and **semaphores** for synchronization in C++. The producer generates items and places them in a shared buffer (table), and the consumer retrieves items from the buffer. The buffer can hold only two items at a time, and the producer and consumer synchronize their actions using semaphores.

> **Note:** This code was developed and tested in **Kali Linux on a Virtual Machine (VM)**. The program works correctly in this environment. There may be compatibility issues if you attempt to run it in other environments such as VSCode or other operating systems without modifications.

## Features 
- Producer generates items and places them in a shared buffer.
- Consumer consumes items from the shared buffer.
- Shared memory is used for the buffer.
- Semaphores are used for synchronization and mutual exclusion:
  - `empty`: Tracks empty slots in the buffer.
  - `full`: Tracks full slots in the buffer.
  - `mutex`: Ensures mutual exclusion when accessing the shared buffer.
 
## Setup

### Requirements:
- **GCC Compiler**
- **POSIX Shared Memory**
- **POSIX Semaphores** (`-lrt` for real-time library)

### Steps to Compile and Run: 
1. Copy and create the files 'producer.cpp' and 'consumer.cpp'
2. Compile the Producer program:
   ```bash
   g++ producer.cpp -pthread -lrt -o producer
3. Compile the Consumer program:
   ```bash
   gcc consumer.cpp -pthread -lrt -o consumer
4. Run the Producer and Consumer
   ```bash
   ./producer & ./consumer &

### This is the example output 
Producer produced item 86 <br />
Consumer consumed item 86 <br />
Producer produced item 77 <br />
Producer produced item 15 <br />
Consumer consumed item 77 <br />

## Explanation of Code

Producer Code (producer.cpp)
1. The producer waits for an empty slot (empty semaphore), enters the critical section (mutex semaphore), and inserts an item into the buffer.
2. After producing an item, the producer signals the consumer that an item is available (full semaphore).
3. The producer sleeps for 1 second before producing the next item.

Consumer Code (consumer.cpp)
1. The consumer waits for an item to be available (full semaphore), enters the critical section (mutex semaphore), and consumes the item from the buffer.
2. After consuming an item, the consumer signals the producer that space is available (empty semaphore).
3. The consumer sleeps for 2 seconds before consuming the next item.

Semaphore Explanation:
empty: Semaphore that tracks the number of empty slots in the buffer. Initially, it's set to 2 (the buffer can hold two items). <br />
full: Semaphore that tracks the number of items in the buffer. Initially, it's set to 0 (no items are in the buffer). <br />
mutex: A binary semaphore used to ensure mutual exclusion when accessing the shared buffer. <br />
