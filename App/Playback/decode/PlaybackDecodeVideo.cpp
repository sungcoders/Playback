#include "PlaybackDecodeVideo.h"
#include "PlaybackFrame.h"

PlaybackDecodeVideo::PlaybackDecodeVideo()
: m_pCFrame(nullptr)
, m_fmtCtx(nullptr)
, m_codecCtx(nullptr)
, m_pCOutputVideo(nullptr)
{
    m_pCFrame = std::make_shared<PlaybackFrame>();
    m_pCOutputVideo = std::make_unique<PlaybackOutputVideo>();
}

void PlaybackDecodeVideo::Init(AVCodecContext* codecCtx, AVFormatContext* fmtCtx, std::shared_ptr<PlaybackPacket> packet)
{
    this->m_codecCtx = codecCtx;
    this->m_fmtCtx = fmtCtx;
    this->m_pCPacket = packet;
    LOGW("Video decode thread started");
}

void PlaybackDecodeVideo::Decode()
{
    LOGW("Starting video decode loop...");
    AVPacket* avpacket = av_packet_alloc();
    AVFrame* avframe = av_frame_alloc();
    
    m_pCOutputVideo->Init(m_pCFrame);
    m_pCOutputVideo->Start();
    
    while (true)
    {
        FrameInfo sFrame = {};
        m_pCPacket->pop(avpacket);
        int ret = avcodec_send_packet(m_codecCtx, avpacket);
        if (ret < 0)
        {
            av_packet_unref(avpacket);
            continue;
        }

        while (ret >= 0)
        {
            ret = avcodec_receive_frame(m_codecCtx, avframe);
            if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF || ret < 0)
            {
                break;
            }
            
            sFrame.frame = av_frame_alloc();
            sFrame.timestamp = avframe->pts * av_q2d(m_fmtCtx->streams[avpacket->stream_index]->time_base);
            av_frame_move_ref(sFrame.frame, avframe);
            
            LOGW("[{}]decode frame: {:.3f}s size {}", avpacket->stream_index, sFrame.timestamp, m_pCFrame->size());
            m_pCFrame->push(sFrame);
            
            avframe = av_frame_alloc();
        }
        av_packet_unref(avpacket);
    }

    av_packet_free(&avpacket);
    av_frame_free(&avframe);
}
