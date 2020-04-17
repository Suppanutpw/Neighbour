#include "game.hpp"
#include "TextureManage.hpp"
#include "gameObject.hpp"

GameObject* villager[12];
GameObject* house[12];
GameObject* backgroundGame;

GameObject* scoreBar; // Moves Banner
GameObject* backToMenu; // Back to menu bar

GameObject* result; // ye Score Bar
GameObject* playAgain; // back to menu
GameObject* exitButton; // exit button

GameObject* titleGame; // Title Game
GameObject* difficulty[3]; // Difficulty Game easy normal hard

Game::Game(){}
Game::~Game(){}

int highScore[3] = {-1, -1, -1}; // put leastest moves point in 3 modes

int tableGame[5][5] = {0};
char filename[12][50] = {"assets/Sprite_black.png", "assets/Sprite_cum.png", "assets/Sprite-blue.png", "assets/Sprite-dark_blue.png", "assets/Sprite-lavender.png", "assets/Sprite-pig_pink.png", "assets/Sprite-poop.png", "assets/Sprite-purple.png", "assets/Sprite-red.png", "assets/Sprite-verdant.png", "assets/Sprite-yellow.png", "assets/Sprite-zelda_green.png"};
char difFilename[3][50] = {"assets/sword_easy.png", "assets/sword_medium.png", "assets/sword_hell.png"};
char numFilename[10][50] = {"assets/num0.png", "assets/num1.png", "assets/num2.png", "assets/num3.png", "assets/num4.png", "assets/num5.png", "assets/num6.png", "assets/num7.png", "assets/num8.png", "assets/num9.png"};

int Game::findVillagerIdOnHouse(int houseID){ // find is villager on this house
    for (int i = 0; i < row*col; i++){
        if (villager[i]->onHouse == houseID){
            return i; // return villager id
        }
    }return -1; // not found
}

void Game::readFile() {
    // เรียกไฟล์เก็บข้อมูล
    FILE* fpointer;
    fpointer = fopen("setting.txt", "r");
    
    if (fpointer == NULL) { // IF no file here create new one
        fclose(fpointer);
        
        // close current point and write new one
        updateFile();
        
        fpointer = fopen("setting.txt", "r");
    }
    for (int i = 0; i < 4; i++) { // Read the file data
        char inp[150];
        fscanf(fpointer, "%s", inp);
        if (strcmp(inp, "resolution:") != 0) { // mode data
            fscanf(fpointer, "%d", &highScore[i]);
        }else { // monitor resolution
            fscanf(fpointer, "%d%d", &WIDTH, &HEIGHT);
        }
    }
    fclose(fpointer);
}

void Game::updateFile() {
    // เรียกไฟล์เก็บข้อมูล
    FILE* fpointer;
    fpointer = fopen("setting.txt", "w+");
    
    // ข้อมูลในไฟล์
    fprintf(fpointer, "easy: %d\n", highScore[0]);
    fprintf(fpointer, "normal: %d\n", highScore[1]);
    fprintf(fpointer, "hard: %d\n", highScore[2]);
    fprintf(fpointer, "resolution: %d %d\n", WIDTH, HEIGHT);
    fclose(fpointer);
}

