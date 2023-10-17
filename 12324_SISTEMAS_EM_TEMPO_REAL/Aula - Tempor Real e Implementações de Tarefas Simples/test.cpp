#include<ctime>
#include<cstdio>
using namespace std;

int foo(){
  printf("in the foo()\n");
}

int main(){
  /* timespec is a struct defined in ctime as
   * struct timespec {
   *   time_t   tv_sec;        // seconds 
   *   long     tv_nsec;       // nanoseconds
   * };
   */
  timespec start, end;
  /*
   * the first argument to clock_gettime specifies
   * which clock to read from.
   *
   * the second arugment to clock_gettime is a pointer
   * to a timespec struct. The fields of the timespec
   * struct are modified to return the clock time.
   */
  clock_gettime(CLOCK_REALTIME, &start); // get the start time
  foo();
  clock_gettime(CLOCK_REALTIME, &end); // get the finishing time
  long delta_nsec = end.tv_nsec - start.tv_nsec;
  printf("foo() execution time: %ld ns\n", delta_nsec);
}