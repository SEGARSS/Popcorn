﻿#include "Engin.h"

// Глобальные переменные:/
enum Brick_Type
{
   EBT_None,
   EBT_Red,
   EBT_Blue
};

HPEN Brick_Red_Pen, Brick_Blue_Pen;
HBRUSH Brick_Red_Brush, Brick_Blue_Brush;

const int Global_Scale = 3;         // Ширина окна
const int Brick_Width = 15;         // Ширина кирпича
const int Brick_Heigth = 7;         // Высота кирпича
const int Cell_Width = 16;          // Ширина ячейки
const int Cell_Heigth = 8;          // Высота ячейки
const int Level_X_Offset = 8;       // Смещение по оси X
const int Level_Y_Offset = 6;       // Смещение по оси Y

char Level_01[14][12] =
{
   0,0,0,0,0,0,0,0,0,0,0,0,
   1,1,1,1,1,1,1,1,1,1,1,1,
   1,1,1,1,1,1,1,1,1,1,1,1,
   2,2,2,2,2,2,2,2,2,2,2,2,
   2,2,2,2,2,2,2,2,2,2,2,2,
   1,1,1,1,1,1,1,1,1,1,1,1,
   1,1,1,1,1,1,1,1,1,1,1,1,
   2,2,2,2,2,2,2,2,2,2,2,2,
   2,2,2,2,2,2,2,2,2,2,2,2,
   0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0
};

//------------------------------------------------------------------------------------------------------------
void Init()//Настройка игры при старте
{
   Brick_Red_Pen = CreatePen(PS_SOLID, 0, RGB(255, 85, 85));
   Brick_Red_Brush = CreateSolidBrush(RGB(255, 85, 85));

   Brick_Blue_Pen = CreatePen(PS_SOLID, 0, RGB(87, 255, 255));
   Brick_Blue_Brush = CreateSolidBrush(RGB(87, 255, 255));
}
//------------------------------------------------------------------------------------------------------------
void Draw_Brick(HDC hdc, int x, int y, Brick_Type brick_type)//Вывод кирпича
{
   HPEN pen;
   HBRUSH brush;

   switch (brick_type)
   {
   case EBT_None:
   {
      return;
   }
   case EBT_Red:
   {
      pen = Brick_Red_Pen;
      brush = Brick_Red_Brush;
      break;
   }
   case EBT_Blue:
   {
      pen = Brick_Blue_Pen;
      brush = Brick_Blue_Brush;
      break;
   }
   default:
      return;
   }

   SelectObject(hdc, pen);
   SelectObject(hdc, brush);
   RoundRect(hdc, x * Global_Scale, y * Global_Scale, (x + Brick_Width) * Global_Scale, (y + Brick_Heigth) * Global_Scale, 2 * Global_Scale, Global_Scale * 2);
}
//------------------------------------------------------------------------------------------------------------
void Draw_Level(HDC hdc)//Вывод всех кирпичей
{
   for (int i = 0; i < 14; i++)
   {
      for (int j = 0; j < 12; j++)
      {
         Draw_Brick(hdc, Level_X_Offset + j * Cell_Width, Level_Y_Offset + i * Cell_Heigth, (Brick_Type)Level_01[i][j]);
      }
   }
}
//------------------------------------------------------------------------------------------------------------
void Draw_Frame(HDC hdc)//отрисовка экрана игры
{
   Draw_Level(hdc);
}
//------------------------------------------------------------------------------------------------------------