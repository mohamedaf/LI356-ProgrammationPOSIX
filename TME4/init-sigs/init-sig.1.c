#define _POSIX_SOURCE 1

#include <stdio.h>
#include <unistd.h>

int main () {
  pause ();                  //attend un signal quelconque 
  printf ("1er signal\n");   //pas d'affichage signal reçu SIGINT
  pause ();
  printf ("2e signal\n");
  return 0;
}
