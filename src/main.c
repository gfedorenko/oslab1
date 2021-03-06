#include "mgr.h"

int         main()
{
  mgrcore_t manager;

  init_core(&manager);
  exec_funcs_pool(&manager);
  polling_funcs(&manager);
  kill_funcs_pool(&manager);
  dprintf(2, "finished\n");
  return 0;
}
