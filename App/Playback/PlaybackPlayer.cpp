#include "PlaybackPlayer.h"
#include <iostream>
#include <windows.h>

PlaybackPlayer::PlaybackPlayer()
: m_bIsExit(true)
, m_pCdemux(nullptr)
{
    
}

PlaybackPlayer::~PlaybackPlayer()
{
    // inputThread.join();
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

