#include"Config.h"

//AColor
//------------------------------------------------------------------------------------------------------------
AColor::AColor(unsigned char r, unsigned char g, unsigned char b)
: R(r), G(g), B(b), Pen(0), Brush(0)
{
   Pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
   Brush = CreateSolidBrush(RGB(r, g, b));
}
//------------------------------------------------------------------------------------------------------------
int AColor::Get_RGB() const
{
   return RGB(R, G, B);
}
//------------------------------------------------------------------------------------------------------------
void AColor::Select(HDC hdc) const
{
   SelectObject(hdc, Pen);
   SelectObject(hdc, Brush);
}
//------------------------------------------------------------------------------------------------------------




//AsConfig
//------------------------------------------------------------------------------------------------------------
bool AsConfig::Level_Has_Floor = false;
int AsConfig::Current_Timer_Tick = 0;

const AColor AsConfig::BG_Color(15, 63, 31);
const AColor AsConfig::Red_Brick_Color(255, 85, 85);
const AColor AsConfig::Blue_Brick_Color(87, 255, 255);
const AColor AsConfig::White_Color(255, 255, 255);

//HPEN AsConfig::BG_Pen;
//HBRUSH AsConfig::BG_Brush;
HWND AsConfig::Hwnd;
//HPEN AsConfig::Brick_Red_Pen, AsConfig::Brick_Blue_Pen, AsConfig::Brick_White_Pen, AsConfig::Letter_Pen;
//HBRUSH AsConfig::Brick_Red_Brush, AsConfig::Brick_Blue_Brush, AsConfig::Brick_White_Brush;
HPEN AsConfig::Letter_Pen;

const double AsConfig::Moving_Step_Size = 1.0 / AsConfig::Global_Scale;
//------------------------------------------------------------------------------------------------------------
void AsConfig::Setup_Color()
{
   AsConfig::Letter_Pen = CreatePen(PS_SOLID, AsConfig::Global_Scale, RGB(255, 255, 255));

   //AsConfig::Create_Pen_Brush(AsConfig::BG_Color, BG_Pen, BG_Brush);
   //AsConfig::Create_Pen_Brush(AsConfig::Red_Brick_Color, AsConfig::Brick_Red_Pen, AsConfig::Brick_Red_Brush);
   //AsConfig::Create_Pen_Brush(AsConfig::Blue_Brick_Color, AsConfig::Brick_Blue_Pen, AsConfig::Brick_Blue_Brush);
   //AsConfig::Create_Pen_Brush(AsConfig::White_Color, AsConfig::Brick_White_Pen, AsConfig::Brick_White_Brush);
}
//------------------------------------------------------------------------------------------------------------
void AsConfig::Create_Pen_Brush(const AColor &color, HPEN &pen, HBRUSH &bruch)//Функция рисования
{
   pen = CreatePen(PS_SOLID, 0, RGB(color.R, color.G, color.B));
   bruch = CreateSolidBrush(RGB(color.R, color.G, color.B));
}
//------------------------------------------------------------------------------------------------------------
void AsConfig::Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN &pen, HBRUSH &bruch)//Функция рисования
{
   pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
   bruch = CreateSolidBrush(RGB(r, g, b));
}
//------------------------------------------------------------------------------------------------------------
int AsConfig::Rand(int range)//Вычисляет псевдо случайные числа [0, .. range - 1]
{
   return rand() * range / RAND_MAX;
}
//------------------------------------------------------------------------------------------------------------