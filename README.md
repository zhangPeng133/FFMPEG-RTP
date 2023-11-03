# FFMPEG-RTP
基于FFMPEG的RTP收流播放器
简单的demo。Socket线程使用UDP方式收RTP包，ffmpeg通过回调方式获取数据并解码交给SDL渲染。
