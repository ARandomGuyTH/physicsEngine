#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <math.h>
#include "./constants.h"

int game_is_running = FALSE;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int lastFrameTime = 0;

typedef struct ball {
    float x;
    float y;
    float radius;

} ball;

ball ball1;


int initialiseWindow(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) { //initialise SDL
        fprintf(stderr, "Error initialising SDL. \n");
        return FALSE; 
    }
    window = SDL_CreateWindow(
        NULL, //No name
        SDL_WINDOWPOS_CENTERED, //window centred
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, //800x600 default
        WINDOW_HEIGHT,
        SDL_WINDOW_BORDERLESS //borderless
    );
    if (!window) { //if null pointer returned
        fprintf(stderr, "Error creating SDL window. \n");
        return FALSE;
    }
    renderer = SDL_CreateRenderer(window, -1, 0); //render onto window, using default driver, no flags
    if (!renderer) { //if null pointer returned
        fprintf(stderr, "Error creating SDL Renderer. \n");
        return FALSE;
    }

    return TRUE;
}


void setup(void) {
    ball1.x = 20.0f;
    ball1.y = 20.0f;
    ball1.radius = 15.0f;
}

//processes user inputs
void process_input(void) {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) { //checks what events that happened between frames
        case SDL_QUIT: //when x button in window clicked (or alt f4)
            game_is_running = FALSE; 
            break;
        case SDL_KEYDOWN: //if key pressed
            if(event.key.keysym.sym == SDLK_ESCAPE) { //if escape key pressed
                game_is_running = FALSE;
            }
            break;
    }
}

//updates values to match user input
void update(void) {
    //sleep the execution until we reach the target frame time in ms
    int timeToWait =  FRAME_TARGET_TIME - (SDL_GetTicks() - lastFrameTime);

    if (timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME) {
        SDL_Delay(timeToWait); //delays until time passed to cap framerate
    }

    float deltaTime = (SDL_GetTicks() - lastFrameTime) / 1000.0f; //difference in s of time between frames
    lastFrameTime = SDL_GetTicks(); //updates last time.

    ball1.x += 70 * deltaTime;
    ball1.y += 70 * deltaTime;
}

//renders changes onto screen
void render(void) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); //colours in backbuffer
    SDL_RenderClear(renderer);

    //Draw a rectangle
    SDL_Rect ballRect = {(int)ball1.x, (int)ball1.y, (int)ball1.radius, (int)ball1.radius};

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &ballRect);

    SDL_RenderPresent(renderer); //renders the backbuffer
}

void destroy_window(void) { //destroys in reverse order as initialised
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int n, char* args[n]) {
    game_is_running = initialiseWindow();

    setup();

    while (game_is_running) {
        process_input();
        update();
        render();
    }

    destroy_window();

    return EXIT_SUCCESS;
}
