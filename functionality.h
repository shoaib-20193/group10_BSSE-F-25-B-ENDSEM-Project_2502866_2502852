/* PROGRAMMING FUNDAMENTAL'S PROJECT FOR FALL 2022 BS(CS)
 * You need to define the required function in the part instructed here below.
 * Avoid making any unnecessary changes, particularly the ones you don't understand.
 * The different pieces should be defined using an array - uncomment the following code once you are done creating the array.
 * TIP: Understand thoroughly before getting started with coding.
 * */
/*
Muhammad Shoaib 2502866
Haider Ali Naushad 2502852
BSSE-F-25-B
*/
int currentPiece;
int nextPiece;
int allowedPieces[7];
int allowedCount = 4; // beginner default
extern int nextColor;
extern int currentColor;


void saveHighScore(int score);



/////////////////////////////////////////////
///*** START CODING YOUR FUNTIONS HERE ***///
void setBeginnerLevel()
{
    allowedPieces[0] = 0; // I
    allowedPieces[1] = 3; // T
    allowedPieces[2] = 4; // L
    allowedPieces[3] = 6; // O
    allowedCount = 4;
}

void setAdvancedLevel()
{
    for (int i = 0; i < 7; i++)
        allowedPieces[i] = i;

    allowedCount = 7;
}
int calculateLineScore(int lines, int level)
{
    switch (lines)
    {
        case 1: return 10 * level;
        case 2: return 30 * level;
        case 3: return 60 * level;
        case 4: return 100 * level;
        default: return 0;
    }
}

void movePiece(int dx, GameContext& ctx)
{
    if (dx == 0) return;
    for (int i = 0; i < 4; i++)
    {
        point_2[i][0] = point_1[i][0];
        point_2[i][1] = point_1[i][1];
        point_1[i][0] += dx;
    }

    if (!anamoly(ctx))
    {
        for (int i = 0; i < 4; i++)
        {
            point_1[i][0] = point_2[i][0];
            point_1[i][1] = point_2[i][1];
        }


    }
}
void rotatePiece(GameContext& ctx)
{
    // O piece never rotates
    if (currentPiece == 6) return;

    int backup[4][2];
    for (int i = 0; i < 4; i++) {
        backup[i][0] = point_1[i][0];
        backup[i][1] = point_1[i][1];
    }

    // -----------------------------
    // SELECT PIVOT PER PIECE
    // -----------------------------

    int px = point_1[1][0];
    int py = point_1[1][1];


    // -----------------------------
    // ROTATE
    // -----------------------------
    for (int i = 0; i < 4; i++) {
        int x = point_1[i][1] - py;
        int y = point_1[i][0] - px;
        point_1[i][0] = px - x;
        point_1[i][1] = py + y;
    }

    // -----------------------------
    // IF VALID — DONE
    // -----------------------------
    if (anamoly(ctx)) return;

    // -----------------------------
    // WALL KICKS (TAILORED)
    // -----------------------------

    int kicks[5][2];

    if (currentPiece == 0) { // I-piece (needs stronger kicks)
        int temp[5][2] = { {2,0}, {-2,0}, {1,0}, {-1,0}, {0,0} };
        memcpy(kicks, temp, sizeof kicks);
    }
    else { // T, L, J, S, Z
        int temp[5][2] = { {1,0}, {-1,0}, {0,1}, {0,-1}, {0,0} };
        memcpy(kicks, temp, sizeof kicks);
    }

    for (int k = 0; k < 5; k++) {
        for (int i = 0; i < 4; i++) {
            point_1[i][0] += kicks[k][0];
            point_1[i][1] += kicks[k][1];
        }

        if (anamoly(ctx)) return;

        // revert kick
        for (int i = 0; i < 4; i++) {
            point_1[i][0] -= kicks[k][0];
            point_1[i][1] -= kicks[k][1];
        }
    }

    // -----------------------------
    // FAILED — REVERT
    // -----------------------------
    for (int i = 0; i < 4; i++) {
        point_1[i][0] = backup[i][0];
        point_1[i][1] = backup[i][1];
    }
}




