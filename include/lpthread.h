#ifndef _LPTHREAD_H
#define _LPTHREAD_H  

#include<steque.h>
#include<stdio.h>
#include<stdlib.h>
#include<ucontext.h>
#include<unistd.h>
#include<stdbool.h> 
#include<signal.h>
#include<string.h>
#include<sys/types.h>
#include<sys/time.h>

#define STACK_SIZE 512 * 1024
#define QUANTUM 2000

#define RUNNING 	0
#define ZOMBIE 		2
#define DONE	 	1

/*
*	Useful links:
*
*	https://code.woboq.org/userspace/glibc/nptl/pthread_create.c.html
*	https://www2.cs.duke.edu/courses/spring09/cps110/projects/1/project1.text
*	http://nitish712.blogspot.com/2012/10/thread-library-using-context-switching.html
*	https://courses.engr.illinois.edu/cs423/sp2011/mps/mp3/mp3.pdf
*	https://pages.mtu.edu/~shene/PUBLICATIONS/1999/fie99-mtp.pdf
*	http://web.eecs.umich.edu/~farnam/482/Winter99/pa2-v3.html
*/

/*  
*   GTThread--A User Level Thread Library
*   This library is a modification obtained at the website https://github.com/LancelotGT/gtthread
*   Creator: Ning Wang. Nov 23th, 2019.
*/

typedef unsigned long int lthread_t;

typedef struct {
    
} lthread_attr_t;

typedef struct {
	bool lock;
	unsigned int id;
	lthread_t blocking_thread;
} lthread_mutex_t;

typedef struct {
	
} lthread_mutexattr_t;

typedef struct {
	ucontext_t * context;
	int state;
	void * retval;
	lthread_t joining;
	bool joinable;
	lthread_attr_t * attr;
	lthread_t id;
} lthread;

int  lthread_create(lthread_t * thread, const lthread_attr_t * attr, void *(*routine) (void*), void * arg);
void lthread_exit(void * retval);
int  lthread_yield(void);
int  lthread_join(lthread_t thread, void **retval);
int  lthread_detach(lthread_t thread);
void lthread_mutex_init(lthread_mutex_t *restrict mutex, const lthread_mutexattr_t *restrict attr);
void lthread_mutex_destroy(lthread_mutex_t *mutex);
void lthread_mutex_unlock(lthread_mutex_t *mutex);
void lthread_mutex_trylock(lthread_mutex_t *mutex);

#endif