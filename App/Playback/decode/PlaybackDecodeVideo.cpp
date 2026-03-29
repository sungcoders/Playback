#include "PlaybackDecodeVideo.h"
#include "PlaybackFrame.h"

PlaybackDecodeVideo::PlaybackDecodeVideo(std::shared_ptr<PlaybackFrame> frame)
: m_pCFrame(frame)
, m_bExit(false)
, m_dTimebase(0.0)
, m_codecCtx(nullptr)
{
}

void PlaybackDecodeVideo::Init(AVCodecContext* codecCtx, double timebase, std::shared_ptr<PlaybackPacket> packet)
{
    this->m_codecCtx = codecCtx;
    this->m_dTimebase = timebase;
    this->m_pCPacket = packet;
    LOGW("Video decode thread started");
}

void PlaybackDecodeVideo::Stop()
{
    m_pCFrame->abortFrame();
    m_bExit.store(true);
}

void PlaybackDecodeVideo::Decode()
{
    LOGW("Starting video decode loop...");
    AVPacket* avpacket = av_packet_alloc();
    AVFrame* avframe = av_frame_alloc();
        
    while(!m_bExit.load())
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
            sFrame.timestamp = avframe->pts * m_dTimebase;
            av_frame_move_ref(sFrame.frame, avframe);
            
            handleEnoughFrame();
            LOGW("[{}]push frame: {:.3f}s size {}", avpacket->stream_index, sFrame.timestamp, m_pCFrame->size());
            m_pCFrame->push(sFrame);
            
            avframe = av_frame_alloc();
        }
        av_packet_unref(avpacket);
    }

    av_packet_free(&avpacket);
    av_frame_free(&avframe);
    LOGE("Decoding process finished");
}

void PlaybackDecodeVideo::handleEnoughFrame()
{
    m_pCFrame->waitFrame();
}
