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
    //����Socket
    if ((sock = creatSocket(port)) == -1) {
        printf("initSocket error");
        exit(1);
    }
    //��ȡ�����߳�
    recvThread = std::thread(recvMessage,sock);
    //����
    myplayer *player = new myplayer();
    playerThread = std::thread(doshow, player);

    printf("init success\n");

    //�ȴ�����
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