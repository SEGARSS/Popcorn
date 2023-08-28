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

HWND Hwnd;
HPEN BG_Pen, Letter_Pen, Highlight_Pen, Brick_Red_Pen, Brick_Blue_Pen, Platform_Cercle_Pen, Platform_Inner_Pen;
HPEN Ball_Pen, Border_Blue_Pen, Border_White_Pen;
HBRUSH BG_Brush, Brick_Red_Brush, Brick_Blue_Brush, Platform_Cercle_Brush, Platform_Inner_Brush, Ball_Brush;
HBRUSH Border_Blue_Brush, Border_White_Brush;

const int Global_Scale = 3;         // Ширина окна
const int Brick_Width = 15;         // Ширина кирпича
const int Brick_Heigth = 7;         // Высота кирпича
const int Cell_Width = 16;          // Ширина ячейки
const int Cell_Heigth = 8;          // Высота ячейки
const int Level_X_Offset = 8;       // Смещение по оси X
const int Level_Y_Offset = 6;       // Смещение по оси Y
const int Level_Width = 12;         // Ширина уровня в ячейках
const int Level_Heigth = 14;        // Высота уровня в ячейках
const int Circle_Size = 7;
const int Platform_Y_Pos = 185;
const int Platform_Height = 7;
const int Ball_Size = 4;
const int Max_X_Pos = Level_X_Offset + Cell_Width * Level_Width;
const int Max_Y_Pos = 199 - Ball_Size;
const int Border_X_offset = 6;
const int Border_Y_offset = 4;

int Inner_Width = 21;
int Platform_X_Pos = Border_X_offset;
int Platform_X_Step = Global_Scale * 2;
int Platform_width = 28;
int Ball_X_Pos = 20;
int Ball_Y_Pos = 170;

double Ball_Speed = 3.0, Ball_Direction = M_PI - M_PI_4;

RECT Platform_Rect, Prev_Platform_Rect;
RECT Level_Rect;
RECT Ball_Rect;
RECT Prev_Ball_Rect;

