#include <stdio.h>
#include <math.h>
#include <Windows.h>
#include <stdlib.h>
#include <time.h>

#define PI 3.14159226535897

void moveCursor(int x, int y)
{
    printf("\x1b[%d;%dH", y, x);
}

// 색상
#define RESET "\x1b[0m"
#define RED   "\x1b[31m"
#define YEL   "\x1b[33m"
#define BOLD  "\x1b[1m"

// 폭발 메시지 후보
const char* boomMsg[] = { "BOOM!", "BANG!", "KABOOM!", "!!POW!!" };

// 폭발 애니메이션
void explosionEffect(int bombX, int bombY)
{
    int i;
    srand((unsigned)time(NULL));

    // 랜덤 메시지
    const char* msg = boomMsg[rand() % 4];

    for (i = 0; i < 6; i++) {
        moveCursor(bombX, bombY);
        if (i % 2 == 0) {
            printf(RED BOLD "^^^^^^^" RESET);
            printf("\x1b[B\x1b[7D" YEL BOLD "%s" RESET, msg);
            printf("\x1b[B\x1b[7D" RED BOLD "^^^^^^^" RESET);
        }
        else {
            printf(YEL BOLD "*******" RESET);
            printf("\x1b[B\x1b[7D" RED BOLD "%s" RESET, msg);
            printf("\x1b[B\x1b[7D" YEL BOLD "*******" RESET);
        }
        Sleep(150);
    }
}

// 폭탄 출력
void printBomb(int isExploded, int bombX, int bombY)
{
    moveCursor(bombX, bombY);
    if (isExploded) {
        explosionEffect(bombX, bombY);
    }
    else {
        printf("(  b  )");
    }
}

int main()
{
    int startX = 14, startY = 9;      // 불꽃 시작 좌표
    int x = startX, y = startY;

    int bombX = 2, bombY = 2;         // 폭탄 고정 좌표
    const int BOMB_LEN = 7;           // "(  b  )" 폭 7칸

    /* 방향: ↓ → ↑ ← (반시계) */
    int dirX[4] = { 0, 1, 0, -1 };
    int dirY[4] = { 1, 0,-1,  0 };

    int tx, ty, nx, ny;
    int i, j, d;

    /* 1) 심지(#) 전체 그리기 */
    tx = x; ty = y;
    for (i = 0; i < 12; i++) {
        d = i % 4;
        int step = (i == 0 ? 1 : (i == 11 ? 13 : i + 2));

        for (j = 0; j < step; j++) {
            tx += dirX[d];
            ty += dirY[d];
            moveCursor(tx, ty);
            printf(RED "#" RESET);
        }
    }

    /* 2) 폭탄 출력 */
    printBomb(0, bombX, bombY);

    /* 3) 불꽃 이동 */
    tx = x; ty = y;
    moveCursor(tx, ty); printf(YEL "*" RESET);

    for (i = 0; i < 12; i++) {
        d = i % 4;
        int step = (i == 0 ? 1 : (i == 11 ? 13 : i + 2));

        for (j = 0; j < step; j++) {
            Sleep(150);

            // 불꽃 자취 남김 (흰 점)
            moveCursor(tx, ty); printf(".");

            nx = tx + dirX[d];
            ny = ty + dirY[d];

            if (ny == bombY && nx >= bombX && nx <= bombX + BOMB_LEN - 1) {
                printBomb(1, bombX, bombY);
                moveCursor(0, 20); // 커서 내려놓기
                return 0;
            }

            tx = nx; ty = ny;
            moveCursor(tx, ty); printf(YEL "*" RESET);
        }
    }

    moveCursor(0, 20);
    return 0;
}
