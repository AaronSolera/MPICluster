#include "../include/lpthread.h"

// Main queue for living threads
static steque_t ready_queue;
static steque_t zombie_queue;
// Base indexes for structure ids
lthread_t thread_base_id = 0;
int mutex_base_id = 0;
// Time interval setting structures
struct itimerval timer;
struct sigaction act;
sigset_t vtalrm;
// Thread structs that stores relevant information
lthread * current_thread = NULL;

// This program schedules context switching of enqueued threads
void scheduler(int signal){
	// block the signal
    sigprocmask(SIG_BLOCK, &vtalrm, NULL);
	// if no thread in the ready queue, resume execution
    if (steque_isempty(&ready_queue))
        return;
    if (ready_queue.front == NULL)
        return;
	// get the next runnable thread and use preemptive scheduling
    lthread * next = (lthread *) steque_pop(&ready_queue);
    while(next->state == DONE){
        // Free up memory allocated for exit thread
	    free(next->context->uc_stack.ss_sp); 
	    free(next->context);  
	    free(next);
        if (ready_queue.front == NULL) return;
        next = (lthread *) steque_pop(&ready_queue);
    }
    lthread * prev = current_thread;
    steque_enqueue(&ready_queue, prev);
    next->state = RUNNING; 
    current_thread = next;
	// unblock the signal
    sigprocmask(SIG_UNBLOCK, &vtalrm, NULL); 

	swapcontext(prev->context, current_thread->context);
}

// This function verifies if and specific thread is enqueued
lthread * findThreadInQueues(lthread_t thread){
	steque_node_t* current = ready_queue.front;   
    while (current != NULL){
        lthread * t = (lthread *) current->item;
        if (t->id == thread)
            return t;
        current = current->next;
    }

    current = zombie_queue.front;
    while (current != NULL)
    {
        lthread * t= (lthread *) current->item;
        if (t->id == thread)
            return t;
        current = current->next;
    } 
    return NULL;
}

void startSubroutine(void* (*routine) (void*), void* args){
	// Unblock signal comes from gtthread_create
    sigprocmask(SIG_UNBLOCK, &vtalrm, NULL);
    // Start executing the start routine
	current_thread->retval = (*routine)(args);
	// When start_rountine returns, call lthread_exit
    lthread_exit(current_thread->retval);
}

/*  
*   Sigaction and setitimer in C
*   lthread_create function has a code modification obtained at the website https://stackoverflow.com/questions/59011345/sigaction-and-setitimer-in-c
*   Creator: Aidan Goldfarb. Nov 23th, 2019.
*/
int lthread_create(lthread_t * thread, const lthread_attr_t * attr, void *(*routine) (void*), void * arg){
	// If this is the first call to lthread_create
	if(thread_base_id == 0){
		// Initilizing thread queue
		steque_init(&ready_queue);
		steque_init(&zombie_queue);
		// Setting main thread as previous thread
		lthread * main_thread = (lthread *) malloc(sizeof(lthread));
		main_thread->id = thread_base_id++;
		main_thread->state = RUNNING;
		main_thread->context = (ucontext_t*) malloc(sizeof(ucontext_t));
		memset(main_thread->context, '\0', sizeof(ucontext_t));
		current_thread = main_thread;
		// Setting uo the signal mask
    	sigemptyset(&vtalrm);
    	sigaddset(&vtalrm, SIGALRM);
    	// In case this is blocked previously
   		sigprocmask(SIG_UNBLOCK, &vtalrm, NULL); 
	
    	// Set alarm signal and signal handler
	    timer.it_interval.tv_usec = QUANTUM;
	    timer.it_interval.tv_sec = 0;
	    timer.it_value.tv_usec = QUANTUM;
	    timer.it_value.tv_sec = 0; 

	    if (getcontext(main_thread->context) == -1)
	    {
	      perror("getcontext");
	      exit(EXIT_FAILURE);
	    }
    
	    if (setitimer(ITIMER_REAL, &timer, NULL) < 0)
	    {
	        perror("setitimer");
	        exit(EXIT_FAILURE);
	    }

	    // install signal handler for SIGVTALRM 
	    memset(&act, '\0', sizeof(act));
	    act.sa_handler = &scheduler;
	    if (sigaction(SIGALRM, &act, NULL) < 0)
	    {
	      perror ("sigaction");
	      exit(EXIT_FAILURE);
	    }
	}
	// Block SIGVTALRM signal
    sigprocmask(SIG_BLOCK, &vtalrm, NULL);
	// Setting default attributes for enqueueing a new thread
	lthread * new_thread = (lthread *) malloc(sizeof(lthread));
	new_thread->id = thread_base_id++;
	new_thread->attr = (lthread_attr_t *) attr;
	new_thread->state = RUNNING;
	new_thread->joining = -1;
	new_thread->joinable = true;
	new_thread->context = (ucontext_t*) malloc(sizeof(ucontext_t));
	*thread = new_thread->id;
	memset(new_thread->context, '\0', sizeof(ucontext_t));
	
	if (getcontext(new_thread->context) == -1)
    {
      perror("getcontext");
      exit(EXIT_FAILURE);
    }
    /* allocate stack for the newly created context 
       here we allocate the stack size using the canonical
       size for signal stack. */
    new_thread->context->uc_stack.ss_sp = malloc(STACK_SIZE);
    new_thread->context->uc_stack.ss_size = STACK_SIZE;
    new_thread->context->uc_stack.ss_flags = 0;
    new_thread->context->uc_link = NULL;

    makecontext(new_thread->context, (void (*))startSubroutine, 2, routine, arg);
	steque_enqueue(&ready_queue, new_thread);
	// Unlocking vtalarm
	sigprocmask(SIG_UNBLOCK, &vtalrm, NULL);
	return 0;
}

