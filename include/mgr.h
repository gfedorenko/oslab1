#include "stdlib.h"
#include "stdio.h"
#include <signal.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>


typedef struct    func_s
{
  int             in;
  int             out;
  int             id;
  int             status;
  int             result;
}                 func_t;

typedef struct    mgrcore_s
{
  int             x;
  int             timeout;
  int             nmb_of_func;
  int             log_fd;
  func_t          **functions;
}                 mgrcore_t;

void             init_core (mgrcore_t *core);
int              exec_funcs_pool(mgrcore_t *core);
int              kill_funcs_pool(mgrcore_t *core);
int              polling_funcs(mgrcore_t *core);
