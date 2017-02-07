/*
	https://www.youtube.com/watch?v=Vhw_GVfFzGE#t=167.294189
	https://www.youtube.com/watch?v=HfX8ByUXfPY
	https://www.youtube.com/watch?v=rR7xWyUWcBA
*/

/*
	// Clear the window and make it all green
	SDL_RenderClear( render );

	// Change color to blue
	SDL_SetRenderDrawColor( render, 0, 0, 255, 255 );

	// Render our "player"
	SDL_RenderFillRect( render, &player );

	// Change color to green
	SDL_SetRenderDrawColor( render, 0, 255, 0, 255 );

	// Render the changes above
	SDL_RenderPresent( render );
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h> //Main SDL library
#include <SDL2/SDL_ttf.h> //Optional SDL library used to display text using renderers

const uint32_t WINDOW_WIDTH = 800;
const uint32_t WINDOW_HEIGTH = 800;

//Directions
typedef enum { 
	DIR_NOCHANGE = -2,
	DIR_STOP = -1,
	DIR_UP = 0,
	DIR_LEFT = 1,
	DIR_DOWN = 2,
	DIR_RIGHT = 3,
 } direction;

static direction get_dir_from_key ( SDL_Scancode scancode ) {
	direction result = DIR_STOP;

	switch ( scancode ){
		case SDL_SCANCODE_UP:
		case SDL_SCANCODE_W:
			result = DIR_UP;
			break;
		case SDL_SCANCODE_LEFT:
		case SDL_SCANCODE_A:
			result = DIR_LEFT;
			break;
		case SDL_SCANCODE_DOWN:
		case SDL_SCANCODE_S:
			result = DIR_DOWN;
			break;
		case SDL_SCANCODE_RIGHT:
		case SDL_SCANCODE_D:
			result = DIR_RIGHT;
			break;
		default:
			result = DIR_NOCHANGE;
			break;
	}
	return result;
}

void init();
void destroy();

bool CreateWindow ( ) {
	SDL_Window *window = SDL_CreateWindow ( "SNAKE beta 1.0", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
			WINDOW_WIDTH, WINDOW_HEIGTH, SDL_WINDOW_OPENGL );

	if ( window == NULL ) {
	 	fprintf ( stderr, "Az ablakot nem sikerült létrehozni! SDL_Error: %s\n", SDL_GetError() );
		return false;
	}

	return true;
}

bool CreateRenderer ( SDL_Window *window ) {
	SDL_Renderer *renderer = SDL_CreateRenderer ( window, -1, SDL_RENDERER_ACCELERATED );

	if ( renderer == NULL ) {
	 	fprintf ( stderr, "A renderer-t nem sikerült létrehozni! SDL_Error: %s\n", SDL_GetError() );
		return false;
	}

	return true;
}

void renderPlayer ( SDL_Renderer *renderer, SDL_Rect player, int x, int y, int scale ) {
	player.w = scale;
	player.h = scale;

	SDL_SetRenderDrawColor ( renderer, 0, 0, 0, 0 );

	player.x = x;
	player.y = y;

	SDL_RenderFillRect ( renderer, &player );
}

void renderScore ();
void createMap ();
bool checkColloison ();

int main ( int argc, char* args[] ){
	bool quit = true;

	//Window
 	SDL_Window *window = NULL;

	//Render
	SDL_Renderer *render = NULL;

	//SDL key event
	SDL_Event event;

	//Direction default values
	direction dir = DIR_STOP;

	//Player
	int scale = 20;
	SDL_Rect player = { 0, 0, scale, scale };

	//SDL inicializálása
	if( SDL_Init ( SDL_INIT_VIDEO ) < 0 ){
		fprintf ( stderr, "SDL-t nem sikerült inicializálni! SDL_Error: %s\n", SDL_GetError() );
	}
	else{
		//SDL window és renderer egyszerre
		//SDL_CreateWindowAndRenderer ( );

		//SDL ablak létrehozása
		window = SDL_CreateWindow ( "Hello SDL2", 
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
			WINDOW_WIDTH, WINDOW_HEIGTH, SDL_WINDOW_OPENGL );
	 	if ( window == NULL ) {
	 		fprintf ( stderr, "Az ablakot nem sikerült létrehozni! SDL_Error: %s\n", SDL_GetError() );
	 		exit (1);
	 	}

	 	//SDL renderer létrehozása
	 	render = SDL_CreateRenderer ( window, -1, SDL_RENDERER_ACCELERATED );

		//SDL_RenderClear ( render );
		SDL_SetRenderDrawColor ( render, 255, 255, 255, 255 );
		SDL_RenderFillRect ( render, &player);
		//SDL_SetRenderDrawColor ( render, 0, 0, 0, 0 );
		//SDL_RenderPresent ( render );
	 	
	 	//Main loop! :)
		while ( quit ) {
			SDL_RenderClear ( render );
			if ( SDL_PollEvent ( &event ) != 0 ) {
				if ( event.type == SDL_QUIT ) { 
					quit = false;
					exit ( EXIT_FAILURE );
				}
				if (event.type == SDL_KEYDOWN) {
					//A q billentyű hatására kilép, ideiglenes megoldás.
					if( event.key.keysym.sym == SDLK_q ) {
						printf ( "Quit!\n" );
						exit ( EXIT_FAILURE );
					}

					direction new_dir = get_dir_from_key ( event.key.keysym.scancode );
					switch ( new_dir ) {
						case DIR_UP:
							if ( dir != DIR_DOWN ) {
								dir = new_dir;
								player.y -= 10; 
							}
							break;
						case DIR_LEFT:
							if ( dir != DIR_RIGHT ){ 
								dir = new_dir;
								player.x -= 10; 
							}
							break;
						case DIR_DOWN:
							if ( dir != DIR_UP ) {
								dir = new_dir;
								player.y += 10; 
							}
							break;
						case DIR_RIGHT:
							if ( dir != DIR_LEFT ) {
								dir = new_dir;
								player.x += 10; 
							}
							break;
						default:
							break;
					}
				}
				
				renderPlayer ( render, player, player.x, player.y, scale );
				SDL_SetRenderDrawColor ( render, 255, 255, 255, 255 );
				SDL_RenderPresent ( render );

			}
			else{
				SDL_Delay (50);
			}
		}
	}

    SDL_DestroyRenderer ( render );
	SDL_DestroyWindow ( window );
	SDL_Quit ();

	return 0;
}