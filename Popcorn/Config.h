#pragma once
#include<Windows.h>
#define _USE_MATH_DEFINES
#include <math.h>

//AColor
//------------------------------------------------------------------------------------------------------------
class AColor
{
public:
   AColor(unsigned char r, unsigned char g, unsigned char b);

   unsigned char R, G, B;

};
//------------------------------------------------------------------------------------------------------------




//AsConfig
//------------------------------------------------------------------------------------------------------------
class AsConfig
{
public:
   static void Setup_Color();
   static void Create_Pen_Brush(const AColor &color, HPEN &pen, HBRUSH &bruch);
   static void Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN &pen, HBRUSH &bruch);
   static int Rand(int range);

   static int Current_Timer_Tick;
   static const AColor BG_Color, Red_Brick_Color, Blue_Brick_Color;
   static HPEN BG_Pen;
   static HBRUSH BG_Brush;
   static HWND Hwnd;

   static const int Global_Scale = 3;         // Ширина окна
   static const int Border_X_Offset = 6;
   static const int Border_Y_Offset = 4;
   static const int Level_Width = 12;         // Ширина уровня в ячейках
   static const int Level_Heigth = 14;        // Высота уровня в ячейках
   static const int Level_X_Offset = 8;       // Смещение по оси X
   static const int Level_Y_Offset = 6;       // Смещение по оси Y
   static const int Cell_Width = 16;          // Ширина ячейки
   static const int Cell_Heigth = 8;          // Высота ячейки
   static const int Brick_Width = 15;         // Ширина кирпича
   static const int Brick_Heigth = 7;         // Высота кирпича
   static const int Ball_Size = 4;            // Размер шарика
   static const int Max_X_Pos = Level_X_Offset + Cell_Width * Level_Width;
   static const int Max_Y_Pos = 199;
   static const int Platform_Y_Pos = 185;
   static const int FPS = 20;
};
//------------------------------------------------------------------------------------------------------------
