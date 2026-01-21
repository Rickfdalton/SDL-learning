#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

/**
 * color keying is treating a specific color in an image as transparent
 */

#include "LTexture.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 400;

bool load_media();
void close();
bool init();

SDL_Window* gWindow = NULL;
LTexture gFooTexture; //front texture
LTexture gBackTexture; //bg texture
SDL_Renderer* gRenderer = NULL;


/*
Implementation of the LTexture class
*/
LTexture::LTexture(){
    m_texture = NULL;
    m_height = 0;
    m_width = 0;
}

LTexture::~LTexture(){
    free();
}

bool LTexture::load_from_file(string path){
    free();
    SDL_Surface* loadedSurface = NULL;
    SDL_Texture* newTexture = NULL;
    loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL){
        cout << "unable to load image" << endl;
        cout << SDL_GetError() << endl;
    } else{
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0,0xFF,0xFF));
        newTexture = SDL_CreateTextureFromSurface(gRenderer,loadedSurface);
        if (!newTexture) {
            cout<< "unable to create texture" <<endl;
            cout<< SDL_GetError() <<endl;
        }else{
            m_height = loadedSurface->h;
            m_width = loadedSurface->w;
            SDL_FreeSurface(loadedSurface);
        }
    }
    m_texture= newTexture;
    return m_texture !=NULL;
}

void LTexture::free() {
    if(m_texture){
        SDL_DestroyTexture(m_texture);
        m_texture= NULL;
        m_height=0;
        m_width=0;
    }
}

void LTexture::render(int x, int y){
    SDL_Rect render_quad= {x,y,m_width,m_height};
    SDL_GetRendererOutputSize(gRenderer, &x, &y);
    SDL_RenderCopy( gRenderer, m_texture, NULL, &render_quad );

}

/*
load 2  LTextures for 2 images
*/
bool load_media(){
    bool success = true;
    
    if (!gFooTexture.load_from_file("images/Group 1.png")){
        cout<< "failed to load foo" <<endl;
        success=false;
    }
    if (!gBackTexture.load_from_file("images/bg.png")){
        cout<< "failed to load bg" <<endl;
        success=false;
    }
    return success;
}

/*
de allocators
*/

void close(){
    gFooTexture.free();
    gBackTexture.free();

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow=NULL;
    gRenderer=NULL;

    IMG_Quit();
    SDL_Quit();
}


/*
initialization function
*/
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
                SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 0);

                //initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)){
                    cout<< "SDL_image could not initialize! SDL_image Error:" << IMG_GetError() <<endl;
                    success = false;
                }
                success = true;
            }
        }
    }
    return success;
}



/*
Main program
*/
int main (int argc, char* argv[]) {
    if (!init()){
        cout << "Failed to initialize! " << endl;
    }else{
        if (!load_media() ){
            cout << "Failed to load media " <<endl;
        } else{
      
            //this makes the window stays up
            SDL_Event e;
            bool quit = false;
            while (!quit){
                while (SDL_PollEvent(&e)){
                    if (e.type == SDL_QUIT) {quit = true;}  
                }
                SDL_SetRenderDrawColor(gRenderer,0xFF,0xFF,0xFF,0xFF);
                SDL_RenderClear(gRenderer);

                gBackTexture.render(0,0);
                gFooTexture.render(200,100);

                SDL_RenderPresent(gRenderer);
            }
            close();

        }
    }
    return 0;
}