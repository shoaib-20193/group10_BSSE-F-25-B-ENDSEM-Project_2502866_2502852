/* PROGRAMMING FUNDAMENTAL'S PROJECT FOR FALL 2022 BS(CS)
 * You don't need to change anything in the driver code/main function except the part(s) where instructed.
 * You are expected to define your functionalities in the "functionality.h" header and call them here in the instructed part(s).
 * The game is started with a small box, you need to create other possible in "pieces.h" file in form of array.
    E.g., Syntax for 2 pieces, each having four coordinates -> int Pieces[2][4];
 * Load the images into the textures objects from img directory.
 * Don't make any changes to the "utils.h" header file except for changing the coordinate of screen or global variables of the game.
 * SUGGESTION: If you want to make changes in "utils.h", do it once you complete rest of the functionalities.
 * TIP: You can study SFML for drawing scenes, sprites and window.
 * */
/*
Muhammad Shoaib 2502866
Haider Ali Naushad 2502852
BSSE-F-25-B
*/
#include <SFML/Graphics.hpp>
#include <ctime>
#include <iomanip>
#include <cstring>   // for memset
#include <optional>          // needed for std::optional
#include "utils.h"
#include "pieces.h"
#include "functionality.h"
#include <fstream>

using namespace sf;

#define STATE_MENU      0
#define STATE_PLAYING   1
#define STATE_PAUSED    2
#define STATE_HIGHSCORE 3
#define STATE_HELP      4
#define STATE_GAMEOVER  5

GameContext ctx = {
    STATE_MENU,   // gameState
    STATE_MENU,   // prevState
    0,            // score
    1,            // level
    0.f,          // survivalTime
    0,            // blockedRows
    false,        // isTouchingGround
    0.f           // lockTimer
};
const float LOCK_DELAY = 1.0f; //piece lock delay

//Menus
constexpr int GAMEOVER_OPTIONS = 2;
int gameOverIndex = 0;

constexpr int PAUSE_OPTIONS = 5;

constexpr int START_MENU_OPTIONS = 4;
int pauseIndex = 0;

