#include "Engin.h"

// Глобальные переменные:/
enum Brick_Type
{
   EBT_None,
   EBT_Red,
   EBT_Blue
};

HPEN Highlight_Pen, Brick_Red_Pen, Brick_Blue_Pen, Platform_Cercle_Pen, Platform_Inner_Pen;
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
   Create_Pen_Brush(255, 85, 85, Brick_Red_Pen, Brick_Red_Brush);
   Create_Pen_Brush(87, 255, 255, Brick_Blue_Pen, Brick_Blue_Brush);
   Create_Pen_Brush(151, 0, 0, Platform_Cercle_Pen, Platform_Cercle_Brush);
   Create_Pen_Brush(0, 128, 192, Platform_Inner_Pen, Platform_Inner_Brush);
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
   RoundRect(hdc, x * Global_Scale, y * Global_Scale, (x + Brick_Width) * Global_Scale, 
                 (y + Brick_Heigth) * Global_Scale, 2 * Global_Scale, Global_Scale * 2);
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
   Draw_Level(hdc);
   Draw_Platform(hdc, 50, 100);

}
//------------------------------------------------------------------------------------------------------------