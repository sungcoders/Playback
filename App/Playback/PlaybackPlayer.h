#ifndef PLAYBACKPLAYER_H
#define PLAYBACKPLAYER_H

#include <thread>
#include <atomic>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
}

#include "PlaybackDemux.h"
#include "PlaybackDecodeVideo.h"
#include "PlaybackOutputVideo.h"
#include "PlaybackWindow.h"

class PlaybackPlayer
{
public:
    PlaybackPlayer();
    ~PlaybackPlayer();

    void start();
    void SetConfig();
    void SetPlayInfo();
    void PlayStart(std::string filename);
    void Pause();
    void PlayStop();

    void outPutView();
    void handleEvent();

private:    
    std::atomic<bool> m_bIsExit;
    std::unique_ptr<PlaybackDemux> m_pCdemux;
    std::shared_ptr<PlaybackDecodeVideo> m_pCdecode;
    std::shared_ptr<PlaybackFrame> m_pCFrame;
    std::thread inputThread;
    std::thread outPutThread;
    PlaybackWindow win;

};


#endif