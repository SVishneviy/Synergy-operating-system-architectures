#include <iostream>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>

#include "socket.h"

using namespace std;

int main() {
    int sock;
    int bytes;
    char buf[MAX_LEN_BUF];
    char buf_server[BUFSIZ];
    struct sockaddr_in addr;

    setbuf(stdout, NULL);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0) {
        perror("Socket failed");
        return errno;
    }

    // Устанавливаем соединение. После установки соединения
    // сокет готов к передаче и приему данных
    if (connect(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        perror("Connect failed");
        return errno;
    }

    while (true) {
        cout << "Input message: ";
        cin >> buf;
    
        if (send(sock, buf, strlen(buf) + 1, 0) < 0)
        {
            perror("Send failed");
            return errno;
        }
        cout << "The message has been sent" << endl;
    
        bytes = recv(sock, buf_server, sizeof(buf_server), 0);
        cout << endl << "Server response: " << buf_server << endl;
    }

    close(sock);
    return 0;
}