#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <syslog.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <stdbool.h>

#define PORT 9000
#define BACKLOG 10
#define BUFFER_SIZE 1024
#define FILE_PATH "/var/tmp/aesdsocketdata"

int server_socket = -1;
int client_socket = -1;
int file_fd = -1;

void cleanup() {
    if (client_socket != -1) {
        close(client_socket);
        client_socket = -1;
    }
    if (server_socket != -1) {
        close(server_socket);
        server_socket = -1;
    }
    if (file_fd != -1) {
        close(file_fd);
        file_fd = -1;
        remove(FILE_PATH);
    }
}

void signal_handler(int signo) {
    syslog(LOG_INFO, "Caught signal, exiting");
    cleanup();
    exit(0);
}

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_received;
    ssize_t bytes_written;

    // Open the file for appending
    file_fd = open(FILE_PATH, O_CREAT | O_APPEND | O_RDWR, 0644);
    if (file_fd == -1) {
        syslog(LOG_ERR, "Failed to open file: %s", strerror(errno));
        return;
    }

    // Receive data from the client
    while ((bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        // Write received data to the file
        if (write(file_fd, buffer, bytes_received) != bytes_received) {
            syslog(LOG_ERR, "Failed to write to file: %s", strerror(errno));
            break;
        }

        // Check for newline and send the file content back to the client
        if (strchr(buffer, '\n')) {
            lseek(file_fd, 0, SEEK_SET); // Rewind the file
            while ((bytes_written = read(file_fd, buffer, BUFFER_SIZE)) > 0) {
                if (send(client_socket, buffer, bytes_written, 0) != bytes_written) {
                    syslog(LOG_ERR, "Failed to send data to client: %s", strerror(errno));
                    break;
                }
            }
        }
    }

    if (bytes_received == -1) {
        syslog(LOG_ERR, "Failed to receive data: %s", strerror(errno));
    }

    close(file_fd);
    file_fd = -1;
}

int main(int argc, char *argv[]) {
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    bool daemon_mode = false;

    // Parse command-line arguments
    if (argc == 2 && strcmp(argv[1], "-d") == 0) {
        daemon_mode = true;
    }

    // Open syslog
    openlog("aesdsocket", LOG_PID, LOG_USER);

    // Set up signal handlers
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    // Create a socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        syslog(LOG_ERR, "Failed to create socket: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Bind the socket to port 9000
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        syslog(LOG_ERR, "Failed to bind socket: %s", strerror(errno));
        cleanup();
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, BACKLOG) == -1) {
        syslog(LOG_ERR, "Failed to listen on socket: %s", strerror(errno));
        cleanup();
        exit(EXIT_FAILURE);
    }

    // Run as a daemon if requested
    if (daemon_mode) {
        if (daemon(0, 0) == -1) {
            syslog(LOG_ERR, "Failed to run as daemon: %s", strerror(errno));
            cleanup();
            exit(EXIT_FAILURE);
        }
    }

    syslog(LOG_INFO, "Server started on port %d", PORT);

    // Accept and handle connections
    while (1) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket == -1) {
            syslog(LOG_ERR, "Failed to accept connection: %s", strerror(errno));
            continue;
        }

        syslog(LOG_INFO, "Accepted connection from %s", inet_ntoa(client_addr.sin_addr));
        handle_client(client_socket);
        syslog(LOG_INFO, "Closed connection from %s", inet_ntoa(client_addr.sin_addr));

        close(client_socket);
        client_socket = -1;
    }

    cleanup();
    return 0;
}