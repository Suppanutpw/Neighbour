#ifndef game_hpp
#define game_hpp

#define SDL_MAIN_HANDLED

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class Game {
public:
    Game();
    ~Game();
    
    int mode = 0; // mode 0 is title game 1 is playing 2 is game over
    
    void readFile(); // for read or update data
    void updateFile(); // for read or update data
    
    void init(const char* title, int xpos, int ypos, int width, int height, int fullscreen);

    void randomMapping(int inpRow, int inpCol); // generate
    int aroundMapping(int xrow, int ycol, int axrow, int aycol); // mapping genete table in struct
    
    // IN GAME FUNC
    void inGameInit(); // setting difficultly and generate table
    void inGameHandleEvents();
    void inGameUpdate();
    void inGameRender();
    
    // MENU FUNC
    void menuHandleEvents();
    void menuUpdate();
    void menuRender();
    
    // GAME OVER FUNC
    void overHandleEvents();
    void overUpdate();
    void overRender();
    
    void showNumber(float posX, float posY, int width, int height, int number, const char* align); // Display number position percentage
    
    void clean();
    
    int findVillagerIdOnHouse(int houseID);
    
    int gameMapping(int* isAngry); // check status in game
    int checkAngry(int xrow, int ycol);
    int checkAround(int xrow, int ycol, int tableRow, int tableCols);
    
    int running() {return isRunning;}

private:
    // modePicked is difficultly
    // inGamePicked 0-11 are villager AND 13 is back to menu AND 14 is play again AND 15 is exit
    int isRunning, isMouseClicked = 0, inGamePicked = -1, modePicked = -1;
    int scoreMoves = 0;
    
    // for mouse button down
    int mouseX, mouseY;
    int row, col;
    
    int WIDTH, HEIGHT;
    
    struct Neighbour{ // for check status of Neighbour in village
        int length = 0; // default is four direction
        int near[4];
    }gameNeighbour[5][5];
     
    SDL_Window *window;
    SDL_Renderer *renderer;
};

#endif /* game_hpp */
