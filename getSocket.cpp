#include "getSocket.h"

WSADATA wsaData;
SOCKET wsaSocket;
SOCKADDR_IN servadr;
int buflen = 0;
int blen = sizeof(buflen);

int initSocket() {
	return WSAStartup(MAKEWORD(2, 2), &wsaData);
}

SOCKET creatSocket(int port) {
	wsaSocket = socket(PF_INET, SOCK_DGRAM, 0);
	memset(&servadr, 0, sizeof(servadr));
	servadr.sin_addr.s_addr = htonl(INADDR_ANY);
	servadr.sin_port = htons(port);
	servadr.sin_family = AF_INET;

	getsockopt(wsaSocket, SOL_SOCKET, SO_RCVBUF, (char *)&buflen, &blen);
	buflen = buflen * 10;
	setsockopt(wsaSocket, SOL_SOCKET, SO_RCVBUF, (char*)&buflen, blen);

	if (bind(wsaSocket, (struct sockaddr*)&servadr, sizeof(servadr)) == SOCKET_ERROR) {
		return -1;
	}
	return wsaSocket;
}