#include <SDL3\SDL.h>
#include <stdio.h>

#define WIDTH 900
#define HEIGHT 600
#define COLOR_WHITE 0xffffffff
#define SHADOW_COLOR 0xee0e0e
#define COLOR_BACKGROUND 0xa1a5ab
#define GRAVITY 0.2
#define LINE 40 

typedef struct {
    double x;
    double y;
    double r;
    double velocity_x;
    double velocity_y;
    double friction;
} SDL_Circle;

void SDL_FillSurfaceCircle(SDL_Surface* surface , SDL_Circle circle ,  Uint32 color){
    double radius = SDL_pow(circle.r , 2);
    for(double x = circle.x - circle.r ; x < circle.x + circle.r ; x++){
        for(double y = circle.y - circle.r ; y < circle.y + circle.r ; y++){
            double distance = SDL_pow(x - circle.x , 2) + SDL_pow(y - circle.y , 2);
            if(distance < radius){
                SDL_Rect pixel = {x , y , 1 , 1};
                SDL_FillSurfaceRect(surface , &pixel , color);
            }
        }
    }
}

void Check_Circle(SDL_Circle* circle){

        circle->x += circle->velocity_x;
        circle->y += circle->velocity_y;
        circle->velocity_y += GRAVITY;
        
        // The ball hits the sky 
        if (circle->y - circle->r < 0){
            circle->y = circle->r;
            circle->velocity_y = -circle->velocity_y;  
        }
        // The Ball hits the ground 
        if (circle->y + circle->r > HEIGHT){
            circle->y = HEIGHT - circle->r;
            circle->velocity_y = -circle->velocity_y * circle->friction;
        }
        // The Ball hits left border
        if (circle->x - circle->r < 0){
            circle->x = circle->r;
            circle->velocity_x = -circle->velocity_x * circle->friction;  
        }
        // The Ball hits right border
        if (circle->x + circle->r > WIDTH){
            circle->x = WIDTH - circle->r;
            circle->velocity_x = -circle->velocity_x * circle->friction;  
        }
}

void SDL_FillShadowCircle(SDL_Surface* surface , SDL_Circle* shadow , Uint32 color){
    for (int i = 0 ; i < LINE ; i++){
        SDL_FillSurfaceCircle(surface , shadow[i] , color);
    }
}

void UpdateShadow(SDL_Circle ball , SDL_Circle* shadow){
    static int i = 0;

    shadow[i].x = ball.x;
    shadow[i].y = ball.y;

    i += 1;

    if (i > LINE - 1) i += -i;
}

void SDL_FillShadowArray(SDL_Circle* shadow , SDL_Circle ball){
    double shadow_size = 5;
    for (int i = 0 ; i < LINE ; i++)
        shadow[i] = (SDL_Circle) {ball.x , ball.y , shadow_size - (i % (int)shadow_size)  , 0 , 0}; 
}

void main() {
    // I don't want to save this project yet until I learn about the sound Effects

    // NOTE: some function may seems in SDL library but they're not just  they just share the same naming method

    // SDL Main function to open a window and a surface to draw on
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Bouncy Ball" , WIDTH , HEIGHT , SDL_WINDOW_BORDERLESS);
    SDL_Surface* surface = SDL_GetWindowSurface(window);

    // The Structure will be using in the application 
    SDL_Circle ball = (SDL_Circle) {200 , 200 , 50 , SDL_rand(4) + 4, 0 , 0.99};
    SDL_Circle shadow[LINE];
    SDL_FillShadowArray(shadow , ball);
    SDL_Rect erase = (SDL_Rect) {0 , 0 , WIDTH , HEIGHT};

    int running = 1;
    SDL_Event event;

    // application main loop
    while(running){
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_EVENT_QUIT)
                running = 0;
            if(event.key.scancode == SDL_SCANCODE_ESCAPE)
                running = 0;
        }
        // double animation buffer
        SDL_FillSurfaceRect(surface , &erase , COLOR_BACKGROUND);
        // Draw The ball on the surface 
        SDL_FillSurfaceCircle(surface , ball , COLOR_WHITE);
        // Draw The Shadow Ball on the surface 
        SDL_FillShadowCircle(surface , shadow , SHADOW_COLOR);
        // Updates The Shadow Pos
        UpdateShadow(ball , shadow);
        // Checking if the circle is out of window
        Check_Circle(&ball);
        // Updating the drawing
        SDL_UpdateWindowSurface(window);
        SDL_Delay(30);
    }

    SDL_Quit();
    SDL_DestroyWindowSurface(window);
    SDL_DestroyWindow(window);
}