#include "common.hh"
#include "Config.hh"
#include <omp.h>
#include <sched.h>

Config cfg;

int getNumThreads()
{
#ifdef _OPENMP
   cpu_set_t cs;
   CPU_ZERO(&cs);
   sched_getaffinity(0, sizeof(cs), &cs);
   int cnt = 0;
   REP(i, CPU_SETSIZE)
      cnt += CPU_ISSET(i, &cs);
   return cnt;
#else
   return 1;
#endif
}
