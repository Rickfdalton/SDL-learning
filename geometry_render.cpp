#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
using namespace std;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;


SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gCurrentSurface = NULL;
SDL_Texture* gTexture = NULL;
SDL_Renderer* gRenderer = NULL;

bool init();

void close();
SDL_Surface* loadSurface(string path); 
SDL_Texture* loadTexture(string path); 


int main (int argc, char* argv[]) {
    if (!init()){
        cout << "Failed to initialize! " << endl;
    }else{
      
            //this makes the window stays up
            SDL_Event e;
            bool quit = false;
            while (!quit){
                while (SDL_PollEvent(&e)){
                    if (e.type == SDL_QUIT) {quit = true;}
                    //clear screen
                    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                    SDL_RenderClear(gRenderer); 

                    //rendering colored rectangle
                    SDL_Rect fill_rect = {WINDOW_WIDTH/4, WINDOW_HEIGHT/4, WINDOW_WIDTH/2, WINDOW_HEIGHT/2};
                    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
                    SDL_RenderFillRect(gRenderer, &fill_rect);

                    //render green outline
                    SDL_Rect outline_rect = {WINDOW_WIDTH/5, WINDOW_HEIGHT/5, 3* WINDOW_WIDTH/5 , 3*WINDOW_HEIGHT/5};
                    SDL_SetRenderDrawColor(gRenderer,0x00,0xFF,0x00,0xFF);
                    SDL_RenderDrawRect(gRenderer,  &outline_rect);

                    //render blue lines 
                    SDL_SetRenderDrawColor(gRenderer, 0,0,255,1);
                    SDL_RenderDrawLine(gRenderer, 0,0,WINDOW_WIDTH, WINDOW_HEIGHT);
                    SDL_RenderDrawLine(gRenderer, 0,WINDOW_HEIGHT, WINDOW_WIDTH,0);

                    //Draw vertical dotted lines
                    SDL_SetRenderDrawColor(gRenderer, 255,255,0,1);
                    for(int i =WINDOW_HEIGHT/4; i< 3*WINDOW_HEIGHT/4 ; i+=4){
                        SDL_RenderDrawPoint(gRenderer, WINDOW_WIDTH/2, i);
                    }
                    for(int i =WINDOW_WIDTH/4; i< 3*WINDOW_WIDTH/4 ; i+=4){
                        SDL_RenderDrawPoint(gRenderer, i, WINDOW_HEIGHT/2);
                    }

                    //Update screen
                    SDL_RenderPresent( gRenderer );

                }
            }
            close();


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
            //initialize renderer
            gRenderer = SDL_CreateRenderer(gWindow,-1, SDL_RENDERER_ACCELERATED);

            if (gRenderer == NULL){
                cout << "Cannot initiate renderer" << endl;
                cout << SDL_GetError() << endl;
            } else{
                //initialize renderer color
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

                //initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)){
                    cout<< "SDL_image could not initialize! SDL_image Error:" << IMG_GetError() <<endl;
                    success = false;
                }

                gScreenSurface = SDL_GetWindowSurface(gWindow);
                success = true;
            }

    
        }
    }
    return success;
}



// load individual surfaces 
SDL_Surface* loadSurface(string path){
    // SDL_Surface* surface = SDL_LoadBMP(path.c_str());
    SDL_Surface* surface = NULL;
    SDL_Surface* optimized_surface = NULL;

    if (path.substr(path.size()-4)== ".bmp"){
        surface = SDL_LoadBMP(path.c_str());
        if (!surface) {
        cout << "IMG_Load failed: " << SDL_GetError()<< endl;
    }
    }else{
        surface = IMG_Load(path.c_str());
        if (!surface) {
        cout << "IMG_Load failed: " << IMG_GetError() << endl;
    }
    }
    if (surface){
        //optimize surface
        optimized_surface = SDL_ConvertSurface(surface,gScreenSurface->format, 0);

        if (optimized_surface == NULL){
            cout << "Error during image optimization: " << SDL_GetError() << endl;
        }
        SDL_FreeSurface(surface);
    }
    return optimized_surface;
}

SDL_Texture* loadTexture(string path){
    SDL_Texture* newTexture = NULL;
    newTexture = IMG_LoadTexture(gRenderer,path.c_str());
    if (newTexture == NULL){
        cout << "unable to load texture" << endl;
        cout << IMG_GetError() << endl;
    }
    return newTexture;
}

//close
void close(){
    SDL_DestroyTexture(gTexture);
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gTexture = NULL;
    gWindow = NULL;
    gRenderer = NULL;

    IMG_Quit();
    SDL_Quit();
}