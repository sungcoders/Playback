#ifndef PLAYBACKPLAYER_H
#define PLAYBACKPLAYER_H

#include <thread>
#include <atomic>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
}

#include <SDL2/SDL.h>
#include "PlaybackDemux.h"

class PlaybackPlayer {
public:
    PlaybackPlayer();
    ~PlaybackPlayer();

    void start();   // start thread
    void stop();    // stop thread

    void SetConfig();
    void SetPlayInfo();
    void PlayStart();
    void Pause();
    void PlayStop();

    void inputLoop();

private:    
    std::atomic<bool> m_bIsExit;
    std::unique_ptr<PlaybackDemux> m_pCdemux;
    std::thread inputThread;

};


#endif