void Game::init(const char* title, int xpos, int ypos, int width, int height, int fullscreen){ // initialize program
    int flags = 0;
    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }
    
    // Set Width And Height Srceen
    WIDTH = width;
    HEIGHT = height;
    
    // update data from file
    readFile();
    
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0){

        printf("Subsystem initialize...\n");
        window = SDL_CreateWindow(title, xpos, ypos, WIDTH, HEIGHT, flags);
        if (window) {
            printf("Window Created!\n");
        }

        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            
            printf("Renderer Created!\n");
        }
        isRunning = true;
    }else {
        isRunning = false;
    }
    
    // ประกาศ object เพื่อนำไปเรียกแสดงผล renderer
    // GameObject (filename, renderer, pos x, pos y, width original image, height original image, width display, height display, type, srceen width, srceen height)
    backgroundGame = new GameObject("assets/BG-Game.png", renderer, 0, 0, WIDTH, HEIGHT, WIDTH, HEIGHT, "background", WIDTH, HEIGHT); // ฉากหลัง
    
    // เมื่อได้ asset ต้องปรับขนาดภาพด้วย
    titleGame = new GameObject("assets/logo_start.png", renderer, 50, 25, 406, 414, WIDTH/2, HEIGHT/2.67,  "percent", WIDTH, HEIGHT);
    for (int i = 0 ; i < 3; i++) {
        // pos x pos y is percent of sceen
        difficulty[i] = new GameObject(difFilename[i], renderer, 50, 48+(18*i), 232, 140, WIDTH/2.67, HEIGHT/5.72, "percent", WIDTH, HEIGHT);
    }
    
    // IN GAME ASSET
    scoreBar = new GameObject("assets/moves.png", renderer, 10, 25, 300, 150, 150, 75, "background", WIDTH, HEIGHT);
    backToMenu = new GameObject("assets/exit.png", renderer, 95, 5, 100, 100, 50, 50, "percent", WIDTH, HEIGHT);
    
    // RESULT ASSET
    result = new GameObject("assets/score_page.png", renderer, 50, 40, 500, 500, WIDTH/1.6, HEIGHT/1.6, "percent", WIDTH, HEIGHT);
    exitButton = new GameObject("assets/exit.png", renderer, 30, 85, 100, 100, WIDTH/8, HEIGHT/8, "percent", WIDTH, HEIGHT);
    playAgain = new GameObject("assets/menu_button.png", renderer, 70, 85, 200, 200, WIDTH/4, HEIGHT/4, "percent", WIDTH, HEIGHT);
}

// ------------------ START RENDER&UPDATE GAME PLAY ------------------

// ------------------ MODE 0 MENU BEFORE START GAME ------------------
void Game::menuHandleEvents() {
    SDL_Event event;
    SDL_PollEvent(&event);
    
    SDL_GetMouseState(&mouseX, &mouseY);
    
    switch (event.type) { // event detect
        case SDL_QUIT: // กดปิดหน้าจอ
            isRunning = false;
            break;
        case SDL_MOUSEBUTTONDOWN: // หากคลิกเมาส์
            modePicked = -1; // reset picker
            isMouseClicked = 1;
            for (int i = 0; i < 3; i++) { // check on mode Easy Normal OR Hard? if not will return -1
                if (difficulty[i]->isHover()) {
                    modePicked = i;
                }
            }
            break; // end case SDL_MOUSEBUTTONDOWN
        case SDL_MOUSEBUTTONUP:
            isMouseClicked = 0;
            if (modePicked != -1) { // if pick any mode
                if (difficulty[modePicked]->isHover() == 0) {
                    modePicked = -1; // Drag mouse out before click
                }else {
                    mode = 1;
                    Game::inGameInit();
                }
            }
            break; // end case SDL_MOUSEBUTTONUP
        default:
            break;
    }
}
void Game::menuUpdate() {
    for (int i = 0; i < 3; i++) {
        difficulty[i]->hoverX = mouseX;
        difficulty[i]->hoverY = mouseY;
        difficulty[i]->menuUpdate();
    }
}
void Game::menuRender() {
    SDL_RenderClear(renderer);
    // ไว้เพิ่ม object สำหรับเรนเดอร์ layer ล่างจะทับบน (บรรทัดล่างจะทับบน)
    backgroundGame->Render(); // Background renderer
    titleGame->Render(); // title game renderer
    for (int i = 0; i < 3; i++) { // Easy Normal Hard renderer
        difficulty[i]->Render();
    }
    SDL_RenderPresent(renderer);
}

