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
#include <time.h>
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

typedef struct {
	SDL_Rect coord;
	bool active;
	uint32_t food_number;
} food_t;

uint32_t random_place_x;
uint32_t random_place_y;

static food_t food;

/*
Ha megvan az, hogy hány elemű a pálya, akkor lemodulózod % azt az értéket, amit kaptál a rand-tól.

time.h
*/

static direction_t get_dir_from_key ( SDL_Scancode scancode ) {
	direction_t result = DIR_STOP;

	switch ( scancode ){
		case SDL_SCANCODE_SPACE:
			result = DIR_STOP;
			break;

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

static Uint32 timer_callback ( Uint32 ms, void *param ) {

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
static void draw_player ( SDL_Renderer *renderer, const player_t *player ) {
	uint32_t index;

	SDL_SetRenderDrawColor ( renderer, 0, 0, 0, 255 );

	for ( index = 0; index < player->snake_length; index++ ) {
		SDL_RenderFillRect ( renderer, &player->snake[index] );
	}

}

//Player léptetése 1-et fel, balra, ...
static void step_player ( player_t *player ) {
	uint32_t index;

	//A kígyó teste lép egyet
	if ( player->snake_length == 0 )
			return;

	for ( index = player->snake_length - 1; index > 0; index-- ){

		player->snake[index] = player->snake[index - 1];
	}

	//A kígyó feje lép egyet (először rálép a fejére a teste. :D )
	SDL_Rect *snake_head = &player->snake[0];

	if(DIR_STOP == player->dir) {
    	return;
  	}

	switch ( player->dir ) {
		case DIR_UP:
			snake_head->y -= snake_head->h;
			break;
						
		case DIR_LEFT:
			snake_head->x -= snake_head->w;
			break;
			
		case DIR_DOWN:
			snake_head->y += snake_head->h;
			break;
				
		case DIR_RIGHT:
			snake_head->x += snake_head->w;
			break;
						
		default:
			break;
	}

	//Itt ha felfele kimegy a képernyőről, akkor alul bejön az y tengelyen.
	if ( snake_head->y < 0 ) {
        snake_head->y = WINDOW_HEIGTH - snake_head->h;
    }
    snake_head->y %= WINDOW_HEIGTH; //Ez meg a maradék, tehát ha alul kimegy, felül bejön...

	//Ha a játékos balra kimegy a képernyőről, jobbra bejön és! ha jobbra kimegy, balra bejön...
	if ( snake_head->x < 0 ) {
        snake_head->x = WINDOW_WIDTH - snake_head->w;
    }
    snake_head->x %= WINDOW_WIDTH;

}

static void draw_food ( SDL_Renderer *renderer, const food_t *food ) {

	SDL_SetRenderDrawColor ( renderer, 255, 0, 0, 255 );
	SDL_RenderFillRect ( renderer, &food->coord );

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

	//SDL inicializálása
	if( SDL_Init ( SDL_INIT_VIDEO | SDL_INIT_TIMER ) < 0 ){
		fprintf ( stderr, "SDL-t nem sikerült inicializálni! SDL_Error: %s\n", SDL_GetError() );
		exit ( EXIT_FAILURE );
	}

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

 	srand ( time ( NULL ) );

	random_place_x = rand() % ( ( ( WINDOW_WIDTH  / SCALE ) - 1 ) + 1 );
	random_place_y = rand() % ( ( ( WINDOW_HEIGTH / SCALE ) - 1 ) + 1 );

	food.active = true;
	food.food_number = 1;
	food.coord.x = SCALE * random_place_x;
	food.coord.y = SCALE * random_place_y;
	food.coord.h = SCALE;
	food.coord.w = SCALE;

	SDL_SetRenderDrawColor ( renderer, 255, 255, 255, 255 );
	SDL_RenderClear ( renderer );
	draw_player ( renderer, &player );
	SDL_RenderPresent ( renderer );

	//Timer
	SDL_TimerID timer_id = SDL_AddTimer ( 10, timer_callback, NULL );

 	//Main loop! :)
	while ( !quit ) {
		if ( SDL_WaitEvent ( &event ) != 0 ) {
			switch ( event.type ){
				case SDL_QUIT:
					quit = true;
					break;

				case SDL_USEREVENT:
					step_player ( &player );
					break;

				case SDL_KEYDOWN:
					//Kilépés, speedUP, ...				
					switch ( event.key.keysym.sym ) {
						case SDLK_q:
							quit = true;
							break;
						case SDLK_m:
							if ( timer_limit > 1 ) {
								timer_limit--;
							}
							break;

						case SDLK_n:
							if ( timer_limit < 20 ) {
								timer_limit++;
							}
							break;
					}

					//Irányítás
					direction_t new_dir = get_dir_from_key ( event.key.keysym.scancode );
					dir = player.dir;
					
					switch ( new_dir ) {
						case DIR_STOP:
							dir = new_dir;
							break;

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
					player.dir = dir;

					break;
			}

			SDL_SetRenderDrawColor ( renderer, 255, 255, 255, 255 );
			SDL_RenderClear ( renderer );
			draw_player ( renderer, &player );
			draw_food ( renderer, &food);
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