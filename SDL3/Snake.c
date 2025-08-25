#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define WIDTH 900
#define HEIGHT 600
#define SNAKE_SIZE 20 
#define SNAKE_COLOR 0xa1a5ab
#define APPLE_COLOR 0xff0000
#define GRID_COLOR 0xffffffff

int x_square = WIDTH / SNAKE_SIZE;
int y_square = HEIGHT / SNAKE_SIZE;

typedef struct 
{
    int x;
    int y;
} Vector2;

typedef struct snake
{
    SDL_Rect body;
    struct snake* next;
} snake_t;

void FillGrid(SDL_Surface* surface){
    for (double x = SNAKE_SIZE ; x < WIDTH ; x+= SNAKE_SIZE){
        for (double y = 0 ; y < HEIGHT ; y++){
            SDL_Rect pixel = (SDL_Rect) {x , y , 1 , 1};
            SDL_FillSurfaceRect(surface , &pixel , GRID_COLOR);
        }
    }
    for (double y = SNAKE_SIZE ; y < HEIGHT ; y+= SNAKE_SIZE){
        for (double x = 0 ; x < WIDTH ; x++){
            SDL_Rect pixel = (SDL_Rect) {x , y , 1 , 1};
            SDL_FillSurfaceRect(surface , &pixel , GRID_COLOR);
        }
    }
}

void snake_direction(SDL_Event event , int* move_x , int* move_y){
    if (event.key.scancode == SDL_SCANCODE_UP || event.key.scancode == SDL_SCANCODE_W){
        if (*move_y != SNAKE_SIZE){
            *move_x = 0;
            *move_y = -SNAKE_SIZE;
        } 
    }
    if (event.key.scancode == SDL_SCANCODE_DOWN || event.key.scancode == SDL_SCANCODE_S){
        if (*move_y != -SNAKE_SIZE){
            *move_x = 0;
            *move_y = SNAKE_SIZE;
        }
    }
    if (event.key.scancode == SDL_SCANCODE_RIGHT || event.key.scancode == SDL_SCANCODE_D){
        if (*move_x != -SNAKE_SIZE){
            *move_y = 0;
            *move_x = SNAKE_SIZE;
        }
    }
    if (event.key.scancode == SDL_SCANCODE_LEFT || event.key.scancode == SDL_SCANCODE_A){
        if (*move_x != SNAKE_SIZE){
            *move_y = 0;
            *move_x = -SNAKE_SIZE;
        }
    }
}

void create_snake(snake_t* head , SDL_Rect body)
{
    static int i = 0;
    if (i <= 0)
    {
        head->next = NULL;
        head->body = body; 
        i++;
        return;
    }
    else
    {
        snake_t* new = malloc(sizeof(snake_t));
        new->body = body;

        snake_t* tale = head;
        while(tale->next != NULL)
            tale = tale->next;

        tale->next = new;
        new->next = NULL;
        return;
    }
}

void draw_snake(SDL_Surface* surface , snake_t* head)
{
    snake_t* snake = head;

    while (snake != NULL)
    {
        SDL_FillSurfaceRect(surface , &snake->body , SNAKE_COLOR);
        snake = snake->next;
    }
}

// there is a room for improvement here for sure
// please improve me if you have time
void move_snake(snake_t* head , int move_x , int move_y)
{
    snake_t* snake = head;

    Vector2 vector = {snake->body.x , snake->body.y};

    snake->body.x += move_x;
    snake->body.y += move_y;

    int x,y;

    while (snake->next != NULL)
    {
        if (snake->next->next != NULL)
        {
            x = snake->next->body.x;
            y = snake->next->body.y;
        }

        snake->next->body.x = vector.x;
        snake->next->body.y = vector.y;

        vector.x = x;
        vector.y = y;

        if (snake->body.y < 0) snake->body.y = HEIGHT - SNAKE_SIZE;
        if (snake->body.y + SNAKE_SIZE > HEIGHT) snake->body.y = 0;
        if (snake->body.x < 0) snake->body.x = WIDTH - SNAKE_SIZE;
        if (snake->body.x + SNAKE_SIZE > WIDTH) snake->body.x = 0;

        snake = snake->next;
    } 

    snake->body.x = vector.x;
    snake->body.y = vector.y;

    if (snake->body.y < 0) snake->body.y = HEIGHT - SNAKE_SIZE;
    if (snake->body.y + SNAKE_SIZE > HEIGHT) snake->body.y = 0;
    if (snake->body.x < 0) snake->body.x = WIDTH - SNAKE_SIZE;
    if (snake->body.x + SNAKE_SIZE > WIDTH) snake->body.x = 0;
}

