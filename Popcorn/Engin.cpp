﻿#include "Engin.h"
#define _USE_MATH_DEFINES
#include <math.h>

// Глобальные переменные:

enum ELetter_Type
{
   ELT_None,
   ELT_O
};

enum EBrick_Type
{
   EBT_None,
   EBT_Red,
   EBT_Blue
};

HPEN Letter_Pen, Highlight_Pen, Brick_Red_Pen, Brick_Blue_Pen, Platform_Cercle_Pen, Platform_Inner_Pen;
HBRUSH Brick_Red_Brush, Brick_Blue_Brush , Platform_Cercle_Brush, Platform_Inner_Brush;

const int Global_Scale = 3;         // Ширина окна
const int Brick_Width = 15;         // Ширина кирпича
const int Brick_Heigth = 7;         // Высота кирпича
const int Cell_Width = 16;          // Ширина ячейки
const int Cell_Heigth = 8;          // Высота ячейки
const int Level_X_Offset = 8;       // Смещение по оси X
const int Level_Y_Offset = 6;       // Смещение по оси Y
const int Circle_Size = 7;
int Inner_Width = 21;

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
void Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN &pen, HBRUSH &bruch)//Функция рисования
{
   pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
   bruch = CreateSolidBrush(RGB(r, g, b));
}
//------------------------------------------------------------------------------------------------------------
void Init()//Настройка игры при старте
{
   Highlight_Pen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
   Letter_Pen = CreatePen(PS_SOLID, Global_Scale, RGB(255, 255, 255));
   Create_Pen_Brush(255, 85, 85, Brick_Red_Pen, Brick_Red_Brush);
   Create_Pen_Brush(87, 255, 255, Brick_Blue_Pen, Brick_Blue_Brush);
   Create_Pen_Brush(151, 0, 0, Platform_Cercle_Pen, Platform_Cercle_Brush);
   Create_Pen_Brush(0, 128, 192, Platform_Inner_Pen, Platform_Inner_Brush);
}
//------------------------------------------------------------------------------------------------------------
void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type)//Вывод кирпича
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
   RoundRect(hdc, x * Global_Scale, y * Global_Scale, (x + Brick_Width) * Global_Scale, 
                 (y + Brick_Heigth) * Global_Scale, 2 * Global_Scale, Global_Scale * 2);
}
//------------------------------------------------------------------------------------------------------------
void Set_Brick_Letter_Colors(bool is_switch_color, HPEN &front_pen, HBRUSH &front_brush, HPEN& back_pen, HBRUSH& back_brush)
{
   if (is_switch_color)
   {
      front_pen = Brick_Red_Pen;
      front_brush = Brick_Red_Brush;

      back_pen = Brick_Blue_Pen;
      back_brush = Brick_Blue_Brush;
   }
   else
   {
      front_pen = Brick_Blue_Pen;
      front_brush = Brick_Blue_Brush;

      back_pen = Brick_Red_Pen;
      back_brush = Brick_Red_Brush;
   }
}
//------------------------------------------------------------------------------------------------------------
void Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step)//Вывод падающией буквы
{
   bool switch_color;
   double offset;
   double rotation_angle; // Преобразование шага в угол поворота
   int brick_half_heigt = (Brick_Heigth * Global_Scale / 2);
   int back_part_offset;
   HPEN front_pen, back_pen;
   HBRUSH front_brush, back_brush;
   XFORM xform, old_xform;

   if (!(brick_type == EBT_Blue || brick_type == EBT_Red)) // Падающие буквы могут быть только от кирпичей такого типа 
   {
      return;
   }

   // Корректируем шаг вращения и угол поворота
   rotation_step = rotation_step % 16;

   if (rotation_step < 8)
   {
      rotation_angle = 2.0 * M_PI / 16.0 * (double)rotation_step;
   }
   else
   {
      rotation_angle = 2.0 * M_PI / 16.0 * (double)(8L - (long long)rotation_step);
   }

   if (rotation_step > 4 && rotation_step <= 12)
   {
      if (brick_type == EBT_Blue)
      {
         switch_color = true;
      }
      else
      {
         switch_color = false;
      }
   }
   else
   {
      if (brick_type == EBT_Red)
      {
         switch_color = true;
      }
      else
      {
         switch_color = false;
      }
   }
   Set_Brick_Letter_Colors(switch_color, front_pen, front_brush, back_pen, back_brush);

   if (rotation_step == 4 || rotation_step == 12)
   {
      // Выводим фон
      SelectObject(hdc, back_pen);
      SelectObject(hdc, back_brush);

      Rectangle(hdc, x, y + brick_half_heigt - Global_Scale, x + Brick_Width * Global_Scale, y + brick_half_heigt);

      //Выводим передний план
      SelectObject(hdc, front_pen);
      SelectObject(hdc, front_brush);

      Rectangle(hdc, x, y + brick_half_heigt, x + Brick_Width * Global_Scale, y + brick_half_heigt + Global_Scale - 1);
   }
   else
   {
      SetGraphicsMode(hdc, GM_ADVANCED);

      // Настраиваем матрицу "переворота" буквы
      xform.eM11 = 1.0f;
      xform.eM12 = 0.0f;
      xform.eM21 = 0.0f;
      xform.eM22 = (float)cos(rotation_angle);
      xform.eDx = (float)x;
      xform.eDy = (float)y + (float)(brick_half_heigt);
      GetWorldTransform(hdc, &old_xform);
      SetWorldTransform(hdc, &xform);

      // Выводим фон
      SelectObject(hdc, back_pen);
      SelectObject(hdc, back_brush);

      offset = 3.0 * (1.0 - fabs(xform.eM22)) * (double)Global_Scale;
      back_part_offset = (int)round(offset);
      Rectangle(hdc, 0, -brick_half_heigt - back_part_offset, Brick_Width * Global_Scale, brick_half_heigt - back_part_offset);

      //Выводим передний план
      SelectObject(hdc, front_pen);
      SelectObject(hdc, front_brush);

      Rectangle(hdc, 0, -brick_half_heigt, Brick_Width * Global_Scale, brick_half_heigt);

      if (rotation_step > 4 && rotation_step <= 12)
      {
         if (letter_type == ELT_O)
         {
            SelectObject(hdc, Letter_Pen);
            Ellipse(hdc, 0 + 5 * Global_Scale, (-5 * Global_Scale) / 2, 0 + 10 * Global_Scale, 5 * Global_Scale / 2);
         }
      }
      SetWorldTransform(hdc, &old_xform);
   }
}
//------------------------------------------------------------------------------------------------------------
void Draw_Level(HDC hdc)//Вывод всех кирпичей
{
   for (int i = 0; i < 14; i++)
   {
      for (int j = 0; j < 12; j++)
      {
         Draw_Brick(hdc, Level_X_Offset + j * Cell_Width, Level_Y_Offset + i * Cell_Heigth, (EBrick_Type)Level_01[i][j]);
      }
   }
}
//------------------------------------------------------------------------------------------------------------
void Draw_Platform(HDC hdc, int x, int y) // Функция вывода управляемой платформой.
{
   //1. Рисуем боковые шарики
   SelectObject(hdc, Platform_Cercle_Pen);
   SelectObject(hdc, Platform_Cercle_Brush);
   Ellipse(hdc, x * Global_Scale, y * Global_Scale, (x + Circle_Size) * Global_Scale, (y + Circle_Size) * Global_Scale);
   Ellipse(hdc, (x + Inner_Width) * Global_Scale, y * Global_Scale, (x + Circle_Size + Inner_Width) * Global_Scale, 
                (y + Circle_Size) * Global_Scale);
   //2. Рисуем блик
   SelectObject(hdc, Highlight_Pen);
   Arc(hdc, (x + 1) * Global_Scale, (y + 1) * Global_Scale, (x + Circle_Size - 1) * Global_Scale, 
            (y + Circle_Size - 1) * Global_Scale,(x + 1 + 1) * Global_Scale, (y + 1) * Global_Scale, 
            (x + 1) * Global_Scale, (y + 1 +2) * Global_Scale);
   //3. Рисуем среднюю часть.
   SelectObject(hdc, Platform_Inner_Pen);
   SelectObject(hdc, Platform_Inner_Brush);
   RoundRect(hdc, (x + 4) * Global_Scale, (y + 1) * Global_Scale, (x + 4 + Inner_Width - 1) * Global_Scale, 
                  (y + 1 + 5) * Global_Scale, 3 * Global_Scale, Global_Scale * 3);
}
//------------------------------------------------------------------------------------------------------------
void Draw_Frame(HDC hdc)//отрисовка экрана игры
{
   /*Draw_Level(hdc);
   Draw_Platform(hdc, 50, 100);*/
   for (int i = 0; i < 16; i++)
   {
      Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale, 100, EBT_Blue, ELT_O, i);
      Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale, 130, EBT_Red, ELT_O, i);
   }
         
}
//------------------------------------------------------------------------------------------------------------