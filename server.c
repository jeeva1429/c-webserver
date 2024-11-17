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
#include <pthread.h>


// Function to create and bind a socket to a specified port
int* make_socket(uint16_t port) {
    int *sockptr;
    struct sockaddr_in name;
    socklen_t add_len = sizeof(name);

    // Create a socket using PF_INET and SOCK_STREAM for TCP
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    sockptr = &sock;
    if (sock < 0) {
        perror("socket");
        exit(1);
    }

    // Set up the address structure
    name.sin_family = AF_INET;
    name.sin_port = htons(port);
    name.sin_addr.s_addr = htonl(INADDR_ANY);

    // Bind the socket to the specified port
    if (bind(sock, (struct sockaddr *) &name, sizeof(name))) {
        perror("bind");
        exit(1);
    }

    return sockptr;  // Return the socket descriptor
}

  struct sock_data {
        int* sock_id;
        int clientCount;
    };

void* handle_client(void *arg) {
    int client_fd = (int)(intptr_t)arg;
    if(client_fd < 0) {
        perror("Error in connecting to server");
        return NULL;
    }

    char data_to_client[] = "Hello from server";
    char buffer[1024];
    
    ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer));
    if (bytes_read < 0) {
        perror("Error reading from socket");
        close(client_fd);
        return NULL;
    }
    printf("Received message from client: %s\n", buffer);

    ssize_t bytes_sent = send(client_fd, data_to_client, sizeof(data_to_client), 0);
    if (bytes_sent < 0) {
        perror("Error sending to client");
        close(client_fd);
        return NULL;
    }

    close(client_fd); // Close the client socket after communication is done
    return NULL;
}


struct client_data {
    int client_fd;

};
int main() {
    int port = 5000;
    int client_sock;
    int *sock_fd = make_socket(port);  
    struct sockaddr_in client_add;
    socklen_t addr_len = sizeof(client_add);
    int threads_count = 0;
    // Set the socket to listen for connections
    if (listen(*sock_fd, 5) < 0) {
        perror("requests limit reached");
        exit(1);
    }
    printf("Server is listening on port %d\n", port);

    while (1)
    // infinite loop to accepts client connections.
    {
        int client_fd = accept(*sock_fd, (struct sockaddr*)&client_add, &addr_len); 
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, &handle_client, (void*)(intptr_t)client_fd);
        threads_count++;
        pthread_detach(thread_id);

    }

    close(*sock_fd);
    return 0;
   
}



