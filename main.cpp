#include "game.hpp"

Game *game = nullptr;
int WIDTH = 800, HEIGHT = 800;

int main(int argc, const char * argv[]) {
    
    const int FPS = 60;
    const int frameDelay = 1000/FPS;
    
    Uint32 frameStart;
    int frameTime;
    
    game = new Game(); // ประกาศ class Game
    game->init("Neighbour", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, false);
    
    while (game->running()){
        frameStart = SDL_GetTicks();
        
        if (game->mode == 1) { // Game Start
            game->inGameHandleEvents();
            game->inGameUpdate();
            game->inGameRender();
        }else if(game->mode == 0) { // Menu Game
            game->menuHandleEvents();
            game->menuUpdate();
            game->menuRender();
        }else { // Game Over
            game->overHandleEvents();
            game->overUpdate();
            game->overRender();
        }
        
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }
    game->clean();
    
    return EXIT_SUCCESS;
}
