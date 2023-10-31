#include "Active_Brick.h"

//AGraphics_Object
//------------------------------------------------------------------------------------------------------------
AGraphics_Object::~AGraphics_Object()
{
}
//------------------------------------------------------------------------------------------------------------




//AActive_Brick
//------------------------------------------------------------------------------------------------------------
AActive_Brick::~AActive_Brick()
{
}
//------------------------------------------------------------------------------------------------------------
AActive_Brick::AActive_Brick(EBrick_Type brick_type, int level_x, int level_y)
: Brick_Type(brick_type), Brick_Rect{}
{
   Brick_Rect.left = (AsConfig::Level_X_Offset + level_x * AsConfig::Cell_Width) * AsConfig::Global_Scale;
   Brick_Rect.top = (AsConfig::Level_Y_Offset + level_y * AsConfig::Cell_Height) * AsConfig::Global_Scale;
   Brick_Rect.right = Brick_Rect.left + AsConfig::Brick_Width * AsConfig::Global_Scale;
   Brick_Rect.bottom = Brick_Rect.top + AsConfig::Brick_Height * AsConfig::Global_Scale;
}
//------------------------------------------------------------------------------------------------------------




//AActive_Brick_Red_Blue
//------------------------------------------------------------------------------------------------------------
AColor AActive_Brick_Red_Blue::Fading_Red_Colors[Max_Fade_Step];
AColor AActive_Brick_Red_Blue::Fading_Blue_Colors[Max_Fade_Step];
//------------------------------------------------------------------------------------------------------------
AActive_Brick_Red_Blue::~AActive_Brick_Red_Blue()
{
}
//------------------------------------------------------------------------------------------------------------
AActive_Brick_Red_Blue::AActive_Brick_Red_Blue(EBrick_Type brick_type, int level_x, int level_y)
: AActive_Brick(brick_type, level_x, level_y), Fade_Step(0)
{
   if (! (brick_type == EBT_Red || brick_type == EBT_Blue) )
      throw 13;
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Red_Blue::Act()
{
	if (Fade_Step < Max_Fade_Step - 1)
	{
		++Fade_Step;
		InvalidateRect(AsConfig::Hwnd, &Brick_Rect, FALSE);
	}
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Red_Blue::Draw(HDC hdc, RECT &paint_area)
{
   AColor *color = 0;

   switch (Brick_Type)   
   {
   case EBT_Red:
      color = &Fading_Red_Colors[Fade_Step];
      break;

   case EBT_Blue:
      color = &Fading_Blue_Colors[Fade_Step];
      break;
   }

   if (color != 0)
      color->Select(hdc);

   AsConfig::Round_Rect(hdc, Brick_Rect);
}
//------------------------------------------------------------------------------------------------------------
bool AActive_Brick_Red_Blue::Is_Finished()
{
   if (Fade_Step >= Max_Fade_Step -1)
      return true;
   else
      return false;
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Red_Blue::Setup_Color()
{
   for (int i = 0; i < Max_Fade_Step; i++)
   {
      Get_Fading_Color(AsConfig::Red_Color, i, Fading_Red_Colors[i]);
      Get_Fading_Color(AsConfig::Blue_Color, i, Fading_Blue_Colors[i]);
   }
}
//------------------------------------------------------------------------------------------------------------
unsigned char AActive_Brick_Red_Blue::Get_Fading_Channel(unsigned char color, unsigned char bg_color, int step)
{
   return color - step * (color - bg_color) / (Max_Fade_Step - 1);
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Red_Blue::Get_Fading_Color(const AColor &origin_color, int step, AColor &result_color)
{
   unsigned char r, g, b;

	r = Get_Fading_Channel(origin_color.R, AsConfig::BG_Color.R, step);
	g = Get_Fading_Channel(origin_color.G, AsConfig::BG_Color.G, step);
	b = Get_Fading_Channel(origin_color.B, AsConfig::BG_Color.B, step);

   result_color = AColor(r, g, b);
}
//------------------------------------------------------------------------------------------------------------




//AActive_Brick_Unbreakable
AColor AActive_Brick_Unbreakable::Blue_Highlight(AsConfig::Blue_Color, AsConfig::Global_Scale);
AColor AActive_Brick_Unbreakable::Red_Highlight(AsConfig::Red_Color, 3 * AsConfig::Global_Scale);
//------------------------------------------------------------------------------------------------------------
AActive_Brick_Unbreakable::~AActive_Brick_Unbreakable()
{
}
//------------------------------------------------------------------------------------------------------------
AActive_Brick_Unbreakable::AActive_Brick_Unbreakable(int level_x, int level_y)
: AActive_Brick(EBT_Unbreakable, level_x, level_y), Unreakable_Animation_step(0)
{
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Unbreakable::Act()
{
	if (Unreakable_Animation_step <= Max_Unreakable_Animation_step)
	{
		++Unreakable_Animation_step;
		InvalidateRect(AsConfig::Hwnd, &Brick_Rect, FALSE);
	}
}
//------------------------------------------------------------------------------------------------------------
void AActive_Brick_Unbreakable::Draw(HDC hdc, RECT &paint_area)
{
   const int scale = AsConfig::Global_Scale;
   //HPEN red_pen, blue_pen;
   HRGN region;

   AsConfig::White_Color.Select(hdc);
   AsConfig::Round_Rect(hdc, Brick_Rect);

   //AsConfig::Red_Color.Select(hdc);
   //MoveToEx(hdc,Brick_Rect.left + 4 * scale, Brick_Rect.top + 7 * scale - 1, 0);
   //LineTo(hdc, Brick_Rect.left + 11 * scale - 1, Brick_Rect.top + 0 * scale);
   //LineTo(hdc, Brick_Rect.left + 14 * scale - 1, Brick_Rect.top + 0 * scale);
   //LineTo(hdc, Brick_Rect.left + 7 * scale, Brick_Rect.top + 7 * scale - 1);
   //LineTo(hdc, Brick_Rect.left + 4 * scale, Brick_Rect.top + 7 * scale - 1);

   //FloodFill(hdc, Brick_Rect.left + 11 * scale - 1, Brick_Rect.top + 1 * scale,  AsConfig::Red_Color.Get_RGB() );

   //red_pen = CreatePen(PS_SOLID, 3 * scale, AsConfig::Red_Color.Get_RGB() );
   //blue_pen = CreatePen(PS_SOLID, scale, AsConfig::Blue_Color.Get_RGB() );

   region = CreateRoundRectRgn(Brick_Rect.left, Brick_Rect.top, Brick_Rect.right + 1, Brick_Rect.bottom + 1, 2 * scale - 1, 2 * scale - 1);
   SelectClipRgn(hdc, region);

   Blue_Highlight.Select_Pen(hdc);
   MoveToEx(hdc,Brick_Rect.left + 4 * scale, Brick_Rect.bottom + scale, 0);
   LineTo(hdc, Brick_Rect.left + 13 * scale - 1, Brick_Rect.top - 1 * scale);

   Red_Highlight.Select_Pen(hdc);
   MoveToEx(hdc,Brick_Rect.left + 6 * scale, Brick_Rect.bottom + scale, 0);
   LineTo(hdc, Brick_Rect.left + 15 * scale - 1, Brick_Rect.top - 1 * scale);

   SelectClipRgn(hdc, 0);
}
//------------------------------------------------------------------------------------------------------------
bool AActive_Brick_Unbreakable::Is_Finished()
{
   //if (Fade_Step >= Max_Fade_Step -1)
   //   return true;
   //else
      return false;
}
//------------------------------------------------------------------------------------------------------------
