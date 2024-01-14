#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <ftw.h>


using namespace std;

char buf[1024] = "";
int main() {

    int sock;
    struct sockaddr_in addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0) {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(3425); // или любой другой порт...
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("connect");
        exit(2);
    }

    while(1) {
        cout << "\nEnter path: ";
        char path[1024];
        cin >> path;
        if (path[0] != '/') break;

        send(sock, path, sizeof(path), 0);
        printf("\nMessage sent\n");
        printf(path);
        recv(sock, buf, sizeof(buf), 0);
        printf("\nMessage is received\n");
        printf(buf);
    }


    close(sock);

    return 0;
}
