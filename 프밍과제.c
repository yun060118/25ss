#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <string.h>
#include <direct.h>

struct Dealer
{
    char* name;
    char* talk;
    char* answer;
    char* talk_O;
    char* talk_X;
};

int main(void)
{
    struct Dealer dealers[5] = {
        {"금이빨아저씨", "이빨 한 세트 얼마 줄래?", "금이빨아저씨", "정답! 금값 오르기 전에 팔아야지.", "틀렸어, 이건 진짜 금이야. 네 거랑 바꿀래?"},
        {"시계아저씨", "이 롤렉스... 진짜 같지? 응?", "시계아저씨", "정답! 진짜 아니야, 그래도 예뻐서 팔린다.", "틀렸어, 이건 플라스틱이지. 눈 좀 떠."},
        {"가죽점퍼아저씨", "이 점퍼, 92년도 한정판이야. 얼마 줄건데?", "가죽점퍼아저씨", "정답이야! 이건 역사 그 자체야.", "틀렸어, 가죽이 아니라 고무야!"},
        {"휴대폰아저씨", "이 폰, 3일 전에 산 거야. 배터리만 살짝...", "휴대폰아저씨", "정답! 배터리 빼고 전부 고철이지.", "틀렸어, 이건 2G야."},
        {"선글라스아저씨", "이거 끼면 인생이 달라져. 믿어?", "선글라스아저씨", "정답이야, 세상이 더 어두워지지.", "틀렸어, 거울에 비친 네가 달라질 뿐이야."}
    };

    char cwd[1024];
    if (_getcwd(cwd, sizeof(cwd)) != NULL)
        printf("현재 작업 디렉터리: %s\n\n", cwd);

    FILE* fp = NULL;
    int total_games = 0, correct_games = 0;

    if (fopen_s(&fp, "log.txt", "r") == 0 && fp != NULL)
    {
        char name[128], result[32];
        while (fscanf_s(fp, "%127s %31s", name, (unsigned)_countof(name), result, (unsigned)_countof(result)) == 2)
        {
            total_games++;
            if (strcmp(result, "정답") == 0)
                correct_games++;
        }
        fclose(fp);
    }

    printf("오늘은 %d번째 거래입니다.\n", total_games + 1);
    if (total_games > 0)
        printf("성공률: %d/%d (%.1f%%)\n\n", correct_games, total_games, (correct_games * 100.0) / total_games);
    else
        printf("첫 거래를 시작하지.\n\n");

    srand((unsigned int)time(NULL));
    char buf[64];

    while (1)
    {
        printf("손님이 문을 연다. 거래할까? (Y/N): ");
        if (fgets(buf, sizeof(buf), stdin) == NULL)
        {
            printf("입력 오류 - 종료합니다.\n");
            return 1;
        }

        char YN = buf[0];

        if (YN == 'Y' || YN == 'y')
        {
            printf("문이 삐걱 열린다");
            for (int k = 0; k < 6; k++)
            {
                Sleep(300);
                printf(".");
                fflush(stdout);
            }
            printf("\n");

            int idx = rand() % 5;
            printf("%s가 들어왔다!\n", dealers[idx].name);
            printf("%s: %s\n", dealers[idx].name, dealers[idx].talk);

            char answer[128];
            printf("상인 이름을 입력: ");
            if (fgets(answer, sizeof(answer), stdin) == NULL)
            {
                printf("입력 오류 - 종료합니다.\n");
                return 1;
            }

            size_t len = strlen(answer);
            if (len > 0 && answer[len - 1] == '\n')
                answer[len - 1] = '\0';

            int correct = 0;
            if (strcmp(answer, dealers[idx].answer) == 0)
            {
                printf("%s: %s\n", dealers[idx].name, dealers[idx].talk_O);
                correct = 1;
            }
            else
            {
                printf("%s: %s\n", dealers[idx].name, dealers[idx].talk_X);
            }

            if (fopen_s(&fp, "log.txt", "a") != 0 || fp == NULL)
            {
                printf("⚠️ log.txt 파일 생성 실패!\n");
                return 1;
            }

            fprintf(fp, "%s %s\n", dealers[idx].name, correct ? "정답" : "오답");
            fclose(fp);

            printf("\n결과가 log.txt에 저장되었습니다.\n");
            printf("거래 종료.\n");
            break;
        }
        else if (YN == 'N' || YN == 'n')
        {
            Sleep(300);
            printf("문을 닫고 불을 끈다... 오늘은 장사 접자.\n");
            break;
        }
        else
        {
            printf("잘못된 입력이야. Y 또는 N으로 말해.\n\n");
        }
    }

    return 0;
}
