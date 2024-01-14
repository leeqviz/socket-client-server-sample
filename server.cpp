#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ftw.h>

using namespace std;

static int callback(const char *fpath, //путь и имя к файлу или каталогу
                    const struct stat *sb,
                    int typeflag) { // флаги для файлов

    if (typeflag == FTW_F) { // если обычный файл

        cout << fpath << endl;

        string s = fpath;
        char str[200] = "";
        char buf[200] = "";
        int i = 0;
        int k = 0;
        bool flag = false;
        for (int j = strlen(fpath)-1; j >= 0; j--){
            if (s[j] == '/' && !flag) {
                cout << endl;
                for (int c = j-1; c >= 0; c--){
                    buf[k] = s[c];
                    cout << buf[k];
                    k++;
                }
                flag = true;
            }
        }
        cout << endl;
        for (int j = strlen(fpath)-1; j >= 0; j--){
            if (s[j] == '/') break;
            str[i] = s[j];
            cout << str[i];
            i++;

        }
        int f = 0;
        char buff[200] = "";
        cout << endl;
        for (int h = strlen(buf)-1; h >= 0; h--){
            buff[f] = buf[h];
            //cout << buff[f];
            f++;
        }
        cout << endl << buff << endl;
        cout << endl << str << endl;
        strcat(buff, "/");
        strcat(buff, str);
        cout << endl << buff << endl;

        if (rename (fpath, buff) < 0) {
            cout << "err" << endl;
        }
    }

    return 0;
}

int main() {
    int sock, listener;
    struct sockaddr_in addr;
    char path[1024];
    int bytes_read;

    listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener < 0) {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(3425);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        exit(2);
    }

    listen(listener, 1);

    while(1) {
        printf("Server waiting...\n");
        sock = accept(listener, NULL, NULL);
        if(sock < 0) {
            perror("accept");
            exit(3);
        }

        while(1) {
            bytes_read = recv(sock, path, 1024, 0);
            if(bytes_read <= 0) break;

            printf("Message is received\n");
            ftw(path, callback, 5);
            send(sock, path, bytes_read, 0);
            printf("Message sent\n");
        }

        close(sock);
    }
    return 0;
}