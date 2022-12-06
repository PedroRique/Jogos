#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <SDL_image.h>

int BALL_SIZE = 100;

void DrawRackets(SDL_Renderer* renderer, int y1, int y2) {
    /* Draws the rackets */
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_Rect racket1 = { 0, y1 - 50, 20, 100 };
    SDL_Rect racket2 = { 480, y2 - 50, 20, 100 };
    SDL_RenderFillRect(renderer, &racket1);
    SDL_RenderFillRect(renderer, &racket2);
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

float arcSpeed(int x) {
    float a = (x - 200) * (x - 200);
    float r = -(a / 20) + 100;
    printf("speed %f\n", r);
    return r;
}

int main(int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(0);
    SDL_Window* win = SDL_CreateWindow("Ex 1.7",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        500, 300, SDL_WINDOW_SHOWN
    );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);
    SDL_Texture* img = IMG_LoadTexture(ren, "ball_sprite.png");
    assert(img != NULL);
    SDL_Event evt;

    /* EXECUÇÃO */
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
    SDL_RenderClear(ren);

    /* EXECUÇÃO */
    int x = 50;
    int y = 100;
    int r1 = 150;
    int r2 = 150;
    int going = 1;
    Uint32 timeout = 10;

    int inGame = 1;
    int speed = 1;
    int maxSpeed = 50;
    int minSpeed = 0;

    SDL_Rect ball = { x, y, BALL_SIZE, BALL_SIZE };

    while (inGame) {
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
        SDL_RenderClear(ren);
        DrawRackets(ren, r1, r2);
        SDL_Rect c;
        if (ball.x < 20) {
            c = (SDL_Rect){ 0,0, 100,100 };
        }
        else if (ball.x >= 20 && ball.x < 380) {
            c = (SDL_Rect){ 100,0, 100,100 };
        }
        else if(ball.x >= 380) {
            c = (SDL_Rect){ 200,0, 100,100 };
        }
        SDL_RenderCopy(ren, img, &c, &ball);

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
                ball.x = x - BALL_SIZE/2;
                ball.y = y - BALL_SIZE/2;
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
            going = ball.x > 400 ? 0 : ball.x < 0 ? 1 : going;
            //speed = arcSpeed(ball.x);
            ball.x += going ? speed : -speed;
        }
    }

    SDL_RenderPresent(ren);

    /* FINALIZACAO */
    SDL_DestroyTexture(img);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    IMG_Quit();
    SDL_Quit();

    return 0;
}