void place_apple(SDL_Rect* apple , snake_t* head)
{
    snake_t* snake = head;
    
    if (head->body.x == apple->x && head->body.y == apple->y) 
    {
        int x = SDL_rand(x_square);
        int y = SDL_rand(y_square);
        create_snake(head , (SDL_Rect) {head->next->body.x , head->next->body.y , SNAKE_SIZE - 1 , SNAKE_SIZE - 1});
        while(snake != NULL)
        {
            
            apple->x = x * SNAKE_SIZE;
            apple->y = y * SNAKE_SIZE;

            if (apple->x == snake->body.x && apple->y == snake->body.y)
            {
                x = SDL_rand(x_square);
                y = SDL_rand(y_square);
                snake = head;
            }
            snake = snake->next;
        }
    }
}

int check_lose(snake_t* head)
{
    snake_t* snake = head->next;

    while(snake != NULL)
    {
        if (head->body.x == snake->body.x && head->body.y == snake->body.y)
        {
            return 1;
        }
        snake = snake->next;
    }
    return 0;
}

void free_snake(snake_t* head)
{
    snake_t* snake = head;
    while(snake != NULL)
    {
        snake_t* temp = snake;
        free(temp);
        snake = snake->next;
    }
}

// Things I want to add before sending it to friends
// Game screen // learn to play with textures
// sound effects // learn to play with sounds
// fix the input thing
// adjust the background color to be nice 
// thats all for now Good Luck
void main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Lo3bat L7nech" , WIDTH , HEIGHT , 0);
    SDL_Surface* surface = SDL_GetWindowSurface(window);

    SDL_Rect apple = (SDL_Rect) {SDL_rand(x_square)*SNAKE_SIZE , SDL_rand(y_square)*SNAKE_SIZE , SNAKE_SIZE , SNAKE_SIZE};
    SDL_Rect erase = (SDL_Rect) {0 , 0 , WIDTH , HEIGHT};

    snake_t* head = malloc(sizeof(snake_t));    

    for (int i = 0 ; i < 4 ; i++)
        create_snake(head  , (SDL_Rect){8*SNAKE_SIZE - (i*SNAKE_SIZE) , 8*SNAKE_SIZE , SNAKE_SIZE-1 , SNAKE_SIZE-1});

    int running = 1;
    int Game_screen = 1;
    int move_x = SNAKE_SIZE;
    int move_y = 0;
    SDL_Event event;


    while(running){
        while(SDL_PollEvent(&event)){
            if (event.key.scancode == SDL_SCANCODE_ESCAPE || event.type == SDL_EVENT_QUIT)
                running = 0;
            if (event.key.down != 0)
                snake_direction(event , &move_x , &move_y);
            if (event.key.scancode == SDL_SCANCODE_SPACE)
                create_snake(head , (SDL_Rect) {head->next->body.x , head->next->body.y , SNAKE_SIZE - 1 , SNAKE_SIZE - 1});
        }
        SDL_FillSurfaceRect(surface , &erase , 0);
        // FillGrid(surface);
        draw_snake(surface , head);
        move_snake(head , move_x , move_y);
        place_apple(&apple , head);
        if(check_lose(head))
        {
            printf("You Lost\n");
            free_snake(head);
            running = 0;
        } 
        SDL_FillSurfaceRect(surface , &apple , 0x00ff0000);
        SDL_UpdateWindowSurface(window);
        SDL_Delay(100);
    }
    SDL_Quit();
    SDL_DestroyWindowSurface(window);
    SDL_DestroyWindow(window);
}