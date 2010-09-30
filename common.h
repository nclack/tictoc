/* 
 * History
 * -------
 *  - pulled from <fetch> repo commit df3d14488ba7d47db39c1dc187f131c46ce07df0
 *    <fetch>/fetch/common.[ch]
 *  - 2010-08-04 removed/ported windows specfic stuff.
 */
#pragma once

#include <stdio.h>
#include <stdlib.h>


// -----------------
// Utility functions
// -----------------
#define IS_POW2_OR_ZERO(v) ( ((v) & ((v) - 1)) ==  0  )
#define IS_POW2(v)         (!((v) & ((v) - 1)) && (v) )

#define MOD_UNSIGNED_POW2(n,d)   ( (n) & ((d)-1) )

#define SWAP(a,b)          (((a) ^ (b)) && ((b) ^= (a) ^= (b), (a) ^= (b)))

#define MAX(a,b) __max(a,b)
#define MIN(a,b) __min(a,b)

#define SIGN(a) (((a)>0) - ((a)<0))

inline u8     _next_pow2_u8    (u8  v);
inline u32    _next_pow2_u32   (u32 v);
inline u64    _next_pow2_u64   (u64 v);
inline size_t _next_pow2_size_t(size_t v);

// ---------
// Profiling
// ---------
 
typedef struct _tic_toc_timer
{ u64 last; // last observation on the counter
  u64 rate; // counts per second  
} TicTocTimer;

TicTocTimer tic(void);
double      toc(TicTocTimer *last); // returns time since last in seconds and updates timer 
                    
// -------------------------------------------
// Threading and Atomic Operations : Utilities
// -------------------------------------------

// (atomic) must evaluate to a pointer of the correct type

#ifdef _WIN32
#define Interlocked_Inc_u32(atomic)          (InterlockedIncrement((atomic)))
#define Interlocked_Dec_u32(atomic)          (InterlockedDecrement((atomic)))
#define Interlocked_Inc_u64(atomic)          (InterlockedIncrement64((atomic)))
#define Interlocked_Dec_u64(atomic)          (InterlockedDecrement64((atomic)))
#define Interlocked_Dec_And_Test_u32(atomic) (InterlockedExchangeAdd((atomic),-1)==1)
#define Interlocked_Dec_And_Test_u64(atomic) (InterlockedExchangeAdd64((atomic),-1)==1)
#else // gcc
#define Interlocked_Inc_u32(atomic)          (__sync_add_and_fetch((atomic),1))
#define Interlocked_Dec_u32(atomic)          (__sync_sub_and_fetch((atomic),1))
#define Interlocked_Inc_u64(atomic)          (__sync_add_and_fetch((atomic),1)) 
#define Interlocked_Dec_u64(atomic)          (__sync_sub_and_fetch((atomic),1)) 
#define Interlocked_Dec_And_Test_u32(atomic) (__sync_sub_and_fetch((atomic),1)==0)
#define Interlocked_Dec_And_Test_u64(atomic) (__sync_sub_and_fetch((atomic),1)==0) 
#endif
                    
// --------
// Shutdown
// --------
//
// Shutdown callback functions
//   - take no arguments
//   - do not exit/abort under any circumstances
//   - return 0 only on success
//   - are called in reverse order of when they were
//     added.
//
// After all callbacks return, the system is assumed
// to be in a state where exit/abort can safely be
// called.
//
typedef unsigned int (*pf_shutdown_callback)(void);

size_t       Register_New_Shutdown_Callback ( pf_shutdown_callback callback );
void         Shutdown_Hard          ( unsigned int err);   // Calls shutdown functions and exits
unsigned int Shutdown_Soft          ( void );              // Calls shutdown functions
unsigned int Shutdown_Soft_Nonblocking(void);              // Uses the windows thread pool to call shutdown functions

// -----------------------
// Reporting and Asserting
// -----------------------

typedef void (*pf_reporting_callback)(const char* msg);

size_t Register_New_Error_Reporting_Callback  ( pf_reporting_callback callback );
size_t Register_New_Warning_Reporting_Callback( pf_reporting_callback callback );
size_t Register_New_Debug_Reporting_Callback  ( pf_reporting_callback callback );

void Error_Reporting_Remove_All_Callbacks  (void);
void Warning_Reporting_Remove_All_Callbacks(void);
void Debug_Reporting_Remove_All_Callbacks  (void);

void Reporting_Setup_Log_To_VSDebugger_Console(void);
void Reporting_Setup_Log_To_File( FILE *file );
void Reporting_Setup_Log_To_Filename( const char* filename );
void Reporting_Setup_Log_To_Stdout(void);

void ReportLastWindowsError(void);

typedef void (*pf_reporter)(const char* fmt, ...);

void error  (const char* fmt, ...);
void warning(const char* fmt, ...);
void debug  (const char* fmt, ...);

