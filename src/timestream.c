#include "common.h"

#if 0
#define ECHO(e)      LOG(e)
#else
#define ECHO(e)
#endif
#define LOG(...)     debug(__VA_ARGS__)
#define REPORT(e)    LOG("%s(%d) - %s():"ENDL "\t%s"ENDL "\tExpression evaluated as false."ENDL,__FILE__,__LINE__,__FUNCTION__,e)
#define TRY(e)       do{ ECHO(#e); if(!(e)) {REPORT(#e); goto Error;}} while(0)
#define NEW(T,e,N)   TRY((e)=(T*)malloc(sizeof(T)*N))
#define ZERO(T,e,N)  (memset((e),0,sizeof(T)*N))

#define countof(e)   (sizeof(e)/sizeof(*(e)))

struct _timestream_t
{ FILE  *fp;
  TicTocTimer timer;
  int    n;
  float  history[1024]; // 4kB
};

typedef struct _timestream_t* timestream_t;

static void timestream_flush(timestream_t self)
{ if(self->n>0)
    fwrite(self->history,sizeof(*self->history),self->n,self->fp);
  self->n=0;
}

void timestream_close(timestream_t self)
{ if(!self) return;
  timestream_flush(self);
  if(self->fp) fclose(self->fp);
  free(self);
}

timestream_t timestream_open(const char* filename_fmt,...)
{ timestream_t self=0;
  char buf[1024] = {0};
  va_list args;
  size_t n;
  va_start(args,filename_fmt);
  n=_vscprintf(filename_fmt,args)+1;
  TRY(n<=countof(buf));
  vsprintf(buf,filename_fmt,args);
  va_end(args);

  NEW(struct _timestream_t,self,1);
  ZERO(struct _timestream_t,self,1);
  TRY(self->fp=fopen(buf,"wb"));
  LOG("--- [TIMESTREAM] Opened %s\n",buf);
  self->timer=tic();
  return self;
Error:
  if(self) timestream_close(self);
  return 0;
}

void timestream_tic(timestream_t self)
{ if(self) toc(&self->timer);
}

void timestream_toc(timestream_t self)
{ if(!self) return;
  self->history[self->n++]=toc(&self->timer);
  if(self->n==countof(self->history))
    timestream_flush(self);
}


