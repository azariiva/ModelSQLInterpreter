#include <sys/types.h>
#include <sys/socket.hpp>
#include <sys/un.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define ADDRESS "mysocket" // адрес для связи
int main()
{
    char c;
    int i, s, len;
    FILE *fp;
    struct sockaddr_un sa;
    // получаем свой сокет-дескриптор:
    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
    {
        perror("client: socket");
        exit(1);
    }
    // создаем адрес, по которому будем связываться с сервером:
    sa.sun_family = AF_UNIX;
    strcpy(sa.sun_path, ADDRESS);
    // пытаемся связаться с сервером:
    len = sizeof(sa.sun_family) + strlen(sa.sun_path);
    if (connect(s, (struct sockaddr *)&sa, len) < 0)
    {
        perror("client: connect");
        exit(1);
    }
    /*--------------------------------------------- */
    // читаем сообщения сервера, пишем серверу:
    fp = fdopen(s, "r");
    send(s, "client", 7, 0);
    // while ((c = fgetc(fp)) != EOF)
    //     putchar(c);
    close(s);
    exit(0);
    return 0;
}