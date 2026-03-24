#include "PlaybackDemux.h"
#include "PlaybackWindow.h"

void PlaybackDemux::Start()
{
    LOGD("Starting demuxing process...");
    demuxThread = std::thread(&PlaybackDemux::Demux, this);
}

void PlaybackDemux::Init(const std::string& filename)
{
    avformat_network_init();
    if (avformat_open_input(&fmtCtx, filename.c_str(), nullptr, nullptr) != 0)
    {
        LOGE("Không mở được file");
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
        LOGE("Không tìm thấy video stream");
        return;
    }

    codecPar = fmtCtx->streams[videoStream]->codecpar;
    codec = avcodec_find_decoder(codecPar->codec_id);
    codecCtx = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(codecCtx, codecPar);
    avcodec_open2(codecCtx, codec, nullptr);
    width = codecCtx->width;
    height = codecCtx->height;
    av_dump_format(fmtCtx, 0, filename.c_str(), 0);
}

void PlaybackDemux::Demux()
{
    Init("video.mp4");
    PlaybackDecodeVideo decodeVideo(packetVideoQueue);
    decodeVideo.Init(codecCtx, fmtCtx);

    while (av_read_frame(fmtCtx, packet) >= 0)
    {
        if (packet->stream_index == videoStream)
        {
            LOGD("[{}] push packet: {} size {}", packet->stream_index, av_q2d(fmtCtx->streams[packet->stream_index]->time_base) * packet->pts, packetVideoQueue->size());
            packetVideoQueue->push(packet);
            av_packet_unref(packet);
        }
    }

    av_packet_free(&packet);
    avcodec_free_context(&codecCtx);
    avformat_close_input(&fmtCtx);
    LOGE("Demuxing process finished");
}
