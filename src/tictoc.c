// ---------
// Profiling
// ---------
#include <stdint.h>
#include <tictoc.h>

#ifdef DEBUG
#define debug(...) printf(__VA_ARGS__)
#else
#define debug(...)
#endif

typedef uint64_t u64;

#ifdef _APPLE_
#define HAVE_MACH_TIMER
#include <mach/mach_time.h>
#endif

#ifdef _WIN32
#define HAVE_WIN32_TIMER
#include <WinBase.h>
#define Guarded_Assert_WinErr(expression) \
  if(!(expression))\
  { ReportLastWindowsError();\
    error("Windows call failed: %s\n\tIn %s (line: %u)\n", #expression, __FILE__ , __LINE__ );\
  }
void ReportLastWindowsError(void) 
{ //EnterCriticalSection( _get_reporting_critical_section() );
  { // Retrieve the system error message for the last-error code

    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError(); 

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    // Display the error message and exit the process

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
        (lstrlen((LPCTSTR)lpMsgBuf) + 40) * sizeof(TCHAR)); 
    StringCchPrintf((LPTSTR)lpDisplayBuf, 
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("Failed with error %d: %s"), 
        dw, lpMsgBuf); 
    
    // spam formated string to listeners
    { 
      fprintf(stderr,"%s",lpDisplayBuf);
    }

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
  }
  //LeaveCriticalSection( _get_reporting_critical_section() );
}
#endif

TicTocTimer tic(void)
{ TicTocTimer t = {0,0};
#ifdef HAVE_MACH_TIMER
  mach_timebase_info_data_t rate_nsec;
  mach_timebase_info(&rate_nsec);
  t.rate = 1000000000LL * rate_nsec.numel / rate_nsec.denom;
  t.last = mach_absolute_time();
#endif
#ifdef HAVE_WIN32_TIMER
  { LARGE_INTEGER tmp;
 
    Guarded_Assert_WinErr( QueryPerformanceFrequency( &tmp ) );
    t.rate = tmp.QuadPart;
    Guarded_Assert_WinErr( QueryPerformanceCounter  ( &tmp ) );
    t.last = tmp.QuadPart;
  }
#endif
  
#ifdef DEBUG_TIC_TOC_TIMER  
  //Guarded_Assert( t.rate > 0 );
  debug("Tic() timer frequency: %I64u Hz\r\n"
        "           resolution: %g ns\r\n"
        "               counts: %I64u\r\n",t.rate, 
                                        1e9/(double)t.rate, 
                                        t.last);
#endif  
  
  return t;
}

// - returns time since last in seconds
// - updates timer
double toc(TicTocTimer *t)
{ u64 now;
  double delta;
#ifdef HAVE_MACH_TIMER
  now = mach_absolute_time();
#endif
#ifdef HAVE_WIN32_TIMER
  { LARGE_INTEGER tmp;
    Guarded_Assert_WinErr( QueryPerformanceCounter( &tmp ) );
    now = tmp.QuadPart;
  }
#endif
  delta = ( now - t->last) / (double)t->rate;
  t->last = now;
  return delta;
}

