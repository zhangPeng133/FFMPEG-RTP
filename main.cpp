#include <iostream>
#include <thread>
#include "getSocket.h"
#include "dataHandle.h"
#include "myPlayer.h"

std::thread recvThread;
std::thread playerThread;

void doshow(myplayer* player);
int main(int argc, char* argv[])
{

    int port = 9190;
    SOCKET sock;
    if (initSocket() != 0) {
        printf("initSocket error");
        exit(1);
    }
    //创建Socket
    if ((sock = creatSocket(port)) == -1) {
        printf("initSocket error");
        exit(1);
    }
    //读取数据线程
    recvThread = std::thread(recvMessage,sock);
    //播放
    myplayer *player = new myplayer();
    playerThread = std::thread(doshow, player);

    printf("init success\n");

    //等待结束
    recvThread.join();
    playerThread.join();
    closesocket(sock);
    delete(player);
    return 0;
}

void doshow(myplayer *player)
{
    player->init();
    player->decodeAndShow();
}