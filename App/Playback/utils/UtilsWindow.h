#ifndef UTILS_WINDOW_H
#define UTILS_WINDOW_H

#include <windows.h>
#include <SDL2/SDL.h>
#include "PlaybackFrame.h"
#include "UtilsLog.h"

struct ButtonInfo
{
    int x;
    int y;
    int width;
    int height;
};

class UtilsWindow
{
public:
    UtilsWindow();
    ~UtilsWindow();

    void drawButton(SDL_Renderer* renderer, SDL_Rect& buttonRect, bool isPaused);
    void renderButton(SDL_Renderer* renderer, SDL_Rect& buttonRect, bool isPaused);

};

#endif // UTILS_WINDOW_H
