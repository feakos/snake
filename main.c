#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGTH = 600;

int main( int argc, char* args[] ){
	
	//SDL inicializálása
	SDL_Init(SDL_INIT_VIDEO);
	
	//SDL ablak létrehozása
	SDL_Window *window = SDL_CreateWindow("Hello SDL2", 
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
		WINDOW_WIDTH, WINDOW_HEIGTH, SDL_WINDOW_OPENGL);


	SDL_Delay(2000);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}