#pragma once
#include<Windows.h>
#define _USE_MATH_DEFINES
#include <math.h>
//------------------------------------------------------------------------------------------------------------
enum ELetter_Type
{
   ELT_None,
   ELT_O
};
//------------------------------------------------------------------------------------------------------------
enum EBrick_Type
{
   EBT_None,
   EBT_Red,
   EBT_Blue
};
//------------------------------------------------------------------------------------------------------------
enum EKey_Type
{
   EKT_Left,
   EKT_Right,
   EKT_Space
};

const int Timer_ID = WM_USER + 1;
//------------------------------------------------------------------------------------------------------------
class AsEngine
{
public:
   AsEngine();

   void Init_Engine(HWND hwnd);
   void Draw_Frame(HDC hdc, RECT& paint_area);
   int On_Key_Down(EKey_Type key_type);
   int On_Timer();

   static const int Level_Width = 12;         // Ширина уровня в ячейках
   static const int Level_Heigth = 14;        // Высота уровня в ячейках

private:

   void Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN& pen, HBRUSH& bruch);
   void Redraw_Platform();
   void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type);
   void Set_Brick_Letter_Colors(bool is_switch_color, HPEN& front_pen, HBRUSH& front_brush, HPEN& back_pen, HBRUSH& back_brush);
   void Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step);
   void Draw_Level(HDC hdc);
   void Draw_Platform(HDC hdc, int x, int y);
   void Draw_BaLL(HDC hdc, RECT& paint_area);
   void Draw_Border(HDC hdc, int x, int y, bool top_boder);
   void Draw_Bounds(HDC hdc, RECT& paint_area);
   void Check_Level_Brick_Hit(int& next_y_pos);
   void Move_Ball();

   // Глобальные переменные:

   HWND Hwnd;
   HPEN BG_Pen, Letter_Pen, Highlight_Pen, Brick_Red_Pen, Brick_Blue_Pen, Platform_Cercle_Pen, Platform_Inner_Pen;
   HPEN Ball_Pen, Border_Blue_Pen, Border_White_Pen;
   HBRUSH BG_Brush, Brick_Red_Brush, Brick_Blue_Brush, Platform_Cercle_Brush, Platform_Inner_Brush, Ball_Brush;
   HBRUSH Border_Blue_Brush, Border_White_Brush;

   int Inner_Width;
   int Platform_X_Pos;
   int Platform_X_Step;
   int Platform_width;
   int Ball_X_Pos;
   int Ball_Y_Pos;
   double Ball_Speed;
   double Ball_Direction;

   RECT Platform_Rect, Prev_Platform_Rect;
   RECT Level_Rect;
   RECT Ball_Rect;
   RECT Prev_Ball_Rect;

   static const int Global_Scale = 3;         // Ширина окна
   static const int Brick_Width = 15;         // Ширина кирпича
   static const int Brick_Heigth = 7;         // Высота кирпича
   static const int Cell_Width = 16;          // Ширина ячейки
   static const int Cell_Heigth = 8;          // Высота ячейки
   static const int Level_X_Offset = 8;       // Смещение по оси X
   static const int Level_Y_Offset = 6;       // Смещение по оси Y
   static const int Circle_Size = 7;
   static const int Platform_Y_Pos = 185;
   static const int Platform_Height = 7;
   static const int Ball_Size = 4;
   static const int Max_X_Pos = Level_X_Offset + Cell_Width * Level_Width;
   static const int Max_Y_Pos = 199 - Ball_Size;
   static const int Border_X_offset = 6;
   static const int Border_Y_offset = 4;
};
//------------------------------------------------------------------------------------------------------------

