#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

int main( int argc, char* args[] ){
	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		printf("%s\n", SDL_GetError());
		return 1;
	}
	SDL_Quit();
	return 0;
}