char Level_01[Level_Heigth][Level_Width] =
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
void Redraw_Platform()
{
   Prev_Platform_Rect = Platform_Rect;

   Platform_Rect.left = Platform_X_Pos * Global_Scale;
   Platform_Rect.top = Platform_Y_Pos * Global_Scale;
   Platform_Rect.right = Platform_Rect.left + Platform_width * Global_Scale;
   Platform_Rect.bottom = Platform_Rect.top + Platform_Height * Global_Scale;

   InvalidateRect(Hwnd, &Prev_Platform_Rect, FALSE);
   InvalidateRect(Hwnd, &Platform_Rect, FALSE);
}
//------------------------------------------------------------------------------------------------------------
void Init_Engine(HWND hwnd)//Настройка игры при старте
{
   Hwnd = hwnd;

   Highlight_Pen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
   Letter_Pen = CreatePen(PS_SOLID, Global_Scale, RGB(255, 255, 255));

   Create_Pen_Brush(15, 63, 31, BG_Pen, BG_Brush);
   Create_Pen_Brush(255, 85, 85, Brick_Red_Pen, Brick_Red_Brush);
   Create_Pen_Brush(87, 255, 255, Brick_Blue_Pen, Brick_Blue_Brush);
   Create_Pen_Brush(151, 0, 0, Platform_Cercle_Pen, Platform_Cercle_Brush);
   Create_Pen_Brush(0, 128, 192, Platform_Inner_Pen, Platform_Inner_Brush);
   Create_Pen_Brush(255, 255, 255, Ball_Pen, Ball_Brush);
   Create_Pen_Brush(87, 255, 255, Border_Blue_Pen, Border_Blue_Brush);
   Create_Pen_Brush(255, 255, 255, Border_White_Pen, Border_White_Brush);


   Level_Rect.left = Level_X_Offset * Global_Scale;
   Level_Rect.top = Level_Y_Offset * Global_Scale;
   Level_Rect.right = Level_Rect.left + Cell_Width * Level_Width * Global_Scale;
   Level_Rect.bottom = Level_Rect.top + Cell_Heigth * Level_Heigth * Global_Scale;

   Redraw_Platform();

   SetTimer(Hwnd, Timer_ID, 50, 0);
 
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
   for (int i = 0; i < Level_Heigth; i++)
   {
      for (int j = 0; j < Level_Width; j++)
      {
         Draw_Brick(hdc, Level_X_Offset + j * Cell_Width, Level_Y_Offset + i * Cell_Heigth, (EBrick_Type)Level_01[i][j]);
      }
   }
}
//------------------------------------------------------------------------------------------------------------
void Draw_Platform(HDC hdc, int x, int y) // Функция вывода управляемой платформой.
{
   //1. Рисуем платформу
   SelectObject(hdc, BG_Pen);
   SelectObject(hdc, BG_Brush);

   Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top, Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);

   //2. Рисуем боковые шарики
   SelectObject(hdc, Platform_Cercle_Pen);
   SelectObject(hdc, Platform_Cercle_Brush);
   Ellipse(hdc, x * Global_Scale, y * Global_Scale, (x + Circle_Size) * Global_Scale, (y + Circle_Size) * Global_Scale);
   Ellipse(hdc, (x + Inner_Width) * Global_Scale, y * Global_Scale, (x + Circle_Size + Inner_Width) * Global_Scale, 
                (y + Circle_Size) * Global_Scale);
   //3. Рисуем блик
   SelectObject(hdc, Highlight_Pen);
   Arc(hdc, (x + 1) * Global_Scale, (y + 1) * Global_Scale, (x + Circle_Size - 1) * Global_Scale, 
            (y + Circle_Size - 1) * Global_Scale,(x + 1 + 1) * Global_Scale, (y + 1) * Global_Scale, 
            (x + 1) * Global_Scale, (y + 1 +2) * Global_Scale);
   //4. Рисуем среднюю часть.
   SelectObject(hdc, Platform_Inner_Pen);
   SelectObject(hdc, Platform_Inner_Brush);
   RoundRect(hdc, (x + 4) * Global_Scale, (y + 1) * Global_Scale, (x + 4 + Inner_Width - 1) * Global_Scale, 
                  (y + 1 + 5) * Global_Scale, 3 * Global_Scale, Global_Scale * 3);
}
//------------------------------------------------------------------------------------------------------------
void Draw_BaLL(HDC hdc, RECT& paint_area)//отрисовка шарика
{
   //1. Очищаем фон.
   SelectObject(hdc, BG_Pen);
   SelectObject(hdc, BG_Brush);

   Ellipse(hdc, Prev_Ball_Rect.left, Prev_Ball_Rect.top, Prev_Ball_Rect.right - 1, Prev_Ball_Rect.bottom - 1);

   //2. Рисуем шарик.
   SelectObject(hdc, Ball_Pen);
   SelectObject(hdc, Ball_Brush);

   Ellipse(hdc, Ball_Rect.left, Ball_Rect.top, Ball_Rect.right - 1, Ball_Rect.bottom - 1);
}
//------------------------------------------------------------------------------------------------------------
void Draw_Border(HDC hdc, int x, int y, bool top_boder)//Отрисовка элемента боковой рамки
{
   //Выводиться основная линия
   SelectObject(hdc, Border_Blue_Pen);
   SelectObject(hdc, Border_Blue_Brush);

   if (top_boder)
   {
      Rectangle(hdc, x * Global_Scale, (y + 1) * Global_Scale, (x + 4) * Global_Scale, (y + 4) * Global_Scale);
   }
   else
   {
      Rectangle(hdc, (x + 1) * Global_Scale, y * Global_Scale, (x + 4) * Global_Scale, (y + 4) * Global_Scale);
   }

   //Белая каяма
   SelectObject(hdc, Border_White_Pen);
   SelectObject(hdc, Border_White_Brush);

   if (top_boder)
   {
      Rectangle(hdc, x * Global_Scale, y * Global_Scale, (x + 4) * Global_Scale, (y + 1) * Global_Scale);
   }
   else
   {
      Rectangle(hdc, x * Global_Scale, y * Global_Scale, (x + 1) * Global_Scale, (y + 4) * Global_Scale);
   }

   //Перфорация (Точки в нутри рамки)
   SelectObject(hdc, BG_Pen);
   SelectObject(hdc, BG_Brush);

   if (top_boder)
   {
      Rectangle(hdc, (x + 2) * Global_Scale, (y + 2) * Global_Scale, (x + 3) * Global_Scale, (y + 3) * Global_Scale);
   }
   else
   {
      Rectangle(hdc, (x + 2) * Global_Scale, (y + 1) * Global_Scale, (x + 3) * Global_Scale, (y + 2) * Global_Scale);
   }
}
//------------------------------------------------------------------------------------------------------------
void Draw_Bounds(HDC hdc, RECT& paint_area)//Рисуем полную рамку
{
   //Линия слева
   for (int i = 0; i < 50; i++)
   {
      Draw_Border(hdc, 2, 1 + i * 4, false);
   }
   //Линия справа
   for (int i = 0; i < 50; i++)
   {
      Draw_Border(hdc, 201, 1 + i * 4, false);
   }
   //Линия сверху
   for (int i = 0; i < 50; i++)
   {
      Draw_Border(hdc, 3 + i * 4, 0, true);
   }
}
//------------------------------------------------------------------------------------------------------------
void Draw_Frame(HDC hdc, RECT &paint_area)//отрисовка экрана игры
{
   RECT intersectRect;

   if (IntersectRect(&intersectRect, &paint_area, &Level_Rect))
   {
      Draw_Level(hdc);
   }

   if (IntersectRect(&intersectRect, &paint_area, &Platform_Rect))
   {
      Draw_Platform(hdc, Platform_X_Pos, Platform_Y_Pos);
   }

   /*for (int i = 0; i < 16; i++)
   {
      Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale, 100, EBT_Blue, ELT_O, i);
      Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale, 130, EBT_Red, ELT_O, i);
   }*/

   if (IntersectRect(&intersectRect, &paint_area, &Ball_Rect))
   {
      Draw_BaLL(hdc, paint_area);
   }
   
   Draw_Bounds(hdc, paint_area);
  
}
//------------------------------------------------------------------------------------------------------------
int On_Key_Down(EKey_Type key_type)
{
   switch (key_type)
   {
   case EKT_Left:
      Platform_X_Pos -= Platform_X_Step;

      if (Platform_X_Pos <= Border_X_offset)
      {
         Platform_X_Pos = Border_X_offset;
      }

      Redraw_Platform();
      break;

   case EKT_Right:
      Platform_X_Pos += Platform_X_Step;

      if (Platform_X_Pos >= Max_X_Pos - Platform_width + 1)
      {
         Platform_X_Pos = Max_X_Pos - Platform_width + 1;
      }
      Redraw_Platform();
      break;

   case EKT_Space:
      break;
   }
   return 0;
}
//------------------------------------------------------------------------------------------------------------
void Move_Ball() // Смещение шарика
{
   int next_x_pos, next_y_pos;
   int max_x_pos = Max_X_Pos - Ball_Size;

   Prev_Ball_Rect = Ball_Rect;

   next_x_pos = Ball_X_Pos + (int)(Ball_Speed * cos(Ball_Direction));
   next_y_pos = Ball_Y_Pos - (int)(Ball_Speed * sin(Ball_Direction));

   // Корректируем позиции по отражению
   if (next_x_pos < Border_X_offset)
   {
      next_x_pos = Level_X_Offset - (next_x_pos - Level_X_Offset);
      Ball_Direction = M_PI - Ball_Direction;
   }

   if (next_y_pos < Border_Y_offset)
   {
      next_y_pos = Level_Y_Offset - (next_y_pos - Level_Y_Offset);
      Ball_Direction = -Ball_Direction;
   }

   if (next_x_pos > max_x_pos)
   {
      next_x_pos = max_x_pos - (next_x_pos - max_x_pos);
      Ball_Direction = M_PI - Ball_Direction;
   }

   if (next_y_pos > Max_Y_Pos)
   {
      next_y_pos = Max_Y_Pos - (next_y_pos - Max_Y_Pos);
      Ball_Direction = M_PI + (M_PI - Ball_Direction);
   }

   //Смещаем шарик
   Ball_X_Pos = next_x_pos;
   Ball_Y_Pos = next_y_pos;

   Ball_Rect.left = Ball_X_Pos * Global_Scale;
   Ball_Rect.top = Ball_Y_Pos * Global_Scale;
   Ball_Rect.right = Ball_Rect.left + Ball_Size * Global_Scale;
   Ball_Rect.bottom = Ball_Rect.top + Ball_Size * Global_Scale;

   InvalidateRect(Hwnd, &Prev_Ball_Rect, FALSE);
   InvalidateRect(Hwnd, &Ball_Rect, FALSE);
}
//------------------------------------------------------------------------------------------------------------
int On_Timer() // Смещение по таймеру
{
   Move_Ball();
   return 0;
}
//c 47 минуты 13 видео продолжить