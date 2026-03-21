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


class PlaybackPlayer {
public:
    PlaybackPlayer();
    ~PlaybackPlayer();

    void start();   // start thread
    void stop();    // stop thread
    void update();  // handle input (call in main loop)

    void SetConfig();
    void SetPlayInfo();
    void PlayStart();
    void Pause();
    void PlayStop();

private:
    void inputLoop(); // thread function to check key presses
    void HandlePlayback(); // thread function to handle video playback
    
    std::thread inputThread;
    std::thread mainThread;
    std::atomic<bool> running;
    std::atomic<char> keyPressed;
    
};


#endif