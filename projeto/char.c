#include <SDL.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#define LENGTH(x)  (sizeof(x) / sizeof((x)[0]))

enum ScreenState { Menu, Game, Credits };

typedef struct Level
{
    SDL_Rect initCell;
    int maxCells;
    SDL_Rect hole;
} Level;


int AUX_WaitEventTimeoutCount(SDL_Event* evt, Uint32* ms) {
    Uint32 antes = SDL_GetTicks();
    int hadEvent = SDL_WaitEventTimeout(evt, *ms);
    if (hadEvent) {
        int newTimeout = *ms - (SDL_GetTicks() - antes);
        *ms = newTimeout < 0 ? 0 : newTimeout;
    }
    return hadEvent;
}

void renderBackground(SDL_Renderer* ren) {
    SDL_Surface* image = SDL_LoadBMP("bg-dark.bmp");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(ren, image);

    SDL_RenderCopy(ren, texture, NULL, NULL);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(image);
}

void renderText(SDL_Renderer* ren, TTF_Font* font, char str[50], SDL_Color color, int ptsize, int x, int y) {
    int texW = 0;
    int texH = 0;
    TTF_SetFontSize(font, ptsize);
    SDL_Surface* surface = TTF_RenderText_Solid(font, str, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(ren, surface);
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    SDL_Rect dstrect = { x - texW / 2, y - texH / 2, texW, texH };
    SDL_RenderCopy(ren, texture, NULL, &dstrect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

SDL_bool checkCollisionWithObs(SDL_Rect rect, struct SDL_Rect obs[], int nObs) {
    SDL_bool hadCollision = SDL_FALSE;

    for (int i = 0; i < nObs; i++)
    {
        SDL_bool had = SDL_HasIntersection(&rect, &obs[i]);
        hadCollision = had || hadCollision;
        if (hadCollision) {
            break;
        }
    }

    return hadCollision;
}

int main(int argc, char* args[]) {
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    IMG_Init(0);
    SDL_Window* win = SDL_CreateWindow("Divide",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        600, 300, SDL_WINDOW_SHOWN
    );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    TTF_Font* font = TTF_OpenFont("AGENTORANGE.ttf", 25);

    SDL_Texture* cellBg = IMG_LoadTexture(ren, "cell.bmp");
    SDL_Texture* cellActiveBg = IMG_LoadTexture(ren, "cell-active.bmp");
    SDL_Texture* holeBg = IMG_LoadTexture(ren, "hole.bmp");
    SDL_Texture* patternBg = IMG_LoadTexture(ren, "pattern.bmp");

    SDL_Color white = { 255, 255, 255 };
    SDL_Color green = { 0, 255, 0 };

    SDL_Event evt;
    Uint32 timeoutBase = 10;
    Uint32 timeout = timeoutBase;

    enum ScreenState screen = Menu;

    int inGame = 1;

    while (inGame) {
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);

        if (screen == Menu) {
            char* menu[] = { "Start", "Credits" };

            int menuFocus = 0;

            while (screen == Menu) {
                SDL_RenderClear(ren);
                renderBackground(ren);

                renderText(ren, font, "CITOPLASMA", white, 50, 300, 100);

                for (int i = 0; i < 2; i++)
                {
                    SDL_Color menuColor = i == menuFocus ? green : white;
                    renderText(ren, font, menu[i], menuColor, 25, 300, (200 + i * 50));
                }

                int hadEvent = AUX_WaitEventTimeoutCount(&evt, &timeout);

                if (hadEvent) {
                    switch (evt.type) {
                    case SDL_KEYDOWN:
                        switch (evt.key.keysym.sym) {
                        case SDLK_UP:
                            menuFocus = menuFocus <= 0 ? 0 : menuFocus - 1;
                            break;
                        case SDLK_DOWN:
                            menuFocus = menuFocus >= 1 ? 1 : menuFocus + 1;
                            break;
                        case SDLK_RETURN:
                            screen = menuFocus == 0 ? Game : Credits;
                            break;
                        case SDL_WINDOWEVENT:
                            inGame = evt.window.event == SDL_WINDOWEVENT_CLOSE ? 0 : 1;
                            break;
                        }
                    }

                }
                else {
                    timeout = timeoutBase;
                }

                SDL_RenderPresent(ren);
            }
        }
        else if(screen == Game) {

            struct SDL_Rect cells[50];
            struct SDL_Rect obs[] = { { 0, 250, 600, 50 } , { 100, 0, 400, 229 } };

            SDL_Rect f = { 20, 200, 40, 40 };
            SDL_Rect hole = { 540, 200, 40, 40 };

            cells[0] = f;
            int nCells = 1;

            int maxCells = 4;
            int focusedCell = 0;

            while (screen == Game) {
                SDL_RenderClear(ren);
                renderBackground(ren);

                SDL_RenderCopy(ren, holeBg, NULL, &hole);

                for (int i = 0; i < nCells; i++)
                {
                    SDL_Rect c = cells[i];
                    if (i == focusedCell) {
                        SDL_RenderCopy(ren, cellActiveBg, NULL, &c);
                    }
                    else {
                        SDL_RenderCopy(ren, cellBg, NULL, &c);
                    }

                    SDL_Rect result;
                    SDL_bool intersected = SDL_IntersectRect(&c, &hole, &result);

                    if (
                        intersected &&
                        c.w - result.w < 10 &&
                        c.h - result.h < 10 &&
                        hole.w == c.w &&
                        hole.h == c.h
                    ) {
                        for (int j = i; j <= nCells - 1; j++) {
                            cells[j] = cells[j + 1];
                        }
                        if (i == nCells - 1) {
                            focusedCell = 0;
                        }
                        nCells--;   
                    }
                }

                for (int i = 0; i < LENGTH(obs); i++)
                {
                    SDL_RenderCopy(ren, patternBg, NULL, &obs[i]);
                }

                int hadEvent = AUX_WaitEventTimeoutCount(&evt, &timeout);

                if (hadEvent) {
                    switch (evt.type) {
                        case SDL_KEYDOWN:
                            switch (evt.key.keysym.sym) {
                                case SDLK_c:
                                    if (nCells < maxCells) {
                                        SDL_Rect focused = cells[focusedCell];
                                        SDL_Rect newFocused = { focused.x, focused.y + focused.h / 2, focused.w / 2,focused.h / 2 };
                                        SDL_Rect newCell = { focused.x + focused.w / 2, focused.y + focused.h / 2, focused.w / 2,focused.h / 2 };
                                        cells[focusedCell] = newFocused;

                                        for (int i = nCells - 1; i > focusedCell; i--) {
                                            cells[i + 1] = cells[i];
                                        }
                                        focusedCell++;
                                        cells[focusedCell] = newCell;
                                        nCells++;
                                    }
                                    break;

                                case SDLK_v:
                                    if (SDL_TRUE) {
                                        SDL_Rect focused = cells[focusedCell];
                                        int touchingCell = -1;

                                        for (int i = 0; i < nCells; i++)
                                        {
                                            if (i != focusedCell) {
                                                SDL_bool isTouching = SDL_HasIntersection(&focused, &cells[i]);
                                                if (isTouching) {
                                                    touchingCell = i;
                                                }
                                            }
                                        }

                                        if (touchingCell >= 0) {
                                            SDL_Rect touched = cells[touchingCell];
                                            SDL_Rect newFocused = { focused.x, focused.y - touched.h, focused.w + touched.w,focused.h + touched.h };
                                            cells[focusedCell] = newFocused;

                                            for (int j = touchingCell; j <= nCells - 1; j++) {
                                                cells[j] = cells[j + 1];
                                            }
                                            nCells--;
                                            if (touchingCell < focusedCell) {
                                                focusedCell--;
                                            }
                                        }
                                    }
                                    break;

                                case SDLK_z:
                                    if (focusedCell == nCells - 1) {
                                        focusedCell = 0;
                                    }
                                    else {
                                        focusedCell++;
                                    }
                                    break;
                                case SDLK_ESCAPE:
                                    screen = Menu;
                                    break;
                            }
                            break;

                        case SDL_WINDOWEVENT:
                            inGame = evt.window.event == SDL_WINDOWEVENT_CLOSE ? 0 : 1;
                            break;
                    }
                }
                else {
                    Uint8* keys = SDL_GetKeyboardState(NULL);

                    int step = 3;

                    SDL_Rect tryToMoveCell = cells[focusedCell];

                    if (keys[SDL_SCANCODE_LEFT]) {
                        tryToMoveCell.x -= step;
                    }
                    if (keys[SDL_SCANCODE_RIGHT]) {
                        tryToMoveCell.x += step;
                    }
                    if (keys[SDL_SCANCODE_UP]) {
                        tryToMoveCell.y -= step;
                    }
                    if (keys[SDL_SCANCODE_DOWN]) {
                        tryToMoveCell.y += step;
                    }

                    SDL_bool hadCollision = checkCollisionWithObs(tryToMoveCell, obs, LENGTH(obs));

                    if (!hadCollision) {
                        cells[focusedCell] = tryToMoveCell;
                    }

                    timeout = timeoutBase;
                }

                SDL_RenderPresent(ren);
            }
        }
        else if (screen == Credits) {
            SDL_RenderClear(ren);
            renderBackground(ren);

            renderText(ren, font, "Credits", white, 25, 300, 100);

            
            renderText(ren, font, "Pedro Rique", white, 15, 300, 200);

            int hadEvent = AUX_WaitEventTimeoutCount(&evt, &timeout);

            if (hadEvent) {
                switch (evt.type) {
                    case SDL_KEYDOWN:
                        switch (evt.key.keysym.sym) {
                            case SDLK_ESCAPE:
                                screen = Menu;
                        }
                        break;
                    case SDL_WINDOWEVENT:
                        inGame = evt.window.event == SDL_WINDOWEVENT_CLOSE ? 0 : 1;
                        break;
                }

            }
            else {
                timeout = timeoutBase;
            }

            SDL_RenderPresent(ren);
        }
    }

    /* FINALIZACAO */
    SDL_DestroyTexture(patternBg);
    SDL_DestroyTexture(holeBg);
    SDL_DestroyTexture(cellActiveBg);
    SDL_DestroyTexture(cellBg);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();

    return 0;
}