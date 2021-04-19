#ifndef __SOCKET_H__
#define __SOCKET_H__

#include "socket_address.hpp"
#include "exception.hpp"

namespace ModelSQL
{
    // BaseSocket --- базовый класс для сокетов
    class BaseSocket
    {
    public:
        explicit BaseSocket(int sockfd = -1, SocketAddress *pAddr = NULL);
        virtual ~BaseSocket();
        void Write(void *buf, int len) const;
        void PutChar(int c) const;
        void PutString(const char *str) const;
        void PutString(const std::string &s) const;
        int Read(void *buf, int len) const;
        int GetChar() const;
        std::string GetString() const;
        int GetSockDescriptor() const;

    protected:
        int m_Socket;
        SocketAddress *m_pAddr;
        void CheckSocket();

    private:
        static const size_t MAX_BUF_LENGTH = 4096;
    };

    // ClientSocket --- базовый класс для клиентских сокетов
    class ClientSocket : public BaseSocket
    {
    public:
        void Connect();
    };
    
    // ServerSocket --- базовый класс для серверных сокетов
    class ServerSocket : public BaseSocket
    {
    public:
        BaseSocket *Accept();

    protected:
        void Bind();
        void Listen(int BackLog);
        virtual void OnAccept(BaseSocket *pConn) {}
    };

    // UnClientSocket --- представление клиентского сокета семейства // AF_UNIX
    class UnClientSocket : public ClientSocket
    {
    public:
        UnClientSocket(const char *Address);
    };

    // InClientSocket --- представление клиентского сокета семейства
    // AF_INET
    class InClientSocket : public ClientSocket
    {
    public:
        InClientSocket(const char *HostName, short PortNum);
    };

    // UnServerSocket --- представление серверного сокета семейства
    // AF_UNIX
    class UnServerSocket : public ServerSocket
    {
    public:
        UnServerSocket(const char *Address);
    };

    // InServerSocket --- представление серверного сокета семейства
    // AF_INET
    class InServerSocket : public ServerSocket
    {
    public:
        InServerSocket(short PortNum);
    };
}

#endif