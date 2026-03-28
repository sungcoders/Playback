#include "PlaybackPlayer.h"
#include <iostream>
#include <windows.h>

PlaybackPlayer::PlaybackPlayer()
: m_bIsExit(true)
, m_pCdemux(nullptr)
, m_pCFrame(nullptr)
{
    
}

PlaybackPlayer::~PlaybackPlayer()
{
    // inputThread.join();
}

void PlaybackPlayer::fwdFrameOut(std::shared_ptr<PlaybackFrame> MediaFrame)
{
    LOGI("Received frame in PlaybackPlayer");
    m_pCFrame = MediaFrame;
}

void PlaybackPlayer::start()
{
    m_pCdemux = std::make_unique<PlaybackDemux>();
    if (m_pCdemux == nullptr)
    {
        LOGE("Failed to create PlaybackDemux");
        return;
    }
    m_pCdemux->Init("D:/SLV/WorkSpace/App/Playback/Media/video.mp4");
    m_pCdemux->Start();

    // inputThread = std::thread(&PlaybackPlayer::inputLoop, this);
    // outPutThread = std::thread(&PlaybackPlayer::outPutView, this);
    LOGD("PlaybackPlayer started");
}

void PlaybackPlayer::stop()
{
}

void PlaybackPlayer::inputLoop()
{
    std::string s;
    while (m_bIsExit.load())
    {
        std::getline(std::cin, s);
        if(s == "")
        {
            continue;
        }
        else if(s == "trigger")
        {
            LOGI("trigger event");
        }
        else if(s == "clear")
        {
            LOGI("reset event");
        }
        else if(s == "exit")
        {
            LOGI("Thoat");
            m_bIsExit.store(false);
            return;
        }
        else
        {
            LOGI("input = {}", s);
            LOGI("errno = {}", errno);
        }
    }
    LOGI("data = {}", errno);
}

void PlaybackPlayer::outPutView()
{
    // win.createWindow(1920, 1044);
    while (m_bIsExit.load())
    {
        // win.WindowEvent();
        if(m_pCFrame == nullptr)
        {
            LOGE("Frame queue is not initialized");
            Sleep(500);
            continue;
        }
        FrameInfo sFrame = {};
        m_pCFrame->pop(sFrame);

        LOGE("Display frame video: {:.3f}s", sFrame.timestamp);
        // win.renderFrame(sFrame.frame);
        // win.delay(33);

        av_frame_free(&sFrame.frame);
    }
}

void PlaybackPlayer::SetPlayInfo()
{
    // Code để thiết lập thông tin phát video
}

void PlaybackPlayer::SetConfig()
{
    // Code để thiết lập cấu hình phát video
}

void PlaybackPlayer::PlayStart()
{
    // Code để bắt đầu phát video
}

void PlaybackPlayer::Pause()
{
    // Code để tạm dừng phát video
}

void PlaybackPlayer::PlayStop()
{
    // Code để dừng phát video
}

