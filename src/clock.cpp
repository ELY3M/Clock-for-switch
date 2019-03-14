/*

Clock for switch 
ELY M. 

*/



//includes
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <switch.h>
#include <iostream>
#include <random>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <time.h>


using namespace std;





const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

const char* const months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
const char* const weekDays[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};



//const float MAX_ANGLE = 3.142 / 5.0;


void Initialize(SDL_Renderer **ren, SDL_Window **win);
void Cleanup(SDL_Renderer **ren, SDL_Window **win);


SDL_Rect charRect[3][26];



//function to render a texture to a surface at x, y, w, y
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w=-1, int h=-1) {
	
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	dest.w = w;
	dest.h = h;
	
	if (w == -1 || h == -1) {
		SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h); //if w and h not set get texture w and h
	}

	SDL_RenderCopy(ren, tex, NULL, &dest); // render the texture

}






int main(int argc, char **argv) {
	

	romfsInit();
	SDL_Init(SDL_INIT_EVERYTHING); //init sdl
	IMG_Init(IMG_INIT_JPG); //init image lib
	


	
	int prevTime = SDL_GetTicks(); // Get ticks
	bool quit = false; // quit variable. if true quits and exits application
	bool paused = false; //if the game is paused or not
	int frames = 0; // frames
	float fps; //fps

	
    //Setup window
    SDL_Window* window = SDL_CreateWindow(nullptr, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP);
	
	//Setup renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	
	
	TTF_Init();
    TTF_Font *font = TTF_OpenFont("romfs:/fonts/FSEX300.ttf", 30);
	SDL_Color Aqua = {0, 255, 255};  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color

	
	
	//Load mice
	SDL_Surface* miceImage = IMG_Load("romfs:/mymice-500x500.jpg");
	SDL_Texture* miceTexture = SDL_CreateTextureFromSurface(renderer, miceImage);

	
	while(appletMainLoop() && !quit) { //main game loop
		
		
		//Scan all the inputs. This should be done once for each frame
		hidScanInput();
		
		//clear screen every update//
		SDL_RenderClear(renderer);
		
		//Clock
		char finaltime[100] = "time...";
		time_t unixTime = time(NULL);
		//struct tm* timeStruct = gmtime((const time_t *)&unixTime);
		struct tm* timeStruct = localtime((const time_t *)&unixTime);

		int hours = timeStruct->tm_hour;
		int minutes = timeStruct->tm_min;
		int seconds = timeStruct->tm_sec;
		int day = timeStruct->tm_mday;
		int month = timeStruct->tm_mon;
		int year = timeStruct->tm_year +1900;
		int wday = timeStruct->tm_wday;
		
		
		//we need to convert timezones...  
		
		
		
		
		snprintf(finaltime, 100, "%i:%i:%i", hours, minutes, seconds);	
		SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, finaltime, Aqua); 
		SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage); //now you can convert it into a texture
		SDL_Rect Message_rect; //create a rect
		Message_rect.x = SCREEN_WIDTH / 2;  //controls the rect's x coordinate 
		Message_rect.y = 10; // controls the rect's y coordinte
		Message_rect.w = 300; // controls the width of the rect
		Message_rect.h = 100; // controls the height of the rect
		SDL_FreeSurface(surfaceMessage);
		SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
		SDL_DestroyTexture(Message);
		//end of clock  
		
		
		//mice image
		renderTexture(miceTexture, renderer, 300, 300, SCREEN_WIDTH, SCREEN_HEIGHT); //render the splash screen
		
		
		
		//load everything
		SDL_RenderPresent(renderer); 
		
		u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
		
		
		
		//quit by press plus button
		if (kDown & KEY_PLUS) {
		quit = true; 
		break; 
		}

			

	}
	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	return 0;
}