void lthread_exit(void * retval){
	// Block alarm signal
    sigprocmask(SIG_BLOCK, &vtalrm, NULL);

    if (steque_isempty(&ready_queue)){ 
        sigprocmask(SIG_UNBLOCK, &vtalrm, NULL); 
        exit((long) retval);
    }
    // if the main thread call lthread_exit
    if (current_thread->id == 0){
        while (!steque_isempty(&ready_queue)){
            sigprocmask(SIG_UNBLOCK, &vtalrm, NULL);  
            scheduler(SIGVTALRM);
            sigprocmask(SIG_BLOCK, &vtalrm, NULL);
        }
        sigprocmask(SIG_UNBLOCK, &vtalrm, NULL);   
        exit((long) retval);
    }
    // Dequeueing current thread
    lthread * prev = current_thread; 
    current_thread = (lthread *) steque_pop(&ready_queue);
    current_thread->state = RUNNING; 
    
    // Mark the exit thread as DONE and add to zombie_queue
    prev->state = ZOMBIE; 
    prev->retval = retval;
    prev->joining = -1;

    if(!(prev->joinable)){
    	steque_enqueue(&zombie_queue, prev);
    }else{
    	prev->state = DONE;
    	steque_enqueue(&ready_queue, prev);
    }

    // Unblock alarm signal and setcontext for next thread
    sigprocmask(SIG_UNBLOCK, &vtalrm, NULL); 
    setcontext(current_thread->context);
}

// This function changes current context to put a new one to run
int lthread_yield(void){
	// Block SIGVTALRM signal
    sigprocmask(SIG_BLOCK, &vtalrm, NULL);
    // If no thread to yield, simply return
    if (steque_isempty(&ready_queue))
        return 0;
    // Dequeueing current thread
    lthread * next = (lthread *) steque_pop(&ready_queue);
    lthread * prev = current_thread;
    steque_enqueue(&ready_queue, current_thread);
    current_thread = next;
    // Unblock the signal
    sigprocmask(SIG_UNBLOCK, &vtalrm, NULL); 
    swapcontext(prev->context, current_thread->context); 
    return 0;
}

// This function waits to an specified thread until it finishes
int lthread_join(lthread_t thread, void **retval){
	// if a thread tries to join itself
    if (thread == current_thread->id)
        return -1;
    // If a thread is not created
    lthread * t;
    if ((t = findThreadInQueues(thread)) == NULL){
        return -1;
    }
    // Check if that thread is joining on me
    if (t->joining == current_thread->id)
        return -1;
    // Check if thread is detached
    if (!(t->joinable))
    	return -1;
    // Wait on the thread to terminate 
    current_thread->joining = t->id;
    while (t->state == RUNNING){
        sigprocmask(SIG_UNBLOCK, &vtalrm, NULL);
        scheduler(SIGVTALRM);
        sigprocmask(SIG_BLOCK, &vtalrm, NULL);
    }
    if (retval == NULL)
        return 0;
    else 
        *retval = t->retval;

    return 0;
}

int lthread_detach(lthread_t thread){
	sigprocmask(SIG_BLOCK, &vtalrm, NULL);
	lthread * t;
	printf("%li\n", thread);
	if ((t = findThreadInQueues(thread)) != NULL){
        t->joinable = false;
        sigprocmask(SIG_BLOCK, &vtalrm, NULL);
        return 0;
    }
    sigprocmask(SIG_BLOCK, &vtalrm, NULL);
    return -1;
}

// This function sets initial values for lthread_mutex_t 
void lthread_mutex_init(lthread_mutex_t *restrict mutex, const lthread_mutexattr_t *restrict attr){
	mutex->lock = false;
	mutex->id = mutex_base_id++;
}

// This functions sets free given lthread_mutex_t memory
void lthread_mutex_destroy(lthread_mutex_t *mutex){
	free(mutex);
}

// This function unlock a lthread_mutex_t setting its lock value to false. It only works if the thread which locks it is unlocking it.
void lthread_mutex_unlock(lthread_mutex_t *mutex){
	if(current_thread == NULL) return;
	if(mutex->blocking_thread == current_thread->id){
		mutex->lock = false;
	}
}
 
// This function tries to lock a lthread_mutex_t setting its lock value to true. If it is already lock, the calling thread is put at the queue end  
void lthread_mutex_trylock(lthread_mutex_t *mutex){
	if(current_thread == NULL) return;
	while(mutex->lock){
        scheduler(SIGVTALRM);
	}
	mutex->lock = true;
	mutex->blocking_thread = current_thread->id;
}