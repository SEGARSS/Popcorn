﻿#include"Config.h"

//AColor
//------------------------------------------------------------------------------------------------------------
AColor::AColor(unsigned char r, unsigned char g, unsigned char b)
: R(r), G(g), B(b)
{
}
//------------------------------------------------------------------------------------------------------------




//AsConfig
//------------------------------------------------------------------------------------------------------------
const AColor AsConfig::BG_Color(15, 63, 31);
const AColor AsConfig::Red_Brick_Color(255, 85, 85);
const AColor AsConfig::Blue_Brick_Color(87, 255, 255);

HPEN AsConfig::BG_Pen;
HBRUSH AsConfig::BG_Brush;
//------------------------------------------------------------------------------------------------------------
void AsConfig::Setup_Color()
{
   AsConfig::Create_Pen_Brush(AsConfig::BG_Color, BG_Pen, BG_Brush);
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