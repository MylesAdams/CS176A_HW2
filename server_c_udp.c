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
  if (argc < 2)
  {
    perror("Invalid number of arguments");
    exit(EXIT_FAILURE);
  }

  int Sockfd;
  char Buffer[BUFFERSIZE];

  struct sockaddr_in ServAddr, CliAddr;

  int Port = strtol(argv[1], (char **)NULL, 10);

  printf("port: %d\n", Port);

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

  socklen_t len;
  int n;

  n = recvfrom(
      Sockfd,
      (char *)Buffer,
      BUFFERSIZE, MSG_WAITALL,
      (struct sockaddr *)& CliAddr,
      &len);

  Buffer[n] ='\0';

  printf("Client says: %s\n", Buffer);

}
