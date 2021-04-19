#include <sys/types.h>
#include <sys/socket.hpp>
#include <sys/un.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define ADDRESS "mysocket"
 // адрес для связи
int main()
{
    char c;
    int i, d, d1, len;
    socklen_t ca_len;
    FILE *fp;
    struct sockaddr_un sa, ca;
    // получаем свой сокет-дескриптор:
    if ((d = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
    {
        perror("client: socket");
        exit(1);
    }
    // создаем адрес, c которым будут связываться клиенты
    sa.sun_family = AF_UNIX;
    strcpy(sa.sun_path, ADDRESS);
    // связываем адрес с сокетом;
    // уничтожаем файл с именем ADDRESS, если он существует,
    // для того, чтобы вызов bind завершился успешно
    unlink(ADDRESS);
    len = sizeof(sa.sun_family) + strlen(sa.sun_path);
    if (bind(d, (struct sockaddr *)&sa, len) < 0)
    {
        perror("server: bind");
        exit(1);
    }
    // слушаем запросы на сокет
    if (listen(d, 5) < 0)
    {
        perror("server: listen");
        exit(1);
    }
    // связываемся с клиентом через неименованный сокет с дескриптором d1:
    ca_len = sizeof ca;
    if ((d1 = accept(d, (struct sockaddr *)&ca, &ca_len)) < 0)
    {
        perror("server: accept");
        exit(1);
    }
    /* ------------------------------------------ */
    // читаем запросы клиента, пишем клиенту:
    fp = fdopen(d1, "r");
    while ((c = fgetc(fp)) != EOF)
        putchar(c);
    // send(d1, "server", 7, 0);
    close(d1);
    exit(0);
    return 0;
}