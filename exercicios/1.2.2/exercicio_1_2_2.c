#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>


int main(int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Ex 1.2.2",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        500, 300, SDL_WINDOW_SHOWN
    );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */

	SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
	SDL_RenderClear(ren);
	int face = filledCircleColor(ren, 250, 150, 100, 0xFF00FFFF);


	int eye1 = filledCircleColor(ren, 200, 120, 20, 0xFF000000);
	int eye2 = filledCircleColor(ren, 300, 120, 20, 0xFF000000);

	int month = arcColor(ren, 250, 120, 80, 60, 120, 0xFF000000);

	int S = characterColor(ren, 210, 20, 'S', 0xFF000000);
	int M = characterColor(ren, 230, 20, 'M', 0xFF000000);
	int I = characterColor(ren, 250, 20, 'I', 0xFF000000);
	int L = characterColor(ren, 270, 20, 'L', 0xFF000000);
	int E = characterColor(ren, 290, 20, 'E', 0xFF000000);
	SDL_RenderPresent(ren);

SDL_Delay(2000);

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}
