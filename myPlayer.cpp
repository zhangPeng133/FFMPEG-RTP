#include "myPlayer.h"

myplayer::myplayer()
{
}

myplayer::~myplayer()
{
	if (avFrame) {
		av_frame_free(&avFrame);
	}

	if (avCodecContext) {
		avcodec_close(avCodecContext);
	}

	if (avCodecParameters) {
		avcodec_parameters_free(&avCodecParameters);
	}

	if (avFormatContext) {
		avformat_close_input(&avFormatContext);
	}

	if (sdl_window) {
		SDL_DestroyWindow(sdl_window);
	}

	if (sdl_renderer) {
		SDL_DestroyRenderer(sdl_renderer);
	}

	if (sdl_texture) {
		SDL_DestroyTexture(sdl_texture);
	}

	SDL_Quit();

}

int myplayer::init() 
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER))
	{
		printf("SDL init error\n");
		return -1;
	}

	//����avFormatContext
	avFormatContext = avformat_alloc_context();
	//�����ڴ�ռ�	
	unsigned char* dataBuffer = (unsigned char*)av_malloc(1024 * 600);
	//����avioContext
	avioContext=avio_alloc_context(dataBuffer, sizeof(dataBuffer), 0, NULL, ffmpegGetData, NULL, NULL);
	//̽��������ȡ������ʽ��
	Sleep(1000);
	int flag = av_probe_input_buffer2(avioContext, (const AVInputFormat**)&avInputFormat, "", NULL, 0, 0);
	if ( flag < 0)
	{
		printf("av_probe_input_buffer error\n");
		return -1;
	}
	//����avioContext
	avFormatContext->pb = avioContext;

	//����
	if (avformat_open_input(&avFormatContext, "", avInputFormat, NULL) != 0)
	{
		printf("avformat_open_input error\n");
		return -1;
	}
	//��ȡ����Ϣ
	if (avformat_find_stream_info(avFormatContext, NULL) < 0)
	{
		printf("avformat_find_stream_info error\n");
		return -1;
	}
	//��ȡ��Ƶ��
	if ((videoStream=av_find_best_stream(avFormatContext, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0))==-1)
	{
		printf("av_find_best_stream error\n");
		return -1;
	}
	//��ȡ������Ϣ
	avCodecParameters=avFormatContext->streams[videoStream]->codecpar;
	//��ȡ������
	if ((avCodec = avcodec_find_decoder(avCodecParameters->codec_id)) == NULL)
	{
		printf("avcodec_find_decoder error\n");
		return -1;
	}
	//��ȡ������������
	avCodecContext=avcodec_alloc_context3(avCodec);
	if (avcodec_parameters_to_context(avCodecContext, avCodecParameters) != 0)
	{
		printf("avcodec_parameters_to_context error\n");
		return -1;
	}
	avCodecContext->thread_count = 4;
	//�򿪽�����
	if (avcodec_open2(avCodecContext, avCodec, NULL) < 0)
	{
		printf("avCodecContext error\n");
		return -1;
	}
	//���������֡�ռ�
	avFrame=av_frame_alloc();
	//����SDL����
	if (!(sdl_window = SDL_CreateWindow("Media Player", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE)))
	{
		printf("SDL_CreateWindow error\n");
		return -1;
	}
	//������Ⱦ��
	if (!(sdl_renderer = SDL_CreateRenderer(sdl_window, -1, 0)))
	{
		printf("SDL_CreateRenderer error\n");
		return -1;
	}
	//��������
	if (!(sdl_texture = SDL_CreateTexture(sdl_renderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, avCodecContext->width, avCodecContext->height)))
	{
		printf("SDL_CreateTexture error\n");
		return -1;
	}
}

int myplayer::decodeAndShow()
{
	printf("decodeAndShow start\n");

	//���벢��Ⱦ
	while (av_read_frame(avFormatContext, &avPacket) >= 0) {
		if (avPacket.stream_index == videoStream) 
		{
			ret = avcodec_send_packet(avCodecContext, &avPacket);
			while (avcodec_receive_frame(avCodecContext, avFrame)==0) 
			{
				//ret = avcodec_receive_frame(avCodecContext, avFrame);
				//if (0 == ret) {
					printf("avcodec_receive_frame ok : ret=%d\n", ret);
					SDL_UpdateYUVTexture(sdl_texture, NULL,
						avFrame->data[0], avFrame->linesize[0],
						avFrame->data[1], avFrame->linesize[1],
						avFrame->data[2], avFrame->linesize[2]);
					// ���ô�С
					sdl_rect.x = 0;
					sdl_rect.y = 0;
					sdl_rect.w = width;
					sdl_rect.h = height;
					SDL_RenderClear(sdl_renderer);
					SDL_RenderCopy(sdl_renderer, sdl_texture, NULL, &sdl_rect);
					SDL_RenderPresent(sdl_renderer);
				//}
				//else {
				//	printf("avcodec_receive_frame error : ret=%d\n", ret);
				//	break;
				//}
			}
		}
		av_packet_unref(&avPacket);
		SDL_Event event;
		SDL_PollEvent(&event);
	}
	printf("decodeAndShow end\n");
	return -1;
}