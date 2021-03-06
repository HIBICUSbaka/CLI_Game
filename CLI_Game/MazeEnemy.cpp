#include "MazeEnemy.h"
#include "AppDeclared.h"
#include "Tools.h"
#include "SceneManager.h"
#include "BattleEnemy.h"

void InitMazeEnemy()
{
    char temp[3] = "‘";
    for (int i = 0; i < MAZEENEMYSIZE; i++)
    {
        (GetMazeEnemyArray() + i)->ID = -1;
        *((GetMazeEnemyArray() + i)->Sprite) = temp[0];
        *((GetMazeEnemyArray() + i)->Sprite + 1) = temp[1];
        (GetMazeEnemyArray() + i)->ObjSelf.Position = POSITION_2D(-1, -1);
        (GetMazeEnemyArray() + i)->ObjSelf.ColliRadius = 0.5f;
        (GetMazeEnemyArray() + i)->IsDead = 0;
        (GetMazeEnemyArray() + i)->TurnOff();
        (GetMazeEnemyArray() + i)->ScanFlag = STOP_MOVING;
        (GetMazeEnemyArray() + i)->MovingFlag = STOP_MOVING;
    }
}

void SetMazeEnemy(POSITION_2D pos)
{
    int index = 0;
    while ((GetMazeEnemyArray() + index)->ID != -1)
    {
        ++index;
    }

    (GetMazeEnemyArray() + index)->ID = index;
    (GetMazeEnemyArray() + index)->ObjSelf.Position = pos;
    (GetMazeEnemyArray() + index)->TurnOn();
}

void TurnOnAllEnemy()
{
    int index = 0;
    while ((GetMazeEnemyArray() + index)->ID != -1)
    {
        if (!(GetMazeEnemyArray() + index)->IsDead)
        {
            (GetMazeEnemyArray() + index)->TurnOn();
        }
        ++index;
    }
}

void TurnOffAllEnemy()
{
    int index = 0;
    while ((GetMazeEnemyArray() + index)->ID != -1)
    {
        if (!(GetMazeEnemyArray() + index)->IsDead)
        {
            (GetMazeEnemyArray() + index)->TurnOff();
        }
        ++index;
    }
}

void UpdateMazeEnemy()
{
    int index = 0;
    while ((GetMazeEnemyArray() + index)->ID != -1)
    {
        if ((GetMazeEnemyArray() + index)->Visible == 1)
        {
            UpdateSingleMazeEnemy(GetMazeEnemyArray() + index);
        }
        ++index;
    }
}

void UpdateSingleMazeEnemy(MAZEENEMY* mazeEnemy)
{
    ScanPlayer(mazeEnemy);
    ManageMazeEnemyMove(mazeEnemy);

    int width = GetCurrScene()->GetCamAddr()->CameraWidth;
    int height = GetCurrScene()->GetCamAddr()->CameraHeight;
    POSITION_2D deltaWithPlayer =
        mazeEnemy->ObjSelf.Position - GetPlayer()->ObjSelf.Position;
    if ((deltaWithPlayer.posX * deltaWithPlayer.posX <
        (width / 2 * width / 2)) &&
        (deltaWithPlayer.posY * deltaWithPlayer.posY <
            (height / 2 * height / 2)))
    {
        deltaWithPlayer =
            POSITION_2D(width / 2, height / 2) + deltaWithPlayer;
        char* buffer = GetCurrScene()->GetCamAddr()->GetCamBuffer();
        *(buffer + deltaWithPlayer.posY * width + deltaWithPlayer.posX) =
            *(mazeEnemy->Sprite);
        *(buffer + deltaWithPlayer.posY * width + deltaWithPlayer.posX + 1) =
            *(mazeEnemy->Sprite + 1);
    }

    if (mazeEnemy->ObjSelf.IsCollied(GetPlayer()->ObjSelf))
    {
        DebugLog("get collied with player");
        mazeEnemy->IsDead = 1;
        mazeEnemy->TurnOff();
        SwitchSceneToName("battle");
        RandomlySetBattleStageOffset();
        SetEnemyToBattleRandomly();
        SetIsPlayingMaze(0);
        SetIsPlayingBattle(1);
        TurnOffAllEnemy();
    }
}

