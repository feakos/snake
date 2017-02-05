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

	//SDL inicializálása
	SDL_Init( SDL_INIT_VIDEO );
	
	//SDL ablak létrehozása
	SDL_Window *window = SDL_CreateWindow( "Hello SDL2", 
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
		WINDOW_WIDTH, WINDOW_HEIGTH, SDL_WINDOW_OPENGL );
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
	}
	
	SDL_DestroyWindow( window );
	SDL_Quit();
	return 0;
}