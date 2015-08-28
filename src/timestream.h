#pragma once
#ifdef __cplusplus
extern "C" {
#endif

typedef struct _timestream_t *timestream_t;

timestream_t timestream_open (const char* filename,...); ///< uses printf-style formatting for filename
void         timestream_close(timestream_t self);
void         timestream_tic  (timestream_t self);
void         timestream_toc  (timestream_t self);

#ifdef __cplusplus
}
#endif