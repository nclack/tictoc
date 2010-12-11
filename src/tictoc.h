#pragma once
#ifdef __cplusplus
extern "C"{
#endif
// ---------
// Profiling
// ---------
#include <stdint.h>

typedef struct _tic_toc_timer
{ uint64_t last; // last observation on the counter
  uint64_t rate; // counts per second  
} TicTocTimer;

TicTocTimer tic(void);
double      toc(TicTocTimer *last); // returns time since last in seconds and updates timer 
double      toc(TicTocTimer *last); // returns time since last in seconds and updates timer


#ifdef __cplusplus
}
#endif
                    
