#include <iostream>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>

using namespace std;

#define SOCKET_NAME "/home/svishneviy/Synergy-operating-system-architectures/Sockets/File socket/server.soc"

int main() {
    int sock;
    string msg;
    struct sockaddr_un addr;
    
    sock = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("Socket failed");
        return errno;
    }

    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCKET_NAME);
    printf("sun_path=%s, len=%ld\n", addr.sun_path, strlen(SOCKET_NAME));

    if (connect(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        perror("Connect failed");
        return errno;
    }

    cout << "Input message: ";
    cin >> msg;
    
    if (send(sock, msg.c_str(), msg.length(), 0) < 0) {
        perror("Send failed");
        return errno;
    }

    close(sock);
    return 0;
}