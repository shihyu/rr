/* -*- Mode: C; tab-width: 8; c-basic-offset: 2; indent-tabs-mode: nil; -*- */

#include "rrutil.h"

int main(void) {
  pid_t child;
  int status;
  int i;
  /* These setpriority calls may appear to fail but rr will honor them */
  setpriority(PRIO_PROCESS, 0, 1);
  child = fork();
  if (!child) {
    /* Doesn't matter what we do here; we intend for the child to never run */
    return 0;
  }
  setpriority(PRIO_PROCESS, 0, 0);
  /* Hopefully we've raised our priority so the child doesn't run at all. */
  test_assert(0 == kill(child, SIGKILL));
  for (i = 0; i < 200; ++i) {
    test_assert(1 == write(STDOUT_FILENO, ".", 1));
  }
  test_assert(child == wait(&status));
  test_assert(WIFSIGNALED(status));
  test_assert(WTERMSIG(status) == SIGKILL);
  atomic_puts("EXIT-SUCCESS");
  return 0;
}
