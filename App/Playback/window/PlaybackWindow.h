#ifndef WINDOW_H
#define WINDOW_H

#include <windows.h>
#include "PlaybackFrame.h"
#include <SDL2/SDL.h>

class PlaybackWindow
{
public:
    PlaybackWindow();
    ~PlaybackWindow();

    void createWindow(int width, int height);
    void renderFrame(AVFrame* frame);
    void delay(int ms);
    void destroyWindow();
    void WindowEvent();
    
private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* texture = nullptr;
    SDL_Event event;
};

#endif // WINDOW_H
