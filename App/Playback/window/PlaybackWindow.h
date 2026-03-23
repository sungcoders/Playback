#ifndef PLAYBACKWINDOW_H
#define PLAYBACKWINDOW_H

#include <windows.h>
#include <SDL2/SDL.h>
#include "PlaybackFrame.h"
#include "UtilsLog.h"

class PlaybackWindow
{
public:
    PlaybackWindow();
    ~PlaybackWindow();

    void createWindow(int width, int height);
    void renderFrame(AVFrame* frame);
    void delay(int ms);
    void destroyWindow();
    
private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* texture = nullptr;
    SDL_Event event;

    void WindowEvent();
};

#endif // WINDOW_H
