#include <iostream>
#include "sock_wrap.hpp"
#include <errno.h>

const char *address = "mysocket";

int main(int argc, char *argv[])
{
    try {
        ModelSQL::UnClientSocket sock(address);
        sock.Connect();
        sock.PutString("Hello from client!");
        std::cout << "Read from server: " << sock.GetString() << '\n';
    } catch (ModelSQL::Exception &e) {
        e.Report();
    }
    return 0;
}