#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <limits.h>
#include <ctype.h>

#define BUFFERSIZE 128

int main(int argc, char **argv)
{
  if (argc < 3)
  {
    perror("Invalid number of arguments");
    exit(EXIT_FAILURE);
  }

  int Sockfd;
  char Buffer[BUFFERSIZE];

  struct sockaddr_in ServAddr;

  int Port = strtol(argv[2], (char **)NULL, 10);

  if ((Sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
  {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  memset(&ServAddr, 0, sizeof(ServAddr));

  ServAddr.sin_family = AF_INET;
  ServAddr.sin_port = htons(Port);
  ServAddr.sin_addr.s_addr = inet_addr(argv[1]);

  printf("Enter string: ");
  fgets(Buffer, BUFFERSIZE, stdin);

  socklen_t len;
  int n;

  sendto(
      Sockfd,
      (const char *)Buffer,
      strlen(Buffer),
      0,
      (const struct sockaddr *) &ServAddr,
      sizeof(ServAddr));

  int CurrentValue = INT_MAX;

  while(CurrentValue >= 10)
  {
    memset(Buffer, 0, BUFFERSIZE);

    n = recvfrom(
        Sockfd,
        (char *)Buffer,
        BUFFERSIZE,
        MSG_WAITALL,
        (struct sockaddr *)& ServAddr,
        &len);

    Buffer[n] ='\0';

    printf("From server: %s\n", Buffer);

    CurrentValue = strtol(Buffer, (char **)NULL, 10);

    for (int i = 0; i < n; i++)
    {
      if (!isdigit(Buffer[i]))
      {
        CurrentValue = -1;
      }
    }
  }

  return 0;
}
