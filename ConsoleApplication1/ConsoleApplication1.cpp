#include<SDL.h>
#include<stdio.h>
#include<SDL_image.h>

#define WIDTH 1920
#define HEIGHT 1080
#define RIGHT 1
#define LEFT 2


SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

void DeInit(int value) {
	if (renderer != NULL) SDL_DestroyRenderer(renderer);
	if (window != NULL) SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
	exit(value);
}


void Init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) !=0) {
		printf("sadadadaddadad\n");
		DeInit(1);
	}
	if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) !=(IMG_INIT_JPG|IMG_INIT_PNG)) {
		printf("sadadadaddadad\n");
		DeInit(1);
	}
	window = SDL_CreateWindow("SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
	if (window == NULL) {
		printf("sadadadaddadad\n");
		printf("could init window , %s", SDL_GetError());
		DeInit(1);
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		printf("sadadadaddadad\n");
		printf("could init window , %s", SDL_GetError());
		DeInit(1);
	}

}

SDL_Texture* loadTexturefromFIle(const char* name, SDL_Rect* rect) {
	SDL_Surface* surface = IMG_Load(name);
	if(surface==NULL){
		printf("sadadadaddadad\n");
		DeInit(1);
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_QueryTexture(texture, &surface->format->format, NULL, NULL, NULL);
	*rect = { 0,0,surface->w,surface->h };
	SDL_FreeSurface(surface);
	return texture;
}


int main(int argc, char** argv) {

	Init();

	SDL_Rect back_rect;
	SDL_Texture* back_tex = loadTexturefromFIle("Forest1.jpg", &back_rect);
	SDL_Rect player_rect;
	SDL_Texture* player_tex = loadTexturefromFIle("spritesheet.png", &player_rect);

	player_rect.w = player_rect.h;
	
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	int running = 1;
	SDL_Event event;
	int angle=0;
	int x = 0;
	int y = 0;
	SDL_Rect dst_rect;
	int frame = 0;
	int frame_count = 6;
	int max_frametime = 100;
	int cur_frametime = 0;
	int lasttime = SDL_GetTicks();
	int newtime;
	int dt=0;
	int direction = 1;

	bool isup, isdown, isleft, isright;
	isup = isdown = isleft = isright = false;
	bool animate = false;
	while (running) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = 0;
			}
			else if (event.type == SDL_KEYDOWN) {
				
				if (event.key.keysym.sym == SDLK_LEFT) {
					 isleft = true;

				}
				else if (event.key.keysym.sym == SDLK_RIGHT) {
					 isright = true;

				}
				else if (event.key.keysym.sym == SDLK_UP) {
					 isup = true;
				} 
				else if (event.key.keysym.sym == SDLK_DOWN) {
					 isdown = true;
				}
			}
			else if (event.type == SDL_KEYUP) {
				if (event.key.keysym.sym == SDLK_LEFT) {
					isleft = false;

				}
				else if (event.key.keysym.sym == SDLK_RIGHT) {
					isright = false;

				}
				else if (event.key.keysym.sym == SDLK_UP) {
					isup = false;
				}
				else if (event.key.keysym.sym == SDLK_DOWN) {
					isdown = false;
				}
			
			}
		}
		if (isup && !isdown) y -= 10, animate = true ;
		if (isdown && !isup) y += 10, animate = true  ;
		if (isleft&&!isright) x -= 10, animate = true,direction=LEFT;
		if (isright&&!isleft)x += 10, animate = true,direction=RIGHT;
		newtime = SDL_GetTicks();
		dt = newtime - lasttime;
		lasttime = newtime;
		cur_frametime += dt;
		if (animate) {
			if (cur_frametime > max_frametime) {
				cur_frametime %= max_frametime;
				frame = (frame + 1) % frame_count;
				player_rect.x = frame * player_rect.w;
			}
		}
		dst_rect = { x,y,player_rect.w,player_rect.h };
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, back_tex ,&back_rect,NULL);

		if(direction == RIGHT){
			SDL_RenderCopy(renderer, player_tex, &player_rect, &dst_rect);
		}
		else {
			SDL_RenderCopyEx(renderer, player_tex, &player_rect, &dst_rect,0,NULL,SDL_FLIP_HORIZONTAL);
		}
		SDL_RenderPresent(renderer);
		SDL_Delay(20);
		animate = false;
	}



	SDL_DestroyTexture(back_tex);
	SDL_DestroyTexture(player_tex);

	DeInit(0);
	return 0;
}