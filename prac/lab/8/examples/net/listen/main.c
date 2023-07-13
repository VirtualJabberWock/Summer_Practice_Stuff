// Пример использования WinSock для ожидания входящих соединений.
//
// 


#include <string.h>
#include <fcntl.h>
//#include <winsock.h>
#include <winsock2.h>
#include <stdio.h>




static int ProcessingRequest (SOCKET sock);
static SOCKET InitSock (int port);


int main (unsigned int argc, char *argv[], char *envp[]) {

SOCKET sock;
SOCKET connectSock;
WSADATA wsaData;
size_t size = sizeof(SOCKADDR_IN);
int port;
SOCKADDR_IN ptr;


    WSAStartup (MAKEWORD (2, 2), &wsaData);

    if (argc <= 1) {
        port = 6666;
        }
    else {
        port = atoi(argv[1]);
        }

    if (!port) {
        printf ("Error port %d\n", port);
        return 1;
        }

    sock = InitSock (port);
    if (sock == INVALID_SOCKET) {
        return 1;
        }

    while ((connectSock = accept (sock, (LPSOCKADDR)&ptr, &size)) != INVALID_SOCKET) {
        
        ProcessingRequest (connectSock);
        
        closesocket (connectSock);
        }

    WSACleanup();
    
    return 0;
}



SOCKET InitSock (int port) {

SOCKET sock;
SOCKADDR_IN addr;

    sock = WSASocket (AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
    if (sock == INVALID_SOCKET) {
        return INVALID_SOCKET;
        }
	
    memset (&addr, 0, sizeof(addr));

	addr.sin_family = AF_INET;
    addr.sin_port = htons (port);
    addr.sin_addr.s_addr = INADDR_ANY;

	if (bind (sock, (LPSOCKADDR)&addr, sizeof (struct sockaddr_in)) == SOCKET_ERROR) {
        puts ("Error bind address");
        closesocket (sock);
        return -1;
        }

    listen (sock, 10);

    return sock;
}



static int ProcessingRequest (SOCKET sock) {

size_t i = 0;
char buf[100];
char *info = "read %d bytes:\n";
char c;
int size;

    size = recv (sock, buf, sizeof(buf)-1, 0);
    if (size > 0) {
        buf[size] = 0;
        printf ("read %d bytes: %s\n", size, buf);

        send (sock, "hello\n", 6, 0);
        send (sock, buf, size, 0);
        }

    return 0;
}
