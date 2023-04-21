#include "dataHandle.h"
#define MESSAGE_SIZE 200000000
#define RECV_BUF_SIZE 10000

SOCKADDR_IN clntadr;
char recv_buf[RECV_BUF_SIZE];
int recvLen = 0, clntadr_size = sizeof(clntadr);
std::atomic<char> message[MESSAGE_SIZE];
std::atomic_int count = 0;

int ffmpegGetData(void* opaque,uint8_t* buf,int buffsize)
{
	int size_flag = 0;
	if (count >= buffsize)
	{
		memcpy(buf, message, buffsize);		
		count -= buffsize;
		memmove(message, message + buffsize, count);
		size_flag = buffsize;
		printf("ffmpegGetData:%d\n", size_flag);
	}
	return size_flag;
}

void recvMessage(SOCKET socket) 
{
	while (true)
	{	
		recvLen = recvfrom(socket, recv_buf, RECV_BUF_SIZE, 0, (sockaddr*)&clntadr, &clntadr_size);
		memcpy(message+count, recv_buf + 12, recvLen-12);
		count += recvLen - 12;

		//保存到文件
		//recvLen = recvfrom(socket, recv_buf, RECV_BUF_SIZE, 0, (sockaddr*)&clntadr, &clntadr_size);
		//memcpy(len,recv_buf+2,2);
		//printf("seq:%X%X\n",len[0],len[1]);
		//fwrite(recv_buf + 12, 1, recvLen - 12, testfile);
	}
}