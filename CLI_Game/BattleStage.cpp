#include "BattleStage.h"
#include "SceneNode.h"
#include "Tools.h"

void InitBattleStage()
{
    for (int i = 0; i < BATTLESTAGECOUNT; i++)
    {
        for (int j = 0; j < BATTLESTAGEHEIGHT * BATTLESTAGEWIDTH; j++)
        {
            *(GetBattleStageByOffset(i) + j) = ' ';
        }
    }

    LoadBattleStage();
}

void LoadBattleStage()
{
    FILE* pFile = NULL;
    int startX = 0;
    int startY = 0;
    int flag = 0;
    char fileName[20];

    for (int i = 0; i < 3; i++)
    {
        startX = 0;
        startY = 0;
        flag = 0;
        sprintf_s(fileName, sizeof(fileName),
            "BattleStage%d.txt", i + 1);
        fopen_s(&pFile, fileName, "r");
        if (pFile == NULL)
        {
            ErrorLog("cannot open file");
            return;
        }

        while (flag != 3)
        {
            fscanf_s(pFile, "%d", &flag);
            switch (flag)
            {
            case 0:
                ++startX;
                break;

            case 1:
                *(GetBattleStageByOffset(i) +
                    startY * BATTLESTAGEWIDTH + startX) = '-';
                ++startX;
                break;

            case 2:
                startX = 0;
                ++startY;
                break;

            case 3:
                break;

            default:
                break;
            }
        }
    }
}

void DrawStageToCamBuffer(int offset)
{
    char* camBuffer = GetSceneNodeByName("battle")->
        GetCamAddr()->GetCamBuffer();
    int camWidth = GetSceneNodeByName("battle")->
        GetCamAddr()->CameraWidth;
    char* stageBuffer = GetBattleStageByOffset(offset);

    for (int i = 0; i < BATTLESTAGEHEIGHT; i++)
    {
        for (int j = 0; j < BATTLESTAGEWIDTH; j++)
        {
            *(camBuffer + (i + 1) * camWidth + 1 + j) =
                *(stageBuffer + i * BATTLESTAGEWIDTH + j);
        }
    }
}
