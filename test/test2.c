#include "mgr.h"

int main()
{
  mgrcore_t     manager;

  manager.nmb_of_func = 2;
  dprintf(manager.log_fd, "number of functions: %d\n", manager.nmb_of_func);
  manager.timeout = 10;
  dprintf(manager.log_fd, "timeout in seconds: %d\n", manager.timeout);
  manager.x = 20;
  dprintf(manager.log_fd, "x: %d\n", manager.x);

  manager.functions = malloc(manager.nmb_of_func * sizeof(func_t));
  for (int i = 0; i < manager.nmb_of_func; i++)
    manager.functions[i] = malloc(sizeof(func_t));

  exec_funcs_pool(&manager);
  polling_funcs(&manager);
  kill_funcs_pool(&manager);
}