int startMenuIndex = 0;
constexpr int MAX_HIGHSCORES = 10;
//menus end
//gamemem
int currentColor;
int nextColor;
int colorNum = 1;
//gamemem end
//main function signatures
void resetGame(int& dx, bool& rotate, float& delay, float& timer);
void drawGhostPiece(sf::Sprite& sprite,sf::RenderWindow& window);
void saveHighScore(int score);
void loadHighScores();
std::string highScoreString();
//main function signatures end
int main()
{

    std::srand(static_cast<unsigned>(std::time(nullptr)));
//game window creation
    ContextSettings settings;
    settings.antiAliasingLevel= 8; 
    String title = "Tetris";

    RenderWindow window(
    VideoMode({640, 960}),//640,960
    title,
    State::Windowed,  // 3rd arg is State
    settings              // last arg
);

sf::View gameView(FloatRect({0.f, 0.f},{ 320.f, 480.f}));
window.setView(gameView);
gameView.setSize(Vector2f (320.f, 480.f));
gameView.zoom(1.f); 
window.setView(gameView);
    window.setVerticalSyncEnabled(true);
//gamewindow creation end
//assest loading
    Texture obj1, obj2, obj3;
    // Load "frames.png","tiles.png" and "background.png" for Texture objects
    if (!obj1.loadFromFile("img/tiles.png"))      return -1;
    if (!obj2.loadFromFile("img/background.png")) return -1;
    if (!obj3.loadFromFile("img/frame.png"))     return -1;

    Sprite sprite(obj1), background(obj2), frame(obj3);
    Font font;
    if (!font.openFromFile("img/JetBrainsMonoNL-Regular.ttf"))   // make sure you have a .ttf file in img folder
    return -1;
//assest loading end
//local variables
    int delta_x = 0;
    float timer = 0, delay = 0.3f;
    bool rotate = false, hardDrop = false;
    Clock clock;
    nextPiece  = allowedPieces[rand() % allowedCount];
    nextColor  = 1 + rand() % 7;
//local variables end
//menues and and other texts
    Text scoreLabel(font);
    scoreLabel.setCharacterSize(18);
    scoreLabel.setFillColor(Color::Yellow);
    scoreLabel.setPosition(Vector2f(230.f, 260.f));
    scoreLabel.setString("Score:");

    Text scoreValue(font);
    scoreValue.setCharacterSize(16);
    scoreValue.setFillColor(COLOR_TEXT_PRIMARY );
    scoreValue.setPosition(Vector2f(230.f, 280.f));

    Text levelLabel(font);
    levelLabel.setCharacterSize(18);
    levelLabel.setFillColor(Color::Yellow);
    levelLabel.setPosition(Vector2f(230.f, 320.f));
    levelLabel.setString("Level:");

    Text levelValue(font);
    levelValue.setCharacterSize(16);
    levelValue.setFillColor(COLOR_TEXT_PRIMARY );
    levelValue.setPosition(Vector2f(230.f, 340.f));

    Text highTitle(font, "HIGH SCORES", 28);
    highTitle.setFillColor(COLOR_TEXT_PRIMARY);
    highTitle.setPosition({75.f, 120.f});



    Text pauseTitle(font, "PAUSED", 30);
    pauseTitle.setPosition({100.f, 120.f});
    pauseTitle.setFillColor(COLOR_TEXT_PRIMARY);


    Text pauseMenu[PAUSE_OPTIONS] = {
        Text(font, "Resume", 20),
        Text(font, "Restart", 20),
        Text(font, "Highscore", 20),
        Text(font, "Help", 20),
        Text(font, "Quit", 20)
    };

    for (int i = 0; i < PAUSE_OPTIONS; i++) {
        pauseMenu[i].setFillColor(COLOR_TEXT_SECONDARY);
        pauseMenu[i].setPosition(sf::Vector2f(110.f, 180.f + i * 30.f));
    }

    Text menuTitle(font, "TETRIS", 36);
    menuTitle.setFillColor(COLOR_TEXT_PRIMARY);
    menuTitle.setPosition({90.f, 120.f});


    Text startMenu[START_MENU_OPTIONS] = {
        Text(font, "Start Game", 20),
        Text(font, "High Scores", 20),
        Text(font, "Help", 20),
        Text(font, "Exit", 20)
    };

    for (int i = 0; i < START_MENU_OPTIONS; i++) {
        startMenu[i].setFillColor(COLOR_TEXT_SECONDARY);
        startMenu[i].setPosition({90.f, 190.f + i * 35.f});
    }

    Text helpTitle(font, "HELP", 28);
    helpTitle.setFillColor(COLOR_TEXT_PRIMARY);
    helpTitle.setPosition({120.f, 120.f});


    Text helpText(font);
        helpText.setCharacterSize(16);
        helpText.setFillColor(Color::White);
        helpText.setPosition(Vector2f(40.f, 160.f));

        helpText.setString(
            "Arrow Left  : Move Left\n"
            "Arrow Right : Move Right\n"
            "Arrow Up    : Rotate\n"
            "Arrow Down  : Soft Drop\n"
            "Space       : Hard Drop\n"
            "P           : Pause\n\n"
            "Backspace   : Return"
        );

    Text gameOverTitle(font, "GAME OVER", 28);
    gameOverTitle.setFillColor(COLOR_TEXT_DANGER );
    gameOverTitle.setPosition(Vector2f(60.f, 120.f));

    Text gameOverScore(font, "", 16);
    gameOverScore.setFillColor(COLOR_TEXT_SECONDARY);
    gameOverScore.setPosition(Vector2f(90.f, 160.f));

    Text gameOverScoreTitle(font, "Score: ", 18);
    gameOverScore.setFillColor(COLOR_TEXT_SECONDARY);
    gameOverScore.setPosition(Vector2f(60.f, 160.f));

    Text gameOverMenu[GAMEOVER_OPTIONS] = {
        Text(font, "Restart", 20),
        Text(font, "Main Menu", 20)
    };

    for (int i = 0; i < GAMEOVER_OPTIONS; i++) {
        gameOverMenu[i].setFillColor(COLOR_TEXT_SECONDARY);
        gameOverMenu[i].setPosition(Vector2f(110.f, 200.f + i * 30.f));
    }
    sf::RectangleShape pausePane;
    pausePane.setSize(sf::Vector2f(200.f, 220.f));
    pausePane.setFillColor(COLOR_PANEL_BG); // semi-transparent black
    pausePane.setPosition(Vector2f (60.f, 100.f)); // adjust as needed

    // Dim background overlay
    sf::RectangleShape dimLayer;
    dimLayer.setSize({320.f, 480.f});
    dimLayer.setFillColor(COLOR_DIM_LAYER);

    // Generic panel (used for menus, help, highscore)
    sf::RectangleShape uiPanel;
    uiPanel.setSize({250.f, 300.f});
    uiPanel.setFillColor(COLOR_PANEL_BG);
    uiPanel.setPosition({35.f, 90.f});

    // Panel border (optional but recommended)
    sf::RectangleShape uiPanelBorder = uiPanel;
    uiPanelBorder.setFillColor(sf::Color::Transparent);
    uiPanelBorder.setOutlineThickness(2.f);
    uiPanelBorder.setOutlineColor(COLOR_TEXT_ACCENT);

    ////menues and and other texts end
    //pre loop calls
    setBeginnerLevel();
    spawnNewPiece(colorNum,ctx);
    //pre loop calls end
    //game loop
    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

    //---Event Listening Part---//
    while (auto eOpt = window.pollEvent()) {      // Event is occurring — until the game is in running state
        auto const &event = *eOpt;

        if (event.is<sf::Event::Closed>())        // If cross/close is clicked/pressed
            window.close();                       // Opened window disposes

        if (ctx.gameState == STATE_PLAYING)       //increase survival time
            ctx.survivalTime += time;

       if (auto const *keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        // Backspace key for help and highscore
        if ((ctx.gameState == STATE_HIGHSCORE || ctx.gameState == STATE_HELP) &&
            keyPressed->scancode == sf::Keyboard::Scan::Backspace)
        {
            ctx.gameState = ctx.prevState;
        }


        if (ctx.gameState == STATE_GAMEOVER) {
            if (keyPressed->scancode == Keyboard::Scan::Up) {
                gameOverIndex = (gameOverIndex - 1 + GAMEOVER_OPTIONS) % GAMEOVER_OPTIONS;
            }
            else if (keyPressed->scancode == Keyboard::Scan::Down) {
                gameOverIndex = (gameOverIndex + 1) % GAMEOVER_OPTIONS;
            }
            else if (keyPressed->scancode == Keyboard::Scan::Enter) {
                if (gameOverIndex == 0) {
                    resetGame(delta_x, rotate, delay, timer);
                    ctx.gameState = STATE_PLAYING;
                } else {
                    ctx.gameState = STATE_MENU; // back to main menu
                }
            }
            continue;
        }
            //Startup Menu
            if (ctx.gameState == STATE_MENU) {
                if (keyPressed->scancode == sf::Keyboard::Scan::Up) {
                    startMenuIndex = (startMenuIndex - 1 + START_MENU_OPTIONS) % START_MENU_OPTIONS;
                }
                else if (keyPressed->scancode == sf::Keyboard::Scan::Down) {
                    startMenuIndex = (startMenuIndex + 1) % START_MENU_OPTIONS;
                }
                else if (keyPressed->scancode == sf::Keyboard::Scan::Enter) {
                    switch (startMenuIndex) {
                        case 0: // Start Game
                            resetGame(delta_x, rotate, delay, timer);
                            ctx.gameState = STATE_PLAYING;
                            break;

                        case 1: // High Scores
                            ctx.prevState = STATE_MENU;
                            ctx.gameState = STATE_HIGHSCORE;
                            break;

                        case 2: // Help
                            ctx.prevState = STATE_MENU;
                            ctx.gameState = STATE_HELP;
                            break;
                        case 3: // Exit
                            window.close();
                            break;
                    }
                }
                continue;
            }


            //Pause Menu
            if (ctx.gameState == STATE_PAUSED) {
                // Pause menu navigation
                if (keyPressed->scancode == sf::Keyboard::Scan::Up) {
                    pauseIndex = (pauseIndex - 1 + PAUSE_OPTIONS) % PAUSE_OPTIONS;
                }
                else if (keyPressed->scancode == sf::Keyboard::Scan::Down) {
                    pauseIndex = (pauseIndex + 1) % PAUSE_OPTIONS;
                }
                else if (keyPressed->scancode == sf::Keyboard::Scan::Enter) {
                    switch (pauseIndex) {
                        case 0: // Resume
                            ctx.gameState = STATE_PLAYING;
                            break;
                        case 1: // Restart
                            resetGame(delta_x,rotate,delay,timer);
                            ctx.gameState = STATE_PLAYING;
                            break;
                       case 2: { // Highscore
                            ctx.prevState = STATE_PAUSED;
                            ctx.gameState = STATE_HIGHSCORE;
                            break;
                        }
                        case 3: // Help
                            ctx.prevState = STATE_PAUSED;
                            ctx.gameState = STATE_HELP;
                            break;

                        case 4: // Quit
                            saveHighScore(ctx.score);
                            window.close();
                            break;
                    }
                }
                else if (keyPressed->scancode == sf::Keyboard::Scan::P) {
                    // Optional: allow toggling pause with P even while paused
                    ctx.gameState = STATE_PLAYING;
                }
            }
            // Actual game controls
            else {
                // Game controls (only active when not paused)
                if (keyPressed->scancode == sf::Keyboard::Scan::Up)
                    rotate = true;
                else if (keyPressed->scancode == sf::Keyboard::Scan::Left)
                    delta_x = -1;
                else if (keyPressed->scancode == sf::Keyboard::Scan::Right)
                    delta_x = 1;
                else if (keyPressed->scancode == sf::Keyboard::Scan::P && ctx.gameState == STATE_PLAYING) {
                    ctx.gameState = STATE_PAUSED;
                }
                else if (keyPressed->scancode == sf::Keyboard::Scan::Space)
                    hardDrop = true;
            }
        }

    }


        // Detect Down key separately 
        if (ctx.level == 1)
            delay = 0.35f;   // Beginner
        else
            delay = 0.20f;   // Advanced

        if (Keyboard::isKeyPressed(Keyboard::Key::Down))
            delay = 0.05f;

            

        ///////////////////////////////////////////////
        ///*** START CALLING YOUR FUNCTIONS HERE ***///
            if (ctx.gameState == STATE_PLAYING) {
                //hardrop
                if (hardDrop) {
                    ctx.score += 2 * ctx.level;
                    hardDropPiece(colorNum,ctx);
                    hardDrop = false;
                    timer = 0;   // prevent double lock
                }
                else {
                    if (!ctx.isTouchingGround)
                        fallingPiece(timer, delay, colorNum,ctx);
                }
                if (ctx.isTouchingGround) {
                    ctx.lockTimer += time;

                    if (ctx.lockTimer >= LOCK_DELAY) {
                        lockPiece(colorNum,ctx);

                        int lines = clearLines(ctx);
                        if (lines > 0) {
                            ctx.score += calculateLineScore(lines, ctx.level);
                            if (ctx.level == 1 && ctx.score >= 1000) {
                                ctx.level = 2;
                                setAdvancedLevel();
                            }


                        }

                        spawnNewPiece(colorNum,ctx);

                        // reset lock state
                        ctx.isTouchingGround = false;
                        ctx.lockTimer = 0.f;
                    }
                }



                movePiece(delta_x,ctx);
                delta_x = 0;

                if (rotate)
                {
                    rotatePiece(ctx);
                    rotate = false;
                }
                // AFTER falling, moving, rotating
                if (!canFall()) {
                    ctx.isTouchingGround = true;
                } else {
                    ctx.isTouchingGround = false;
                    ctx.lockTimer = 0.f;   // ← THIS is what you’re missing
                }

                updateDifficulty(ctx);
            }

        ///*** YOUR CALLING STATEMENTS END HERE ***///
        //////////////////////////////////////////////

        window.clear(Color::Black);
        window.draw(background);
        drawGhostPiece(sprite, window);
        //Draw menues
            //Start Menue
        if (ctx.gameState == STATE_MENU) {
            window.clear(Color::Black);
            window.draw(background);
            window.draw(dimLayer);
            window.draw(uiPanel);
            window.draw(uiPanelBorder);
            window.draw(menuTitle);

            for (int i = 0; i < START_MENU_OPTIONS; i++) {
                startMenu[i].setFillColor(
                    i == startMenuIndex ? COLOR_TEXT_ACCENT : COLOR_TEXT_SECONDARY
                );
                window.draw(startMenu[i]);
            }

            window.display();
            continue;
        }
        // highscore option
        if (ctx.gameState == STATE_HIGHSCORE) {
            window.clear(Color::Black);
            window.draw(background);
            window.draw(dimLayer);
            window.draw(uiPanel);
            window.draw(uiPanelBorder);
            window.draw(highTitle);

            Text highText(font);
            highText.setCharacterSize(16);
            highText.setFillColor(COLOR_TEXT_SECONDARY);
            highText.setPosition({80.f, 170.f});
            highText.setString(highScoreString());

            window.draw(highText);

            window.display();
            continue;

    }
        //help option
        if (ctx.gameState == STATE_HELP) {
                window.clear(Color::Black);
                window.draw(background);
                window.draw(dimLayer);
                window.draw(uiPanel);
                window.draw(uiPanelBorder);
                window.draw(helpTitle);
                window.draw(helpText);
                window.display();
                continue;

    }
        //gameover menu
 
        if (ctx.gameState == STATE_GAMEOVER) {
            window.clear(Color::Black);
            window.draw(background);

            // Dim everything
            window.draw(dimLayer);

            // Centered panel
            window.draw(uiPanel);
            window.draw(uiPanelBorder);

            // Title
            gameOverTitle.setCharacterSize(34);
            gameOverTitle.setFillColor(COLOR_TEXT_DANGER);
            gameOverTitle.setPosition({60.f, 120.f});
            window.draw(gameOverTitle);

            // Score
            gameOverScoreTitle.setFillColor(Color::Yellow);
            gameOverScoreTitle.setPosition({60.f, 170.f});
            window.draw(gameOverScoreTitle);

            gameOverScore.setString(std::to_string(ctx.score));
            gameOverScore.setFillColor(COLOR_TEXT_PRIMARY);
            gameOverScore.setPosition({140.f, 172.f});
            window.draw(gameOverScore);

            // Menu options
            for (int i = 0; i < GAMEOVER_OPTIONS; i++) {
                gameOverMenu[i].setFillColor(
                    i == gameOverIndex ? COLOR_TEXT_ACCENT : COLOR_TEXT_SECONDARY
                );
                gameOverMenu[i].setPosition({105.f, 220.f + i * 35.f});
                window.draw(gameOverMenu[i]);
            }

            window.display();
            continue;
        }


        // Draw placed blocks
        for (int i = 0; i < M; i++)
        {
            for (int j = 0; j < N; j++)
            {
                if (gameGrid[i][j] == 0) continue;
                sprite.setTextureRect(IntRect({gameGrid[i][j] * 18, 0}, {18, 18}));
                sprite.setPosition({j * 18.f, i * 18.f});
                sprite.move({28.f, 31.f});          // offset
                window.draw(sprite);
                if (i >= M - ctx.blockedRows)
                    sprite.setColor(sf::Color(80, 80, 80));
                else
                    sprite.setColor(sf::Color::White);
            }
        }

        

        // ---- Draw next piece preview ----
        for (int i = 0; i < 4; i++)
        {
            sprite.setColor(sf::Color::White);

            float x = (BLOCKS[nextPiece][i] % 2);  // integer division works too
            float y = (BLOCKS[nextPiece][i] / 2);

            // Correct IntRect constructor
            sprite.setTextureRect(IntRect({nextColor * 18,0}, { 18, 18}));

            // Correct setPosition usage
            sprite.setPosition(Vector2f(x * 18.f + 230.f, y * 18.f + 100.f));

            window.draw(sprite);
        }


        // Draw current falling piece
        sprite.setColor(sf::Color::White);

        for (int i = 0; i < 4; i++)
        {
            sprite.setTextureRect(IntRect({colorNum * 18, 0}, {18, 18}));
            sprite.setPosition({point_1[i][0] * 18.f, point_1[i][1] * 18.f});
            sprite.move({28.f, 31.f});
            window.draw(sprite);
        }

        // Draw score and level
            scoreValue.setString(std::to_string(ctx.score));

            if (ctx.level == 1)
                levelValue.setString("Beginner");
            else
                levelValue.setString("Advanced");

            window.draw(scoreLabel);
            window.draw(scoreValue);
            window.draw(levelLabel);
            window.draw(levelValue);


        //---The Final on Which Everything is Drawn Over is Loaded---//
        window.draw(frame);
        //pause menu
        if (ctx.gameState == STATE_PAUSED) {
            window.draw(dimLayer);
            window.draw(uiPanel);
            window.draw(uiPanelBorder);
            window.draw(pauseTitle);

            for (int i = 0; i < PAUSE_OPTIONS; i++) {
                pauseMenu[i].setFillColor(
                    i == pauseIndex ? COLOR_TEXT_ACCENT : COLOR_TEXT_SECONDARY
                );
                window.draw(pauseMenu[i]);
            }

            }
  //---The Window that now Contains the Frame is Displayed---//
        window.display();
    }

    return 0;
}