// ------------------ MODE 1 IN GAME ------------------
void Game::inGameInit() {
    scoreMoves = 0; // Reset Score
    if (modePicked == 0) { // Easy
        Game::randomMapping(2, 3);
    }else if (modePicked == 1) { // Normal
        Game::randomMapping(2, 4);
    }else { // Hard
        Game::randomMapping(3, 4);
    }
}
void Game::inGameHandleEvents() {
    SDL_Event event;
    SDL_PollEvent(&event);
    
    SDL_GetMouseState(&mouseX, &mouseY);
    
    switch (event.type) { // event detect
        case SDL_QUIT: // กดปิดหน้าจอ
            isRunning = false;
            break;
        case SDL_MOUSEBUTTONDOWN: // หากคลิกเมาส์
            // Get Position of character
            isMouseClicked = 1;
            inGamePicked = -1;
            
            // inGamePicked var description read in game.hpp file
            
            // backtomenu has low priority so I check them first
            if (backToMenu->isHover()) {
                inGamePicked = 13;
                villager[inGamePicked]->onMouseDown = 1;
            }
            
            // find character picker
            for (int i = 0; i < row*col; i++) {
                if (villager[i]->isHover()) {
                    inGamePicked = i;
                    villager[inGamePicked]->onMouseDown = 1;
                }
            }
            break; // end case SDL_MOUSEBUTTONDOWN
        case SDL_MOUSEBUTTONUP: // หากปล่อยเมาส์
            // check pos of droped house
            if (inGamePicked == 13) { // check back to menu button first
                if (backToMenu->isHover()) {
                    mode = 0; // back to menu
                }
            }else if (inGamePicked != -1) { // check villager
            
                int beforePos = villager[inGamePicked]->onHouse; // To consider moves count
                
                // when drop from ground to house
                int foundHouse = 0; // Does character drop on house?
                for (int i = 0; i < row*col; i++) {
                    if (house[i]->isHover()){ // is pointer hover on house?
                        // this is house that on drag down
                        int vIDInHouse = findVillagerIdOnHouse(i);
                        if (vIDInHouse != -1) { // if there has villager on house
                            villager[vIDInHouse]->onHouse = villager[inGamePicked]->onHouse;
                            if (villager[vIDInHouse]->onHouse != -1) { // if old villager on ground
                                villager[vIDInHouse]->displayX = house[villager[inGamePicked]->onHouse]->displayX + 85;
                                villager[vIDInHouse]->displayY = house[villager[inGamePicked]->onHouse]->displayY + 65;
                            }
                        }
                        // set inGamePicked to destination
                        villager[inGamePicked]->onHouse = i;
                        villager[inGamePicked]->displayX = house[i]->displayX + 85;
                        villager[inGamePicked]->displayY = house[i]->displayY + 65;
                        foundHouse = 1;
                    }
                }
                if (foundHouse == 0) { // if it doesn't drop on house
                    villager[inGamePicked]->onHouse = -1;
                }
                
                if (beforePos != villager[inGamePicked]->onHouse) { // if pos change it mean object has moved
                    scoreMoves += 1;
                }
                villager[inGamePicked]->onMouseDown = 0; // reset charecter picker
            }
            
            inGamePicked = -1; // reset charecter picker
            isMouseClicked = 0;
            break; // end case SDL_MOUSEBUTTONUP
        default:
            break;
    }
    
    // mouse position checker
    if (inGamePicked != -1) { // if character is picked disable hover check
        mouseY = -1; mouseY = -1;
    }
}
void Game::inGameUpdate(){
    // Render updated object
    
    int isAngry[25] = {0}; // is Angry ref by house position in 2d array
    int isEndGame = Game::gameMapping(isAngry);
    
    for (int i = 0; i < row*col; i++) {
        villager[i]->hoverX = mouseX;
        villager[i]->hoverY = mouseY;
        if (villager[i]->onMouseDown) {
            SDL_GetMouseState(&mouseX, &mouseY);
            villager[i]->hoverX = mouseX - 25;
            villager[i]->hoverY = mouseY - 23.25;
        }
        
        if (villager[i]->onHouse == -1 || isAngry[villager[i]->onHouse] == 0) { // not angry
            villager[i]->charUpdate("normal"); // default animation
        }else {
            villager[i]->charUpdate("angry"); // angry animation
        }
    }
    backToMenu->menuUpdate();
    
    if (isEndGame) {
        // if scoreMoves less than highscore so update new record
        if (highScore[modePicked] == -1 || scoreMoves < highScore[modePicked]) {
            highScore[modePicked] = scoreMoves;
        }
        updateFile();
        
        mode = 2; // Game Over
    }
}
void Game::inGameRender(){ // render the updated position
    SDL_RenderClear(renderer);
    // ไว้เพิ่ม object สำหรับเรนเดอร์ layer ล่างจะทับบน
    backgroundGame->Render(); // Background renderer
    for (int i = 0; i < row*col; i++) { // house renderer
        house[i]->Render();
    }
    // ที่แยกลูปเพราะต้องให้ตัวละครทุกตัวทับบ้าน (มีกรณีที่ชาวบ้านลำดับน้อยกว่าต้องทับบ้านที่ลำดับมากกว่า)
    for (int i = 0; i < row*col; i++) {
        villager[i]->Render();
    }
    scoreBar->Render();
    backToMenu->Render();
    
    showNumber(145, 15, 80, 80, scoreMoves, "left"); // Render Number Score align left

    SDL_RenderPresent(renderer);
}

