# Project 1 Design Document
### (11610615 Guo Mengwei)



## **Task 1: Efficient Alarm Clock**



### Data structures and functions

**Data structures:** I only create several new variables and one function and won't  use any specific data structure here.

**Functions:** I will use the following functions:

- **intr_disable () & intr_set_level:**  Make sure the operation in timer_sleep won't be interrupted.
- **thread_block () & thread_unblock():**  As their name describes, the functions will block and unblock the threads.
- **thread_foreach():** This function will take another function as parameter and execute it on every thread.
- **timer_interrupt():**  This function will be executed every tick, which can be used to do something regularly.
- **thread_sleep_check():** The function I design. It will take a thread object as parameter and judge if it has slept enough ticks base on an mark I defined in the thread structure. If the answer is yes, it will unblock the thread.

### Algorithms

First of all, I will add a new member variable **sleepTick** to the thread. which will mark the rest ticks the thread need to sleep. After that, I will delete the while loop in the timer_sleep function and block the thread while put the parameter ticks in the sleepTick. Then I will change the  timer_interrupt and add a **thread_foreach** instruction with a parameter of function **thread_sleep_check**. Finally, I will implement this function in the thread.c and check if a thread is about to wake up(**unblock**) or keep sleeping(**sleepTick -1**). 


### Synchronization

I think my solution is safe for synchronization. The function I write in the thread.c will take the thread as parameter,w hich guarantees the accuracy even there are two or more threads are executing it.  In additional to that, this function can only change the value of single argument of thread and unblock it. It means that only blocked thread can be changed. 

### Rationale

The previous method costs more CPU resource compare to my solution because of the frequent operation on the schedule() method and switch of threads' status. I only need to add a member variable in the thread structure and check each thread's status without switching between them. That's why I think my idea is better. 



## **Task 2: Priority Scheduler**



### Data structures and functions



**Data structure:** for the priority scheduler, the most important data structure is list, which rank the thread in the list with the order of their priority.   In order to implement a priority donation, I will add the record of the bind threads of every lock by using **thread_list**. At the same time, I will use **lock_list** to store the lock message of every thread and set this list in the structure of thread.  in the lock structure, a member variable named **max_thread** will be used to record the locked thread with highest priority.

**Function:** I will use the following functions:

- **thread_get_priority():** get the priority of current thread.
- **thread_set_priority(int new_priority):** set the priority of current thread. It will be added to a thread_yield to make sure the next thread is right.
- **thread_yield():** this function will throw the current thread back to priority list and the thread with highest priority.
- **thread_block () & thread_unblock():**  As their name describes, the functions will block and unblock the threads.
- **list_insert_ordered():** this function will be modified to maintain the priority list everytime new thread is inserted.
- **thread_cmp_priority():** the comparator of the function above.
- **lock_acquire():** set the lock to the right thread
- **thread_priority_donate(thread t):** this function will take the thread as parameter and donation it's priority to the corresponding threads.
- **thread_lock_release():**  this function will handle the recovery of the priority of donated threads.





### Algorithms

**For the priority list:** I will make change the current methods the source codes offered (like **list_insert_ordered**) to implement the priority list with a modified comparator (**thread_cmp_priority**).  The changes will be applied to the places where the new threads are inserted. Another detail is that, when a new thread with a higher priority compares to current one has been inserted. This thread must be executed immediately. To implement this, we must use thread_yield to throw the current thread back to priority list and pick the new thread to run.

**For the priority donation:** The main purpose of priority donation is to prevent the inversion of priority. There are several problems needs to be considered at this part.

