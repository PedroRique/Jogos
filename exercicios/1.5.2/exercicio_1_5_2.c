#include <SDL.h>


int WINDOW_WIDTH = 500;
int WINDOW_HEIGHT = 300;
int TRIANGLE_SIZE = 50;

typedef struct SDL_Triangle
{
    int x, y;
    int h;
} SDL_Triangle;

int AUX_WaitEventTimeoutCount(SDL_Event* evt, Uint32* ms) {
    Uint32 antes = SDL_GetTicks();
    int hadEvent = SDL_WaitEventTimeout(evt, *ms);
    if (hadEvent) {
        int newTimeout = *ms - (SDL_GetTicks() - antes);
        *ms = newTimeout < 0 ? 0 : newTimeout;
    }
    return hadEvent;
}

void DrawTriangle(SDL_Renderer* renderer, SDL_Triangle t) {
    int s = t.h / 2;
    SDL_Point tri[4] = {
        {t.x, t.y - s},
        {t.x - s, t.y + s},
        {t.x + s, t.y + s},
        {t.x, t.y - s}
    };
    SDL_RenderDrawLines(renderer, &tri, 4);
}

void StartListening(SDL_Renderer* renderer) {
    SDL_Triangle t1 = { 100, 150, TRIANGLE_SIZE };
    SDL_Triangle t2 = { 250, 150, TRIANGLE_SIZE };
    SDL_Triangle t3 = { 400, 150, TRIANGLE_SIZE };

    SDL_Event evt;
    int going = 1;
    int hadEvent = 0;
    Uint32 timeout = 10;

    while (1) {
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);

        /* Draw Triangles */
        SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0x00);
        DrawTriangle(renderer, t1);
        SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0x00);
        DrawTriangle(renderer, t2);
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0x00);
        DrawTriangle(renderer, t3);

        
        SDL_RenderPresent(renderer);

        hadEvent = AUX_WaitEventTimeoutCount(&evt, &timeout);

        if (hadEvent) {
            if (evt.type == SDL_KEYDOWN) {
                switch (evt.key.keysym.sym) {
                case SDLK_UP:
                    t2.y -= t2.y <= TRIANGLE_SIZE / 2 ? 0 : 5;
                    break;
                case SDLK_DOWN:
                    t2.y += t2.y >= WINDOW_HEIGHT - TRIANGLE_SIZE / 2 ? 0 : 5;
                    break;
                case SDLK_LEFT:
                    t2.x -= t2.x <= TRIANGLE_SIZE / 2 ? 0 : 5;
                    break;
                case SDLK_RIGHT:
                    t2.x += t2.x >= WINDOW_WIDTH - TRIANGLE_SIZE / 2 ? 0 : 5;
                    break;
                }
            }
            if (evt.type == SDL_MOUSEMOTION) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                t3.x = x;
                t3.y = y;
            }
        }
        else {
            /* Animate */
            if (going && t1.y < 200) {
                t1.y += 5;
            }
            else if (going && t1.y >= 200) {
                going = 0;
            }
            else if (!going && t1.y > 100) {
                t1.y -= 5;
            }
            else if (!going && t1.y <= 100) {
                going = 1;
            }

            timeout = 10;
        }
    }
}

int main(int argc, char* args[])
{

    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Ex 1.5.2",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN
    );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    StartListening(ren);

    /* FINALIZACAO */

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