// ------------------ MODE 2 GAME OVER SHOW RESULT ------------------
void Game::overHandleEvents() {
    SDL_Event event;
    SDL_PollEvent(&event);
    
    SDL_GetMouseState(&mouseX, &mouseY);
    
    switch (event.type) { // event detect
        case SDL_QUIT: // กดปิดหน้าจอ
            isRunning = false;
            break;
        case SDL_MOUSEBUTTONDOWN: // หากคลิกเมาส์
            inGamePicked = -1; // reset picker
            isMouseClicked = 1;
            
            if (playAgain->isHover()) { // play again on mouse button down
                inGamePicked = 14;
            }else if (exitButton->isHover()) { // exit game on mouse button down
                inGamePicked = 15;
            }
            
            break; // end case SDL_MOUSEBUTTONDOWN
        case SDL_MOUSEBUTTONUP:
            isMouseClicked = 0;
            
            if (inGamePicked == 14 && playAgain->isHover()) { // play again on mouse button up
                mode = 0;
            }else if (inGamePicked == 15 && exitButton->isHover()) { // exit game on mouse button up
                isRunning = false;
            }
            
            inGamePicked = -1;
            break; // end case SDL_MOUSEBUTTONUP
        default:
            break;
    }
}
void Game::overUpdate() {
    playAgain->menuUpdate(); // title game renderer
    exitButton->menuUpdate(); // title game renderer
}
void Game::overRender() {
    SDL_RenderClear(renderer);
    // ไว้เพิ่ม object สำหรับเรนเดอร์ layer ล่างจะทับบน (บรรทัดล่างจะทับบน)
    backgroundGame->Render();

    result->Render(); // Background renderer
    playAgain->Render(); // title game renderer
    exitButton->Render(); // Exit game button
    
    showNumber(50, 35, 120, 120, scoreMoves, "center"); // Render Number Score align center
    showNumber(50, 67.5, 80, 80, highScore[modePicked], "center"); // Render Number High Score align center
    
    SDL_RenderPresent(renderer);
}

// ------------------ END RENDER&UPDATE GAME PLAY ------------------

void Game::clean(){ // Clear sceen and exit
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    
    printf("Game cleaned!\n");
    return;
}

int Game::gameMapping(int* isAngry){ // check neighbour status
    int isEndGame = 1;
    
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            int currentID = findVillagerIdOnHouse(i*col + j);
            if (currentID != -1) { // that housePos have villager
                isAngry[i*col + j] = Game::checkAngry(i, j); // 1 is angry 0 is not angry
                if (isAngry[i*col + j]) { // if found angry then game will continue
                    isEndGame = 0;
                }
            }else {
                isEndGame = 0;
            }
        }
    }
    return isEndGame;
}

int Game::checkAngry(int xrow, int ycol) { // 1 is angry 0 is not angry
    int tableRow = -1, tableCol = -1;
    
    int currentID = findVillagerIdOnHouse(xrow*col + ycol);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++){
            if (currentID == tableGame[i][j]) { // find current villager ID in tableGame(random init table)
                tableRow = i;
                tableCol = j;
            }
        }
    }
    
    // find suitable Neighbour if found will return 0 (not angry) if found not suit will return 1 (angry)
    return checkAround(tableRow, tableCol, xrow-1, ycol) || checkAround(tableRow, tableCol, xrow, ycol-1) || checkAround(tableRow, tableCol, xrow+1, ycol) || checkAround(tableRow, tableCol, xrow, ycol+1);
}

int Game::checkAround(int tableRow, int tableCol, int axrow, int aycol) {
    int foundNotSuitNear = 1;
    if (axrow >= 0 && axrow < row && aycol >= 0 && aycol < col) {
    
        if (findVillagerIdOnHouse(axrow*col + aycol) == -1) { // if hasn't anyone in house
            foundNotSuitNear = 0;
        }
        
        // find suitable Neighbour
        for (int i = 0; i < gameNeighbour[tableRow][tableCol].length; i++) {
            if (gameNeighbour[tableRow][tableCol].near[i] == findVillagerIdOnHouse(axrow*col + aycol)){
                foundNotSuitNear = 0;
            }
        }
    }else {
        foundNotSuitNear = 0;
    }
    return foundNotSuitNear;
}


