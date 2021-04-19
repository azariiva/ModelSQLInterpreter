#ifndef __SOCKET_ADDRESS__
#define __SOCKET_ADDRESS__

#include <sys/socket.h>
#include <cstddef>
#include <sys/un.h>
#include <netinet/in.h>

namespace ModelSQL
{
    typedef struct sockaddr t_sockaddr;
    typedef struct sockaddr_un t_sockaddr_un;
    typedef struct sockaddr_in t_sockaddr_in;

    // SocketAddress --- базовый абстрактный класс для представления
    // сетевых адресов
    class SocketAddress
    {
    public:
        SocketAddress();
        virtual ~SocketAddress();
        virtual int GetLength() const = 0;
        virtual SocketAddress *Clone() const = 0; // Эта штука заменяет собой copy-constructor
        virtual operator t_sockaddr *();

    protected:
        t_sockaddr *m_pAddr;
    };

    // UnSocketAddress --- представление адреса семейства AF_UNIX
    class UnSocketAddress : public SocketAddress
    {
    public:
        explicit UnSocketAddress(const char *SockName);
        UnSocketAddress(t_sockaddr_un *sa);
        ~UnSocketAddress();
        int GetLength() const;
        SocketAddress *Clone() const;
    };

    // InSocketAddress --- представление адреса семейства AF_INET
    class InSocketAddress : public SocketAddress
    {
    public:
        InSocketAddress(const char *HostName, short PortNum);
        InSocketAddress(t_sockaddr_in *sa);
        ~InSocketAddress();
        int GetLength() const;
        SocketAddress *Clone() const;
    };
}

#endif