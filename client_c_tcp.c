/*
  I haven't strictly copied any code from the internet, but I used one specific article/reference
  on geeksforgeeks.org for the TCP client/server programs. I have done socket programming before
  so I used it mainly to remind myself the syntax and some smaller specific things, like the more
  standard variable naming a lot of people use for socket programming.

  Here is the link: https://www.geeksforgeeks.org/socket-programming-cc/.
*/

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

#define BUFFERSIZE 130

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

  if ((Sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  memset(&ServAddr, 0, sizeof(ServAddr));

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

  while(1)
  {
    memset(Buffer, 0, BUFFERSIZE);

    if (recv(Sockfd, (char *)& InMsgLength, sizeof(InMsgLength), 0) == 0)
    {
      break;
    }

    InMsgLength = ntohl(InMsgLength);

    if (recv(Sockfd, Buffer, InMsgLength, 0) == 0)
    {
      break;
    }

    printf("From Server: %s\n", Buffer);
  }

  return 0;
}
