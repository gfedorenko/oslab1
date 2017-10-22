
#include "stdlib.h"
#include "stdio.h"
#include <unistd.h>
#include "mgr.h"
#include <signal.h>

#define READ_SIDE   (0)
#define WRITE_SIDE  (1)

int          f()
{

  char        str[12];
  int         num;

  read(0, str, 11);
  num = atoi(str);

  switch (num)
  {
    case 1:
    {
      sleep(4);
      dprintf(1, "5\n");
    }
    case 2:
    {
      sleep(10);
      dprintf(1, "5\n");
    }
    case 3:
    {
      sleep(5);
      dprintf(1,"0\n");
    }
    case 4:
    {
      sleep(1000);
    }
    case 5:
    {
      sleep(5);
      dprintf(1,"5\n");
    }
    case 6:
    {
      sleep(1000);
    }
  }
}

int          g()
{

    char        str[12];
    int         num;

    read(0, str, 11);
    num = atoi(str);

    switch (num)
    {
      case 1:
      {
        sleep(10);
        dprintf(1, "3\n");
      }
      case 2:
      {
        sleep(4);
        dprintf(1, "3\n");
      }
      case 3:
      {
        sleep(1000);
      }
      case 4:
      {
        sleep(5);
        dprintf(1,"0\n");
      }
      case 5:
      {
        sleep(1000);
      }
      case 6:
      {
        sleep(5);
        dprintf(1,"5\n");
      }
    }
}

// redirect the input (STDIN to the pipe)
 int      exec_funcc(int read, int write, int func_nmb)
 {
   dup2(read, 0);
   dup2(write, 1);
   if (func_nmb % 2 == 0)
    f();
   else
    g();
 }

int        exec_func(mgrcore_t *core, func_t *func, int func_nmb)
{
  pid_t    cpid;
  int pipe_req[2];
  int pipe_ans[2];

  pipe(pipe_req);
  pipe(pipe_ans);
  cpid = fork();

  if(cpid == -1)
  {
    perror("fork");
    exit(EXIT_FAILURE);
  }
  if (cpid == 0)
  {
    close(pipe_req[WRITE_SIDE]);
    close(pipe_ans[READ_SIDE]);
    dprintf(core->log_fd, "------------- Child process: \n");
    exec_funcc(pipe_req[READ_SIDE], pipe_ans[WRITE_SIDE], func_nmb);
  } else {
    func->id = cpid;
    func->in = pipe_ans[READ_SIDE];
    func->out = pipe_req[WRITE_SIDE];
    close(pipe_req[READ_SIDE]);
    close(pipe_ans[WRITE_SIDE]);
    return(0);
  }
  exit(EXIT_SUCCESS);
}

int     exec_funcs_pool(mgrcore_t *core)
{
  for (int i = 0; i < core->nmb_of_func; i++)
  {
    dprintf(core->log_fd, "exec_func[%d]\n", i);
    exec_func(core, core->functions[i], i);
  }
  dprintf(core->log_fd, "exec finished\n");
  return 0;
}


int kill_funcs_pool(mgrcore_t *core)
{
  for (int i = 0; i < core->nmb_of_func; i++)
  {
    kill(core->functions[i]->id, SIGKILL);
  }
}
