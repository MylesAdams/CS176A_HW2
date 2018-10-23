#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>

#define BUFFERSIZE 128

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    perror("Invalid number of arguments");
    exit(EXIT_FAILURE);
  }

  int Sockfd, Connfd;
  char InBuffer[BUFFERSIZE];
  char OutBuffer[BUFFERSIZE];

  struct sockaddr_in ServAddr, CliAddr;

  int Port = strtol(argv[1], (char **)NULL, 10);

  if ((Sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    perror("Failed to create socket");
    exit(EXIT_FAILURE);
  }

  memset(&ServAddr, 0, sizeof(ServAddr));
  memset(&CliAddr, 0, sizeof(CliAddr));

  ServAddr.sin_family = AF_INET;
  ServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  ServAddr.sin_port = htons(Port);

  if (setsockopt(Sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0)
  {
    perror("setsockopt(SO_REUSEADDR) failed");
    exit(EXIT_FAILURE);
  }

  if (bind(Sockfd, (const struct sockaddr *)& ServAddr, sizeof(ServAddr)) < 0)
  {
    perror("Failed to bind");
    exit(EXIT_FAILURE);
  }

  if (listen(Sockfd, 3) < 0)
  {
    perror("Listening error");
    exit(EXIT_FAILURE);
  }

  socklen_t len = sizeof(CliAddr);

  if ((Connfd = accept(Sockfd, (struct sockaddr *)&CliAddr,
                           (socklen_t*)&len))<0) 
  {
    perror("Accepting error");
    exit(EXIT_FAILURE);
  }

  char* hello = "Hello there, how is it going as well?\0";

  read(Connfd, InBuffer, sizeof(InBuffer));

  printf("Inbuffer: %s\n", InBuffer);

  write(Connfd, hello, strlen(hello));

  return 0;
}

