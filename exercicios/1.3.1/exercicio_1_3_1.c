#include <SDL.h>

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

void DrawRackets(SDL_Renderer* renderer) {
    /* Draws the rackets */
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_Rect racket1 = { 0, 100, 20, 100 };
    SDL_Rect racket2 = { 480, 100, 20, 100 };
    SDL_RenderFillRect(renderer, &racket1);
    SDL_RenderFillRect(renderer, &racket2);
}

void DrawBall(SDL_Renderer* renderer, int x, int y) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderFillCircle(renderer, x, y, 30);
}

int main(int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Ex 1.3.1",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        500, 300, SDL_WINDOW_SHOWN
    );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
    SDL_RenderClear(ren);

    /* EXECUÇÃO */
    int x = 50;
    int going = 1;
    while (x < 500) {
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
        SDL_RenderClear(ren);
        DrawRackets(ren);
        DrawBall(ren, x, 150);
        SDL_RenderPresent(ren);
        SDL_Delay(5);
        going = x > 450 ? 0 : x < 50 ? 1 : going;
        x += going ? 1 : -1;
    }

    SDL_RenderPresent(ren);

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}