// End of main
// start of main functions
    void drawGhostPiece(sf::Sprite& sprite, sf::RenderWindow& window)
        {
            int ghost[4][2];
            for (int i = 0; i < 4; i++) {
                ghost[i][0] = point_1[i][0];
                ghost[i][1] = point_1[i][1];
            }

            while (true) {
                for (int i = 0; i < 4; i++)
                    ghost[i][1]++;

                bool valid = true;
                for (int i = 0; i < 4; i++) {
                    int x = ghost[i][0];
                    int y = ghost[i][1];
                    if (y >= M || x < 0 || x >= N || gameGrid[y][x]) {
                        valid = false;
                        break;
                    }
                }

                if (!valid) {
                    for (int i = 0; i < 4; i++)
                        ghost[i][1]--;
                    break;
                }
            }

            //  SET TEXTURE RECT EXPLICITLY
            sprite.setTextureRect(sf::IntRect({ colorNum * 18, 0 }, {18, 18}));
            sprite.setColor(sf::Color(255, 255, 255, 70));

            for (int i = 0; i < 4; i++) {
                sprite.setPosition(Vector2f (ghost[i][0] * 18.f + 28.f,ghost[i][1] * 18.f + 31.f));
                window.draw(sprite);
            }

            sprite.setColor(sf::Color::White);
        }