void ScanPlayer(MAZEENEMY* mazeEnemy)
{
    int deltaX = mazeEnemy->ObjSelf.GetDeltaXWith(GetPlayer()->ObjSelf) / 2;
    int deltaY = mazeEnemy->ObjSelf.GetDeltaYWith(GetPlayer()->ObjSelf);

    if ((deltaX >= -15 && deltaX <= 0 && deltaY >= -5 && deltaY <= 0) ||
        (deltaX >= 0 && deltaX <= 15 && deltaY >= -5 && deltaY <= 0) ||
        (deltaX >= -15 && deltaX <= 0 && deltaY >= 0 && deltaY <= 5) ||
        (deltaX >= 0 && deltaX <= 15 && deltaY >= 0 && deltaY <= 5))
    {
        if ((deltaX * deltaX - deltaY * deltaY) >= 0)
        {
            if (deltaX >= 0)
            {
                mazeEnemy->ScanFlag = TURN_LEFT;
            }
            else
            {
                mazeEnemy->ScanFlag = TURN_RIGHT;
            }
        }
        else
        {
            if (deltaY >= 0)
            {
                mazeEnemy->ScanFlag = GO_FORWARD;
            }
            else
            {
                mazeEnemy->ScanFlag = GO_BACK;
            }
        }
    }
    else
    {
        mazeEnemy->ScanFlag = STOP_MOVING;
    }
}

