// Пример использования WinSock для подключения.
//
// 


#include <string.h>
#include <fcntl.h>
//#include <winsock.h>
#include <winsock2.h>
#include <stdio.h>




static int ProcessingConnect (SOCKET sock);
static SOCKET ConnectSock (char *host, int port);


int main (unsigned int argc, char *argv[], char *envp[]) {

SOCKET sock;
WSADATA wsaData;
size_t size = sizeof(SOCKADDR_IN);
int port = 80;
char *host = "ya.ru";


    WSAStartup (MAKEWORD (2, 2), &wsaData);

    if (argc > 1) {
        host = argv[1];
        }

    if (argc > 2) {
        port = atoi (argv[2]);
        }

    if (!port) {
        printf ("Error port %d\n", port);
        return 1;
        }

    sock = ConnectSock (host, port);
    if (sock == INVALID_SOCKET) {
        return 1;
        }

    ProcessingConnect (sock);

    closesocket (sock);
    
    return 0;
}



SOCKET ConnectSock (char *host, int port) {

SOCKET sock;
SOCKADDR_IN addr;
unsigned long hostAddr;

    sock = WSASocket (AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
    if (sock == INVALID_SOCKET) {
        return INVALID_SOCKET;
        }
	
    memset (&addr, 0, sizeof(addr));

	addr.sin_family = AF_INET;
    addr.sin_port = htons (port);

    hostAddr = inet_addr (host);
    if (hostAddr == INADDR_NONE) {
        HOSTENT *hst = gethostbyname (host);
        if (hst) {
            hostAddr = **(unsigned long**)hst->h_addr_list;
            }
        }
    addr.sin_addr.s_addr = hostAddr;

    printf ("connect to %s:%d\n", inet_ntoa (addr.sin_addr), port);

	connect (sock, (SOCKADDR *) &addr, sizeof(addr));

    return sock;
}



static int ProcessingConnect (SOCKET sock) {

size_t i = 0;
char buf[1000];
char *info = "read %d bytes:\n";
int size;
char *msg = "GET / HTTP/1.1\r\nHost: ya.ru\r\n\r\n";

    send (sock, msg, strlen(msg), 0);
    size = recv (sock, buf, sizeof(buf)-1, 0);
    if (size > 0) {
        buf[size] = 0;
        printf ("read %d bytes: %s\n", size, buf);
        }

    return 0;
}
