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

  // Creating socket file descriptor
  if ((Sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  memset(&ServAddr, 0, sizeof(ServAddr));

  // Filling server information
  ServAddr.sin_family = AF_INET;
  ServAddr.sin_port = htons(Port);
  ServAddr.sin_addr.s_addr = inet_addr(argv[1]);

  if (connect(Sockfd, (struct sockaddr *)& ServAddr, sizeof(ServAddr)) != 0)
  {
    perror("Failed to connect to server");
    exit(EXIT_FAILURE);
  }

  printf("Enter string: ");
  fgets(Buffer, BUFFERSIZE, stdin);

  Buffer[strcspn(Buffer, "\n")] = 0;

  unsigned long OutMsgLength = strlen(Buffer) + 1;

  OutMsgLength = htonl(OutMsgLength);

  send(Sockfd, (char *)& OutMsgLength, sizeof(OutMsgLength), 0);

  send(Sockfd, Buffer, strlen(Buffer) + 1, 0);

  unsigned long InMsgLength;

  int CurrentVal = INT_MAX;

  while(CurrentVal >= 10)
  {
    memset(Buffer, 0, BUFFERSIZE);

    recv(Sockfd, (char *)& InMsgLength, sizeof(InMsgLength), 0);

    InMsgLength = ntohl(InMsgLength);

    recv(Sockfd, Buffer, InMsgLength, 0);

    printf("From Server: %s\n", Buffer);

    CurrentVal = strtol(Buffer, (char **)NULL, 10);

    for (int i = 0; i < InMsgLength - 1; i++)
    {
      if (!isdigit(Buffer[i]))
      {
        CurrentVal = -1;
      }
    }
  }

  return 0;
}
