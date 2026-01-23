#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cmath>

/**
 * color keying is treating a specific color in an image as transparent
 */

#include "LTexture.h"

const int WINDOW_WIDTH = 400;
const int WINDOW_HEIGHT = 400;
const int ALIEN_FRAMES =5;
const int FRAME_RATE_FACTOR =30;

bool load_media();
void close();
bool init();

SDL_Window* gWindow = NULL;
LTexture gFooTexture; //front texture
LTexture gBackTexture; //bg texture
LTexture gSheetTexture; //sprite sheet texture
SDL_Rect gSpriteClips[ALIEN_FRAMES];
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

void LTexture::render(int x, int y, SDL_Rect* clip = NULL){
    SDL_Rect render_quad= {x,y,m_width,m_height};
    if(clip){
        render_quad.w =clip->w;
        render_quad.h =clip->h;

    }
    SDL_GetRendererOutputSize(gRenderer, &x, &y);
    SDL_RenderCopy( gRenderer, m_texture, clip, &render_quad );

}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue ){
    SDL_SetTextureColorMod( m_texture, red, green, blue );
}

void LTexture::setBlendMode(SDL_BlendMode mode){
    SDL_SetTextureBlendMode(m_texture,mode);
}

void LTexture::setAlpha(Uint8 alpha){
    SDL_SetTextureAlphaMod(m_texture, alpha);
}
/*
load 2  LTextures for 2 images
*/
bool load_media(){
    bool success = true;
    
    if (!gSheetTexture.load_from_file("images/animation.png")){
        cout<< "failed to load foo" <<endl;
        success=false;
    }else{
        int texW, texH;
        SDL_QueryTexture(gSheetTexture.getTexture(),NULL, NULL, &texW, &texH );
        int clip_width = texW/ALIEN_FRAMES;
        int clip_height = texH;

        for (int i=0; i<5; i++){
            gSpriteClips[i].x=0+ i*clip_width;
            gSpriteClips[i].y=0;
            gSpriteClips[i].h=clip_height;
            gSpriteClips[i].w=clip_width;
        }

        //set alpha blending
        gSheetTexture.setBlendMode(SDL_BLENDMODE_BLEND);

    }
    
    return success;
}

/*
de allocators
*/

void close(){
    // gFooTexture.free();
    // gBackTexture.free();
    gSheetTexture.free();

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
        gWindow = SDL_CreateWindow("Alien Window",  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,WINDOW_WIDTH,WINDOW_HEIGHT , SDL_WINDOW_SHOWN);
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
            // Uint32 startTime = SDL_GetTicks();
            
            //Modulation components
            Uint8 r = 255;
            Uint8 g = 255;
            Uint8 b = 255;

            //alpha
            Uint8 a = 255;

            //frame
            int frame = 0;

            while (!quit){
                // float seconds = (SDL_GetTicks() - startTime) / 1000.0f;

                while (SDL_PollEvent(&e)){
                    if (e.type == SDL_QUIT) {quit = true;}  
                    else if (e.type == SDL_KEYDOWN){
                        switch( e.key.keysym.sym ){
                             //Increase red
                            case SDLK_q:
                            if(r>238){
                                r=255;
                                break;
                            }
                            r += 16;
                            break;

                             //Decrease red
                            case SDLK_a:
                            if(r<17){
                                r=0;
                                break;
                            }
                            r -= 16;
                            break;

                             //Increase blue
                            case SDLK_w:
                            if(b>238){
                                b=255;
                                break;
                            }
                            b += 16;
                            break;

                             //Decrease blue
                            case SDLK_s:
                            if(b<17){
                                b=0;
                                break;
                            }
                            b -= 16;
                            break;

                              //Increase green
                            case SDLK_e:
                            if(g>238){
                                g=255;
                                break;
                            }
                            g += 16;
                            break;

                             //Decrease green
                            case SDLK_d:
                            if(g<17){
                                g=0;
                                break;
                            }
                            g -= 16;
                            break;

                            //Increase alpha
                            case SDLK_o:
                            if(a>238){
                                a=255;
                                break;
                            }
                            a+=16;
                            break;

                            //Decrease alpha
                            case SDLK_p:
                             if(a<17){
                                a=0;
                                break;
                            }
                            a-=16;
                            break;
                        }
                    }
                }
                SDL_SetRenderDrawColor(gRenderer,0xFF,0xFF,0xFF,0xFF);
                SDL_RenderClear(gRenderer);


                // gBackTexture.render(0,0);
                // gFooTexture.render(200 * sin(seconds),100 * sin(seconds));
                gSheetTexture.setColor(r,g,b);

                SDL_Rect* current_clip = &gSpriteClips[frame/FRAME_RATE_FACTOR];
                gSheetTexture.render(0,0,current_clip);

                gSheetTexture.setAlpha(a);

                SDL_RenderPresent(gRenderer);

                ++frame;
                if (frame/FRAME_RATE_FACTOR >= ALIEN_FRAMES){
                    frame=0;
                }
            }
            close();

        }
    }
    return 0;
}