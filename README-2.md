# ⚙️ Priority-Based Job Scheduler (C / UNIX)

A concurrent job scheduling system written in C that simulates an operating system–style scheduler using **process creation**, **shared memory**, **semaphores**, and **inter-process communication (IPC)**. The program schedules multiple jobs based on priority, executes them in parallel using child processes, and synchronizes execution safely across processes.

This project demonstrates advanced UNIX system programming concepts and real-world process coordination techniques.

---

## 📌 Project Overview

The **Priority-Based Job Scheduler** models how an operating system manages concurrent tasks. Jobs are dynamically created, assigned priorities, and executed as separate child processes. The scheduler ensures safe access to shared resources using semaphores and coordinates job completion using pipes.

This project is ideal for demonstrating:

* Process synchronization
* Inter-process communication (IPC)
* Shared memory management
* Priority-based scheduling logic
* Concurrency control using semaphores

---

## ✨ Key Features

### 🧵 Concurrent Job Execution

* Uses `fork()` to create child processes for each job
* Executes multiple jobs concurrently
* Parent process waits for all jobs to complete

### 📊 Priority-Based Scheduling

* Jobs are assigned random priorities
* Scheduler sorts jobs using `qsort()` based on priority
* Lower priority values execute first

### 🧠 Shared Memory Scheduler

* Scheduler state stored in shared memory (`shmget`, `shmat`)
* Allows safe access across processes
* Clean memory cleanup after execution

### 🔒 Synchronization with Semaphores

* **Mutex semaphore** prevents race conditions on job queue
* **Slot semaphore** limits maximum concurrent jobs
* POSIX named semaphores ensure safe inter-process coordination

### 🔄 Inter-Process Communication (IPC)

* Pipes transmit job completion messages
* Parent process receives execution status from child processes

### 🧹 Resource Management

* Proper cleanup of shared memory
* Semaphore closing and unlinking
* Graceful termination of child processes

---

## 🛠 Technologies & APIs Used

* **C (GCC compatible)**
* **POSIX Process Control**

  * `fork()`, `wait()`, `sleep()`
* **Inter-Process Communication**

  * Pipes (`pipe`, `read`, `write`)
  * Shared Memory (`shmget`, `shmat`, `shmctl`)
* **POSIX Semaphores**

  * `sem_open`, `sem_wait`, `sem_post`, `sem_close`
* **UNIX System Headers**

  * `<unistd.h>`, `<sys/ipc.h>`, `<sys/shm.h>`, `<sys/wait.h>`

---

## ⚙️ Program Configuration

```c
#define Max_Jobs 8
#define Max_Priority 10
#define Job_Duration 2
```

* **Max_Jobs**: Maximum number of concurrent jobs
* **Max_Priority**: Priority range (1 = highest)
* **Job_Duration**: Execution time per job (seconds)

---

## ▶️ How to Compile and Run

### 1️⃣ Compile

```bash
gcc job_scheduler.c -o job_scheduler -pthread
```

### 2️⃣ Run

```bash
./job_scheduler
```

> ⚠️ Note: This program is intended for **Linux / UNIX-based systems** due to its reliance on POSIX APIs.

---

## 📈 Sample Output

```
Job 3 is now executing for 2 seconds
Manager received: Job completed for Job 3!
Job 1 is now executing for 2 seconds
Manager received: Job completed for Job 1!
...
```

---

## 🧠 Concepts Demonstrated

* Operating system–level scheduling
* Mutual exclusion and concurrency control
* Producer–consumer style synchronization
* Priority queues and sorting
* Safe IPC design
* Resource lifecycle management

---

## 🚀 Possible Enhancements

* Preemptive scheduling
* Job aging to prevent starvation
* Configurable job duration per task
* Logging to file
* Thread-based scheduler alternative
* Real-time priority adjustment

---

## 📄 License

This project is intended for educational and academic use. Free to extend and modify.

---

**Built to explore how operating systems schedule work — one process at a time.**
