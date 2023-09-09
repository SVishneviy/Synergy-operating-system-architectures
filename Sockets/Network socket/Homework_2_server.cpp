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

#define MAX_COUNT_SOCK 10

int main() {
    int sock, max_sock;
    int arr_sock[MAX_COUNT_SOCK] = {0};
    fd_set set;
    char buf[BUFSIZ];
    struct sockaddr_in addr;

    setbuf(stdout, NULL);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    max_sock = sock;

    if (sock < 0) {
        perror("Socket failed");
        return errno;
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if (bind(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        perror("Bind failed");
        return errno;
    }

    if (listen(sock, 1) < 0) {
        perror("Listen failed");
        return errno;
    }

    sprintf(buf, "\nPID server: %d\n", getpid());

    cout << "Server it works!" << endl;

    while (true) {
    
        // Очищает набор
        FD_ZERO(&set);
        // Добавляет sock к набору
        FD_SET(sock, &set);

        max_sock = sock;
        
        for (int i = 0; i < MAX_COUNT_SOCK; i++) {
            if (arr_sock[i] > 0) {
                // Добавляем в набор
                FD_SET(arr_sock[i], &set);
                    max_sock = arr_sock[i] > max_sock ? arr_sock[i] : max_sock;
            }
        }
    
        if(select(max_sock + 1, &set, NULL, NULL, NULL) < 0) {
            perror("Select failed");
            return errno;
        }
        
        // Проверяем на новое подключение
        if (FD_ISSET(sock, &set)) {
            cout << "---New connection---" << endl;
            int new_sock = accept(sock, NULL, NULL);
            
            if (new_sock < 0) {
                perror("Accept failed");
                return errno;
            }

            int flag = 1;

            for (int i = 0; i < MAX_COUNT_SOCK && flag; i++) {
                if(arr_sock[i] == 0) {
                    arr_sock[i] = new_sock;
                    cout << "Client with number " << i << endl;
                    flag = 0;
                }
            }

            if (flag) {
                cout << "No more space for new clients" << endl;
            }
        }

        // Проверяем, отправили ли клиенты сообшения
        for (int i = 0; i < MAX_COUNT_SOCK; i++) {
            if (arr_sock[i] && FD_ISSET(arr_sock[i], &set)) {
                char buf_client[MAX_LEN_BUF];

                if (recv(arr_sock[i], buf_client, sizeof(buf_client), 0) == 0) {
                    cout << "Disconnect client with number " << i << endl;
                    close(arr_sock[i]);
                    arr_sock[i] = 0;
                } else {
                    cout << "Client with number " << i << " sent: " << buf_client << endl;
    
                    if (send(arr_sock[i], buf, strlen(buf) + 1, 0) < 0) {
                        perror("Send failed");
                        return errno;
                    }
                }
            }
        }   
    }
    close(sock);
    return 0;
}