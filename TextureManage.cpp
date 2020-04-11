#include "TextureManage.hpp"

SDL_Texture* TextureManager::LoadTexture(const char* texture, SDL_Renderer* ren){
    SDL_Surface* tmpSurface = IMG_Load(texture);
    if (tmpSurface == NULL){
        printf("%s -> %s", SDL_GetError(), SDL_GetBasePath());
    }
    SDL_Texture* tex = SDL_CreateTextureFromSurface (ren, tmpSurface);
    SDL_FreeSurface(tmpSurface);
    
    return tex;
}
