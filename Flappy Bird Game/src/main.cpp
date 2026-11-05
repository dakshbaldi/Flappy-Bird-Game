
#include <raylib.h>

int main() {

    InitWindow(480, 800, "Flappy Bird");
    SetTargetFPS(60);

    Texture2D background = LoadTexture("assets/bg.png");
    Texture2D ground = LoadTexture("assets/ground.png");
    Texture2D birdup = LoadTexture("assets/birdup.png");
    Texture2D birddown = LoadTexture("assets/birddown.png");
    Texture2D pipeup = LoadTexture("assets/pipeup.png");
    Texture2D pipedown = LoadTexture("assets/pipedown.png");

    float groundX = 0;

    float birdY = 350;
    float birdVelocity = 0;
    float birdRotation = 0;

    const int pipeCount = 3;

    float pipeX[pipeCount] = {600, 920, 1240};
    float gapY[pipeCount] = {300, 400, 350};
    bool pipeScored[pipeCount] = {false,false,false};

    int gapSize = 180;
    int pipeSpacing = 320;

    bool gameOver = false;
    int score = 0;

    while (WindowShouldClose() == false) {

        if (gameOver == false) {

            groundX -= 120 * GetFrameTime();

            if (groundX <= -ground.width) {
                groundX = 0;
            }

            birdVelocity += 500 * GetFrameTime();
            birdY += birdVelocity * GetFrameTime();

            if (IsKeyPressed(KEY_SPACE)) {
                birdVelocity = -250;
            }

            if (birdVelocity < 0) {
                birdRotation = -20;
            }
            else {
                birdRotation = 30;
            }

            for (int i = 0; i < pipeCount; i++) {

                pipeX[i] -= 120 * GetFrameTime();

                if(pipeX[i]+pipeup.width < 100 && pipeScored[i] == false) {
                    score += 5;
                    pipeScored[i] = true;
                }

                if (pipeX[i] < -pipeup.width) {

                    float lastPipe = pipeX[0];

                    for (int j = 1; j < pipeCount; j++) {
                        if (pipeX[j] > lastPipe) {
                            lastPipe = pipeX[j];
                        }
                    }

                    pipeX[i] = lastPipe + pipeSpacing;
                    gapY[i] = GetRandomValue(250, 500);
                    pipeScored[i] = false;
                }
            }

            Rectangle birdRect = {
                106,
                birdY + 5,
                36,
                26
            };

            bool collision = false;

            for (int i = 0; i < pipeCount; i++) {

                Rectangle topPipe = {
                    pipeX[i],
                    gapY[i] - gapSize / 2 - pipedown.height,
                    (float)pipedown.width,
                    (float)pipedown.height
                };

                Rectangle bottomPipe = {
                    pipeX[i],
                    gapY[i] + gapSize / 2,
                    (float)pipeup.width,
                    (float)pipeup.height
                };

                if (CheckCollisionRecs(birdRect, topPipe) ||
                    CheckCollisionRecs(birdRect, bottomPipe)) {

                    collision = true;
                }
            }

            if (birdY + 36 >= 700) {
                collision = true;
            }

            if (birdY <= 0) {
                collision = true;
            }

            if (collision) {
                gameOver = true;
            }
        }

        BeginDrawing();

        ClearBackground(SKYBLUE);

        DrawTexture(background, 0, 0, WHITE);

        DrawTexture(ground, groundX, 700, WHITE);
        DrawTexture(ground, groundX + ground.width, 700, WHITE);

        Texture2D currentBird =
            (birdVelocity < 0) ? birdup : birddown;

        DrawTexturePro(
            currentBird,
            {0, 0, (float)currentBird.width, (float)currentBird.height},
            {124, birdY + 18, 48, 36},
            {24, 18},
            birdRotation,
            WHITE
        );

        for (int i = 0; i < pipeCount; i++) {

            DrawTexture(
                pipedown,
                pipeX[i],
                gapY[i] - gapSize / 2 - pipedown.height,
                WHITE
            );

            DrawTexture(
                pipeup,
                pipeX[i],
                gapY[i] + gapSize / 2,
                WHITE
            );
        }

        DrawText(TextFormat("score : %i" , score) , 180 , 50 , 40 , WHITE);

        if (gameOver) {
            DrawText("GAME OVER", 100, 350, 40, RED);
        }

        EndDrawing();
    }

    UnloadTexture(background);
    UnloadTexture(ground);
    UnloadTexture(birdup);
    UnloadTexture(birddown);
    UnloadTexture(pipeup);
    UnloadTexture(pipedown);

    CloseWindow();

    return 0;
}
