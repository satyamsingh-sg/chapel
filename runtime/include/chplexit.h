#ifndef _CHPL_EXIT_H_
#define _CHPL_EXIT_H_

void cleanup_for_exit(void);    // must be exposed to avoid dead-code elim.

void chpl_exit_all(int status);  // must be called by all threads
void chpl_exit_any(int status);  // may be called by any thread

#endif
