#include "PlaybackDemux.h"
#include "PlaybackWindow.h"

void PlaybackDemux::Start()
{
    UtilsLog::debug("Starting demuxing process...");
    demuxThread = std::thread(&PlaybackDemux::Demux, this);
}

void PlaybackDemux::Init(const std::string& filename)
{
    avformat_network_init();
    if (avformat_open_input(&fmtCtx, filename.c_str(), nullptr, nullptr) != 0)
    {
        UtilsLog::error("Không mở được file");
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
        UtilsLog::error("Không tìm thấy video stream");
        return;
    }

    codecPar = fmtCtx->streams[videoStream]->codecpar;
    codec = avcodec_find_decoder(codecPar->codec_id);
    codecCtx = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(codecCtx, codecPar);
    avcodec_open2(codecCtx, codec, nullptr);
    width = codecCtx->width;
    height = codecCtx->height;
    UtilsLog::debug("Codec name: {}", codec->name);
    UtilsLog::info("Codec id: {}", static_cast<int>(codecPar->codec_id));
    UtilsLog::info("Video resolution: {}x{}", width, height);
    UtilsLog::info("Video resolution: {}x{}", width, height);
}

void PlaybackDemux::Demux()
{
    Init("video.mp4");
    // PlaybackWindow win;
    // win.createWindow(width, height);
    PlaybackDecodeVideo decodeVideo(packetVideoQueue);
    decodeVideo.Init(codecCtx, fmtCtx);

    while (av_read_frame(fmtCtx, packet) >= 0)
    {
        PushPacket(packet);
        // sleep(3);
        if(packetVideoQueue->size() < 2)
        {
            // UtilsLog::info("Video packet queue size: {}", packetVideoQueue->size());
            continue;
        }

        // if (packet->stream_index == videoStream)
        // {
            // AVPacket* packetV = av_packet_alloc();
            // packetVideoQueue->pop(packetV);
            // avcodec_send_packet(codecCtx, packetV);
            // while (avcodec_receive_frame(codecCtx, frame) == 0)
            // {
                // UtilsLog::error("[{}] decode frame: {}", packetV->stream_index, frame->pts * av_q2d(fmtCtx->streams[videoStream]->time_base));
                // win.renderFrame(frame);
                // win.delay(33); // ~30 FPS (demo)
                sleep(1);
            // }
        // }
        // av_packet_unref(packetV);
        av_packet_unref(packet);
    }
    av_frame_free(&frame);
    av_packet_free(&packet);
    avcodec_free_context(&codecCtx);
    avformat_close_input(&fmtCtx);
}

void PlaybackDemux::PushPacket(AVPacket* pkt)
{
    if(packet->stream_index == videoStream)
    {
        packetVideoQueue->push(pkt);
    }
    else
    {
        // UtilsLog::warning("[{}] push packet: {}", packet->stream_index, pkt->pts * av_q2d(fmtCtx->streams[videoStream]->time_base));
        // packetAudioQueue->push(pkt);
        // UtilsLog::warning("[{}] packet queue size: {}", packet->stream_index, packetAudioQueue->size());
        // UtilsLog::warning("push packet");
    }
}

