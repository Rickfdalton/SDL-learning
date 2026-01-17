#include <iostream>
#include <SDL2/SDL.h>
using namespace std;



const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

enum KeyPressSurfaces{
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};
SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gImage = NULL;
SDL_Surface* gKeyPressSurfaces[ KEY_PRESS_SURFACE_TOTAL ];
SDL_Surface* gCurrentSurface = NULL;

bool init();
bool loadMedia(string* images_array);
void close();
SDL_Surface* loadSurface(string path); 

int main (int argc, char* argv[]) {
    if (!init()){
        cout << "Failed to initialize! " << endl;
    }else{
        string image_up = "/Users/jathavanmahendrarajah/Desktop/learn-SDL/images/up.bmp";
        string image_down = "/Users/jathavanmahendrarajah/Desktop/learn-SDL/images/down.bmp";
        string image_left = "/Users/jathavanmahendrarajah/Desktop/learn-SDL/images/left.bmp";
        string image_right = "/Users/jathavanmahendrarajah/Desktop/learn-SDL/images/right.bmp";
        string image_press = "/Users/jathavanmahendrarajah/Desktop/learn-SDL/images/press.bmp";

        string images[5] = {image_up,image_down,image_left,image_right,image_press};


        if (!loadMedia(images) ){
            cout << "Failed to load media " <<endl;
        } else{
            // load image to screen
            SDL_BlitSurface(gImage,NULL, gScreenSurface, NULL);
            SDL_UpdateWindowSurface(gWindow);

            //this makes the window stays up
            SDL_Event e;
            bool quit = false;
            gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
            while (!quit){
                while (SDL_PollEvent(&e)){
                    if (e.type == SDL_QUIT) {quit = true;}
                    else if (e.type == SDL_KEYDOWN)
                    {
                        switch(e.key.keysym.sym){
                            case SDLK_UP:
                                gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_UP];
                                break;
                            case SDLK_DOWN:
                                gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
                                break;
                            case SDLK_LEFT:
                                gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
                                break;
                            case SDLK_RIGHT:
                                gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
                                break;
                            default:
                                gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
                                break;

                        }

                           //Apply the current image
                            SDL_BlitSurface( gCurrentSurface, NULL, gScreenSurface, NULL );
                            //Update the surface
                            SDL_UpdateWindowSurface( gWindow );

                    }
                    
                }
            }
            close();

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

bool loadMedia(string* images_array){
    bool success = true;
    // load up surface
    gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] = loadSurface(images_array[0].c_str());
    if (!gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] ){
        cout << "failed to load up image" << endl;
        success = false;
    }
    // load down surface
    gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = loadSurface(images_array[1].c_str());
    if (!gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] ){
        cout << "failed to load down image" << endl;
        success = false;
    }
    // load left surface
    gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = loadSurface(images_array[2].c_str());
    if (!gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] ){
        cout << "failed to load left image" << endl;
        success = false;
    }
    // load right surface
    gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = loadSurface(images_array[3].c_str());
    if (!gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] ){
        cout << "failed to load right image" << endl;
        success = false;
    }
    // load default surface
    gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = loadSurface(images_array[4].c_str());
    if (!gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] ){
        cout << "failed to load default image" << endl;
        success = false;
    }
    return success;
}

// load individual surfaces 
SDL_Surface* loadSurface(string path){
    SDL_Surface* surface = SDL_LoadBMP(path.c_str());
    if (!surface){
        cout << "Failed to load image : "<< path << endl;
        cout << SDL_GetError() << endl;
    }
    return surface;
}

//close
void close(){
    SDL_FreeSurface(gImage);
    gImage= NULL;
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    SDL_Quit();
}