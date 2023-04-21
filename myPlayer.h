#pragma once
#pragma comment(lib, "ws2_32.lib")
#include <String.h>
#include "dataHandle.h"
extern "C"
{
#include <SDL.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

class myplayer 
{
public:
	myplayer();
	~myplayer();
	int init();
	int decodeAndShow();
private:
	int ret = -1;
	int videoStream = -1;
	int width = 648;
	int height = 480;
	AVIOContext* avioContext;
	const AVInputFormat* avInputFormat;
	AVFormatContext* avFormatContext;
	AVCodecParameters* avCodecParameters;
	AVCodecContext* avCodecContext;
	const AVCodec* avCodec;
	AVFrame* avFrame;
	AVPacket avPacket;

	SDL_Rect sdl_rect;
	SDL_Window* sdl_window;
	SDL_Renderer* sdl_renderer;
	SDL_Texture* sdl_texture;
};