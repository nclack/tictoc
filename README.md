# TicTocTimer

A high-resolution timer for Windows, OS X, and Linux.

Performance timers are useful for profiling and optimizing code, 
but using one typically involves system specific calls.  So, I
wrapped these up and put them behind a simple API.

## Incorporating into your project

 - Either build and link against the static library.
 - Or just copy tictoc.[ch] to your project.
 - On linux, you'll need to link against librt.

### With [CMake][4]:

 Unpack the source tree (or use git submodule) to a subdirectory in your project.
 For example, `modules/tictoc`.
```cmake
  add_subdirectory(modules/tictoc) #adds targets for the static library, tests, and docs
  link_directories(${tictoc_BINARY_DIR})
  include_directories(modules/tictoc)
  ...
  target_link_libraries(my-project tictoc)

```

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
  printf("%f\n",toc(0));
}
```

The last example uses a global timer.  Using the global timer from different threads 
may produce strange results.

## Reliability

The test suite runs tests for different resolutions that rely on some
implementation of `usleep()` to pause for semi-controlled lengths of 
time.  However, this isn't a reliable way to measure the actual 
precision of your system's timer.

You'll have to measure that for yourself according to what is appropriate for
your application.

## Details

* On Windows, tictoc uses [QueryPerformanceCounter][1]
* On OS X, tictoc uses [mach_absolute_time][2]
* On Linux, tictoc uses [clock_gettime][3]

[1]: http://msdn.microsoft.com/en-us/library/windows/desktop/ms644904(v=vs.85).aspx
[2]: https://developer.apple.com/library/mac/#documentation/Darwin/Conceptual/KernelProgramming/services/services.html
[3]: http://www.tin.org/bin/man.cgi?section=3&topic=clock_gettime
[4]: http://www.cmake.org/
