#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>

int SDL_RenderFillCircle(SDL_Renderer* renderer, int x, int y, int radius)
{
    int offsetx, offsety, d;
    int status;

    offsetx = 0;
    offsety = radius;
    d = radius - 1;
    status = 0;

    while (offsety >= offsetx) {

        status += SDL_RenderDrawLine(renderer, x - offsety, y + offsetx,
            x + offsety, y + offsetx);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y + offsety,
            x + offsetx, y + offsety);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y - offsety,
            x + offsetx, y - offsety);
        status += SDL_RenderDrawLine(renderer, x - offsety, y - offsetx,
            x + offsety, y - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2 * offsetx) {
            d -= 2 * offsetx + 1;
            offsetx += 1;
        }
        else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
}

void DrawRackets(SDL_Renderer* renderer, int y1, int y2) {
    /* Draws the rackets */
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_Rect racket1 = { 0, y1 - 50, 20, 100 };
    SDL_Rect racket2 = { 480, y2 - 50, 20, 100 };
    SDL_RenderFillRect(renderer, &racket1);
    SDL_RenderFillRect(renderer, &racket2);
}

void DrawBall(SDL_Renderer* renderer, int x, int y) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderFillCircle(renderer, x, y, 30);
}

int AUX_WaitEventTimeoutCount(SDL_Event* evt, Uint32* ms) {
    Uint32 antes = SDL_GetTicks();
    int hadEvent = SDL_WaitEventTimeout(evt, *ms);
    if (hadEvent) {
        int newTimeout = *ms - (SDL_GetTicks() - antes);
        *ms = newTimeout < 0 ? 0 : newTimeout;
    }
    return hadEvent;
}

int main(int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Ex 1.5.3",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        500, 300, SDL_WINDOW_SHOWN
    );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);
    SDL_Event evt;

    /* EXECUÇÃO */
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
    SDL_RenderClear(ren);

    /* EXECUÇÃO */
    int x = 50;
    int y = 150;
    int r1 = 150;
    int r2 = 150;
    int going = 1;
    Uint32 timeout = 10;

    int inGame = 1;
    int speed = 1;
    int maxSpeed = 50;
    int minSpeed = 0;

    while (inGame) {
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
        SDL_RenderClear(ren);
        DrawRackets(ren, r1, r2);
        DrawBall(ren, x, y);
        SDL_RenderPresent(ren);
        int hadEvent = AUX_WaitEventTimeoutCount(&evt, &timeout);

        if (hadEvent) {
            switch (evt.type) {
                case SDL_KEYDOWN:
                    switch (evt.key.keysym.sym) {
                    case SDLK_w:
                        r1 -= r1 <= 50 ? 0 : 5;
                        break;
                    case SDLK_s:
                        r1 += r1 >= 250 ? 0 : 5;
                        break;
                    case SDLK_UP:
                        r2 -= r2 <= 50 ? 0 : 5;
                        break;
                    case SDLK_DOWN:
                        r2 += r2 >= 250 ? 0 : 5;
                        break;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    SDL_GetMouseState(&x, &y);
                    r1 = y;
                    r2 = y;
                    break;
                case SDL_MOUSEWHEEL:
                    speed += evt.wheel.y > 0 ? 1 : -1;
                    speed = max(min(speed, maxSpeed), minSpeed);
                    break;
                case SDL_WINDOWEVENT:
                    inGame = evt.window.event == SDL_WINDOWEVENT_CLOSE ? 0 : 1;
                    break;
            }
        }
        else {
            timeout = 10;
            going = x > 450 ? 0 : x < 50 ? 1 : going;
            x += going ? speed : -speed;
        }
    }

    SDL_RenderPresent(ren);

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}