﻿#include "Engine.h"

HPEN Highlight_Pen, Brick_Red_Pen, Brick_Blue_Pen, Platform_Circle_Pen, Platform_Inner_Pen;
HBRUSH Brick_Red_Brush, Brick_Blue_Brush, Platform_Circle_Brush, Platform_Inner_Brush;

const int Gloval_Scale = 3;
const int Brick_Width = 15;
const int Brick_Height = 7;
const int Cell_Width = 16;
const int Cell_Height = 8;
const int Level_X_Offset = 8;
const int Level_Y_Offset = 6;
const int Citcle_Size = 7;

int Inner_Width = 21;

/// <summary>
/// enum - начинается номерация значений автоматически с нуля.
/// 
/// В данном виде,
/// None = 0,
/// Red = 1,
/// Blue = 2
/// 
/// Если мы бы к примеру захотели бы переназначить данные, тоесть так - None = 7,
/// то вид теперь стал бы такой
/// None = 7,
/// Red = 8,
/// Blue = 9
/// 
/// </summary>
enum EBrick_Type
{
    EBT_None,
    EBT_Red,
    EBT_Blue
};

char Level_01[14][12]
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
//----------------------------------------------------------------------------------------------------------------
void Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN &pen, HBRUSH &brush)
{
    pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
    brush = CreateSolidBrush(RGB(r, g, b));
}
//----------------------------------------------------------------------------------------------------------------
void Init() // Настройка цвета
{
    Highlight_Pen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));

    Create_Pen_Brush(255, 85, 85, Brick_Red_Pen, Brick_Red_Brush);
    Create_Pen_Brush(85, 255, 255, Brick_Blue_Pen, Brick_Blue_Brush);
    Create_Pen_Brush(151, 0, 0, Platform_Circle_Pen, Platform_Circle_Brush);
    Create_Pen_Brush(0, 128, 192, Platform_Inner_Pen, Platform_Inner_Brush);
}
//----------------------------------------------------------------------------------------------------------------
void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type) // Вывод кирпича
{
    HPEN pen;
    HBRUSH brush;   

    switch (brick_type)
    {
    case EBT_None:
        return;


    case EBT_Red:
        pen = Brick_Red_Pen;
        brush = Brick_Red_Brush;
        break;


    case EBT_Blue:
        pen = Brick_Blue_Pen;
        brush = Brick_Blue_Brush;
        break;


    default:
        return;
    }

    SelectObject(hdc, pen);
    SelectObject(hdc, brush);

    RoundRect(hdc, x * Gloval_Scale, y * Gloval_Scale, (x + Brick_Width) * Gloval_Scale, (y + Brick_Height) * Gloval_Scale, 2 * Gloval_Scale, 2 * Gloval_Scale);
}
//----------------------------------------------------------------------------------------------------------------
void Draw_Level(HDC hdc)// Отрисовка кирпичей
{
    for (int i = 0; i < 14; i++)
        for (int j = 0; j < 12; j++)
            Draw_Brick(hdc, Level_X_Offset + j * Cell_Width, Level_Y_Offset + i * Cell_Height, (EBrick_Type)Level_01[i][j]);
}
//----------------------------------------------------------------------------------------------------------------
void Draw_Platform(HDC hdc, int x, int y) //рисуем платформу
{
    // 1. Рисуем боковые шарики.
    SelectObject(hdc, Platform_Circle_Pen);
    SelectObject(hdc, Platform_Circle_Brush);

    Ellipse(hdc, x * Gloval_Scale, y * Gloval_Scale, (x + Citcle_Size) * Gloval_Scale, (y + Citcle_Size) * Gloval_Scale);
    Ellipse(hdc, (x + Inner_Width) * Gloval_Scale, y * Gloval_Scale, (x + Citcle_Size + Inner_Width) * Gloval_Scale, (y + Citcle_Size) * Gloval_Scale);

    // 2. Рисуем блики на боковом шарике платформы.
    SelectObject(hdc, Highlight_Pen);

    Arc(hdc, (x + 1) * Gloval_Scale, (y + 1) * Gloval_Scale, (x + Citcle_Size - 1) * Gloval_Scale, (y + Citcle_Size - 1) * Gloval_Scale,
             (x + 1 + 1) * Gloval_Scale, (y + 1) * Gloval_Scale, (x + 1) * Gloval_Scale, (y + 1 + 2) * Gloval_Scale);

    // 3. Рисуем саму платформу.
    SelectObject(hdc, Platform_Inner_Pen);
    SelectObject(hdc, Platform_Inner_Brush);

    RoundRect(hdc, (x + 4) * Gloval_Scale, (y + 1) * Gloval_Scale, (x + 4 + Inner_Width - 1) * Gloval_Scale, (y + 1 + 5) * Gloval_Scale, 3 * Gloval_Scale, 3 * Gloval_Scale);
}
//----------------------------------------------------------------------------------------------------------------
void Draw_Frame(HDC hdc) // Отрисовка экрана игры
{
	Draw_Level(hdc);
    Draw_Platform(hdc, 50, 100);   
}
//----------------------------------------------------------------------------------------------------------------