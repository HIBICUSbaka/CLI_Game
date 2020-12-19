#pragma once

#include <string.h>
#include <stdio.h>
#include "DefinedValues.h"
#include "LogsOutput.h"

struct POSITION_2D
{
    int posX;
    int posY;

    POSITION_2D()
    {
        posX = -1;
        posY = -1;
    }

    POSITION_2D(int _x, int _y)
    {
        posX = _x;
        posY = _y;
    }

    int operator==(const POSITION_2D other) const
    {
        return (this->posX == other.posX) && (this->posY == other.posY);
    }

    int operator!=(const POSITION_2D other) const
    {
        return (this->posX != other.posX) || (this->posY != other.posY);
    }

    POSITION_2D operator+(const POSITION_2D other) const
    {
        POSITION_2D temp(0, 0);
        temp.posX = this->posX + other.posX;
        temp.posY = this->posY + other.posY;
        return temp;
    }

    POSITION_2D operator-(const POSITION_2D other) const
    {
        POSITION_2D temp(0, 0);
        temp.posX = this->posX - other.posX;
        temp.posY = this->posY - other.posY;
        return temp;
    }

    POSITION_2D operator*(const int value) const
    {
        POSITION_2D temp(0, 0);
        temp.posX = this->posX * value;
        temp.posY = this->posY * value;
        return temp;
    }

    POSITION_2D operator/(const int value) const
    {
        POSITION_2D temp(0, 0);
        temp.posX = this->posX / value;
        temp.posY = this->posY / value;
        return temp;
    }
};

struct QSINGLENODE
{
    int Data;
    QSINGLENODE* Next;
};

struct QUEUE_INT
{
    QSINGLENODE* Top;
    QSINGLENODE* End;
};

struct UI_TEXT
{
    TYPEID TypeID;
    int ID;
    POSITION_2D Position;
    char Text[512];

    UI_TEXT() {}

    UI_TEXT(POSITION_2D _position, char* _text)
    {
        Position = _position;
        strcpy_s(Text, 512 * sizeof(char), _text);
    }

    void ChangeTextTo(const char* text)
    {
        strcpy_s(Text, 512 * sizeof(char), text);
    }

    void ChangeTextTo(const char* text, int value)
    {
        char temp[512];
        sprintf_s(temp, sizeof(temp), "%s%d", text, value);
        strcpy_s(Text, 512 * sizeof(char), temp);
    }

    void ChangeTextTo(const char* text, int value1, int value2)
    {
        char temp[512];
        sprintf_s(temp, sizeof(temp), "%s%d, %d", text, value1, value2);
        strcpy_s(Text, 512 * sizeof(char), temp);
    }
};

enum class BTN_DESIGN
{
    LINE,
    STRAIGHT
};

struct UI_BUTTON
{
    TYPEID TypeID;
    int ID;
    int Event;
    POSITION_2D Position;
    char Text[50];
    int TextLength;
    BTN_DESIGN BorderDesign;
    UI_BUTTON* LeftBtn;
    UI_BUTTON* RightBtn;
    UI_BUTTON* UpBtn;
    UI_BUTTON* DownBtn;

    UI_BUTTON() {}

    UI_BUTTON(int _event, POSITION_2D _position, char* _text,
        BTN_DESIGN _design)
    {
        Event = _event;
        Position = _position;
        strcpy_s(Text, 50 * sizeof(char), _text);
        TextLength = strlen(Text);
        BorderDesign = _design;
        LeftBtn = NULL;
        RightBtn = NULL;
        UpBtn = NULL;
        DownBtn = NULL;
    }
};

enum class UIO_DESIGN
{
    NOTHING,
    STAR,
    STRAIGHT
};

struct UIOBJECT
{
#define TEXTSIZEINUIO 20
#define BTNSIZEINUIO 20
    TYPEID TypeID;
    int ID;
    char UIName[50];
    POSITION_2D StartPoint;
    int Width;
    int Height;
    UIO_DESIGN BorderDesign;
    UIOBJECT* ParentUIO;
    UIOBJECT* ChildUIO;
    int Visiblity;
    UI_TEXT Texts[TEXTSIZEINUIO];
    UI_BUTTON Buttons[BTNSIZEINUIO];

    UIOBJECT() {}

