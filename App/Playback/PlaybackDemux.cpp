#include "PlaybackDemux.h"
#include "PlaybackWindow.h"
#include "UtilsLog.h"

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