void ManageMazeEnemyMove(MAZEENEMY* mazeEnemy)
{
    switch (mazeEnemy->ScanFlag)
    {
    case GO_FORWARD:
        MazeEnemyMoveForward(mazeEnemy);
        return;

    case GO_BACK:
        MazeEnemyMoveBack(mazeEnemy);
        return;

    case TURN_LEFT:
        MazeEnemyTurnLeft(mazeEnemy);
        return;

    case TURN_RIGHT:
        MazeEnemyTurnRight(mazeEnemy);
        return;

    default:
        break;
    }

    switch (mazeEnemy->MovingFlag)
    {
    case GO_FORWARD:
        MazeEnemyMoveForward(mazeEnemy);
        return;

    case GO_BACK:
        MazeEnemyMoveBack(mazeEnemy);
        return;

    case TURN_LEFT:
        MazeEnemyTurnLeft(mazeEnemy);
        return;

    case TURN_RIGHT:
        MazeEnemyTurnRight(mazeEnemy);
        return;

    default:
        break;
    }

    // TODO ?Κ?Cό?_ΚuζΑΉ?’bug(ί?Ω@Δ?)CAωv[ό€C@Κ??ϊuσrolloόΊI?ο?vz??EsfόΊ
    int successFlag = 0;
    int direction = 0;
    SetRandom();
    while (!successFlag)
    {
        direction = CreateRandomNumIn(1, 4);
        switch (direction)
        {
        case GO_FORWARD:
            if (*(GetMazeMap()->GetMap() +
                (mazeEnemy->ObjSelf.Position.posY - 1) * MAZEMAPMAXLENGTH +
                mazeEnemy->ObjSelf.Position.posX) == '-' ||
                *(GetMazeMap()->GetMap() +
                    (mazeEnemy->ObjSelf.Position.posY - 1) * MAZEMAPMAXLENGTH +
                    mazeEnemy->ObjSelf.Position.posX) == '|' ||
                *(GetMazeMap()->GetMap() +
                    (mazeEnemy->ObjSelf.Position.posY - 1) * MAZEMAPMAXLENGTH +
                    mazeEnemy->ObjSelf.Position.posX + 1) == '-' ||
                *(GetMazeMap()->GetMap() +
                    (mazeEnemy->ObjSelf.Position.posY - 1) * MAZEMAPMAXLENGTH +
                    mazeEnemy->ObjSelf.Position.posX + 1) == '|')
            {

            }
            else
            {
                mazeEnemy->MovingFlag = GO_FORWARD;
                successFlag = 1;
            }
            break;

        case GO_BACK:
            if (*(GetMazeMap()->GetMap() +
                (mazeEnemy->ObjSelf.Position.posY + 1) * MAZEMAPMAXLENGTH +
                mazeEnemy->ObjSelf.Position.posX) == '-' ||
                *(GetMazeMap()->GetMap() +
                    (mazeEnemy->ObjSelf.Position.posY + 1) * MAZEMAPMAXLENGTH +
                    mazeEnemy->ObjSelf.Position.posX) == '|' ||
                *(GetMazeMap()->GetMap() +
                    (mazeEnemy->ObjSelf.Position.posY + 1) * MAZEMAPMAXLENGTH +
                    mazeEnemy->ObjSelf.Position.posX + 1) == '-' ||
                *(GetMazeMap()->GetMap() +
                    (mazeEnemy->ObjSelf.Position.posY + 1) * MAZEMAPMAXLENGTH +
                    mazeEnemy->ObjSelf.Position.posX + 1) == '|')
            {

            }
            else
            {
                mazeEnemy->MovingFlag = GO_BACK;
                successFlag = 1;
            }
            break;

        case TURN_LEFT:
            if (*(GetMazeMap()->GetMap() +
                mazeEnemy->ObjSelf.Position.posY * MAZEMAPMAXLENGTH +
                mazeEnemy->ObjSelf.Position.posX - 1) == '-' ||
                *(GetMazeMap()->GetMap() +
                    mazeEnemy->ObjSelf.Position.posY * MAZEMAPMAXLENGTH +
                    mazeEnemy->ObjSelf.Position.posX - 1) == '|' ||
                *(GetMazeMap()->GetMap() +
                    mazeEnemy->ObjSelf.Position.posY * MAZEMAPMAXLENGTH +
                    mazeEnemy->ObjSelf.Position.posX - 2) == '-' ||
                *(GetMazeMap()->GetMap() +
                    mazeEnemy->ObjSelf.Position.posY * MAZEMAPMAXLENGTH +
                    mazeEnemy->ObjSelf.Position.posX - 2) == '|')
            {

            }
            else
            {
                mazeEnemy->MovingFlag = TURN_LEFT;
                successFlag = 1;
            }
            break;

        case TURN_RIGHT:
            if (*(GetMazeMap()->GetMap() +
                mazeEnemy->ObjSelf.Position.posY * MAZEMAPMAXLENGTH +
                mazeEnemy->ObjSelf.Position.posX + 2) == '-' ||
                *(GetMazeMap()->GetMap() +
                    mazeEnemy->ObjSelf.Position.posY * MAZEMAPMAXLENGTH +
                    mazeEnemy->ObjSelf.Position.posX + 2) == '|' ||
                *(GetMazeMap()->GetMap() +
                    mazeEnemy->ObjSelf.Position.posY * MAZEMAPMAXLENGTH +
                    mazeEnemy->ObjSelf.Position.posX + 3) == '-' ||
                *(GetMazeMap()->GetMap() +
                    mazeEnemy->ObjSelf.Position.posY * MAZEMAPMAXLENGTH +
                    mazeEnemy->ObjSelf.Position.posX + 3) == '|')
            {

            }
            else
            {
                mazeEnemy->MovingFlag = TURN_RIGHT;
                successFlag = 1;
            }
            break;

        default:
            ErrorLogI1("cannot dispose with the direct code", direction);
            break;
        }
    }
}

void MazeEnemyMoveForward(MAZEENEMY* mazeEnemy)
{
    if (*(GetMazeMap()->GetMap() +
        (mazeEnemy->ObjSelf.Position.posY - 1) * MAZEMAPMAXLENGTH +
        mazeEnemy->ObjSelf.Position.posX) == '-' ||
        *(GetMazeMap()->GetMap() +
            (mazeEnemy->ObjSelf.Position.posY - 1) * MAZEMAPMAXLENGTH +
            mazeEnemy->ObjSelf.Position.posX) == '|' ||
        *(GetMazeMap()->GetMap() +
            (mazeEnemy->ObjSelf.Position.posY - 1) * MAZEMAPMAXLENGTH +
            mazeEnemy->ObjSelf.Position.posX + 1) == '-' ||
        *(GetMazeMap()->GetMap() +
            (mazeEnemy->ObjSelf.Position.posY - 1) * MAZEMAPMAXLENGTH +
            mazeEnemy->ObjSelf.Position.posX + 1) == '|')
    {
        mazeEnemy->MovingFlag = STOP_MOVING;
    }
    else
    {
        --mazeEnemy->ObjSelf.Position.posY;
    }
}

