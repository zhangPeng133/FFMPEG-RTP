#pragma once
#pragma comment(lib, "ws2_32.lib") 
#include <WinSock2.h>
#include <string.h>



int initSocket();

SOCKET creatSocket(int prot);
