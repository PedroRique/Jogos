#include <SDL.h>

int main(int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Ex 1.2.1",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        500, 300, SDL_WINDOW_SHOWN
    );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 0);
    SDL_RenderClear(ren);

    /* Draws a red X */
    SDL_SetRenderDrawColor(ren, 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(ren, 80, 100, 180, 200);
    SDL_RenderDrawLine(ren, 180, 100, 80, 200);

    /* Draws a blue Square */
    SDL_SetRenderDrawColor(ren, 0, 0, 255, SDL_ALPHA_OPAQUE);
    SDL_Rect rect = { 200, 100, 100, 100 };
    SDL_RenderDrawRect(ren, &rect);

    /* Draws a green Triangle */
    SDL_SetRenderDrawColor(ren, 0, 255, 0, SDL_ALPHA_OPAQUE);
    SDL_Point tri[4] = { {370, 100}, {320, 200}, {420, 200}, {370, 100} };
    SDL_RenderDrawLines(ren, &tri, 4);

    /* Draws points */
    SDL_SetRenderDrawColor(ren, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_Point points[100];
    for (size_t i = 16; i <= 84; i++)
    {
        points[i].x = i * 5;
        points[i].y = 240;
    }
    SDL_RenderDrawPoints(ren, &points, 100);

    SDL_RenderPresent(ren);
    SDL_Delay(5000);

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}
