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
	SDL_RenderFillRect( render, &p_sp_player );

	// Change color to green
	SDL_SetRenderDrawColor( render, 0, 255, 0, 255 );

	// Render the changes above
	SDL_RenderPresent( render );
*/

/*
Ha megvan az, hogy hány elemű a pálya, akkor lemodulózod % azt az értéket, amit kaptál a rand-tól.

time.h
*/

void l_init_sdl_f ();
/* Ebbe belerakom a createwindow, renderer, init-et, addtimert is */

void l_draw_all_f ();
/* 
	SDL_SetRenderDrawColor ( p_sp_renderer, 255, 255, 255, 255 );
	SDL_RenderClear ( p_sp_renderer );
	draw_player ( p_sp_renderer, &p_sp_player );
	SDL_RenderPresent ( p_sp_renderer );
	SDL_SetRenderDrawColor ( p_sp_renderer, 255, 255, 255, 255 );
	SDL_RenderClear ( p_sp_renderer );
	draw_player ( p_sp_renderer, &p_sp_player );
	l_draw_food_f ( p_sp_renderer, &food);
	SDL_RenderPresent ( p_sp_renderer );
*/

/* food, player init egy függvényből legyen */


/* ---------------------- INCLUDES ---------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

/* ---------------------- DEFINE & TYPES ---------------------- */
#define L_U32_WINDOW_WIDTH 400
#define L_U32_WINDOW_HEIGTH 400
#define L_U32_SCALE 20
#define L_U32_MAX_SNAKE_LENGTH 100

/* Directions */
typedef enum { 
	/*  */
	DIR_NOCHANGE = -2,
	/*  */
	DIR_STOP = -1,
	/*  */
	DIR_UP = 0,
	/*  */
	DIR_LEFT = 1,
	/*  */
	DIR_DOWN = 2,
	/*  */
	DIR_RIGHT = 3,
 } l_e_direction_t;

/*  */
typedef struct {
	/*  */
	l_e_direction_t dir;
	/*  */
	l_e_direction_t next_dir; //megnézem a lastot, hogy mi volt, aztán beírom a new-ba.
	/*  */
	l_e_direction_t last_dir; //a lastból olvasom, a next dir szerint lépek és beírom a lastba a nextet.
	/*  */
	SDL_Rect snake[L_U32_MAX_SNAKE_LENGTH];
	/*  */
	uint32_t u32_snake_length;
} l_s_player_t;

/*  */
typedef struct {
	/*  */
	SDL_Rect s_coord;
	/*  */
	bool active;
	/*  */
	uint32_t u32_food_number;
} l_s_food_t;

/* ---------------------- LOCAL FUNCTION PROTOTYPES ---------------------- */
/*  */
static void l_draw_player_f ( SDL_Renderer *p_sp_renderer, const l_s_player_t *p_sp_player );
/*  */
static void l_step_player_f ( l_s_player_t *p_sp_player );
/*  */
static void l_draw_food_f ( SDL_Renderer *p_sp_renderer, const l_s_food_t *p_sp_food );
/*  */
static l_e_direction_t l_get_dir_from_key_f ( SDL_Scancode scancode );
/*  */
static Uint32 l_timer_callback_f ( Uint32 ms, void *param );

/* ---------------------- STATIC VARIABLES ---------------------- */
/*  */
static uint32_t l_u32_timer_limit = 10;
/*  */
static uint32_t l_u32_random_place_x;
/*  */
static uint32_t l_u32_random_place_y;

