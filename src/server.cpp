#include <iostream>
#include "sock_wrap.hpp"
#include <errno.h>

using namespace std;
using namespace ModelSQL;
const char *address = "mysocket"; // имя сокета

class MyServerSocket : public UnServerSocket
{
public:
    MyServerSocket() : UnServerSocket(address) {}

protected:
    void OnAccept(BaseSocket *pConn)
    {
        // установлено соединение с клиентом, читаем сообщение
        cout << "Read from client: " << pConn->GetString() << endl; // отправляем ответ
        pConn->PutString("Hello from server.");
        // продолжаем диалог с клиентом, пока в этом есть необходимость delete pConn;
    }
};
int main(int argc, char *argv[])
{
    try
    { // создаём серверный сокет
        MyServerSocket sock;
        // слушаем запросы на соединение
        for (;;)
            sock.Accept();
    }
    catch (Exception &e)
    {
        // ошибка --- выводим текст сообщения на экран
        e.Report();
    }
    return 0;
}