void spawnNewPiece(int& colorNum, GameContext& ctx)
{
    currentPiece = nextPiece;
    currentColor = nextColor;

    nextPiece = allowedPieces[rand() % allowedCount];
    nextColor = 1 + rand() % 7;

    colorNum = currentColor;

    int spawnX = N / 2 - 1;

    for (int i = 0; i < 4; i++)
    {
        point_1[i][0] = (BLOCKS[currentPiece][i] % 2) + spawnX;
        point_1[i][1] = (BLOCKS[currentPiece][i] / 2);

        if (point_1[i][1] >= 0 &&
            gameGrid[point_1[i][1]][point_1[i][0]])
        {
            saveHighScore(ctx.score);
            ctx.gameState = 5;   // GAME OVER
            return;
        }
    }
}

int clearLines(GameContext& ctx)
{
    int cleared = 0;

    for (int i = M - 1 - ctx.blockedRows; i >= 0; i--)
    {
        bool full = true;
        for (int j = 0; j < N; j++)
            if (gameGrid[i][j] == 0)
            {
                full = false;
                break;
            }

        if (full)
        {
            cleared++;

            for (int k = i; k > 0; k--)
                for (int j = 0; j < N; j++)
                    gameGrid[k][j] = gameGrid[k - 1][j];

            for (int j = 0; j < N; j++)
                gameGrid[0][j] = 0;

            i++;
        }
    }
    return cleared;
}

void lockPiece(int colorNum, GameContext& ctx)
{
    for (int i = 0; i < 4; i++)
    {
        if (point_2[i][1] < 0)
            ctx.gameState = 5;   // GAME OVER

        gameGrid[point_1[i][1]][point_1[i][0]] = colorNum;

    }
}

void hardDropPiece(int& colorNum, GameContext& ctx)
{
    // Move down until collision
    while (true)
    {
        for (int i = 0; i < 4; i++)
        {
            point_2[i][0] = point_1[i][0];
            point_2[i][1] = point_1[i][1];
            point_1[i][1]++;
        }

        if (!anamoly(ctx))
        {
            // revert last move
            for (int i = 0; i < 4; i++)
                point_1[i][1]--;

            lockPiece(colorNum,ctx);
            int lines = clearLines(ctx);

            if (lines > 0)
            {
                ctx.score += calculateLineScore(lines, ctx.level);
                if (ctx.level == 1 && ctx.score >= 1000) {
                    ctx.level = 2;
                    setAdvancedLevel();
                }


            }
            
            spawnNewPiece(colorNum,ctx);
            lines = 0;
            ctx.isTouchingGround = false;
            ctx.lockTimer = 0.f;

            return;
        }
    }
}


void updateDifficulty(GameContext& ctx)
{
    if (ctx.survivalTime >= 30.f) // 5 minutes
    {
        ctx.survivalTime = 0.f;
        ctx.blockedRows++;

        if (ctx.blockedRows > M - 1)
            ctx.blockedRows = M - 1;
    }
}
bool canFall() {
    for (int i = 0; i < 4; i++) {
        int x = point_1[i][0];
        int y = point_1[i][1] + 1; // next row
        if (y >= M || gameGrid[y][x] != 0) {
            return false; // blocked
        }
    }
    return true; // can fall
}



///*** YOUR FUNCTIONS END HERE ***///
/////////////////////////////////////
//---Piece Starts to Fall When Game Starts---//
void fallingPiece(float& timer, float& delay, int& colorNum, GameContext& ctx) {
    if (timer > delay) {
        // move down
        for (int i = 0; i < 4; i++) {
            point_2[i][0] = point_1[i][0];
            point_2[i][1] = point_1[i][1];
            point_1[i][1] += 1;
        }

        if (!anamoly(ctx)) { // collision happened
            for (int i = 0; i < 4; i++)
                point_1[i][1]--; // revert last move
        }


        timer = 0;
    }
}

