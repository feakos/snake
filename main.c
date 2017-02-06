#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h> //Main SDL library
#include <SDL2/SDL_ttf.h> //Optional SDL library used to display text using renderers

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGTH = 800;

//Directions
typedef struct {
	bool W, A, S, D;
	bool UP, LEFT, DOWN, RIGHT;
} directions;

int main ( int argc, char* args[] ){
	directions dir = { 0, 0, 0, 0, 0, 0, 0, 0 };
	bool quit = true;
	SDL_Event event;

	//Window
 	SDL_Window* window = NULL;

	//Surface
 	SDL_Surface* screenSurface = NULL;
	
	//Render
	SDL_Renderer *render;

	//SDL inicializálása
	if( SDL_Init ( SDL_INIT_VIDEO ) < 0 ){
		fprintf ( stderr, "SDL-t nem sikerült inicializálni! SDL_Error: %s\n", SDL_GetError() );
	}
	else{

		//SDL ablak létrehozása
		SDL_Window *window = SDL_CreateWindow ( "Hello SDL2", 
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
			WINDOW_WIDTH, WINDOW_HEIGTH, SDL_WINDOW_OPENGL );
	 	if ( window == NULL ) {
	 		fprintf ( stderr, "Az ablakot nem sikerült létrehozni! SDL_Error: %s\n", SDL_GetError() );
	 	}
	 	
	 	//SDL renderer létrehozása
	 	render = SDL_CreateRenderer ( window, -1, SDL_RENDERER_ACCELERATED );

	 	//Window surface
		screenSurface = SDL_GetWindowSurface ( window );

		//Kitölti az ablakot fehérrel
		SDL_FillRect ( screenSurface, NULL, SDL_MapRGB ( screenSurface->format, 0xFF, 0xFF, 0xFF ) );

	 	//Update the surface 
	 	SDL_UpdateWindowSurface ( window );
	 	
	 	//Main loop! :)
		while( quit ) {
			if( SDL_PollEvent( &event ) != 0 ) {
				if( event.type == SDL_QUIT ) { 
					quit = false;
				}
				if (event.type == SDL_KEYDOWN) {
					//A q billentyű hatására kilép, ideiglenes megoldás.
					if( event.key.keysym.sym == SDLK_q ) {
						printf( "Quit!\n" );
						exit(1);
					}
					//Ellenőrzi a billentyűlenyomást és megváltoztatja irányt annak megfelelően
					if ( ( dir.DOWN == false && event.key.keysym.scancode == SDL_SCANCODE_UP ) || 
						( dir.S == false && event.key.keysym.scancode == SDL_SCANCODE_W ) ) {
						dir.UP = true;
						dir.W = true;
						
						dir.LEFT = false;
						dir.A = false;
						
						dir.RIGHT = false;
						dir.D = false;
						
						dir.DOWN = false;
						dir.S = false;
						
						printf( "UP & W\n" );
					}
					else if ( ( dir.RIGHT == false && event.key.keysym.scancode == SDL_SCANCODE_LEFT ) || 
						( dir.D == false && event.key.keysym.scancode == SDL_SCANCODE_A ) ) {
						dir.UP = false;
						dir.W = false;
						
						dir.LEFT = true;
						dir.A = true;
						
						dir.RIGHT = false;
						dir.D = false;
						
						dir.DOWN = false;
						dir.S = false;
						
						printf( "LEFT & A\n" );						
					}
					else if ( ( dir.UP == false && event.key.keysym.scancode == SDL_SCANCODE_DOWN ) ||
						( dir.W == false && event.key.keysym.scancode == SDL_SCANCODE_S ) ) {
						dir.UP = false;
						dir.W = false;
						
						dir.LEFT = false;
						dir.A = false;
						
						dir.RIGHT = false;
						dir.D = false;
						
						dir.DOWN = true;
						dir.S = true;
						
						printf( "DOWN & S\n" );						
					}
					else if ( ( dir.LEFT == false && event.key.keysym.scancode == SDL_SCANCODE_RIGHT ) || 
						( dir.A == false && event.key.keysym.scancode == SDL_SCANCODE_D ) ) {
						dir.UP = false;
						dir.W = false;
						
						dir.LEFT = false;
						dir.A = false;
						
						dir.RIGHT = true;
						dir.D = true;
						
						dir.DOWN = false;
						dir.S = false;
						
						printf( "RIGHT & D\n" );						
					}				
				}

/*
				else if( event.type == SDL_KEYDOWN ){
					switch( event.key.keysym.sym ){
						case SDLK_w:
						case SDLK_UP:
							printf( "W & UP\n" );
							break;
						case SDLK_a:
						case SDLK_LEFT:
							printf( "A & LEFT\n" );
							break;
						case SDLK_s:
						case SDLK_RIGHT:
							printf( "S & RIGHT\n" );
							break;
						case SDLK_d:
						case SDLK_DOWN:
							printf( "D & DOWN\n" );
							break;
						case SDLK_q:
							SDL_DestroyWindow( window );
							exit(1);
							break;
					}
				}*/
			}
			else{
				SDL_Delay( 50 );
			}
		}
	}

    SDL_RenderPresent( render );
	SDL_DestroyWindow( window );
	SDL_Quit();

	return 0;
}