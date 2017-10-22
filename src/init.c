#include "mgr.h"

int       create_log (mgrcore_t *core)
{
  int     fd;
  int     ret;


  fd = open("mgr.log", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  if (fd < 0)
    dprintf(1, "open(mgr.log) failed and return = %d\n", fd);
  core->log_fd = fd;

  dprintf(core->log_fd, "initializing mgr core with log fd %d\n", core->log_fd);
}

void     init_core (mgrcore_t *core)
{
  char   str[12];

  create_log(core);
  dprintf(2, "Enter number of functions: ");
  read(0, str, 12);
  core->nmb_of_func = atoi(str);
  dprintf(core->log_fd, "number of functions: %d\n", core->nmb_of_func);

  dprintf(2, "Enter timeout in seconds: ");
  read(0, str, 12);
  core->timeout = atoi(str);
  dprintf(core->log_fd, "timeout in seconds: %d\n", core->timeout);

  dprintf(2, "Enter x: ");
  read(0, str, 12);
  core->x = atoi(str);
  dprintf(core->log_fd, "x: %d\n", core->x);


  core->functions = malloc(core->nmb_of_func * sizeof(func_t));
  for (int i = 0; i < core->nmb_of_func; i++)
    core->functions[i] = malloc(sizeof(func_t));
  core->prompt = 1;
}
