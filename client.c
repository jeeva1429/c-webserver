#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

#define PORT            5000
// #define MESSAGE         "Buffer Size: Choose a buffer size that balances between performance and memory usage. Too small a buffer might require more loops, while too large might waste memory"
#define SERVERHOST  "localhost"


// void
// write_to_server (int filedes)
// {
//   int nbytes;

//   nbytes = write (filedes, MESSAGE, strlen (MESSAGE) + 1);
//   if (nbytes < 0)
//     {
//       perror ("write");
//       exit (EXIT_FAILURE);
//     }
// }

void
init_sockaddr (struct sockaddr_in *name,
               const char *hostname,
               uint16_t port)
{
  struct hostent *hostinfo;

  name->sin_family = AF_INET;
  name->sin_port = htons(port);
  hostinfo = gethostbyname (hostname);
  if (hostinfo == NULL)
    {
      fprintf (stderr, "Unknown host %s.\n", hostname);
      exit (EXIT_FAILURE);
    }
  name->sin_addr = *(struct in_addr *) hostinfo->h_addr;
}


int main (int argc, char *argv[])
{
  printf("argument count %d",argc);
  if (argc < 2) {
    perror("to little arguments");
    exit(EXIT_SUCCESS);
  }
void init_sockaddr (struct sockaddr_in *name,const char *hostname,uint16_t port);
  int sock;
  struct sockaddr_in servername;

  /* Create the socket. */
  sock = socket (PF_INET, SOCK_STREAM, 0);
  if (sock < 0)
    {
      perror ("socket (client)");
      exit (EXIT_FAILURE);
    }

  /* Connect to the server. */
  init_sockaddr (&servername, SERVERHOST, PORT);
  if (0 > connect (sock,(struct sockaddr *) &servername, sizeof (servername)))
    {
      perror ("connect (client)");
      exit (EXIT_FAILURE);
    }
printf("the message is %s", argv[1]);
int bytes_sent = send(sock, argv[1], strlen(argv[1]), 0);
if (bytes_sent < 0) {
    perror("send failed");
    exit(EXIT_FAILURE);
}
printf("Sent %d bytes\n", bytes_sent); 


  close (sock);
  exit (EXIT_SUCCESS);
}


