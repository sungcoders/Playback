#include "UtilsWindow.h"

UtilsWindow::UtilsWindow()
{
}

UtilsWindow::~UtilsWindow()
{
}

void UtilsWindow::drawButton(SDL_Rect& button, ButtonInfo& infor)
{
    button = {info.x, info.y, info.width, info.height};
}

void UtilsWindow::renderButton(SDL_Renderer* renderer, SDL_Rect& buttonRect, bool isPaused)
{
    if (isPaused) SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // {render, r, g, b, a} đỏ
    else          SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255); // {render, r, g, b, a} xanh

    SDL_RenderFillRect(renderer, &buttonRect);
}