#ifndef MUTEX_H_
#define MUTEX_H_

class ShockMutex
{

//pthread_mutex_t
   
};

class ShockThread
{
};
// pthread_t t;
	
   
typedef void* (*ThreadProc)(void*);

void ShockMutexCreate(ShockMutex*);
//pthread_mutex_init( &mButtonState[ i ].mutexLock, NULL );

void ShockMutexLock(ShockMutex*);
//pthread_mutex_lock( &mButtonState[ i ].mutexLock );
  

void ShockMutexUnlock(ShockMutex*);
//pthread_mutex_unlock( &mButtonState[ i ].mutexLock );

void ShockMutexDestroy(ShockMutex*);
//pthread_mutex_destroy( &mButtonState[ i ].mutexLock );

int ShockThreadCreate(ShockThread* t, ThreadProc);
   //int result = pthread_create( &t, NULL, PollInput_ThreadProc, NULL );

void ShockThreadDetach(ShockThread& t);
//pthread_detach( t );

#endif