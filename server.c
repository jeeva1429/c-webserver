#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <netdb.h>
#include <string.h>
#define MAXMSG  512
// Function to create and bind a socket to a specified port
int make_socket(uint16_t port) {
    int sock;
    struct sockaddr_in name;

    // Create a socket using PF_INET and SOCK_STREAM for TCP
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(1);
    }

    // Set up the address structure
    name.sin_family = AF_INET;
    name.sin_port = htons(port);
    name.sin_addr.s_addr = htonl(INADDR_ANY);

    // Bind the socket to the specified port
    if (bind(sock, (struct sockaddr *) &name, sizeof(name)) < 0) {
        perror("bind");
        exit(1);
    }

    return sock;  // Return the socket descriptor
}

int
read_from_client (int filedes)
{
  char buffer[MAXMSG];
  int nbytes;

  nbytes = read (filedes, buffer, MAXMSG);
  if (nbytes < 0)
    {
      /* Read error. */
      perror ("read");
      exit (EXIT_FAILURE);
    }
  else if (nbytes == 0)
    /* End-of-file. */
    return -1;
  else
    {
      /* Data read. */
      fprintf (stderr, "Server: got message: `%s'\n", buffer);
      return 0;
    }
}

int main() {
    int port = 5000;
    char buffer[24];

    int sock = make_socket(port);  // Call the make_socket function
    // Set the socket to listen for connections
    if (listen(sock, 5) < 0) {
        perror("listen");
        exit(1);
    }
    printf("Server is listening on port %d\n", port);

    // Accept a client connection (this would block until a connection is made)
    int client_fd = accept(sock, NULL, NULL);
    if (client_fd < 0) {
        perror("accept");
        exit(1);
    }

    printf("Client connected.\n");
  // Receive data from the client
    int msg_size;
    int bytes_received;
    int total_bytes = 0;
    bytes_received = recv(client_fd, &msg_size, sizeof(msg_size)-1, 0);

    if (bytes_received <= 0) {
    perror("Failed to receive message size");
    exit(1);
}

    char *msg = malloc(msg_size);
    while(total_bytes < msg_size) {
    bytes_received = recv(client_fd, msg + total_bytes, msg_size - total_bytes, 0);
    if (bytes_received <= 0) {
        exit(1);  // Handle disconnection or error
    }
    total_bytes += bytes_received;
}
    printf("the message is %s", msg);
    free(msg);
    close(client_fd);
    close(sock);

    return 0;
}
