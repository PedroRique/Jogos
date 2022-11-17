#include <SDL.h>

int main(int argc, char* args[])
{
    int WINDOW_WIDTH = 500;
    int WINDOW_HEIGHT = 300;
    int RECT_SIZE = 10;

    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Movendo um Retangulo",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN
    );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
    SDL_Rect r = { 40,20, RECT_SIZE,RECT_SIZE };
    struct SDL_Rect points[10];
    int nP = 0;
    SDL_Event evt;

    int inGame = 1;

    while (inGame) {
        SDL_SetRenderDrawColor(ren, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(ren);
        SDL_SetRenderDrawColor(ren, 0xFF, 0xFF, 0xFF, 0x00);
        SDL_RenderFillRect(ren, &r);

        /* Draw points */
        
        for (int i = 0; i < nP; i++)
        {
            SDL_SetRenderDrawColor(ren, 0xFF, 0xFF / (i + 1), 0x10 * i, 0x00);
            SDL_RenderFillRect(ren, &points[i]);
        }

        SDL_RenderPresent(ren);

        SDL_WaitEvent(&evt);

        switch (evt.type) {
            case SDL_KEYDOWN:
                switch (evt.key.keysym.sym) {
                case SDLK_UP:
                    r.y -= r.y <= 0 ? 0 : 5;
                    break;
                case SDLK_DOWN:
                    r.y += r.y >= WINDOW_HEIGHT - RECT_SIZE ? 0 : 5;
                    break;
                case SDLK_LEFT:
                    r.x -= r.x <= 0 ? 0 : 5;
                    break;
                case SDLK_RIGHT:
                    r.x += r.x >= WINDOW_WIDTH - RECT_SIZE ? 0 : 5;
                    break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (nP < 10) {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    SDL_Rect newR = { x,y , RECT_SIZE, RECT_SIZE };
                    points[nP] = newR;
                    nP += 1;
                }
                break;
            case SDL_WINDOWEVENT:
                inGame = evt.window.event == SDL_WINDOWEVENT_CLOSE ? 0 : 1;
                break;
        }
    }

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
