#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
using namespace std;

#ifndef L_TEXTURE_H_
#define L_TEXTURE_H_

class LTexture{
public:
    LTexture();
    ~LTexture();
    bool load_from_file(string path);
    void free();
    void render(int x, int y, SDL_Rect* clip );
    int get_width(){return m_width;}
    int get_height(){return m_height;}
    void setColor( Uint8 red, Uint8 green, Uint8 blue );

private:
    SDL_Texture* m_texture;
    int m_width;
    int m_height;
};

#endif