    UIOBJECT(TYPEID _typeID, int _id, char* _name,
        POSITION_2D _startPoint, int _width, int _height,
        UIO_DESIGN _design, UIOBJECT* _parent, UIOBJECT* _child,
        int _visiblity)
    {
        TypeID = _typeID;
        ID = _id;
        strcpy_s(UIName, sizeof(UIName), _name);
        StartPoint = _startPoint;
        Width = _width;
        Height = _height;
        BorderDesign = _design;
        ParentUIO = _parent;
        ChildUIO = _child;
        Visiblity = _visiblity;
        for (int i = 0; i < TEXTSIZEINUIO; i++)
        {
            Texts[i].ID = -1;
        }
        for (int i = 0; i < BTNSIZEINUIO; i++)
        {
            Buttons[i].ID = -1;
            Buttons[i].LeftBtn = NULL;
            Buttons[i].RightBtn = NULL;
            Buttons[i].UpBtn = NULL;
            Buttons[i].DownBtn = NULL;
        }
    }

    void AddParent(UIOBJECT* parent)
    {
        ParentUIO = parent;
    }

    void AddChild(UIOBJECT* child)
    {
        ChildUIO = child;
    }

    void AddText(UI_TEXT text)
    {
        int index = 0;
        while (index < TEXTSIZEINUIO)
        {
            if (Texts[index].ID == -1)
            {
                text.TypeID = TYPEID::TextObj;
                text.ID = 20000 + index;
                text.Position.posX += GetPosition().posX;
                text.Position.posY += GetPosition().posY;
                Texts[index] = text;
                break;
            }
            ++index;
        }
    }

    UI_TEXT* GetTextByID(int id)
    {
        if (Texts[id - 20000].ID != -1)
        {
            return &Texts[id - 20000];
        }
        else
        {
            ErrorLogI1("you don't have a text object which id is", id);
            return NULL;
        }
    }

    void AddBtn(UI_BUTTON btn)
    {
        int index = 0;
        int leastXPlus = 200;
        int leastXMinus = -200;
        int leastYPlus = 100;
        int leastYMinus = -100;

        btn.Position.posX += GetPosition().posX;
        btn.Position.posY += GetPosition().posY;

        while (index < BTNSIZEINUIO)
        {
            if (Buttons[index].ID == -1)
            {
                btn.TypeID = TYPEID::ButtonObj;
                btn.ID = 30000 + index;
                Buttons[index] = btn;

                if (Buttons[index].UpBtn != NULL)
                {
                    Buttons[index].UpBtn->DownBtn = &Buttons[index];
                }
                if (Buttons[index].DownBtn != NULL)
                {
                    Buttons[index].DownBtn->UpBtn = &Buttons[index];
                }
                if (Buttons[index].LeftBtn != NULL)
                {
                    Buttons[index].LeftBtn->RightBtn = &Buttons[index];
                }
                if (Buttons[index].RightBtn != NULL)
                {
                    Buttons[index].RightBtn->LeftBtn = &Buttons[index];
                }

                break;
            }

            int deltaX = (btn.Position.posX - Buttons[index].Position.posX) / 2;
            int deltaY = btn.Position.posY - Buttons[index].Position.posY;
            int deltaXY = deltaX * deltaX - deltaY * deltaY;
            if (deltaXY >= 0)
            {
                // ����
                if (deltaX >= 0 && deltaX <= leastXPlus)
                {
                    btn.LeftBtn = &Buttons[index];
                    leastXPlus = deltaX;
                }
                else if (deltaX <= leastXMinus)
                {
                    btn.RightBtn = &Buttons[index];
                    leastXMinus = deltaX;
                }
            }
            else
            {
                // ����
                if (deltaY >= 0 && deltaY <= leastYPlus)
                {
                    btn.UpBtn = &Buttons[index];
                    leastYPlus = deltaY;
                }
                else if (deltaY <= leastYMinus)
                {
                    btn.DownBtn = &Buttons[index];
                    leastYMinus = deltaY;
                }
            }

            ++index;
        }
    }

    UI_BUTTON* GetBtnByID(int id)
    {
        if (Buttons[id - 30000].ID != -1)
        {
            return &Buttons[id - 30000];
        }
        else
        {
            ErrorLogI1("you don't have a button object which id is", id);
            return NULL;
        }
    }

    void TurnOn()
    {
        Visiblity = 1;
    }

    void TurnOff()
    {
        Visiblity = 0;
    }

    POSITION_2D GetPosition()
    {
        return StartPoint;
    }
};
