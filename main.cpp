#include <iostream>
#include <SDL2/SDL.h>
using namespace std;

bool init();
bool loadMedia(char* path);
void close();

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gImage = NULL;


int main (int argc, char* argv[]) {
    if (!init()){
        cout << "Failed to initialize! " << endl;
    }else{
        char path[] = "/Users/jathavanmahendrarajah/Desktop/learn-SDL/sample.bmp";
        if (!loadMedia(path)){
            cout << "Failed to load media " <<endl;
        } else{
            // load image to screen
            SDL_BlitSurface(gImage,NULL, gScreenSurface, NULL);
            SDL_UpdateWindowSurface(gWindow);

            //this makes the window stays up
            SDL_Event e;
            bool quit = false;
            while (quit == false){
                while (SDL_PollEvent(&e)){
                    if (e.type == SDL_QUIT) {quit = true;}
                }
            }
        }
    }
    return 0;
}

//initialization function
bool init(){
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO)<0){
        cout << "failed to initialize SDL due to error " << SDL_GetError() << endl;
        success = false;
    }else{
        gWindow = SDL_CreateWindow("Hello Window",  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,WINDOW_WIDTH,WINDOW_HEIGHT , SDL_WINDOW_SHOWN);
        if (gWindow == NULL){
            cout << "window could not be created due to error " << SDL_GetError() << endl;
            success = false;
        }
        else{
            gScreenSurface = SDL_GetWindowSurface(gWindow);
            success = true;
        }
    }
    return success;
}

// function to load image
bool loadMedia(char* path){
    bool success = true;
    //load image
    gImage = SDL_LoadBMP(path);
    if (gImage == NULL){
        cout << "Image load failed due to error :" << SDL_GetError() << endl;
        success = false;
    }
    return success;
}

//close
void close(){
    SDL_FreeSurface(gImage);
    gImage= NULL;
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    SDL_Quit();
}