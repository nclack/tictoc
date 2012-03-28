# TicTocTimer

A high-resolution timer for Windows, OS X, and Linux.

Timers are useful for profiling and optimizing code, but access typically
require different non-portable calls to access msec or better precision.  So, I
wrapped these up and pu them behaind a consistent API.

The test suite will provide some feedback about how reliable the timer is at different 
resolutions on your system.  You should run the tests several times.


## TODO

 - do the tests run on Windows?  They rely on usleep
 - Linux timer 

## Incorporating into your project

 - Either build and link against the static library.
 - Or just copy tictoc.[ch] to your project.

## Usage

Simple example:

```c
#include "tictoc.h"
int main(int argc, char* argv[])
{ 
  TicTocTimer clock = tic();
  time_consuming_operation();
  printf("Elapsed time %f seconds.\n",toc(&clock)); // prints time since last tic()  
  another_time_consuming_operation();
  printf("Elapsed time %f seconds.\n",toc(&clock)); // prints time since last toc()
}
```

An even simpler example:

```c
#include "tictoc.h"
int main(int argc, char* argv[])
{ 
  tic();
  printf("%f\n",toc());
}
```

The last example uses a global timer.  Using the global timer from different threads 
may produce strange results.

## Details

* On Windows, tictoc uses [QueryPerformanceCounter][1]
* On OS X, tictoc uses [mach_absolute_time][2]
* On Linux, tictoc uses [clock_gettime][3]

[1]: http://msdn.microsoft.com/en-us/library/windows/desktop/ms644904(v=vs.85).aspx
[2]: https://developer.apple.com/library/mac/#documentation/Darwin/Conceptual/KernelProgramming/services/services.html
[3]: http://www.tin.org/bin/man.cgi?section=3&topic=clock_gettime

