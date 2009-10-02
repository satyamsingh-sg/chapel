#ifndef _chplthreads_h_
#define _chplthreads_h_

#ifndef LAUNCHER

#include <stdint.h>
#include "arg.h"
#include "chpl_cfg_threads.h"


//
// Define the (uppercase) function names in the interface, that is,
// the names of the functions declared below.
//

#include "chplthreads_func_names.h"


// Defined in the generated Chapel code:

extern int32_t maxThreads;


// Mutexes

typedef chpl_mutex_t* chpl_mutex_p;

void CHPL_MUTEX_INIT(chpl_mutex_p);
chpl_mutex_p CHPL_MUTEX_NEW(void);
void CHPL_MUTEX_LOCK(chpl_mutex_p);
void CHPL_MUTEX_UNLOCK(chpl_mutex_p);


// Sync variables

void      CHPL_SYNC_LOCK(chpl_sync_aux_t *);
void      CHPL_SYNC_UNLOCK(chpl_sync_aux_t *);
void      CHPL_SYNC_WAIT_FULL_AND_LOCK(chpl_sync_aux_t *,
                                       int32_t, chpl_string);
void      CHPL_SYNC_WAIT_EMPTY_AND_LOCK(chpl_sync_aux_t *,
                                        int32_t, chpl_string);
void      CHPL_SYNC_MARK_AND_SIGNAL_FULL(chpl_sync_aux_t *);     // and unlock
void      CHPL_SYNC_MARK_AND_SIGNAL_EMPTY(chpl_sync_aux_t *);    // and unlock
chpl_bool CHPL_SYNC_IS_FULL(void *, chpl_sync_aux_t *, chpl_bool);
void      CHPL_INIT_SYNC_AUX(chpl_sync_aux_t *);
void      CHPL_DESTROY_SYNC_AUX(chpl_sync_aux_t *);


// Single variables

void      CHPL_SINGLE_LOCK(chpl_single_aux_t *);
void      CHPL_SINGLE_UNLOCK(chpl_single_aux_t *);
void      CHPL_SINGLE_WAIT_FULL(chpl_single_aux_t *, int32_t, chpl_string);
void      CHPL_SINGLE_MARK_AND_SIGNAL_FULL(chpl_single_aux_t *); // and unlock
chpl_bool CHPL_SINGLE_IS_FULL(void *, chpl_single_aux_t *, chpl_bool);
void      CHPL_INIT_SINGLE_AUX(chpl_single_aux_t *);
void      CHPL_DESTROY_SINGLE_AUX(chpl_single_aux_t *);


// Tasks

void CHPL_TASKING_INIT(void);        // main task initializes tasking
void CHPL_TASKING_EXIT(void);        // called by the main task

typedef struct chpl_task_list* chpl_task_list_p;

void CHPL_ADD_TO_TASK_LIST(
         chpl_fn_int_t,      // function to call for task
         void*,              // argument to the function
         chpl_task_list_p*,  // task list
         int32_t,            // locale where task list resides
         chpl_bool,          // whether to call CHPL_BEGIN
         int,                // line at which function begins
         chpl_string);       // name of file containing functions
void CHPL_PROCESS_TASK_LIST(chpl_task_list_p);
void CHPL_EXECUTE_TASKS_IN_LIST(chpl_task_list_p);
void CHPL_FREE_TASK_LIST(chpl_task_list_p);

// Fork one task.  Do not wait.  Used to implement Chapel's begin statement.
void CHPL_BEGIN(
         chpl_fn_p,         // function to fork
         void*,             // function arg
         chpl_bool,         // ignore_serial = force spawning task regardless
                            // of serial state; as in the case of calling
                            // for on-statement implementation
         chpl_bool,         // serial state (must be "false" except when
                            // called from a comm lib such as gasnet;
                            // otherwise, serial state is that of the
                            // task executing CHPL_BEGIN)
         chpl_task_list_p);

//
// Get and set dynamic serial state.
//
chpl_bool CHPL_GET_SERIAL(void);
void      CHPL_SET_SERIAL(chpl_bool);

//
// returns the number of tasks that are ready to run on the current locale,
// not including any that have already started running.
//
uint32_t CHPL_NUMQUEUEDTASKS(void);

//
// returns the number of tasks that are running on the current locale,
// including any that may be blocked waiting for something.
// Note that the value returned could be larger than the limit on the maximum
// number of threads, since a thread could be "suspended," particularly if it
// is waiting at the end of a cobegin, e.g.  In this case, it could be
// executing a task inside the cobegin, so in effect the same thread would be
// executing more than one task.
//
uint32_t CHPL_NUMRUNNINGTASKS(void);

//
// returns the number of tasks that are blocked waiting on a sync or single
// variable.
// Note that this information may only available if the program is run with
// the -b switch, which enables block reporting and deadlock detection.
// If this switch is not specified, -1 may be returned.
//
int32_t CHPL_NUMBLOCKEDTASKS(void);


// Threads

chpl_threadID_t  CHPL_THREAD_ID(void);                // caller's thread id

void             CHPL_THREAD_CANCEL(chpl_threadID_t); // ask thread to quit

void             CHPL_THREAD_JOIN(chpl_threadID_t);   // wait for thread quit

//
// returns the default maximum number of threads that can be handled by this
// threading layer (initial value of maxThreads); use the sentinel value 0
// if the maximum number of threads is limited only by the system's available
// resources.
//
int32_t CHPL_THREADS_GETMAXTHREADS(void);

//
// returns the upper limit on the maximum number of threads that can be handled
// by this threading layer; use the sentinel value 0 if the maximum number of
// threads is limited only by the system's available resources.
//
int32_t CHPL_THREADS_MAXTHREADSLIMIT(void);

//
// returns the total number of threads that currently exist, whether running,
// blocked, or idle
//
uint32_t CHPL_NUMTHREADS(void);

//
// returns the number of threads that are currently idle
//
uint32_t CHPL_NUMIDLETHREADS(void);

#else // LAUNCHER

#define CHPL_MUTEX_INIT(x)
#define CHPL_MUTEX_LOCK(x)
#define CHPL_MUTEX_UNLOCK(x)
typedef void chpl_sync_aux_t;
typedef void chpl_single_aux_t;
typedef int chpl_mutex_t;
#define CHPL_TASKING_EXIT()

#endif // LAUNCHER

#endif
