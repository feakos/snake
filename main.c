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

//Azért uint32_t, mert ez biztos 32 bites!
#define WINDOW_WIDTH 800
#define WINDOW_HEIGTH 800

#define SPEED 5

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

Uint32 timer_callback ( Uint32 ms, void *param ) {
    SDL_Event event;
    SDL_UserEvent userevent;

    /* In this example, our callback pushes an SDL_USEREVENT event
    into the queue, and causes our callback to be called again at the
    same interval: 
    */

 	// Ez nem tuti, hogy jó    event.type = SDL_USEREVENT;
    userevent.type = SDL_USEREVENT;
    //userevent.code = 0;
    //userevent.data1 = NULL;
    //userevent.data2 = NULL;

    event.type = SDL_USEREVENT;
    event.user = userevent;

    SDL_PushEvent ( &event );
    printf ( "Fuss köcsög\n" );

    return ( ms );
}


void init();
void destroy();

/*

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

*/

void clear_player ( SDL_Renderer *renderer, const SDL_Rect *player ) {

	SDL_SetRenderDrawColor ( renderer, 255, 255, 255, 255 );
	SDL_RenderFillRect ( renderer, player );

}

void draw_player ( SDL_Renderer *renderer, const SDL_Rect *player ) {

	SDL_SetRenderDrawColor ( renderer, 0, 0, 0, 255 );
	SDL_RenderFillRect ( renderer, player );

}

void step_player ( SDL_Rect *player, direction dir ) {
	switch ( dir ) {
		case DIR_UP:
			player->y -= player->h;
			break;
						
		case DIR_LEFT:
			player->x -= player->w;
			break;
			
		case DIR_DOWN:
			player->y += player->h;
			break;
				
		case DIR_RIGHT:
			player->x += player->w;
			break;
						
		default:
			break;
	}

	if ( player->y < 0 ) {
        player->y = WINDOW_HEIGTH - player->h;
    }
    player->y %= WINDOW_HEIGTH;

	if ( player->x < 0 ) {
        player->x = WINDOW_WIDTH - player->w;
    }
    player->x %= WINDOW_WIDTH;

}

void move_player ( SDL_Renderer *renderer, SDL_Rect *player, direction dir ) {
	clear_player ( renderer, player );
	step_player ( player, dir );
	draw_player ( renderer, player );
}

void render_score ();
void create_map ();
bool check_colloison ();

int main ( int argc, char* args[] ){
	bool quit = false;

	//Window
 	SDL_Window *window = NULL;

	//Render
	SDL_Renderer *renderer = NULL;

	//SDL key event
	SDL_Event event;

	//Direction default values
	direction dir = DIR_STOP;

	//Player
	uint32_t scale = 20;
	SDL_Rect player = { 0, 0, scale, scale };

	//SDL inicializálása
	if( SDL_Init ( SDL_INIT_VIDEO | SDL_INIT_TIMER ) < 0 ){
		fprintf ( stderr, "SDL-t nem sikerült inicializálni! SDL_Error: %s\n", SDL_GetError() );
		exit ( EXIT_FAILURE );
	}
	//SDL window és renderer egyszerre
	//SDL_CreateWindowAndRenderer ( );

	//SDL ablak létrehozása
	window = SDL_CreateWindow ( "SNAKE 1.0b", 
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		WINDOW_WIDTH, WINDOW_HEIGTH, SDL_WINDOW_OPENGL );
	if ( window == NULL ) {
		fprintf ( stderr, "Az ablakot nem sikerült létrehozni! SDL_Error: %s\n", SDL_GetError() );
		exit (1);
	}

 	//SDL renderer létrehozása
 	renderer = SDL_CreateRenderer ( window, -1, SDL_RENDERER_ACCELERATED );

	SDL_SetRenderDrawColor ( renderer, 255, 255, 255, 255 );
	SDL_RenderClear ( renderer );
	draw_player ( renderer, &player );
	clear_player ( renderer, &player );
	step_player ( &player, DIR_DOWN );

	draw_player ( renderer, &player );	
	SDL_RenderPresent ( renderer );

	//Timer
	SDL_TimerID timer_id = SDL_AddTimer ( 1000, timer_callback, NULL );

 	//Main loop! :)
	while ( !quit ) {
		if ( SDL_WaitEvent ( &event ) != 0 ) {
			if ( event.type == SDL_QUIT ) { 
				quit = true;
				exit ( EXIT_FAILURE );
			}			
			if ( event.type == SDL_KEYDOWN ) {
				//A q billentyű hatására kilép, ideiglenes megoldás.
				if ( event.key.keysym.sym == SDLK_q ) {
					printf ( "Quit!\n" );
					exit ( EXIT_FAILURE );
				}

				direction new_dir = get_dir_from_key ( event.key.keysym.scancode );
				switch ( new_dir ) {
					case DIR_UP:
						if ( dir != DIR_DOWN ) {
							dir = new_dir;
						}
						break;
						
					case DIR_LEFT:
						if ( dir != DIR_RIGHT ) {
							dir = new_dir;
						}
						break;
						
					case DIR_DOWN:
						if ( dir != DIR_UP ) {
							dir = new_dir;
						}
						break;
					
					case DIR_RIGHT:
						if ( dir != DIR_LEFT ) {
							dir = new_dir;
						}
						break;
						
					default:
						break;
				}
				move_player ( renderer, &player, dir );
			}
			SDL_RenderPresent ( renderer );
		}
		else{
			SDL_Delay (1000);
		}
	}

	SDL_DestroyRenderer ( renderer );
	SDL_DestroyWindow ( window );
	SDL_RemoveTimer ( timer_id );
	SDL_Quit ();

	return 0;
}