#define Guarded_Assert(expression) \
  if(!(expression))\
    error("Assertion failed: %s\n\tIn %s (line: %u)\n", #expression, __FILE__ , __LINE__ )
    
#define Guarded_Assert_WinErr(expression) \
  if(!(expression))\
  { ReportLastWindowsError();\
    error("Windows call failed: %s\n\tIn %s (line: %u)\n", #expression, __FILE__ , __LINE__ );\
  }

#define return_if_fail( cond )          { if(!(cond)) return; }
#define return_if( cond )               { if( (cond)) return; }
#define goto_if_fail( cond, lbl )       { if(!(cond)) goto lbl;}
#define goto_if( cond, lbl )            { if( (cond)) goto lbl;}
#define return_val_if_fail( cond, val ) { if(!(cond)) return (val); }
#define return_val_if( cond, val )      { if( (cond)) return (val); }

// ------
// Memory
// ------
void *Guarded_Malloc( size_t nelem, const char *msg );
void *Guarded_Calloc( size_t nelem, size_t bytes_per_elem, const char *msg );
void  Guarded_Realloc( void **item, size_t nelem, const char *msg );

// Expands array using chunk sizes that linearly increase in size
void RequestStorage( void** array,           // Pointer to array
                     size_t *nelem,          // Pointer to current maximum element count of array
                     size_t request,         // Request that the array be sized so this index is valid
                     size_t bytes_per_elem,  // The chunk size
                     const char *msg );      // A message to use in the case of an error

// Expands array using chunk sizes that exponentially increase in size.  Size is a power of two.
void RequestStorageLog2( void** array,           // Pointer to array
                         size_t *nelem,          // Pointer to current maximum element count of array
                         size_t request,         // Request that the array be sized so this index is valid
                         size_t bytes_per_elem,  // The chunk size
                         const char *msg );      // A message to use in the case of an error

// ---------------
// Container types
// ---------------

#define TYPE_VECTOR_DECLARE(type) \
  typedef struct _vector_##type   \
          { type  *contents;      \
            size_t nelem;         \
            size_t count;         \
            size_t bytes_per_elem;\
          } vector_##type;        \
                                  \
  vector_##type *vector_##type##_alloc   ( size_t nelem );\
  void           vector_##type##_request ( vector_##type *self, size_t idx );\
  void           vector_##type##_request_pow2 ( vector_##type *self, size_t idx );\
  void           vector_##type##_free    ( vector_##type *self )

#define VECTOR_EMPTY { NULL, 0, 0, 0 }

#define TYPE_VECTOR_DEFINE(type) \
vector_##type *vector_##type##_alloc   ( size_t nelem )                                            \
{ size_t bytes_per_elem = sizeof( type );                                                          \
  vector_##type *self   = (vector_##type*) Guarded_Malloc( sizeof(vector_##type), "vector_init" ); \
  self->contents        = (type*) Guarded_Calloc( nelem, bytes_per_elem, "vector_init");           \
  self->bytes_per_elem  = bytes_per_elem; \
  self->count           = 0;              \
  self->nelem           = nelem;          \
  return self;                            \
} \
  \
void vector_##type##_request( vector_##type *self, size_t idx ) \
{ if( !self->bytes_per_elem )                \
    self->bytes_per_elem = sizeof(type);     \
  RequestStorage( (void**) &self->contents,  \
                  &self->nelem,              \
                  idx,                       \
                  self->bytes_per_elem,      \
                  "vector_request" );        \
} \
  \
void vector_##type##_request_pow2( vector_##type *self, size_t idx ) \
{ if( !self->bytes_per_elem )                \
    self->bytes_per_elem = sizeof(type);     \
  RequestStorageLog2( (void**) &self->contents,  \
                      &self->nelem,              \
                      idx,                       \
                      self->bytes_per_elem,      \
                      "vector_log2_request" );   \
} \
  \
void vector_##type##_free( vector_##type *self ) \
{ if(self)                                       \
  { if( self->contents ) free( self->contents ); \
    self->contents = NULL;                       \
    free(self);                                  \
  }                                              \
} \
  \
void vector_##type##_free_contents( vector_##type *self ) \
{ if(self)                                       \
  { if( self->contents ) free( self->contents ); \
    self->contents = NULL;                       \
    self->nelem = 0;                             \
    self->count = 0;                             \
  }                                              \
}

TYPE_VECTOR_DECLARE(char);
TYPE_VECTOR_DECLARE(u8);
TYPE_VECTOR_DECLARE(u16);
TYPE_VECTOR_DECLARE(u32);
TYPE_VECTOR_DECLARE(u64);
TYPE_VECTOR_DECLARE(size_t);
TYPE_VECTOR_DECLARE(i8);
TYPE_VECTOR_DECLARE(i16);
TYPE_VECTOR_DECLARE(i32);
TYPE_VECTOR_DECLARE(i64);
TYPE_VECTOR_DECLARE(f32);
TYPE_VECTOR_DECLARE(f64);
