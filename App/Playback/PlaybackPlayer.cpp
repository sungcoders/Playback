#include "PlaybackPlayer.h"
#include <iostream>
#include <windows.h>

PlaybackPlayer::PlaybackPlayer()
: running(false)
, keyPressed(0)
, demuxPtr(nullptr)
{
    
}

PlaybackPlayer::~PlaybackPlayer()
{
    stop();
}

void PlaybackPlayer::start()
{
    running = true;
    demuxPtr = std::make_unique<PlaybackDemux>();
    if (demuxPtr != nullptr) {
        demuxPtr->Start();
    }
    inputThread = std::thread(&PlaybackPlayer::inputLoop, this);
    std::cout << "PlaybackPlayer started. Press 'Q' to quit.\n";
}

void PlaybackPlayer::stop()
{
    running = false;
    if (inputThread.joinable()) {
        inputThread.join();
    }
}

void PlaybackPlayer::inputLoop()
{
    while (running) {
        for (int i = 0; i < 256; i++) {
            if (GetAsyncKeyState(i) & 0x8000) {
                keyPressed = (char)i;
                Sleep(200);
            }
        }
    }
}

void PlaybackPlayer::update()
{
    if (keyPressed != 0) {
        char k = keyPressed;
        keyPressed = 0;

        std::cout << "Nhan phim: " << k << std::endl;

        if (k == 'q' || k == 'Q') {
            std::cout << "Thoat...\n";
            stop();
        }
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

