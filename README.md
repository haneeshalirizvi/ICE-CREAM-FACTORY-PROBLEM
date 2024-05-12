------------------------------ICE CREAM FACTORY PROBLEM---------------------------------------
COURSE: Operating System
LANGUAGE: C

Introduction:
This project aims to provide a solution to the classic concurrency problem known as the Ice Cream Factory Problem, which demonstrates synchronization challenges in operating systems.
The Ice Cream Factory Problem simulates a scenario where multiple production lines in an ice cream factory are producing different flavors of ice cream. 
Each production line has limited capacity, and workers must coordinate to ensure efficient production while avoiding overproduction or underproduction.
 
Solutions of the synchronization problem:
To address these challenges, various synchronization mechanisms can be employed:
1. Mutex (Mutual Exclusion): Mutexes are used to ensure that only one thread can access
a shared resource at a time. In the ice cream factory problem, a mutex can be used to
control access to the buffer, allowing only one producer or consumer to modify it at any
given time.
2. Semaphore: Semaphores are used to control access to a shared resource by multiple
processes in a concurrent system. They can be used to limit the number of producers or
consumers accessing the buffer simultaneously, preventing overflow or underflow.
3. Monitors: Condition variables are used to coordinate threads and synchronize their
execution. In the ice cream factory problem, condition variables can be used to signal
when the buffer is empty or full, allowing producers and consumers to wait or resume
execution as necessary.
