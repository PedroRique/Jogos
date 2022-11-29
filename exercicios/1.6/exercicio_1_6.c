#include <SDL.h>
#include <stdio.h>
#include <SDL_ttf.h>

int WINDOW_WIDTH = 500;
int WINDOW_HEIGHT = 300;
int RECT_SIZE = 50;
int INITIAL_POS = 100;


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
    TTF_Init();
    SDL_Window* win = SDL_CreateWindow("Ex 1.6",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN
    );
    SDL_Renderer* renderer = SDL_CreateRenderer(win, -1, 0);
    TTF_Font* font = TTF_OpenFont("arial.ttf", 25);

    SDL_Color color = { 255, 255, 255 };
    SDL_Surface* surface = TTF_RenderText_Solid(font,"Press R to reload", color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    /* EXECUCAO */
    SDL_Rect r1 = { INITIAL_POS, 50, RECT_SIZE, RECT_SIZE };
    SDL_Rect r2 = { INITIAL_POS, 125, RECT_SIZE, RECT_SIZE };
    SDL_Rect r3 = { INITIAL_POS, 200, RECT_SIZE, RECT_SIZE };

    SDL_Rect f = { 445, 0, 10, WINDOW_HEIGHT };

    SDL_bool i1 = SDL_FALSE, i2 = SDL_FALSE, i3 = SDL_FALSE;

    SDL_bool hasWinner = SDL_FALSE;

    Uint8 c1, c2, c3;

    SDL_Event evt;
    int hadEvent = 0;
    Uint32 timeout = 10;

    int pX = 0;
    int pY = 0;

    int inGame = 1;
    int stopGame = 0;

    int rightLimit = WINDOW_WIDTH - RECT_SIZE;

    int texW = 0;
    int texH = 0;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    SDL_Rect dstrect = { 250 - texW / 2, 150 - texH / 2, texW, texH };

    while (inGame) {
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);

        /* Draw Triangles */
        SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0x00);
        SDL_RenderFillRect(renderer, &r1);
        SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0x00);
        SDL_RenderFillRect(renderer, &r2);
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0x00);
        SDL_RenderFillRect(renderer, &r3);

        c1 = i1 ? 0xFF : 0x00;
        c2 = i2 ? 0xFF : 0x00;
        c3 = i3 ? 0xFF : 0x00;
        hasWinner = i1 || i2 || i3;
        if (hasWinner) {
            SDL_SetRenderDrawColor(renderer, c1, c2, c3, 0x00);
        }
        else {
            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0x00);
        }

        SDL_RenderFillRect(renderer, &f);

        hadEvent = AUX_WaitEventTimeoutCount(&evt, &timeout);

        if (hadEvent) {
            switch (evt.type) {
            case SDL_KEYDOWN:
                switch (evt.key.keysym.sym) {
                case SDLK_LEFT:
                    if (!stopGame) {
                        r2.x -= r2.x <= 0 ? 0 : 10;
                    }
                    break;
                case SDLK_RIGHT:
                    if (!stopGame) {
                        r2.x += r2.x >= rightLimit ? 0 : 10;
                    }
                    break;
                case SDLK_r:
                    r1.x = INITIAL_POS;
                    r2.x = INITIAL_POS;
                    r3.x = INITIAL_POS;
                    i1 = SDL_FALSE, i2 = SDL_FALSE, i3 = SDL_FALSE;
                    stopGame = 0;
                    break;
                }
                break;
            case SDL_MOUSEMOTION:
                if (!stopGame) {
                    SDL_GetMouseState(&pX, &pY);
                    int newX = pX - RECT_SIZE / 2;
                    r3.x = newX < 0 ? 0 : newX > rightLimit ? rightLimit : newX;
                }
                break;
            case SDL_WINDOWEVENT:
                inGame = evt.window.event == SDL_WINDOWEVENT_CLOSE ? 0 : 1;
                break;
            }
        }
        else {
            if ((r1.x < rightLimit) && !stopGame) {
                r1.x += 5;
            }
            timeout = 10;
        }

        i1 = SDL_HasIntersection(&r1, &f);
        i2 = SDL_HasIntersection(&r2, &f);
        i3 = SDL_HasIntersection(&r3, &f);

        if (i1 || i2 || i3) {
            SDL_RenderCopy(renderer, texture, NULL, &dstrect);
            stopGame = 1;
        }

        SDL_RenderPresent(renderer);
    }

    /* FINALIZACAO */

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
