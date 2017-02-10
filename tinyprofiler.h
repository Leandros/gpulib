#pragma once

// License: Public Domain

#ifdef USE_TINYPROFILER

#include <sys/time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct {
  int i;
  struct {
    char ph;
    int pid;
    int tid;
    unsigned long ts;
    char name[100];
  } * s;
  size_t sample_count;
} _prof_data[/*PROF_MAX_NUM_OF_THREADS*/4] = {};

unsigned long _prof_tv_sec_start = 0;

static inline unsigned long _prof_time()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return 1000000UL * tv.tv_sec + tv.tv_usec;
}

static inline void _prof(int thread_id, char ph, unsigned long ts, int pid, int tid, int size, const char * name)
{
  int ti = thread_id;
  int i = _prof_data[ti].i;
  _prof_data[ti].s[i].ph = ph;
  _prof_data[ti].s[i].pid = pid;
  _prof_data[ti].s[i].tid = tid;
  _prof_data[ti].s[i].ts = ts;
  memcpy(_prof_data[ti].s[i].name, name, (size_t)size);
  _prof_data[ti].i += 1;
}

static inline void profAlloc(size_t sample_count_per_thread)
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  _prof_tv_sec_start = tv.tv_sec;
  size_t sample_struct_bytes = sizeof(struct {
    char ph;
    int pid;
    int tid;
    unsigned long ts;
    char name[100];
  });
  for (int t = 0; t < /*PROF_MAX_NUM_OF_THREADS*/4; t++)
  {
    _prof_data[t].sample_count = sample_count_per_thread;
    _prof_data[t].s = calloc(sample_count_per_thread, sample_struct_bytes);
  }
}

static inline void profPrintAndFree()
{
  unsigned long self_t_begin = _prof_time();
  fprintf(stderr, "{\"traceEvents\":[{}\n");
  for (int t = 0; t < /*PROF_MAX_NUM_OF_THREADS*/4; t++)
  {
    for (size_t i = 0; i < _prof_data[t].sample_count; i++)
    {
      if (_prof_data[t].s[i].ph)
        fprintf(stderr, ",{\"ph\":\"%c\",\"ts\":%zu,\"pid\":%d,\"tid\":%d,\"name\":\"%s\"}\n",
                _prof_data[t].s[i].ph, _prof_data[t].s[i].ts - 1000000UL * _prof_tv_sec_start,
                _prof_data[t].s[i].pid, _prof_data[t].s[i].tid, _prof_data[t].s[i].name);
      else break;
    }
  }
  for (int t = 0; t < /*PROF_MAX_NUM_OF_THREADS*/4; t++)
    free(_prof_data[t].s);
  fprintf(stderr, ",{\"ph\":\"%c\",\"ts\":%zu,\"pid\":%d,\"tid\":%d,\"name\":\"%s\"}\n", 'B', self_t_begin - 1000000UL * _prof_tv_sec_start, 0, 0, __func__);
  fprintf(stderr, ",{\"ph\":\"%c\",\"ts\":%zu,\"pid\":%d,\"tid\":%d,\"name\":\"%s\"}\n", 'E', _prof_time() - 1000000UL * _prof_tv_sec_start, 0, 0, __func__);
  fprintf(stderr, "]}\n");
}

#define profB(name) _prof(0, 'B', _prof_time(), 0, 0, sizeof(name), name);
#define profE(name) _prof(0, 'E', _prof_time(), 0, 0, sizeof(name), name);
#define profBmt(tid, name) _prof(tid, 'B', _prof_time(), 0, tid, sizeof(name), name);
#define profEmt(tid, name) _prof(tid, 'E', _prof_time(), 0, tid, sizeof(name), name);

#else // USE_TINYPROFILER

#define profAlloc(samples)
#define profPrintAndFree()
#define profB(name)
#define profE(name)
#define profBmt(tid, name)
#define profEmt(tid, name)

#endif // USE_TINYPROFILER
