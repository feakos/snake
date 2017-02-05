#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <SDL2/SDL.h>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGTH = 800;

int main( int argc, char* args[] ){
	
	bool quit = true;
	SDL_Event e;

	//Window
 	SDL_Window* window = NULL;

	//Surface
 	SDL_Surface* screenSurface = NULL;
	
	//Render
	//SDL_Renderer *render;

	//SDL inicializálása
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
		fprintf( stderr, "SDL-t nem sikerült inicializálni! SDL_Error: %s\n", SDL_GetError() );
	}
	else{

		//SDL ablak létrehozása
		SDL_Window *window = SDL_CreateWindow( "Hello SDL2", 
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
			WINDOW_WIDTH, WINDOW_HEIGTH, SDL_WINDOW_OPENGL );
	 	if( window == NULL ){
	 		fprintf( stderr, "Az ablakot nem sikerült létrehozni! SDL_Error: %s\n", SDL_GetError() );
	 	}
	 	else{
		 	//Window surface
		 	screenSurface = SDL_GetWindowSurface( window );
		 	
		 	//Kitölti az ablakot fehérrel
		 	SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );

		 	//Update the surface 
		 	SDL_UpdateWindowSurface( window );
	 	}

		while( quit ){
			if( SDL_PollEvent( &e ) != 0 ) {
				if( e.type == SDL_QUIT ) { 
					quit = false;
				}
				else if( e.type == SDL_KEYDOWN ){
					switch( e.key.keysym.sym ){
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
				}
			}
			else{
				SDL_Delay( 50 );
			}
		}
	}
	
	SDL_DestroyWindow( window );
	SDL_Quit();

	return 0;
}