#include "PlaybackDemux.h"
#include <iostream>
#include "PlaybackWindow.h"

PlaybackDemux::PlaybackDemux()
{
}

PlaybackDemux::~PlaybackDemux()
{
    if (demuxThread.joinable()) {
        demuxThread.join();
    }
}
void PlaybackDemux::Start()
{
    std::cout << "Starting demuxing process...\n";
    demuxThread = std::thread(&PlaybackDemux::Demux, this);
}

void PlaybackDemux::Init(const std::string& filename)
{
    avformat_network_init();
    if (avformat_open_input(&fmtCtx, filename.c_str(), nullptr, nullptr) != 0)
    {
        std::cout << "Không mở được file\n";
        return;
    }
    avformat_find_stream_info(fmtCtx, nullptr);

    for (unsigned int i = 0; i < fmtCtx->nb_streams; i++)
    {
        if (fmtCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            videoStream = i;
            break;
        }
    }
    
    if (videoStream == -1) {
        std::cout << "Không tìm thấy video stream\n";
        return;
    }

    codecPar = fmtCtx->streams[videoStream]->codecpar;
    codec = avcodec_find_decoder(codecPar->codec_id);
    codecCtx = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(codecCtx, codecPar);
    avcodec_open2(codecCtx, codec, nullptr);
    width = codecCtx->width;
    height = codecCtx->height;
    std::cout << "Video stream index: " << videoStream << "\n";
    std::cout << "Codec ID: " << codecPar->codec_id << "\n";
    std::cout << "Video resolution: " << width << "x" << height << "\n";
}

void PlaybackDemux::Demux()
{
    Init("video.mp4");
    PlaybackWindow win;
    win.createWindow(width, height);
    
    while (av_read_frame(fmtCtx, packet) >= 0)
    {
        if (packet->stream_index == videoStream)
        {
            avcodec_send_packet(codecCtx, packet);
            while (avcodec_receive_frame(codecCtx, frame) == 0)
            {
                win.renderFrame(frame);
                win.delay(33); // ~30 FPS (demo)
            }
        }
        av_packet_unref(packet);
    }
    av_frame_free(&frame);
    av_packet_free(&packet);
    avcodec_free_context(&codecCtx);
    avformat_close_input(&fmtCtx);
}

