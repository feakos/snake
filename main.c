#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <SDL2/SDL.h>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGTH = 800;

int main( int argc, char* args[] ){
	
	bool quit = false;
	SDL_Event e;

	//Window
 	SDL_Window* window = NULL;

	//Surface
 	SDL_Surface* screenSurface = NULL;
	
	//SDL inicializálása
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
	}
	else{
		//SDL ablak létrehozása
		SDL_Window *window = SDL_CreateWindow( "Hello SDL2", 
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
			WINDOW_WIDTH, WINDOW_HEIGTH, SDL_WINDOW_OPENGL );
	 	if( window == NULL ){
	 		printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
	 	}
	 	else{
		 	//Get window surface 
		 	screenSurface = SDL_GetWindowSurface( window );
		 	//Fill the surface white
		 	SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );
		 	//Update the surface 
		 	SDL_UpdateWindowSurface( window );
		 	//Wait two seconds
		 	SDL_Delay( 2000 );
	 	}

	/*
		while( !quit ){
			while( SDL_PollEvent( &e ) != 0 ){
				if( e.type == SDL_QUIT ) { 
					quit = true;
				}
			}
			SDL_Delay(50);
		}
		*/

		while( !quit ){
			if(SDL_WaitEvent(&e)) {
				if( e.type == SDL_QUIT ) { 
					quit = true;
				}
			}
			else{
				SDL_Delay(50);
			}
		}
	}
	
	SDL_DestroyWindow( window );
	SDL_Quit();
	return 0;
}