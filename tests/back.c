#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <debug.h>


int main (int argc, char **argv)
{
  long int i;

  i=0;
  while (i<15)
    {
      printf ("%ld ", i++);
      fflush (stdout);
      sleep(1);
    }
  printf ("\n");

  return EXIT_SUCCESS;
}
