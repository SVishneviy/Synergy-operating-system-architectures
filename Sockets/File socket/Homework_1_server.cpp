#include <iostream>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>

#define SOCKET_NAME "/home/svishneviy/Synergy-operating-system-architectures/Sockets/File socket/server.soc"
#define BUF_SIZE 256

using namespace std;

static int sock;

void cleanup_socket(int sock) {
    close(sock);
    unlink(SOCKET_NAME);
}

void sighandler(int signum) {
    cleanup_socket(sock);
    exit(signum);
}

int main() {
    char msg[BUF_SIZE];
    struct sockaddr_un addr;

    sock = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("Socket failed");
        return errno;
    }

    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_NAME, strlen(SOCKET_NAME));
    
    if (bind(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        perror("Bind failed");
        cleanup_socket(sock);
        return errno;
    }

    if (signal(SIGINT, sighandler) == SIG_ERR) {
        perror("Signal failed");
        cleanup_socket(sock);
        return errno;
    }

    while(1) {
        int bytes = recv(sock, msg, sizeof(msg), 0);
        if (bytes < 0) {
            perror("Receiver failed");
            cleanup_socket(sock);
            return errno;
        }
        msg[bytes] = '\0';
        cout << "Got message: " << msg << endl;
    }
    
    cleanup_socket(sock);
    return 0;
}