#include"Config.h"

//AsConfig
//------------------------------------------------------------------------------------------------------------
void AsConfig::Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, 
                                HPEN &pen, HBRUSH &bruch)//Функция рисования
{
   pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
   bruch = CreateSolidBrush(RGB(r, g, b));
}
//------------------------------------------------------------------------------------------------------------