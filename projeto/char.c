#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>

int AUX_WaitEventTimeoutCount(SDL_Event* evt, Uint32* ms) {
    Uint32 antes = SDL_GetTicks();
    int hadEvent = SDL_WaitEventTimeout(evt, *ms);
    if (hadEvent) {
        int newTimeout = *ms - (SDL_GetTicks() - antes);
        *ms = newTimeout < 0 ? 0 : newTimeout;
    }
    return hadEvent;
}

int main(int argc, char* args[]) {
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Divide",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        600, 300, SDL_WINDOW_SHOWN
    );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);
    SDL_Event evt;

    int inGame = 1;
    Uint32 timeout = 10;

    
    struct SDL_Rect cells[50];
    SDL_Rect f = { 20, 230, 20, 20 };
    SDL_Rect floor = { 0, 250, 600, 50 };
    SDL_Rect door = { 500, 230, 20, 20 };
    cells[0] = f;
    int nCells = 1;
    int maxCells = 4;
    int focusedCell = 0;

    while (inGame) {
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
        SDL_RenderClear(ren);

        
        for (int i = 0; i < nCells; i++)
        {
            if (i == focusedCell) {
                SDL_SetRenderDrawColor(ren, 12, 233, 233, 255);
            }
            else {
                SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
            }
            SDL_RenderFillRect(ren, &cells[i]);
            SDL_Rect c = cells[i];
            SDL_bool d = SDL_HasIntersection(&c, &door);

            if (d == SDL_TRUE) {
                for (int j = i; j <= nCells - 1; j++) {
                    cells[j] = cells[j + 1];
                }
                if (i == nCells - 1) {
                    focusedCell = 0;
                }
                nCells--;
                
            }
            
        }

        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        SDL_RenderFillRect(ren, &floor);

        SDL_SetRenderDrawColor(ren, 123, 123, 32, 255);
        SDL_RenderFillRect(ren, &door);

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
                        case SDLK_z:
                            if (focusedCell == nCells - 1) {
                                focusedCell = 0;
                            }
                            else {
                                focusedCell++;
                            }
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

            if (keys[SDL_SCANCODE_LEFT]) {
                cells[focusedCell].x -= 2;
            }
            if (keys[SDL_SCANCODE_RIGHT]) {
                cells[focusedCell].x += 2;
            }
            if (keys[SDL_SCANCODE_UP]) {
                cells[focusedCell].y -= 2;
            }
            if (keys[SDL_SCANCODE_DOWN]) {
                cells[focusedCell].y += 2;
            }

            timeout = 10;
        }

        SDL_RenderPresent(ren);
    }

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}