#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdio.h>

#define WIDTH 900
#define HEIGHT 600
#define PAD_SPEED 9 
#define BALL_SPEED 7 
#define PAD_COLOR 0x465e37
#define BALL_COLOR 0x695b48

typedef struct ball
{
	double x;
	double y;
	double r;
	double x_s;
	double y_s;
	double x_d;
	double y_d;
} Ball;

void HalfLine(SDL_Surface* surface)
{
	SDL_Rect pixel = (SDL_Rect) {WIDTH / 2, 0 , 1 , HEIGHT};
	SDL_FillSurfaceRect(surface , &pixel , 0xffffffff);
}

void setBall(Ball *circle)
{
	int rand = SDL_rand(4) + 1;
	circle->x = WIDTH / 2;
	circle->y = HEIGHT / 2;
	circle->r = 20;
	circle->x_s = BALL_SPEED;
	circle->y_s = BALL_SPEED;

	// setting the directions
	if(rand == 1)
	{
		circle->y_d = -1;
		circle->x_d = -1;	
	}
	else if (rand == 2)
	{
		circle->y_d = 1;
		circle->x_d = 1;
	}
	else if (rand == 3)
	{
		circle->y_d = 1;	
		circle->x_d = -1;
	}
	else if (rand == 4)
	{
		circle->y_d = -1;
		circle->x_d = 1;
	}	
}

void switch_state(SDL_Event event , bool* rightup_state , bool* rightdown_state , bool* leftup_state , bool* leftdown_state)
{ 
	// handling Right Pad switch 
	if (event.key.scancode == SDL_SCANCODE_UP)
	{
		*rightup_state = true;
		*rightdown_state = false;
	}
	if (event.key.scancode == SDL_SCANCODE_DOWN)
	{
		*rightdown_state = true;
		*rightup_state = false;
	
	}

	// handling Left Pad switch
	if (event.key.scancode == SDL_SCANCODE_W)
	{
		*leftup_state= true;
		*leftdown_state = false;
	}
	if (event.key.scancode == SDL_SCANCODE_S)
	{
		*leftdown_state = true;
		*leftup_state = false;
	}
}

void move_rightpad(SDL_Rect *rightpad , bool up_state , bool down_state)
{

	if (up_state)
	{
		if (rightpad->y > 0)
			rightpad->y -= PAD_SPEED;	
	}
	if (down_state)
	{
		if (rightpad->y + rightpad->h < HEIGHT)
			rightpad->y += PAD_SPEED;	
	}	
}

void move_leftpad(SDL_Rect *leftpad , bool up_state , bool down_state)
{
	if (up_state)
	{
		if (leftpad->y > 0)
			leftpad->y -= PAD_SPEED;	
	}
	if (down_state)
	{
		if (leftpad->y + leftpad->h < HEIGHT)
			leftpad->y += PAD_SPEED;
	}
}

void SDL_FillBall(SDL_Surface* surface , Ball circle , Uint32 color)
{
	double radius = SDL_pow(circle.r , 2);
	for (double x = circle.x - circle.r  ; x < circle.x + circle.r; x++)
	{
		for (double y = circle.y - circle.r ; y < circle.y + circle.r ; y++)
		{
			double distance = SDL_pow(x - circle.x , 2) + SDL_pow(y - circle.y , 2);
			if (distance < radius)
			{
				SDL_Rect pixel = (SDL_Rect) {x , y , 1 , 1};
				SDL_FillSurfaceRect(surface , &pixel , color);
			}
		}
	}
}

void move_ball(Ball* circle , SDL_Rect rightpad , SDL_Rect leftpad)
{
	
	// moveing the ball
	circle->x += circle->x_s * circle->x_d;
	circle->y += circle->y_s * circle->y_d;
	
	// the ball hits the rightpad
	if ((circle->x + circle->r > rightpad.x) && (circle->y > rightpad.y) && (circle->y < rightpad.y + rightpad.h))
	{
		circle->x_d = -circle->x_d;
	}
	// the ball hits the leftpad 
	if ((circle->x - circle->r < leftpad.x + leftpad.w) && (circle->y > leftpad.y) && (circle->y < leftpad.y + leftpad.h) ) 
	{
		circle->x_d = -circle->x_d;
	}
	// the ball hits the ground 
	if (circle->y + circle->r > HEIGHT)
	{
		circle->y_d = -circle->y_d;
	}
	// the ball hits the roof
	if (circle->y - circle->r < 0)
	{
		circle->y_d = -circle->y_d;
	}
	
}

void printscore(int leftscore , int rightscore)
{
	printf("Right player = %d , Left Player = %d\n", rightscore , leftscore);
}

void CheckWin(SDL_Window* window , SDL_Surface* surface , Ball *circle , int* leftscore , int* rightscore)
{

	// did the ball got out of the border?
	// did it got out of the right side?
	
	// yes => one point for the left player and return the ball to the center 
	if (circle->x - circle->r > WIDTH)
	{
		*leftscore += 1;
		setBall(circle);
		SDL_FillBall(surface , *circle , BALL_COLOR);
		SDL_UpdateWindowSurface(window);

		printscore(*leftscore , *rightscore);

		SDL_Delay(1000);
	}
	// did the ball hot out of the left side?
	
	// yss => one point for the right player and also return the ball to the center
	if (circle->x + circle->r < 0)
	{
		*rightscore += 1;
		setBall(circle);
		SDL_FillBall(surface , *circle , BALL_COLOR);
		SDL_UpdateWindowSurface(window);
		
		printscore(*leftscore , *rightscore);

		SDL_Delay(1000);
	}
}

int main()
{
	// I don't want to save this project until I learn about handling multiple inputs at once

	// SDL3 main functions to Create simple window
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("Ping pong Game" , WIDTH , HEIGHT , 0);
	SDL_Surface* surface = SDL_GetWindowSurface(window);

	SDL_Rect erase = (SDL_Rect) {0 , 0 , WIDTH , HEIGHT};
	SDL_Rect LeftPad = (SDL_Rect) {0 , 0 , 30 , 200};
	SDL_Rect RightPad = (SDL_Rect) {WIDTH - 30 , 0 , 30 , 200};
	Ball ball;

	setBall(&ball);


	bool leftdown = false;
	bool leftup = false;
	bool rightdown = false;
	bool rightup = false;
	int leftscore = 0;
	int rightscore = 0;
	int running = 1;
	SDL_Event event;
	while(running)
	{
		while(SDL_PollEvent(&event))
		{
			if (event.type == SDL_EVENT_QUIT || event.key.scancode == SDL_SCANCODE_ESCAPE)
			{
				running = 0;
			}
			if (event.key.down != 0)
			{
				switch_state(event , &rightup , &rightdown , &leftup , &leftdown);
			}
		}
		SDL_FillSurfaceRect(surface , &erase , 0);
		HalfLine(surface);
		SDL_FillSurfaceRect(surface , &RightPad , PAD_COLOR);
		SDL_FillSurfaceRect(surface , &LeftPad , PAD_COLOR);
		SDL_FillBall(surface , ball , BALL_COLOR); 		
		move_ball(&ball , RightPad , LeftPad);
		move_leftpad(&LeftPad , leftup , leftdown);
		move_rightpad(&RightPad , rightup , rightdown);
		CheckWin(window , surface , &ball , &leftscore , &rightscore);
		SDL_UpdateWindowSurface(window);
		SDL_Delay(16); // ~= 16fps
	}

	SDL_Quit();
	SDL_DestroyWindowSurface(window);
	SDL_DestroyWindow(window);
}
