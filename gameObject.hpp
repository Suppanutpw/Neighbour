#include "game.hpp"

class GameObject {
public:
    GameObject(const char* texturesheet, SDL_Renderer* ren, float posx, float posy, float width, float height, float realX, float realY, const char* texturetype, const int WIDTH, const int HEIGHT);
    ~GameObject();
    
    // hover check
    int onMouseDown = 0;
    int hoverX = -1, hoverY = -1, displayX, displayY; // display position can change (for character only #for now)
    
    int charID, onHouse = -1; // for character only
    
    void menuUpdate();
    
    void charNormal();
    void charAngry();
    void charUpdate(const char* status);
    
    void Render();
    int isHover();
    
private:
    int frame = 0, xpos, ypos; // xpos, ypos is default position
    
    SDL_Texture* objTexture;
    SDL_Rect srcRect, destRect;
    SDL_Renderer* renderer;
};
