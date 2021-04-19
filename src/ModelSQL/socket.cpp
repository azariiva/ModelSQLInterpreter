#include "../../headers/ModelSQL/socket.hpp"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <vector>

#include <errno.h>

using namespace ModelSQL;

BaseSocket::BaseSocket(int sockfd, SocketAddress *pAddr) : m_Socket(sockfd), m_pAddr(pAddr) {}

BaseSocket::~BaseSocket() {}

void BaseSocket::Write(void *buf, int len) const {
    if (send(m_Socket, buf, len, 0) == -1)
        throw SocketException(SocketException::ESE_SOCKSEND);
}

void BaseSocket::PutChar(int c) const {
    if (send(m_Socket, &c, sizeof(c), 0) == -1)
        throw SocketException(SocketException::ESE_SOCKSEND);
}

void BaseSocket::PutString(const char *str) const {
    if (send(m_Socket, str, strlen(str) * sizeof(*str), 0) == -1)
    {
        std::cout << m_Socket << ' ' << strerror(errno) << std::endl;
        throw SocketException(SocketException::ESE_SOCKSEND);
    }
}

void BaseSocket::PutString(const std::string &s) const {
    if (send(m_Socket, s.c_str(), s.size(), 0) == -1)
        throw SocketException(SocketException::ESE_SOCKSEND);
}

int BaseSocket::Read(void *buf, int len) const {
    ssize_t recv_rv = recv(m_Socket, buf, len * sizeof(char), 0);
    if (recv_rv == -1)
        throw SocketException(SocketException::ESE_SOCKRECV);
    return recv(m_Socket, buf, len, 0);
}

int BaseSocket::GetChar() const {
    int c;
    ssize_t recv_rv = recv(m_Socket, &c, sizeof(char), 0);
    if (recv_rv == -1)
        throw SocketException(SocketException::ESE_SOCKRECV);
    return recv_rv == 0 ? EOF : c;
}

// rewrite this trash
std::string BaseSocket::GetString() const {
    std::string str = "";
    ssize_t bytesRecieved;
    std::vector<char> buffer(MAX_BUF_LENGTH);

    do {
        bytesRecieved = recv(m_Socket, buffer.data(), buffer.size(), 0);
        if (bytesRecieved == -1 ) { 
            throw SocketException(SocketException::ESE_SOCKRECV);
        } else {
            str.append(buffer.cbegin(), buffer.cend());
        }
    } while (bytesRecieved == MAX_BUF_LENGTH);
    return str;
}

int BaseSocket::GetSockDescriptor() const {
    return m_Socket;
}

void BaseSocket::CheckSocket() {
    // IDK what should happen here
}

void ClientSocket::Connect() {
    m_Socket = socket(((t_sockaddr *)*m_pAddr)->sa_family, SOCK_STREAM, 0);
    if (m_Socket < 0)
        throw SocketException(SocketException::ESE_SOCKILLEGAL);
    if (connect(m_Socket, (t_sockaddr *)*m_pAddr, m_pAddr->GetLength()) < 0)
        throw SocketException(SocketException::ESE_SOCKCONN);
}

BaseSocket *ServerSocket::Accept() {
    BaseSocket *pConn;
    int sockfd;
    t_sockaddr name;
    socklen_t namelen;

    switch (((t_sockaddr *)*m_pAddr)->sa_family)
    {
    case AF_UNIX:
        namelen = sizeof(t_sockaddr_un);
        sockfd = accept(m_Socket, &name, &namelen);
        pConn = new BaseSocket(sockfd, new UnSocketAddress((t_sockaddr_un *)&name));
        break;
    case AF_INET:
        namelen = sizeof(t_sockaddr_in);
        sockfd = accept(m_Socket, &name, &namelen);
        pConn = new BaseSocket(sockfd, new InSocketAddress((t_sockaddr_in *)&name));
        break;
    default:
        throw SocketException(SocketException::ESE_SOCKILLEGAL);
    }
    OnAccept(pConn);
    return pConn;
}

void ServerSocket::Bind() {
    if (((t_sockaddr *)*m_pAddr)->sa_family == AF_UNIX)
        unlink(((t_sockaddr_un *)(((t_sockaddr *)*m_pAddr)))->sun_path);
    if (bind(m_Socket, (t_sockaddr *)*m_pAddr, m_pAddr->GetLength()) < 0)
        throw SocketException(SocketException::ESE_SOCKBIND);
}

void ServerSocket::Listen(int BackLog) {
    if (listen(m_Socket, BackLog) < 0)
        throw SocketException(SocketException::ESE_SOCKLISTEN);
}

UnClientSocket::UnClientSocket(const char *Address) {
    m_pAddr = new UnSocketAddress(Address);
}

InClientSocket::InClientSocket(const char *HostName, short PortNum) {
    m_pAddr = new InSocketAddress(HostName, PortNum);
}

UnServerSocket::UnServerSocket(const char *Address) {
    m_pAddr = new UnSocketAddress(Address);
    m_Socket = socket(AF_UNIX, SOCK_STREAM, 0);
    Bind();
    Listen(3);
}

InServerSocket::InServerSocket(short PortNum) {
    m_pAddr = new InSocketAddress("localhost", PortNum);
    m_Socket = socket(AF_INET, SOCK_STREAM, 0);
    Bind();
    Listen(3);
}