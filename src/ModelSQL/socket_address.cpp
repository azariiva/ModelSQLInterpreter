#include "../../headers/ModelSQL/socket_address.hpp"
#include "../../headers/ModelSQL/exception.hpp"

#include <cstdlib>
#include <arpa/inet.h>

using namespace ModelSQL;

SocketAddress::SocketAddress() : m_pAddr(NULL) {};

SocketAddress::~SocketAddress() {
    if (m_pAddr != NULL)
        free(m_pAddr);
}

SocketAddress::operator t_sockaddr *() {
    return m_pAddr;
}

UnSocketAddress::UnSocketAddress(const char *SockName) : SocketAddress() {
    m_pAddr = (t_sockaddr*)malloc(sizeof(t_sockaddr_un));
    if (m_pAddr == NULL)
        throw SocketAddressException(SocketAddressException::ESA_BADALLOC);
    t_sockaddr_un *sa = (t_sockaddr_un *)m_pAddr;
    sa->sun_family = AF_UNIX;
    strcpy(sa->sun_path, SockName);
}

UnSocketAddress::UnSocketAddress(t_sockaddr_un *sa)
{
    m_pAddr = (t_sockaddr*)malloc(sizeof(t_sockaddr_un));
    if (m_pAddr == NULL)
        throw SocketAddressException(SocketAddressException::ESA_BADALLOC);
    t_sockaddr_un *m_sa = (t_sockaddr_un *)m_pAddr;
    m_sa->sun_family = AF_UNIX;
    strcpy(m_sa->sun_path, sa->sun_path);
}

UnSocketAddress::~UnSocketAddress() {}

int UnSocketAddress::GetLength() const {
    t_sockaddr_un *sa = (t_sockaddr_un *)m_pAddr;
    return sizeof(sa->sun_family) + strlen(sa->sun_path);
}

SocketAddress *UnSocketAddress::Clone() const {
    t_sockaddr_un *sa = (t_sockaddr_un *)m_pAddr;
    return new UnSocketAddress(sa->sun_path);
}

InSocketAddress::InSocketAddress(const char *HostName, short PortNum) : SocketAddress() {
    m_pAddr = (t_sockaddr*)malloc(sizeof(t_sockaddr_in));
    if (m_pAddr == NULL)
        throw SocketAddressException(SocketAddressException::ESA_BADALLOC);
    t_sockaddr_in *sa = (t_sockaddr_in *)m_pAddr;
    sa->sin_family = AF_INET;
    sa->sin_port = PortNum;
    if (inet_aton(HostName, &(sa->sin_addr)) == 0)
        throw SocketAddressException(SocketAddressException::ESA_SOCKHOSTNAME);
}

InSocketAddress::InSocketAddress(t_sockaddr_in *sa)
{
    m_pAddr = (t_sockaddr*)malloc(sizeof(t_sockaddr_in));
    if (m_pAddr == NULL)
        throw SocketAddressException(SocketAddressException::ESA_BADALLOC);
    t_sockaddr_in *m_sa = (t_sockaddr_in *)m_pAddr;
    m_sa->sin_family = AF_INET;
    m_sa->sin_port = sa->sin_port;
    m_sa->sin_addr = sa->sin_addr;
}

InSocketAddress::~InSocketAddress() {}

int InSocketAddress::GetLength() const {
    return sizeof(t_sockaddr_in);
}

SocketAddress *InSocketAddress::Clone() const {
    // inet_ntoa MT-Safe locale
    t_sockaddr_in *sa = (t_sockaddr_in *)m_pAddr;
    return new InSocketAddress(inet_ntoa(sa->sin_addr), sa->sin_port);
}