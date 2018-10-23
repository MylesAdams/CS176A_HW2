#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define BUFFERSIZE 128

int main(int argc, char **argv)
{
  if (argc < 3)
  {
    perror("Invalid number of arguments");
    exit(EXIT_FAILURE);
  }

  printf("hello");

  int Sockfd;
  char Buffer[BUFFERSIZE];

  struct sockaddr_in ServAddr;

  int Ip = *argv[1];
  int Port = *argv[2];

  // Creating socket file descriptor
  if ((Sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
  {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  memset(&ServAddr, 0, sizeof(ServAddr));

  // Filling server information
  ServAddr.sin_family = AF_INET;
  ServAddr.sin_port = htons(Port);
  ServAddr.sin_addr.s_addr = INADDR_ANY;

  socklen_t len;
  int n;

  fgets(Buffer, BUFFERSIZE, stdin);

  sendto(
      Sockfd,
      (const char *)Buffer,
      strlen(Buffer),
      0,
      (const struct sockaddr *) &ServAddr,
      sizeof(ServAddr));

  printf("Hello message sent.\n");
}