1. **single lock:** If the current thread owns a higher priority can't be executed because of there is a lock in a lower priority thread, we need to directly donate the priority of higher to the lower one.
2. **nested lock:** It is possible that some threads(> 2) are connected by multiple clocks. At this situation, when the thread with a lock is donated, another thread which block the first thread should also be donated. It means that we need to record the information of the threads that can block the current thread. This procedure must be executed recursively, as well as the recovery after donation.
3. **multiple donation:** If one thread is donated by many threads together. The logic behind is that, there are multiple threads with higher priority are blocked by the locks in the thread with lower priority. To let the thread with highest priority run as fast as much, we need to receive the biggest donation. (sounds naturally) 
4. **lock release:** When the donation is end, it's necessary to set the donated thread to their previous priority value. We also need to assure that the next running thread is the thread with the highest priority. 



I feel deeply sorry for haven't figured how to satisfied all the requirements from now on but I will try to explain my current thoughts here. 

**list_insert_ordered():** I have described this in the priority list part.

**lock_acquire():**  I will add several member variables to the lock's structure, includes the list of threads it cares. It will help to refresh the donation message to different threads related to this lock. I will mark the thread require the lock with the highest priority. I'm still figuring how to implement the details in this function.

**thread_set_priority():** this function will take an parameter as the new priority. I will modify this function by add  a judgment. I will compare the new priority with the current priority and observe the **lock_list** of this thread. If the lock_list is empty or the priority is even smaller than the current , I won' t change the priority value. In additional to this, I will add the modified thread to the thread list again and pick the new thread with highest priority.

**thread_priority_donate(thread t):** Haven't found a good way to implement.

**thread_lock_release():** this function will handle the change of priority when a lock is released. It will scan the lock_list of a thread and find the new lock in it with the largest donation priority. We will replace the max priority mark with the new value. If the lock_list is empty, the priority value won't be changed.





### Synchronization

I literally don't know how to finish this part.

### Rationale

Although I have finished the design of this task, I hold the belief that it won't be that hard to finish the coding. All I should do is adding some features to the current functions and structures.





## **Task 3: Multi-level Feedback Queue Scheduler (MLFQS)**



### Data structures and functions

**Data structure:**  Don't know yet

**Function:**

- **thread_get_recent_cpu():** Returns 100 times the current thread’s recent_cpu value, rounded to the nearest integer. (qoute from the lab1 instruction)
- **timer_interrupt():** this function will be regularly  executed, which can be used to implement the dynamic priority.
- **thread_update_priority(thread t):**  this function will calculate the new priority of the input thread.



### Algorithms

We need to use the offered formula to count the priority of all the threads and update them regularly.

The formula of counting the priority will be:
$$
priority = PRI_MAX - (recent_cpu / 4) - (nice * 2)`.
$$
The formula of counting the recent_cpu will be:
$$
recent_cpu = (2*load_avg)/(2*load_avg + 1) * recent_cpu + nice`.
$$
The formula of counting the `load_avg will be:`
$$
load_avg = (59/60)*load_avg + (1/60)*ready_threads`.
$$


We will implement the count of recent cpu in the **thread_get_recent_cpu()**,  implement the count of priority in the **thread_get_recent_cpu():** and put the this function in the **timer_interrupt()** to refresh the priority regularly.

### Synchronization

### Rationale






## **Additional Questions**



1. Fill the table

| Timer ticks | R(A) | R(B) | R(C) | R(A) | R(B) | R(C) | Thread to run |
| :---------: | ---- | ---- | ---- | ---- | ---- | ---- | ------------- |
|      0      |      |      |      |      |      |      |               |
|      4      |      |      |      |      |      |      |               |
|      8      |      |      |      |      |      |      |               |
|     12      |      |      |      |      |      |      |               |
|     16      |      |      |      |      |      |      |               |
|     20      |      |      |      |      |      |      |               |
|     24      |      |      |      |      |      |      |               |
|     28      |      |      |      |      |      |      |               |
|     32      |      |      |      |      |      |      |               |
|     36      |      |      |      |      |      |      |               |
|             |      |      |      |      |      |      |               |

2. Did any ambiguities in the scheduler specification make values in the table (in the previous question) uncertain? If so, what rule did you use to resolve them?