void Game::randomMapping(int inpRow, int inpCol) { // generate puzzle table
    // Set ROW and COL
    row = inpRow;
    col = inpCol;
    
    int setID[row*col + 1], length = row*col; // generate set of number 0, 1, 2, ... , (row*col - 1)
    for (int i = 0; i < row*col; i++){ setID[i] = i; } // generate set of villagers id
    // ทำการสุ่มตารางขึ้นมาโดยใช้หลักการ สุ่มแล้วดึงออกจากเซต
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            int randomErase = rand()%(length--); // random select id
            tableGame[i][j] = setID[randomErase];
            gameNeighbour[i][j] = {0};
            
            for (int current = randomErase; current < length; current++){ // erase selected id
                setID[current] = setID[current+1];
            }
        }
    }
    
    // Mapping Neighbour in four direction
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            int near_length = aroundMapping(i, j, i-1, j);
            near_length += aroundMapping(i, j, i, j-1);
            near_length += aroundMapping(i, j, i+1, j);
            near_length += aroundMapping(i, j, i, j+1);
        }
    }
    
    // ประกาศ object เพื่อนำไปเรียกแสดงผล renderer
    
    // HOUSE DISPLAY
    float displayWidth = WIDTH/5.5, displayHeight = HEIGHT/5.5; // width height object reponsive?
    float remainX = (WIDTH - (displayWidth+20)*col + 20)/2, remainY = (HEIGHT*0.388 + 75*(3 - row));
    for (int i = 0; i < row; i++) { // house
        for (int j = 0; j < col; j++) { // house (width 175) (height 175)
            house[i*col + j] = new GameObject("assets/House.png", renderer, remainX + (displayWidth+20)*j, remainY + displayHeight*i, 250, 250, displayWidth, displayHeight, "house", WIDTH, HEIGHT);
        }
    }
    
    // VILLAGER DISPLAY
    displayWidth = WIDTH/12;
    displayHeight = displayWidth*1.13;
    remainX = (WIDTH - (displayWidth+25)*row*col/2 + 25)/2;
    remainY = HEIGHT*0.15;
    for (int i = 0; i < 2; i++) { // character in 900x900 screen (start at 87.5, 100) (width 100) (height 113) (slide to right 125 per col)
        for (int j = 0; j < row*col/2; j++) {
            // start x with remainX + 125*j and start y at remainY + 100*i
            villager[i*row*col/2 + j] = new GameObject(filename[i*row*col/2 + j], renderer, remainX + (displayWidth+25)*j, remainY + (displayHeight+20)*i, 500, 208, displayWidth, displayHeight, "character", WIDTH, HEIGHT);
        }
    }
    
    // code for print generated table
    /* for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            printf("(row %d, col %d) %d -> ", i, j, tableGame[i][j]);
            for (int k = 0; k < gameNeighbour[i][j].length ; k++) {
                printf("%d ", gameNeighbour[i][j].near[k]);
            }printf("\n");
        }
        printf("\n");
    }printf("\n\n"); */
}

int Game::aroundMapping(int xrow, int ycol, int axrow, int aycol) { // first 2 are current last 2 are around check position
    if (axrow >= 0 && axrow < row && aycol >= 0 && aycol < col) {
        gameNeighbour[xrow][ycol].near[gameNeighbour[xrow][ycol].length++] = tableGame[axrow][aycol];
        return 1; // if in bound put around row/col in struct in plus length 1
    }return 0; // if out of bound do nothing and return 0
}

void Game::showNumber(float posX, float posY, int width, int height, int number, const char* align) { // Display number
    GameObject* numberDisplay[6]; // Number Digit limit

    char numberStr[7];
    sprintf(numberStr, "%d", number);
    
    // Check Align for display
    if (strcmp(align, "center") == 0) {
        // new GameObject("assets/score_page.png", renderer, 50, 40, 500, 500, WIDTH/1.6, HEIGHT/1.6, "percent", WIDTH, HEIGHT) อ้างอิงจาก objects นี้;
        int displaySize = (int) (strlen(numberStr)-1) * (width/2) + width;
        
        float refPosY = HEIGHT*(0.4) - (HEIGHT/1.6)/2;
        
        posX = (posX/100)*WIDTH - (displaySize/2);
        posY = refPosY + (posY/100)*(HEIGHT/1.6);
    }
    
    // Change int to string and slice it to digit for display
    for (int digit=0; digit < strlen(numberStr); digit++) {
        numberDisplay[digit] = new GameObject(numFilename[numberStr[digit]-48], renderer, posX + (width/2)*digit, posY, 100, 100, width, height, "number", WIDTH, HEIGHT);
        numberDisplay[digit]->Render();
    }
}
