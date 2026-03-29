#ifndef PLAYBACKWINDOW_H
#define PLAYBACKWINDOW_H

// #include <windows.h>
// #include <SDL2/SDL.h>
// #include "PlaybackFrame.h"
// #include "UtilsLog.h"
#include "UtilsWindow.h"

class PlaybackWindow
{
public:
    PlaybackWindow();
    ~PlaybackWindow();

    void createWindow(int width, int height);
    void renderFrame(AVFrame* frame);
    void resizeWindow(int width, int height);
    void delay(int ms);
    void destroyWindow();

    void WindowEvent(SDL_Event& eventType);
    
private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* texture = nullptr;
    // SDL_Rect pauseBtn;
    // bool isPaused;
    // int winWidth;
    // int winHeight;
    SDL_Event event;
    std::thread eventThread;
};

#endif // WINDOW_H
