#include <mach/mach_time.h>
#include <stdio.h>
int main(int argc, char* argv[])
{ uint64_t t0,t1;
  t0 = mach_absolute_time();
  {int i=1000000; while(i--) if(i%300000==0) printf(".");
  }
  t1 = mach_absolute_time();
  printf("Time: %lld\n",t1-t0);

  { mach_timebase_info_data_t data;
    mach_timebase_info(&data);
    printf("Info: %u/%u\n",data.numer,data.denom);
  }

  return 0;
}
