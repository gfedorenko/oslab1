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

int        set_fds(mgrcore_t *core, fd_set* rfds, fd_set* wfds)
{
  int max;
  FD_ZERO(rfds);
  FD_ZERO(wfds);
  //FD_SET(0, rfds);
  for (int i = 0; i < core->nmb_of_func; i++)
    if (core->functions[i]->status != 1)
    {
      FD_SET(core->functions[i]->in, rfds);
      if (max < core->functions[i]->in)
       max = core->functions[i]->in;
    }
    return max;
}

int        ask_to_continue()
{
  char     ans;
  
  dprintf(2, "Functions have been silent for a while. Are you sure you want to continue(y/n/w(without prompt)): ");
  read(0, &ans, 1);
  dprintf(2,"resd\n");
  if (ans == 'w')
   return 0;
  if (ans == 'n')
   exit(EXIT_SUCCESS);
  return 1;
}

int         polling_funcs(mgrcore_t *core)
{
  int       ret;
  fd_set    rfds;
  fd_set    wfds;
  int       max = 0;
  struct timeval timeout;

  dprintf(core->log_fd, "polling players\n");

  for (int i = 0; i < core->nmb_of_func; i++)
  {
   dprintf(core->functions[i]->out, "%d\n", core->x);
   dprintf(core->log_fd, "wrote to %d %d\n", core->functions[i]->out, core->x);
  }

  timeout_reset(&timeout, core->timeout);
  while( is_on (core) )
  {
    max = set_fds(core, &rfds, &wfds);
    ret = select(max + 1, &rfds, &wfds, NULL, &timeout);

    // if (FD_ISSET(0, &rfds))
    // {
    //   char ans;
    //   read(0, &ans, 2);
    //   if (ans == 'q')
    //   {
    //     kill_funcs_pool(core);
    //     exit(EXIT_SUCCESS);
    //   }
    // }

    for (int i = 0; i < core->nmb_of_func; i++)
    {
      char str;
      if (FD_ISSET(core->functions[i]->in, &rfds))
      {
        char   temp[12];

        //FD_CLR(core->functions[i]->in, &rfds);
        read(core->functions[i]->in, temp , 11 );
        core->functions[i]->result = atoi(temp);

        dprintf(core->log_fd, "%d wrote %d\n", core->functions[i]->in, core->functions[i]->result);
        if (core->functions[i]->result == 0)
        {
          dprintf(core->log_fd, "result: 0\n");
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
      dprintf(2, "prompt %d\n", core->prompt);
      if (core->prompt)
        core->prompt = ask_to_continue();
      timeout_reset(&timeout, core->timeout);
    }
   dprintf(core->log_fd, "\n---------\n");
  }
  return 0;
}
