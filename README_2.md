# Banker's Algorithm Project

This project implements the Banker's Algorithm for Deadlock Avoidance using C++ in a Linux environment. 
The program reads the system resource state from an input file and determines whether the system is in a safe state.
If the system is safe, the program prints the safe sequence of process execution.

### Steps to Compile and Run: 
1. Copy and create the files 'bankers.cpp' and 'input.txt'
2. Compile the Producer program:
   ```bash
   g++ bankers.cpp -o bankers
This compiles the code and creates an executable file named 'bankers'.

3. After compiling, run the program with:
```bash
    ./bankers
Make sure 'input.txt' is in the same directory as 'bankers.cpp' and the compiled 'bankers' executable.

### Input file format
The input file 'input.txt' is structured as follows:

1. First line: number of processes and number of resource types (e.g., 5 3)
2. Second line: total instances of each resource type (e.g., 10 5 7)
3. Third line: currently available instances of each resource (e.g., 3 3 2)
4. Allocation Matrix: (one line per process)
5. Maximum Need Matrix: (one line per process)

Example input.txt:
5 3 10 5 7 3 3 2
0 1 0 2 0 0 3 0 2 2 1 1 0 0 2
7 5 3 3 2 2 9 0 2 2 2 2 4 3 3

### This is the example output:
<img width="437" alt="image" src="https://github.com/user-attachments/assets/51aaad3d-469f-494b-a61f-e89de1362c90" />
The system is in a safe state. Safe sequence is: P0 -> P1 -> P2 -> P3 -> P4

Code Explanation
-----------------
- The program first reads input from `input.txt`, which contains the number of processes, resources, available resources, allocation matrix, and maximum demand matrix.
- It calculates the **Need matrix** by subtracting the Allocation matrix from the Maximum matrix for each process.
- It uses a **while loop** to simulate the Banker's Algorithm:
  - It looks for any unfinished process whose `Need <= Available`.
  - If found, it simulates that the process finishes by releasing its allocated resources back to the available pool.
  - The process is marked as finished and added to the **safe sequence**.
- If at any point no process can be safely executed, the program declares that the system is **not in a safe state**.
- If all processes finish successfully, it prints the safe sequence.

Important points:
- It uses `vector` containers for dynamic memory management.
- It checks processes in order from P0 to Pn to maintain a consistent checking order.

---

Explanation of Safe Sequence
-----------------------------
- P0 can finish first because its resource needs can be satisfied.
- Once P0 finishes, it releases its resources back to the available pool.
- With additional resources, P1 can finish, then P2, then P3, and finally P4.
- At each step, the system remains in a safe state without any deadlock.

Note:  
- **There can be multiple valid safe sequences** depending on the order in which processes are checked.

---

Files Included
--------------
- bankers.cpp   (C++ source code)
- input.txt     (Input file)
