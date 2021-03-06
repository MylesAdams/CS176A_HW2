/*
  I haven't strictly copied any code from the internet, but I used one specific article/reference
  on geeksforgeeks.org for the UDP client/server programs. I have done socket programming before
  so I used it mainly to remind myself the syntax and some smaller specific things, like the more
  standard variable naming a lot of people use for socket programming.

  Here is the link: https://www.geeksforgeeks.org/udp-server-client-implementation-c/.
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

  int Sockfd;
  char InBuffer[BUFFERSIZE];
  char OutBuffer[BUFFERSIZE];

  struct sockaddr_in ServAddr, CliAddr;

  int Port = strtol(argv[1], (char **)NULL, 10);

  if ((Sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
  {
    perror("Failed to create socket");
    exit(EXIT_FAILURE);
  }

  memset(&ServAddr, 0, sizeof(ServAddr));
  memset(&CliAddr, 0, sizeof(CliAddr));

  ServAddr.sin_family = AF_INET;
  ServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  ServAddr.sin_port = htons(Port);

  if (bind(Sockfd, (const struct sockaddr *)& ServAddr, sizeof(ServAddr)) < 0)
  {
    perror("Failed to bind");
    exit(EXIT_FAILURE);
  }

  int MsgLength, CurrentValue, IsValidMessage;
  socklen_t len = sizeof(CliAddr);

  while(1)
  {
    memset(InBuffer, 0, BUFFERSIZE);
    memset(OutBuffer, 0, BUFFERSIZE);

    IsValidMessage = 1;

    MsgLength = recvfrom(
        Sockfd,
        (char *)InBuffer,
        BUFFERSIZE,
        MSG_WAITALL,
        (struct sockaddr *)& CliAddr,
        &len);

    strncpy(OutBuffer, InBuffer, MsgLength);

    for (int i = 0; i < MsgLength - 1; ++i)
    {
      if (!isdigit(InBuffer[i]))
      {
        IsValidMessage = 0;
        break;
      }
    }

    if (IsValidMessage)
    {
      CurrentValue = INT_MAX;

      while (CurrentValue >= 10)
      {
        CurrentValue = 0;
        for (int i = 0; i < strlen(OutBuffer); ++i)
        {
          CurrentValue += OutBuffer[i] - '0';
        }

        snprintf(OutBuffer, BUFFERSIZE, "%d", CurrentValue);

        sendto(
            Sockfd,
            (const char *)OutBuffer,
            strlen(OutBuffer) + 1,
            0,
            (const struct sockaddr *) &CliAddr,
            len);
      }

    }
    else
    {
      snprintf(OutBuffer, BUFFERSIZE, "Sorry, cannot compute!");

      sendto(
          Sockfd,
          (const char *)OutBuffer,
          strlen(OutBuffer) + 1,
          0,
          (const struct sockaddr *) &CliAddr,
          len);
    }
  }
}
