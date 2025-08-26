#include <SDL3\SDL.h>

// Defining Variables
#define PI 3.14159265358979323846
#define RAYS_NUMBER 500
#define WIDTH 1200
#define HIGHT 900
// Defining Colors
#define COLOR_MOON 0xffaaaaaa
#define COLOR_ERASE 0x00000000
#define COLOR_RAY 0xffffff33
// ShortCuts for Functions
#define ERASE SDL_FillSurfaceRect(surface , &erase ,COLOR_ERASE);
#define FILL_MOON FillCircle(surface , Moon , COLOR_MOON);
#define FILL_SUN FillCircle(surface , Sun , COLOR_RAY);
#define UPDATE SDL_UpdateWindowSurface(window);
#define GENERATE Generate_Rays(angles);
#define CAST RayCasting(surface , Sun , Moon , COLOR_RAY);
#define ANIMATION moon_animation(&Moon);

// Circle Class
typedef struct 
{
    double x , y , r;
} Circle;

void FillCircle(SDL_Surface* surface , Circle circle , Uint32 color)
{
    // calculating the full area of the circle 
    double radius = SDL_pow(circle.r , 2);
    // looping through the rectangle of the circle so we can close the search 
    for(double x = circle.x - circle.r ; x < circle.x + circle.r ; x++)
    {
        for(double y = circle.y - circle.r ; y < circle.y + circle.r ; y++)
        {
            // calculating the distance between two points
            double distance = SDL_pow(x-circle.x , 2) + SDL_pow(y-circle.y , 2);
            // checking if the pixel inside that circle if does the we color it with white
            if(distance < radius)
            {
                SDL_Rect pixel = {x , y , 1 , 1};
                SDL_FillSurfaceRect(surface , &pixel , color);
            }
            // at the end we got fully drawn circle
        }
    }
}

void RayCasting(SDL_Surface* surface, Circle circle , Circle moon , Uint32 color) 
{
    double radius = SDL_pow(moon.r , 2);
    // Drawing The rays into the surface 

    for(double i = 0.00 ; i < (PI * 2) ; i += 0.01)
    {

        int out_of_screen = 0; 
        int hit_object = 0;

        // the rays starts at the circle x , y coordinates
        double x_draw = circle.x;
        double y_draw = circle.y;
        
        // keeps drawing the rays until its out of border or hits an objects
        while (!out_of_screen && !hit_object)
        {
            
            x_draw += SDL_cos(i); 
            y_draw += SDL_sin(i); 

            SDL_Rect pixel = (SDL_Rect) {x_draw , y_draw , 1 , 1};
            SDL_FillSurfaceRect(surface , &pixel , color);

            // calculating the distance to check if the rays collided with the object (woman)
            double distance = SDL_pow(x_draw - moon.x , 2) + SDL_pow(y_draw - moon.y , 2);

            // breaking out of the loop if the rays is out of bound or hits an objects
            if(x_draw < 0 || x_draw > WIDTH)
                out_of_screen = 1;
            if(y_draw < 0 || y_draw > HIGHT)
                out_of_screen = 1;
            if(distance < radius)
                hit_object = 1;
        }
    }
}

void moon_animation(Circle* moon)
{
    // making the moon goes up and down
    static int add = 4;
    moon->y += add;
    

    // reverse the direction of the moon if hits
    if ((moon->y + moon->r) > HIGHT) add = -add;
    if ((moon->y - moon->r) < 0) add = -add;

}

int main() 
{
    // SDL main function
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Ray Tracing" , WIDTH , HIGHT , 0);
    SDL_Surface* surface = SDL_GetWindowSurface(window);

    // declaring variable 
    SDL_Rect erase = {0 , 0 , WIDTH , HIGHT};
    Circle Sun = {200 , 200 , 1};
    Circle Moon = {800 , 300 , 150};

    SDL_SetWindowPosition(window , SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED);

    // application loop
    int running = 1;
    SDL_Event event;
    while(running)
    {
        // waiting for event (mouse event or keyboard event)
        // if nothing pressed returns 0 (exits the loop)
        while(SDL_PollEvent(&event)) 
        {
            // exists if the user pressed the exit button
            if (event.type == SDL_EVENT_QUIT || event.key.scancode == SDL_SCANCODE_ESCAPE)
                running = 0;
            // handling mouse pressing 
            if (event.motion.state != 0)
	        {
                Sun.x = event.motion.x;
                Sun.y = event.motion.y;
            }
        }
        ERASE;
        // FILL_SUN;
        CAST;        
        FILL_MOON;
        ANIMATION;
        UPDATE;
    }

    SDL_DestroyWindowSurface(window);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
