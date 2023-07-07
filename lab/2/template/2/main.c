// Шаблон для задания 2.2


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "getopt.h"



int main (int argc, char *argv[], char *envp[]) {

	int c;
	while ((c = getopt (argc, argv, "hi:o:l:g:n:f:")) != -1) {
		switch (c) {
			case 'h':
				printf ("Usage: %s -i -o -l -g -n -f\n", argv[0]);
				break;

			case 'i':
				printf ("-i arg: %s\n", optarg);
				break;

			case 'o':
				break;

			case 'l':
				break;

			case 'g':
				break;

			case 'n':
				break;

			case 'f':
				break;

			default:
				break;
			}
		}

}
