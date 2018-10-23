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

  char* hello = "Hey there, how's it going\0";

  write(Sockfd , hello , strlen(hello));
  printf("Hello message sent\n");
  int valread = read(Sockfd, Buffer, BUFFERSIZE);

  printf("Buffer: %s\n", Buffer);




  return 0;
}
