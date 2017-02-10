#pragma once

#ifdef USE_TINYPROFILER

struct {
  int i;
  size_t sample_count;
  struct {
    char ph;
    int pid;
    int tid;
    long int ts;
    char name[100];
  } * s;
} _prof_data[/*PROF_MAX_NUM_OF_THREADS*/4] = {};

#include <stdlib.h>
static inline void profAlloc(size_t sample_count_per_thread)
{
  for (int t = 0; t < /*PROF_MAX_NUM_OF_THREADS*/4; t++)
  {
    _prof_data[t].sample_count = sample_count_per_thread;
    size_t sample_struct_bytes = sizeof(struct {
      char ph;
      int pid;
      int tid;
      long int ts;
      char name[100];
    });
    _prof_data[t].s = calloc(sample_count_per_thread, sample_struct_bytes);
  }
}

#include <string.h>
static inline void _prof(int thread_id, char ph, long int ts, int pid, int tid, int size, const char * name)
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

#include <sys/time.h>
static inline long int _prof_time()
{
  static time_t start = 0;
  struct timeval tv;
  gettimeofday(&tv, NULL);
  if (start == 0)
    start = tv.tv_sec;
  tv.tv_sec -= start;
  return 1000000L * tv.tv_sec + tv.tv_usec;
}

#define profB(name) _prof(0, 'B', _prof_time(), 0, 0, sizeof(name), name);
#define profE(name) _prof(0, 'E', _prof_time(), 0, 0, sizeof(name), name);
#define profBmt(tid, name) _prof(tid, 'B', _prof_time(), 0, tid, sizeof(name), name);
#define profEmt(tid, name) _prof(tid, 'E', _prof_time(), 0, tid, sizeof(name), name);

#include <stdio.h>
static inline void profPrintAndFree()
{
  long int self_t_begin = _prof_time();
  fprintf(stderr, "{\"traceEvents\":[{}\n");
  for (int t = 0; t < /*PROF_MAX_NUM_OF_THREADS*/4; t++)
  {
    for (long int i = 0; i < _prof_data[t].sample_count; i++)
    {
      if (_prof_data[t].s[i].ph)
        fprintf(stderr, ",{\"ph\":\"%c\",\"ts\":%zu,\"pid\":%d,\"tid\":%d,\"name\":\"%s\"}\n",
                _prof_data[t].s[i].ph, _prof_data[t].s[i].ts, _prof_data[t].s[i].pid, _prof_data[t].s[i].tid, _prof_data[t].s[i].name);
      else break;
    }
  }
  for (int t = 0; t < /*PROF_MAX_NUM_OF_THREADS*/4; t++)
    free(_prof_data[t].s);
  fprintf(stderr, ",{\"ph\":\"%c\",\"ts\":%zu,\"pid\":%d,\"tid\":%d,\"name\":\"%s\"}\n", 'B', self_t_begin, 0, 0, __func__);
  fprintf(stderr, ",{\"ph\":\"%c\",\"ts\":%zu,\"pid\":%d,\"tid\":%d,\"name\":\"%s\"}\n", 'E', _prof_time(), 0, 0, __func__);
  fprintf(stderr, "]}\n");
}

#else // USE_TINYPROFILER

#define profAlloc(samples)
#define profB(name)
#define profE(name)
#define profBmt(tid, name)
#define profEmt(tid, name)
#define profPrintAndFree()

#endif // USE_TINYPROFILER
