#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include <string>
#include <iostream>

namespace ModelSQL
{
    // Exception --- класс исключений, генерируемых библиотекой
    class Exception
    {
    public:
        Exception(int errcode) : m_ErrCode(errcode) {}
        void Report() {std::cerr << "Get error with errcode: " << m_ErrCode << '\n';}
        virtual std::string GetMessage() = 0;

    protected:
        int m_ErrCode;
    };

    // SocketException --- класс исключений
    class SocketException : public Exception
    {
    public:
        enum SocketExceptionCode
        {
            ESE_SUCCESS,
            ESE_SOCKCREATE, // ?
            ESE_SOCKCONN, // in place
            ESE_SOCKILLEGAL, // in place ?
            ESE_SOCKSEND, // in place
            ESE_SOCKRECV, // in place
            ESE_SOCKBIND, // in place
            ESE_SOCKLISTEN, // in place
            ESE_SOCKACCEPT, // in place
        };
        SocketException(SocketExceptionCode errcode) : Exception(errcode) {}
        std::string GetMessage() {return "TODO: GetMessage";}

    private:
        static char *m_Message[];
    };

    // SocketAddressException --- класс исключений
    class SocketAddressException : public Exception 
    {
    public:
        enum SocketAddressCode
        {
            ESA_BADALLOC = 10,
            ESA_SOCKHOSTNAME = 11
        };
        SocketAddressException(SocketAddressCode errcode) : Exception(errcode) {}
        std::string GetMessage() {return "TODO: GetMessage";}
    
    private:
        static char *m_Message[];
    };
}

#endif