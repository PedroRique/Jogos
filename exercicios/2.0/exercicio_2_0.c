#include <SDL.h>
#include <stdio.h>
#include <SDL_ttf.h>

int WINDOW_WIDTH = 500;
int WINDOW_HEIGHT = 300;
int RECT_SIZE = 75;

void print(SDL_Renderer* renderer, SDL_Texture* texture, TTF_Font* font, SDL_Rect dstrect) {
    int texW = 0;
    int texH = 0;
    SDL_Color color = { 255, 255, 255 };
    SDL_Surface* surface = TTF_RenderText_Solid(font, "Clicked!", color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
}

int main(int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    SDL_Window* win = SDL_CreateWindow("Ex 2.0",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN
    );
    SDL_Renderer* renderer = SDL_CreateRenderer(win, -1, 0);
    TTF_Font* font = TTF_OpenFont("arial.ttf", 25);

    SDL_Color color = { 255, 255, 255 };
    SDL_Surface* surface = TTF_RenderText_Solid(font, "Press R to reload", color);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, surface);;
    SDL_Rect dstrect = { 0, 0, 0, 0 };

    /* EXECUCAO */
    SDL_Rect draggableRect = { 10, 10, RECT_SIZE, RECT_SIZE };

    SDL_Event evt;

    int inGame = 1;

    int isPressing = 0;

    int didDrag = 0;
    int didHandlePointerDown = 0;
    SDL_Point dragOrigin = { 0, 0 };
    SDL_Point origPos = { 0, 0 };

    SDL_Point client = { 0, 0 };
    int cX = 0;
    int cY = 0;

    while (inGame) {
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);

        /* Draw Square */
        SDL_SetRenderDrawColor(renderer, 0x00, 0x33, 0x66, 0x00);
        SDL_RenderFillRect(renderer, &draggableRect);

        while (SDL_PollEvent(&evt)) { /* Repetir até que não haja mais eventos na fila */
            switch (evt.type) { /* Processa o tipo de evento apropriado */
                case SDL_MOUSEBUTTONDOWN:
                    if (SDL_BUTTON(SDL_BUTTON_LEFT)) {
                        isPressing = 1;
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    if (SDL_BUTTON(SDL_BUTTON_LEFT)) {
                        isPressing = 0;
                    }
                    break;
                case SDL_KEYDOWN:
                    if (evt.key.keysym.sym == SDLK_ESCAPE && isPressing) {
                        dragOrigin.x = 0;
                        dragOrigin.y = 0;
                        draggableRect.x = origPos.x;
                        draggableRect.y = origPos.y;
                        printf("cancelled\n");
                    }
                case SDL_WINDOWEVENT:
                    inGame = evt.window.event == SDL_WINDOWEVENT_CLOSE ? 0 : 1;
                    break;
            }
        }

        if (isPressing) {
            SDL_GetMouseState(&client.x, &client.y);
            if (!didHandlePointerDown && SDL_PointInRect(&client, &draggableRect)) {
                dragOrigin = client;
                origPos.x = draggableRect.x;
                origPos.y = draggableRect.y;
                didDrag = 0;
            }

            didHandlePointerDown = 1;

            if (dragOrigin.x > 0 && dragOrigin.y > 0 && (client.x != dragOrigin.x || client.y != dragOrigin.y)) {
                didDrag = 1;
                int deltaX = client.x - dragOrigin.x;
                int deltaY = client.y - dragOrigin.y;
                draggableRect.x = origPos.x + deltaX;
                draggableRect.y = origPos.y + deltaY;
                printf("dragging...\n");
            }
        } else if (dragOrigin.x > 0 && dragOrigin.y > 0) {
            dragOrigin.x = 0;
            dragOrigin.y = 0;

            if (didDrag) {
                printf("dropped\n");
            }
            else {
                printf("clicked\n");
            }
        } else {
            didHandlePointerDown = 0;
        }

        SDL_RenderCopy(renderer, textTexture, NULL, &dstrect);
        SDL_RenderPresent(renderer);
    }

    /* FINALIZACAO */

    SDL_DestroyTexture(textTexture);

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
