/* PROGRAMMING FUNDAMENTAL'S PROJECT FOR FALL 2022 BS(CS)
 * This header contains the global/common variables of the project.
 * You don't really need to change anything.
 * TIP: After completing your project, you can make changes for aesthetics.
 * */
/*
Muhammad Shoaib 2502866
Haider Ali Naushad 2502852
BSSE-F-25-B
*/
// UI Color Pallete
// Main text
const sf::Color COLOR_TEXT_PRIMARY   (245, 245, 245);   // near-white
const sf::Color COLOR_TEXT_SECONDARY (140, 140, 160);   // bluish gray

// Accent / selection
const sf::Color COLOR_TEXT_ACCENT    (0, 255, 180);     // neon mint

// Danger / game over
const sf::Color COLOR_TEXT_DANGER    (255, 60, 60);     // strong red

// Panels & overlays
const sf::Color COLOR_PANEL_BG       (10, 10, 30, 200); // deep blue-black
const sf::Color COLOR_DIM_LAYER      (0, 0, 0, 170);

// Ghost
const sf::Color COLOR_GHOST          (180, 255, 255, 80);



 struct GameContext {
    int gameState;
    int prevState;

    int score;
    int level;

    float survivalTime;
    int blockedRows;

    bool isTouchingGround;
    float lockTimer;
};

//---Title: PF-Project, Fall-2022 for BS(CS)---//
const char title[] = "PF-Project, Fall-2022";



//---Height and Width of the Actual Interactive Game---//
const int M = 20;   //Number of rows for a piece to cover on the screen (not the entire screen) = 20
const int N = 10;   //Number of columns for a piece to cover on the screen (not the entire screen) = 10

//---The Actual Interactive Game Grid - Built Over (MxN)---//
int gameGrid[M][N] = {0};

//---To Hold the Coordinates of the Piece---//
int point_1[4][2], point_2[4][2];

//---Check Uncertain Conditions---//
bool anamoly(GameContext& ctx ){
    for (int i=0;i<4;i++)
        if (point_1[i][0]<0 || point_1[i][0]>=N || point_1[i][1]>=M - ctx.blockedRows)//added blockedRows
            return 0;
        else if (gameGrid[point_1[i][1]][point_1[i][0]])
            return 0;
    return 1;
};