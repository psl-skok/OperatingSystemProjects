<h1>Operating Systems Homework Projects</h1>

This repository contains all homework assignments completed for my Operating Systems class. Each project focuses on a fundamental concept in operating systems, ranging from networking to memory management. Below is an overview of the homework topics and what each project entails.

<h2>1. Webserver Networking</h2>

<b>Description:</b> Development of a multi-client web server using a custom networking library, libWildcatNetworking.so. The server processes HTTP requests, retrieves requested files, and sends appropriate responses back to the client. This project progresses through multiple stages, improving scalability and performance.

<b>Key Concepts:</b>
  -  <b>I/O Operations:</b> Implementing read/write procedures to handle client requests and serve file contents.
  -  <b>Process-Based Concurrency:</b> Using fork() to handle multiple clients simultaneously.
  -  <b>Event-Driven Concurrency:</b> Implementing select() for more efficient multi-client support within a single process.
  -  <b>Thread-Based Concurrency:</b> Creating a thread pool to address performance bottlenecks, ensuring blocking I/O does not stall the entire server.

<b>Highlights:</b>
  -  Utilizes OS-level syscalls for networking and process/thread management.
  -  Demonstrates trade-offs between different concurrency models for web server design.

<h2>2. User-level Multithreading</h2>
<b>Description:</b> Implementation of a user-level threading package in C, utilizing setjmp() and longjmp() for context switching. The project includes thread creation and joining, synchronization primitives, and preemptive scheduling to simulate multitasking within a single process.

<b>Key Concepts:</b>
 -  <b>Thread Scheduler:</b> Manages thread execution and switching using context-saving mechanisms (setjmp()/longjmp()).
 -  <b>Condition Variables:</b> Implements thread synchronization using condition variables and locks to manage producer-consumer problems efficiently.
 -  <b>Preemptive Multitasking:</b> Adds preemption by setting up periodic interrupts (SIGALRM) to enable automatic thread scheduling.

<b>Highlights:</b>
  -  Demonstrates manual implementation of thread management without OS kernel support.
  -  Explores synchronization challenges and preemptive multitasking in user-space environments.

<h2>3. Memory Management</h2>

<b>Description:</b> Development of a simulator for core memory management routines in an operating system, including frame allocation, multi-level page tables, and kernel-level memory allocation (malloc(), free(), and realloc()). The project emulates x86-64 architecture principles while managing a fixed 4MB memory space.

<b>Key Concepts:</b>
  -  <b>Frame Allocation:</b> Implements a frame allocator to manage 4KB memory chunks within the 4MB space.
  -  <b>Translation Tables:</b> Emulates x86-64 multi-level page tables for virtual-to-physical memory translation.
  -  <b>Kernel-Level Memory Allocation:</b> Simulates the hardware’s automatic address translation by implementing custom versions of malloc(), free(), and realloc().

<b>Highlights:</b>
  -  Simulates foundational concepts in OS memory management.
  -  Demonstrates multi-level page table management and virtual-to-physical address translation.
  -  Replaces standard memory allocation mechanisms with custom, kernel-level implementations.

<h2>4. Inode-based Filesystem</h2>

<b>Description:</b> Development of a simplified inode-based filesystem that operates on a 64MB emulated disk. The project includes functionality to format the filesystem, create files, read/write data, and manage disk blocks using indirect block structures and bitmaps.

<b>Key Concepts:</b>
  -  <b>Storage Module:</b> Provides functions (storage_read_block() and storage_write_block()) to manage 4KB disk blocks on a simulated disk.
  -  <b>Filesystem Formatting:</b> Initializes the disk with inode blocks, bitmap blocks, and data blocks for file storage.
  -  <b>File Operations:</b> Implements core file operations, including creation, reading, and writing, using inode structures and indirect blocks for efficient data management.

<b>Highlights:</b>
  -  Demonstrates foundational concepts in filesystem design, including inode structures, block allocation, and indirect addressing.
  -  Provides a fully functional simulation of file creation, reading, and writing on a simplified disk.
  -  Emulates x86-64 concepts of indirect blocks and inodes for efficient file management.
