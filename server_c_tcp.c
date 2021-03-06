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
#include <ctype.h>
#include <errno.h>
#include <limits.h>

#define BUFFERSIZE 130

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
    perror("Failed to set socket as reusable");
    exit(EXIT_FAILURE);
  }

  if (bind(Sockfd, (const struct sockaddr *)& ServAddr, sizeof(ServAddr)) < 0)
  {
    perror("Failed to bind");
    exit(EXIT_FAILURE);
  }

  socklen_t len = sizeof(CliAddr);

  while(1)
  {
    if (listen(Sockfd, 3) < 0)
    {
      perror("Listening error");
      exit(EXIT_FAILURE);
    }

    if ((Connfd = accept(Sockfd, (struct sockaddr *)&CliAddr, (socklen_t*)&len)) < 0)
    {
      perror("Accepting error");
      exit(EXIT_FAILURE);
    }

    unsigned long InMsgLength;

    recv(Connfd, (char *)& InMsgLength, sizeof(InMsgLength), 0);

    InMsgLength = ntohl(InMsgLength);

    recv(Connfd, InBuffer, InMsgLength, 0);

    InBuffer[InMsgLength] = '\0';

    strncpy(OutBuffer, InBuffer, InMsgLength);

    int IsValidMessage = 1;

    for (int i = 0; i < InMsgLength - 1; ++i)
    {
      if (!isdigit(OutBuffer[i]))
      {
        IsValidMessage = 0;
        break;
      }
    }

    int CurrentValue = INT_MAX;

    unsigned long OutMsgLength;

    if (IsValidMessage)
    {
      while(CurrentValue >= 10)
      {
        CurrentValue = 0;

        for (int i = 0; i < strlen(OutBuffer); ++i)
        {
          CurrentValue += OutBuffer[i] - '0';
        }

        snprintf(OutBuffer, sizeof(CurrentValue), "%d", CurrentValue);

        OutMsgLength = htonl(strlen(OutBuffer) + 1);

        send(Connfd, (char *)&OutMsgLength, sizeof(OutMsgLength), 0);

        send(Connfd, OutBuffer, strlen(OutBuffer) + 1, 0);
      }
    }
    else
    {
      char* InvalidInputMessage = "Sorry cannot compute!\0";

      OutMsgLength = htonl(strlen(InvalidInputMessage) + 1);

      send(Connfd, (char *)& OutMsgLength, sizeof(OutMsgLength), 0);

      send(Connfd, InvalidInputMessage, strlen(InvalidInputMessage) + 1, 0);
    }

    close(Connfd);

  }

  return 0;
}

