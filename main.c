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
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h> //Main SDL library
#include <SDL2/SDL_ttf.h> //Optional SDL library used to display text using renderers

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGTH = 800;

//Directions
typedef struct {
	bool UP, LEFT, DOWN, RIGHT;
} directions;

void createMap();
void renderPlayer();
void renderScore();
bool checkColloison();

int main ( int argc, char* args[] ){
	bool quit = true;

	//Window
 	SDL_Window* window = NULL;

	//Render
	SDL_Renderer *render = NULL;

	//SDL key event
	SDL_Event event;

	//Direction default values
	directions dir = { 0, 0, 0, 0 };

	//Player
	SDL_Rect player;
	player.w = 20;
	player.h = 20;
	player.x = 0;
	player.y = 0;

	//SDL inicializálása
	if( SDL_Init ( SDL_INIT_VIDEO ) < 0 ){
		fprintf ( stderr, "SDL-t nem sikerült inicializálni! SDL_Error: %s\n", SDL_GetError() );
	}
	else{

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
			if ( SDL_PollEvent ( &event ) != 0 ) {
				if ( event.type == SDL_QUIT ) { 
					quit = false;
					exit (0);
				}
				if (event.type == SDL_KEYDOWN) {
					//A q billentyű hatására kilép, ideiglenes megoldás.
					if( event.key.keysym.sym == SDLK_q ) {
						printf ( "Quit!\n" );
						exit (2);
					}
					//Ellenőrzi a billentyűlenyomást és megváltoztatja irányt annak megfelelően
					if ( ( dir.DOWN == false && event.key.keysym.scancode == SDL_SCANCODE_UP ) || 
						( dir.DOWN == false && event.key.keysym.scancode == SDL_SCANCODE_W ) ) {
						dir.UP = true;
						dir.LEFT = false;
						dir.RIGHT = false;
						dir.DOWN = false;
						--player.y;

						//printf ( "UP & W\n" );
					}
					else if ( ( dir.RIGHT == false && event.key.keysym.scancode == SDL_SCANCODE_LEFT ) || 
						( dir.RIGHT == false && event.key.keysym.scancode == SDL_SCANCODE_A ) ) {
						dir.UP = false;
						dir.LEFT = true;
						dir.RIGHT = false;
						dir.DOWN = false;
						--player.x;

						//printf ( "LEFT & A\n" );						
					}
					else if ( ( dir.UP == false && event.key.keysym.scancode == SDL_SCANCODE_DOWN ) ||
						( dir.UP == false && event.key.keysym.scancode == SDL_SCANCODE_S ) ) {
						dir.UP = false;
						dir.LEFT = false;
						dir.RIGHT = false;
						dir.DOWN = true;
						++player.y;

						//printf ( "DOWN & S\n" );						
					}
					else if ( ( dir.LEFT == false && event.key.keysym.scancode == SDL_SCANCODE_RIGHT ) || 
						( dir.LEFT == false && event.key.keysym.scancode == SDL_SCANCODE_D ) ) {
						dir.UP = false;
						dir.LEFT = false;
						dir.RIGHT = true;
						dir.DOWN = false;
						++player.x;

						//printf ( "RIGHT & D\n" );						
					}

				}
				SDL_RenderClear ( render );
				SDL_SetRenderDrawColor ( render, 0, 0, 0, 0 );
				SDL_RenderFillRect ( render, &player);
				SDL_SetRenderDrawColor ( render, 255, 255, 255, 255 );
				SDL_RenderPresent ( render );

			}
			else{
				SDL_Delay (50);
			}
		}
	}

	SDL_DestroyWindow ( window );
	SDL_Quit ();

	return 0;
}