#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>

#define CRLF "\r\n"


int parse(char *ENTREE, char *SORTIE, char *SORTIEEX, char *SORTIEEXACTE, char *SORTIEEEX);
