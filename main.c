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

#define WINDOW_WIDTH 400
#define WINDOW_HEIGTH 400
#define SCALE 20
#define MAX_SNAKE_LENGTH 100

//Azért uint32_t, mert ez biztos 32 bites!
static uint32_t timer_limit = 10;

//Directions
typedef enum { 
	DIR_NOCHANGE = -2,
	DIR_STOP = -1,
	DIR_UP = 0,
	DIR_LEFT = 1,
	DIR_DOWN = 2,
	DIR_RIGHT = 3,
 } direction_t;

typedef struct {
	direction_t dir;
	SDL_Rect snake[MAX_SNAKE_LENGTH];
	uint32_t snake_length;
} player_t;

static player_t player = { 
		.dir = DIR_RIGHT, 
		.snake_length = 3,
		.snake = { 
			{ .x = 3 * SCALE, .y = 0, .h = SCALE, .w = SCALE }, 
			{ .x = 2 * SCALE, .y = 0, .h = SCALE, .w = SCALE }, 
			{ .x = SCALE, .y = 0, .h = SCALE, .w = SCALE }
		}
};

#if 0
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
#endif

static direction_t get_dir_from_key ( SDL_Scancode scancode ) {
	direction_t result = DIR_STOP;

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

	static uint32_t timer_cnt = 0;

	timer_cnt++;
	if ( timer_cnt < timer_limit ) {
		return ms;
	}
	timer_cnt = 0;

	//Stacken jön létre. Azokkal az értékekkel jön létre, ami a stack-en volt, ezért ki kellett nullázni!
	memset ( &event, 0, sizeof ( event ) );
	memset ( &userevent, 0, sizeof ( userevent ) );

	userevent.type = SDL_USEREVENT;
	userevent.code = 0;
	userevent.data1 = NULL;
	userevent.data2 = NULL;

	event.type = SDL_USEREVENT;
	event.user = userevent;

	SDL_PushEvent ( &event );

	return ( ms );
}

#if 0
//Player törlése, háttér kirajzolása, nem kell a függvény lassan.

void clear_player ( SDL_Renderer *renderer, const SDL_Rect *player ) {

	SDL_SetRenderDrawColor ( renderer, 255, 255, 255, 255 );
	SDL_RenderFillRect ( renderer, player );

}
#endif

//Player kirajzolása
void draw_player ( SDL_Renderer *renderer, const player_t *player ) {
	int index;

	SDL_SetRenderDrawColor ( renderer, 0, 0, 0, 255 );

	for ( index = 0; index < player->snake_length; index++ ) {
		SDL_RenderFillRect ( renderer, &player->snake[index] );
	}

}

//Player léptetése 1-et fel, balra, ...
void step_player ( SDL_Rect *player, direction_t dir ) {
	
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

	//Itt ha felfele kimegy a képernyőről, akkor alul bejön az y tengelyen.
	if ( player->y < 0 ) {
        player->y = WINDOW_HEIGTH - player->h;
    }
    player->y %= WINDOW_HEIGTH; //Ez meg a maradék, tehát ha alul kimegy, felül bejön...

	//Ha a játékos balra kimegy a képernyőről, jobbra bejön és! ha jobbra kimegy, balra bejön...
	if ( player->x < 0 ) {
        player->x = WINDOW_WIDTH - player->w;
    }
    player->x %= WINDOW_WIDTH;

}

#if 0
//A player mozgatása
void move_player ( SDL_Renderer *renderer, SDL_Rect *player, direction_t dir ) {

	clear_player ( renderer, player );
	step_player ( player, dir );
	draw_player ( renderer, player );

}
#endif

int main ( int argc, char* args[] ){
	bool quit = false;

	//Window
 	SDL_Window *window = NULL;

	//Render
	SDL_Renderer *renderer = NULL;

	//SDL key event
	SDL_Event event;

	//Direction default values
	direction_t dir = DIR_STOP;

	//Player
	//SDL_Rect player = { 0, 0, SCALE, SCALE };

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
	//draw_player ( renderer, &player );
	SDL_RenderPresent ( renderer );

	//Timer
	SDL_TimerID timer_id = SDL_AddTimer ( 10, timer_callback, NULL );

 	//Main loop! :)
	while ( !quit ) {
		if ( SDL_WaitEvent ( &event ) != 0 ) {
			if ( event.type == SDL_QUIT ) { 
				quit = true;
			}			
			
			if ( event.type == SDL_USEREVENT ) { 
				//step_player ( &player, dir );
			}			

			if ( event.type == SDL_KEYDOWN ) {

				//Kilépés, speedUP, ...				
				switch ( event.key.keysym.sym ){
					case SDLK_q:
						quit = true;
						break;
					case SDLK_m:
						if ( timer_limit > 1 ) timer_limit--;
						break;

					case SDLK_n:
						if ( timer_limit < 20 ) timer_limit++;
						break;
				}

				//Irányítás
				direction_t new_dir = get_dir_from_key ( event.key.keysym.scancode );
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
			}
			SDL_SetRenderDrawColor ( renderer, 255, 255, 255, 255 );
			SDL_RenderClear ( renderer );
			//draw_player ( renderer, &player );
			SDL_RenderPresent ( renderer );
		}
		else{
			SDL_Delay ( 1000 );
		}
	}

	SDL_DestroyRenderer ( renderer );
	SDL_DestroyWindow ( window );
	SDL_RemoveTimer ( timer_id );
	SDL_Quit ();

	return 0;
}