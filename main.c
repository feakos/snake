#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <SDL2/SDL.h>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGTH = 800;

int main ( int argc, char* args[] ){
	
	bool quit = true;
	SDL_Event event;

	//Window
 	SDL_Window* window = NULL;

	//Surface
 	SDL_Surface* screenSurface = NULL;
	
	//Render
	SDL_Renderer *render;

    //Booleans to control the movement
	bool UP = false;
	bool W = false;
	bool DOWN = false;
	bool S = false;
	bool RIGHT = false;
	bool D = false;
	bool LEFT = false;
	bool A = false;

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
	 	
	 	render = SDL_CreateRenderer (window, -1, SDL_RENDERER_ACCELERATED);


	 	//Window surface
		screenSurface = SDL_GetWindowSurface ( window );
		//Kitölti az ablakot fehérrel
		SDL_FillRect ( screenSurface, NULL, SDL_MapRGB ( screenSurface->format, 0xFF, 0xFF, 0xFF ) );
	 	//Update the surface 
	 	SDL_UpdateWindowSurface ( window );
	 	
		while( quit ) {
			if( SDL_PollEvent( &event ) != 0 ) {
				if( event.type == SDL_QUIT ) { 
					quit = false;
				}
				if (event.type == SDL_KEYDOWN) {
					if( event.key.keysym.sym == SDLK_q ) {
						printf( "Quit!\n" );
						exit(1);
					}
					//Then check for the key being pressed and change direction accordingly
					if ( ( DOWN == false && event.key.keysym.scancode == SDL_SCANCODE_UP ) || 
						( S == false && event.key.keysym.scancode == SDL_SCANCODE_W ) ) {
						UP = true;
						W = true;
						
						LEFT = false;
						A = false;
						
						RIGHT = false;
						D = false;
						
						DOWN = false;
						S = false;
						
						printf( "UP & W\n" );
					}
					else if ( ( RIGHT == false && event.key.keysym.scancode == SDL_SCANCODE_LEFT ) || 
						( D == false && event.key.keysym.scancode == SDL_SCANCODE_A ) ) {
						UP = false;
						W = false;
						
						LEFT = true;
						A = true;
						
						RIGHT = false;
						D = false;
						
						DOWN = false;
						S = false;
						
						printf( "LEFT & A\n" );						
					}
					else if ( ( UP == false && event.key.keysym.scancode == SDL_SCANCODE_DOWN ) ||
						( W == false && event.key.keysym.scancode == SDL_SCANCODE_S ) ) {
						UP = false;
						W = false;
						
						LEFT = false;
						A = false;
						
						RIGHT = false;
						D = false;
						
						DOWN = true;
						S = true;
						
						printf( "DOWN & S\n" );						
					}
					else if ( ( LEFT == false && event.key.keysym.scancode == SDL_SCANCODE_RIGHT ) || 
						( A == false && event.key.keysym.scancode == SDL_SCANCODE_D ) ) {
						UP = false;
						W = false;
						
						LEFT = false;
						A = false;
						
						RIGHT = true;
						D = true;
						
						DOWN = false;
						S = false;
						
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