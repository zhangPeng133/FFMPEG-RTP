#pragma once
#include <windows.h>
#include <iostream>
#include <mutex>
#include <string.h>
#include <atomic>

int ffmpegGetData(void *opaque,uint8_t* buf,int buffsize);

void recvMessage(SOCKET socket);
