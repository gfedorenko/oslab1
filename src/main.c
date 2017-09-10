#include "mgr.h"

int         main()
{
  mgrcore_t manager;

  init_core(&manager);
  exec_funcs_pool(&manager);
  polling_funcs(&manager);
  kill_funcs_pool(&manager);

  return 0;
}
