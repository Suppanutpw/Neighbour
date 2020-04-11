#include "gameObject.hpp"
#include "TextureManage.hpp"

const int SPIXEL = 39, NEXTP = 83, NOPEPIXEL = 305; // sliding villager pixel

GameObject::GameObject(const char* texturesheet, SDL_Renderer* ren, float posx, float posy, float width, float height, float realX, float realY, const char* texturetype, const int WIDTH, const int HEIGHT){
    renderer = ren;
    objTexture = TextureManager::LoadTexture(texturesheet, ren);
    
    xpos = displayX = posx;
    ypos = displayY = posy;
    
    // Set image_load position
    srcRect.w = width;
    srcRect.h = height;
    destRect.w = realX;
    destRect.h = realY;
    srcRect.x = 0;
    srcRect.y = 0;
    
    if (strcmp(texturetype, "character") == 0) { // character
        srcRect.w = width/4 - SPIXEL; // divide into frame
        srcRect.h = height - 90; // erase char overflow
    }else if (strcmp(texturetype, "percent") == 0) { // percent display
        xpos = displayX = WIDTH*(posx/100) - realX/2;
        ypos = displayY = HEIGHT*(posy/100) - realY/2;
    }
    
    // set position
    destRect.x = xpos;
    destRect.y = ypos;
}

void GameObject::charNormal(){ // char normal render has 3 frames
    srcRect.x = SPIXEL + NEXTP*(frame/3);
    srcRect.y = 20;
    destRect.x = xpos;
    destRect.y = ypos;
    
    frame++;
    if (frame == 9) {
        frame = 0;
    }
}

void GameObject::charAngry(){
    srcRect.x = NOPEPIXEL;
    srcRect.y = 20;
    destRect.x = xpos;
    destRect.y = ypos;
}

void GameObject::charUpdate(const char* status) {
    if (strcmp(status, "normal") == 0) {
        GameObject::charNormal();
    }else {
        GameObject::charAngry();
    }
    
    if (GameObject::onHouse == -1) { // if character not on house
        displayX = xpos;
        displayY = ypos;
    }
    if (onMouseDown) { // if onclick character (drag)
        displayX = hoverX;
        displayY = hoverY;
    }
    
    destRect.x = displayX;
    destRect.y = displayY - 15*GameObject::isHover(); // check onhover
}

void GameObject::menuUpdate() {
    destRect.x = displayX;
    destRect.y = displayY - 15*GameObject::isHover(); // check onhover
}

void GameObject::Render(){
    SDL_RenderCopy(renderer, objTexture, &srcRect, &destRect);
}

int GameObject::isHover(){
    int isPicked = 0; // reset picker to not pick
    SDL_GetMouseState(&hoverX, &hoverY); // Get mouse position

    if (hoverX > displayX && hoverX < displayX + destRect.w) { // width
        if (hoverY > displayY && hoverY < displayY + destRect.h) { // height
            isPicked = 1;
        }
    }
    return isPicked;
}