void sortHighScores(int scores[], int count)
{
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (scores[j] > scores[i]) {
                int temp = scores[i];
                scores[i] = scores[j];
                scores[j] = temp;
            }
        }
    }
}

// Load high scores from file (returns top 5)
void loadHighScores(int scores[], int& count)
{
    std::ifstream file("highscores.txt");
    count = 0;

    if (!file.is_open()) return;

    while (file >> scores[count] && count < MAX_HIGHSCORES)
        count++;

    file.close();
}

    // Save high score to file
void saveHighScore(int newScore)
{
    int scores[MAX_HIGHSCORES + 1];
    int count = 0;

    loadHighScores(scores, count);

    scores[count++] = newScore;

    sortHighScores(scores, count);

    if (count > MAX_HIGHSCORES)
        count = MAX_HIGHSCORES;

    std::ofstream file("highscores.txt", std::ios::trunc);
    for (int i = 0; i < count; i++)
        file << scores[i] << "\n";

    file.close();
}

// Display highscores as string
std::string highScoreString()
{
    int scores[MAX_HIGHSCORES];
    int count = 0;

    loadHighScores(scores, count);
    sortHighScores(scores, count);

    std::string text = "";

    for (int i = 0; i < count; i++){
        std::ostringstream oss;
        oss << std::setw(2) << (i + 1) << ". " << scores[i] << "\n";
        text += oss.str();
    }
        

    return text;
}

void resetGame(int& dx, bool& rotate, float& delay, float& timer)
{
    // Core game stats
    ctx.score = 0;
    ctx.level = 1;  

    // Difficulty state
    ctx.blockedRows = 0;
    ctx.survivalTime = 0.f;

    // Board
    memset(gameGrid, 0, sizeof(gameGrid));

    // Input / movement
    dx = 0;
    rotate = false;
    delay = 0.3f;   // whatever your starting delay is
    timer = 0;

    // Piece state
    setBeginnerLevel(); // or advanced

    nextPiece = allowedPieces[rand() % allowedCount];
    nextColor = 1 + rand() % 7;

    spawnNewPiece(colorNum,ctx);

    ctx.gameState = 1; // playing
}
