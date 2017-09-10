#include "mgr.h"

void      timeout_reset(struct timeval *timer, int sec)
{
  timer->tv_sec = sec;
  timer->tv_usec = 0;
}

void       find_result(mgrcore_t *core)
{
  int   res = 0;
  for (int i = 0; i < core->nmb_of_func; i++)
  if (res == 0)
    res = core->functions[i]->result;
  else
    res = res | core->functions[i]->result;

  dprintf(2, "res: %d\n", res);
}

int         is_on(mgrcore_t *core)
{
  for (int i = 0; i < core->nmb_of_func; i++)
    if (core->functions[i]->status != 1)
      return 1;
  find_result(core);
  return 0;
}

int         polling_funcs(mgrcore_t *core)
{
  int       ret;
  fd_set    rfds;
  fd_set    wfds;
  int       max = 0;
  struct timeval timeout;

  dprintf(core->log_fd, "polling players\n");

      FD_ZERO(&rfds);
      FD_ZERO(&wfds);
  for (int i = 0; i < core->nmb_of_func; i++)
  {
   dprintf(core->functions[i]->out, "%d\n", core->x);
   dprintf(core->log_fd, "wrote to %d %d\n", core->functions[i]->out, core->x);
   FD_SET(core->functions[i]->in, &rfds);
   if (max < core->functions[i]->in)
    max = core->functions[i]->in;
  }
  while( is_on (core) )
  {
    FD_ZERO(&rfds);
    FD_ZERO(&wfds);
    for (int i = 0; i < core->nmb_of_func; i++)
      if (core->functions[i]->status != 1)
      {
        FD_SET(core->functions[i]->in, &rfds);
        if (max < core->functions[i]->in)
         max = core->functions[i]->in;
      }

    timeout_reset(&timeout, core->timeout);
    ret = select(max + 1, &rfds, &wfds, NULL, &timeout);

    for (int i = 0; i < core->nmb_of_func; i++)
    {
      char str;
      if (FD_ISSET(core->functions[i]->in, &rfds))
      {
        char   temp[12];
        read(core->functions[i]->in, temp , 11 );
        core->functions[i]->result = atoi(temp);

        dprintf(core->log_fd, "%d wrote %d\n", core->functions[i]->in, core->functions[i]->result);
        if (core->functions[i]->result == 0)
        {
          kill_funcs_pool(core);
          exit(EXIT_SUCCESS);
        }
        core->functions[i]->status = 1;
      }
    }

    if(ret == 0)
    {
      for (int i = 0; i < core->nmb_of_func; i++)
        if (core->functions[i]->status != 1)
          dprintf(core->log_fd, "Function #%d didn't answer\n", i);
      dprintf(2, "Functions have been silent for a while. Are you sure you want to continue(y/n): ");
      char ans;
      read(0, &ans, 2);
      if (ans == 'n')
       exit(EXIT_SUCCESS);
    }
   dprintf(core->log_fd, "\n---------\n");
  }
}