void MazeEnemyMoveBack(MAZEENEMY* mazeEnemy)
{
    if (*(GetMazeMap()->GetMap() +
        (mazeEnemy->ObjSelf.Position.posY + 1) * MAZEMAPMAXLENGTH +
        mazeEnemy->ObjSelf.Position.posX) == '-' ||
        *(GetMazeMap()->GetMap() +
            (mazeEnemy->ObjSelf.Position.posY + 1) * MAZEMAPMAXLENGTH +
            mazeEnemy->ObjSelf.Position.posX) == '|' ||
        *(GetMazeMap()->GetMap() +
            (mazeEnemy->ObjSelf.Position.posY + 1) * MAZEMAPMAXLENGTH +
            mazeEnemy->ObjSelf.Position.posX + 1) == '-' ||
        *(GetMazeMap()->GetMap() +
            (mazeEnemy->ObjSelf.Position.posY + 1) * MAZEMAPMAXLENGTH +
            mazeEnemy->ObjSelf.Position.posX + 1) == '|')
    {
        mazeEnemy->MovingFlag = STOP_MOVING;
    }
    else
    {
        ++mazeEnemy->ObjSelf.Position.posY;
    }
}

void MazeEnemyTurnLeft(MAZEENEMY* mazeEnemy)
{
    if (*(GetMazeMap()->GetMap() +
        mazeEnemy->ObjSelf.Position.posY * MAZEMAPMAXLENGTH +
        mazeEnemy->ObjSelf.Position.posX - 1) == '-' ||
        *(GetMazeMap()->GetMap() +
            mazeEnemy->ObjSelf.Position.posY * MAZEMAPMAXLENGTH +
            mazeEnemy->ObjSelf.Position.posX - 1) == '|' ||
        *(GetMazeMap()->GetMap() +
            mazeEnemy->ObjSelf.Position.posY * MAZEMAPMAXLENGTH +
            mazeEnemy->ObjSelf.Position.posX - 2) == '-' ||
        *(GetMazeMap()->GetMap() +
            mazeEnemy->ObjSelf.Position.posY * MAZEMAPMAXLENGTH +
            mazeEnemy->ObjSelf.Position.posX - 2) == '|')
    {
        mazeEnemy->MovingFlag = STOP_MOVING;
    }
    else
    {
        mazeEnemy->ObjSelf.Position.posX -= 2;
    }
}

void MazeEnemyTurnRight(MAZEENEMY* mazeEnemy)
{
    if (*(GetMazeMap()->GetMap() +
        mazeEnemy->ObjSelf.Position.posY * MAZEMAPMAXLENGTH +
        mazeEnemy->ObjSelf.Position.posX + 2) == '-' ||
        *(GetMazeMap()->GetMap() +
            mazeEnemy->ObjSelf.Position.posY * MAZEMAPMAXLENGTH +
            mazeEnemy->ObjSelf.Position.posX + 2) == '|' ||
        *(GetMazeMap()->GetMap() +
            mazeEnemy->ObjSelf.Position.posY * MAZEMAPMAXLENGTH +
            mazeEnemy->ObjSelf.Position.posX + 3) == '-' ||
        *(GetMazeMap()->GetMap() +
            mazeEnemy->ObjSelf.Position.posY * MAZEMAPMAXLENGTH +
            mazeEnemy->ObjSelf.Position.posX + 3) == '|')
    {
        mazeEnemy->MovingFlag = STOP_MOVING;
    }
    else
    {
        mazeEnemy->ObjSelf.Position.posX += 2;
    }
}