/* ---------------------- LOCAL FUNCTION "kifejtések" ---------------------- */
static l_e_direction_t l_get_dir_from_key_f ( SDL_Scancode scancode ) {
	l_e_direction_t result = DIR_STOP;

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

static Uint32 l_timer_callback_f ( Uint32 ms, void *param ) {

	SDL_Event event;
	SDL_UserEvent userevent;

	static uint32_t l_u32_timer_cnt = 0;

	l_u32_timer_cnt++;
	if ( l_u32_timer_cnt < l_u32_timer_limit ) {
		return ms;
	}
	l_u32_timer_cnt = 0;

	/* Stacken jön létre. Azokkal az értékekkel jön létre, ami a stack-en volt, ezért ki kellett nullázni! */
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

/* Player kirajzolása */
static void l_draw_player_f ( SDL_Renderer *renderer, const l_s_player_t *p_sp_player ) {
	uint32_t l_u32_index;

	SDL_SetRenderDrawColor ( renderer, 0, 0, 0, 255 );

	for ( l_u32_index = 0; l_u32_index < p_sp_player->u32_snake_length; l_u32_index++ ) {
		SDL_RenderFillRect ( renderer, &p_sp_player->snake[l_u32_index] );
	}

}

/* Player léptetése 1-et fel, balra, jobbra, le */
static void l_step_player_f ( l_s_player_t *p_sp_player ) {
	uint32_t l_u32_index;

	/* A kígyó teste lép egyet */
	if ( p_sp_player->u32_snake_length == 0 )
			return;

	for ( l_u32_index = p_sp_player->u32_snake_length - 1; l_u32_index > 0; l_u32_index-- ){
		p_sp_player->snake[l_u32_index] = p_sp_player->snake[l_u32_index - 1];
	}

	/* A kígyó feje lép egyet (először rálép a fejére a teste. :D ) */
	SDL_Rect *snake_head = &p_sp_player->snake[0];

	if ( DIR_STOP == p_sp_player->dir ) {
    	return;
  	}

	switch ( p_sp_player->dir ) {
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

	/* Itt ha felfele kimegy a képernyőről, akkor alul bejön az y tengelyen. */
	if ( snake_head->y < 0 ) {
        snake_head->y = L_U32_WINDOW_HEIGTH - snake_head->h;
    }
    snake_head->y %= L_U32_WINDOW_HEIGTH; /* Ez meg a maradék, tehát ha alul kimegy, felül bejön... */

	/* Ha a játékos balra kimegy a képernyőről, jobbra bejön és! ha jobbra kimegy, balra bejön... */
	if ( snake_head->x < 0 ) {
        snake_head->x = L_U32_WINDOW_WIDTH - snake_head->w;
    }
    snake_head->x %= L_U32_WINDOW_WIDTH;

}

static void l_draw_food_f ( SDL_Renderer *p_sp_renderer, const l_s_food_t *p_sp_food ) {

	SDL_SetRenderDrawColor ( p_sp_renderer, 255, 0, 0, 255 );
	SDL_RenderFillRect ( p_sp_renderer, &p_sp_food->s_coord );

}

/* ---------------------- STRUCTURE INIT ---------------------- */
/*  */
static l_s_player_t p_sp_player = { 
		.dir = DIR_RIGHT, 
		.u32_snake_length = 3,
		.snake = { 
			{ .x = 3 * L_U32_SCALE, .y = 0, .h = L_U32_SCALE, .w = L_U32_SCALE }, 
			{ .x = 2 * L_U32_SCALE, .y = 0, .h = L_U32_SCALE, .w = L_U32_SCALE }, 
			{ .x = L_U32_SCALE, .y = 0, .h = L_U32_SCALE, .w = L_U32_SCALE }
		}
};

/*  */
static l_s_food_t food;

/**
  *
  *	DOXIGEN
  *
  */

/* ---------------------- MAIN ---------------------- */
int main ( int argc, char* args[] ){
	bool quit = false;

	/* Window */
 	SDL_Window *window = NULL;

	/* Render */
	SDL_Renderer *p_sp_renderer = NULL;

	/* SDL key event */
	SDL_Event event;

	/* Direction default values */
	l_e_direction_t dir = DIR_STOP;

	/* SDL inicializálása */
	if ( SDL_Init ( SDL_INIT_VIDEO | SDL_INIT_TIMER ) < 0 ) {
		fprintf ( stderr, "SDL-t nem sikerült inicializálni! SDL_Error: %s\n", SDL_GetError() );
		exit ( EXIT_FAILURE );
	}

	/* SDL ablak létrehozása */

	window = SDL_CreateWindow ( "SNAKE 1.0b", 
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		L_U32_WINDOW_WIDTH, L_U32_WINDOW_HEIGTH, SDL_WINDOW_OPENGL );
	if ( window == NULL ) {
		fprintf ( stderr, "Az ablakot nem sikerült létrehozni! SDL_Error: %s\n", SDL_GetError() );
		exit (1);
	}

 	/* SDL renderer létrehozása */
 	p_sp_renderer = SDL_CreateRenderer ( window, -1, SDL_RENDERER_ACCELERATED );

 	srand ( time ( NULL ) );

	l_u32_random_place_x = rand() % ( ( ( L_U32_WINDOW_WIDTH  / L_U32_SCALE ) - 1 ) + 1 );
	l_u32_random_place_y = rand() % ( ( ( L_U32_WINDOW_HEIGTH / L_U32_SCALE ) - 1 ) + 1 );

	food.active = true;
	food.u32_food_number = 1;
	food.s_coord.x = L_U32_SCALE * l_u32_random_place_x;
	food.s_coord.y = L_U32_SCALE * l_u32_random_place_y;
	food.s_coord.h = L_U32_SCALE;
	food.s_coord.w = L_U32_SCALE;

	SDL_SetRenderDrawColor ( p_sp_renderer, 255, 255, 255, 255 );
	SDL_RenderClear ( p_sp_renderer );
	l_draw_player_f ( p_sp_renderer, &p_sp_player );
	SDL_RenderPresent ( p_sp_renderer );

	/* Timer */
	SDL_TimerID timer_id = SDL_AddTimer ( 10, l_timer_callback_f, NULL );

 	/* Main loop! :) */
	while ( !quit ) {
		if ( SDL_WaitEvent ( &event ) != 0 ) {
			switch ( event.type ){
				case SDL_QUIT:
					quit = true;
					break;

				case SDL_USEREVENT:
					l_step_player_f ( &p_sp_player );
					break;

				case SDL_KEYDOWN:
					/* Kilépés, speedUP, ...	*/
					switch ( event.key.keysym.sym ) {
						case SDLK_q:
							quit = true;
							break;
						case SDLK_m:
							if ( l_u32_timer_limit > 1 ) {
								l_u32_timer_limit--;
							}
							break;

						case SDLK_n:
							if ( l_u32_timer_limit < 20 ) {
								l_u32_timer_limit++;
							}
							break;
					}

					/* Irányítás */
					l_e_direction_t new_dir = l_get_dir_from_key_f ( event.key.keysym.scancode );
					dir = p_sp_player.dir;
					
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
					p_sp_player.dir = dir;

					break;
			}

			SDL_SetRenderDrawColor ( p_sp_renderer, 255, 255, 255, 255 );
			SDL_RenderClear ( p_sp_renderer );
			l_draw_player_f ( p_sp_renderer, &p_sp_player );
			l_draw_food_f ( p_sp_renderer, &food);
			SDL_RenderPresent ( p_sp_renderer );
		}
		else{
			SDL_Delay ( 1000 );
		}
	}

	SDL_DestroyRenderer ( p_sp_renderer );
	SDL_DestroyWindow ( window );
	SDL_RemoveTimer ( timer_id );
	SDL_Quit ();

	return 0;
}
