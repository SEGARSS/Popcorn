#include"Platform.h"

//AsPlatform
//------------------------------------------------------------------------------------------------------------
AsPlatform::AsPlatform()
: X_Pos(AsConfig::Border_X_offset), X_Step(AsConfig::Global_Scale * 2), Platform_State(EPS_Normal), Inner_Width(21), 
  Width(Normal_Width), Platform_Rect{}, Prev_Platform_Rect{}, Highlight_Pen(0), Platform_Cercle_Pen(0), 
  Platform_Inner_Pen(0), Platform_Cercle_Brush(0), Platform_Inner_Brush(0)
{
   X_Pos = (AsConfig::Max_X_Pos - Width) / 2;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Init()
{
   Highlight_Pen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));

   AsConfig::Create_Pen_Brush(151, 0, 0, Platform_Cercle_Pen, Platform_Cercle_Brush);
   AsConfig::Create_Pen_Brush(0, 128, 192, Platform_Inner_Pen, Platform_Inner_Brush);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Act(HWND hwnd)
{
   int len;

   if (Platform_State != EPS_Meltdown)
   {
      Platform_State = EPS_Meltdown;

      len = sizeof(Meltdown_Platform_Y_Pos) / sizeof(Meltdown_Platform_Y_Pos[0]);

      for (int i = 0; i < len; i++)
      {
         Meltdown_Platform_Y_Pos[i] = Platform_Rect.bottom;
      }
   }

   if (Platform_State == EPS_Meltdown)
   {
      Redraw_Platform(hwnd);
   }
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Redraw_Platform(HWND hwnd)
{
   Prev_Platform_Rect = Platform_Rect;

   Platform_Rect.left = X_Pos * AsConfig::Global_Scale;
   Platform_Rect.top = AsConfig::Platform_Y_Pos * AsConfig::Global_Scale;
   Platform_Rect.right = Platform_Rect.left + Width * AsConfig::Global_Scale;
   Platform_Rect.bottom = Platform_Rect.top + Height * AsConfig::Global_Scale;

   if (Platform_State == EPS_Meltdown)
   {
      Prev_Platform_Rect.bottom = (AsConfig::Max_X_Pos + 1) * AsConfig::Global_Scale;
   }

   InvalidateRect(hwnd, &Prev_Platform_Rect, FALSE);
   InvalidateRect(hwnd, &Platform_Rect, FALSE);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw(HDC hdc, RECT &paint_area)
{
   switch (Platform_State)
   {
   case EPS_Normal:
      Draw_Normal_State(hdc, paint_area);
      break;
   case EPS_Meltdown:
      Draw_Meltdown_State(hdc, paint_area);
      break;
   }
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Normal_State(HDC hdc, RECT &paint_area) //Рисуем платфолрму в нормальном состоянии
{
   int x = X_Pos;
   int y = AsConfig::Platform_Y_Pos;
   RECT intersectRect;

   if (! IntersectRect(&intersectRect, &paint_area, &Platform_Rect))
   {
      return;
   }

   //1. Рисуем платформу
   SelectObject(hdc, AsConfig::BG_Pen);
   SelectObject(hdc, AsConfig::BG_Brush);

   Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top, Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);

   //2. Рисуем боковые шарики
   SelectObject(hdc, Platform_Cercle_Pen);
   SelectObject(hdc, Platform_Cercle_Brush);
   Ellipse(hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + Circle_Size) * AsConfig::Global_Scale, (y + Circle_Size) * AsConfig::Global_Scale);
   Ellipse(hdc, (x + Inner_Width) * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + Circle_Size + Inner_Width) * AsConfig::Global_Scale, 
                (y + Circle_Size) * AsConfig::Global_Scale);
   //3. Рисуем блик
   SelectObject(hdc, Highlight_Pen);
   Arc(hdc, (x + 1) * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale, (x + Circle_Size - 1) * AsConfig::Global_Scale, 
            (y + Circle_Size - 1) * AsConfig::Global_Scale,(x + 1 + 1) * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale, 
            (x + 1) * AsConfig::Global_Scale, (y + 1 +2) * AsConfig::Global_Scale);
   //4. Рисуем среднюю часть.
   SelectObject(hdc, Platform_Inner_Pen);
   SelectObject(hdc, Platform_Inner_Brush);
   RoundRect(hdc, (x + 4) * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale, (x + 4 + Inner_Width - 1) * AsConfig::Global_Scale, 
                  (y + 1 + 5) * AsConfig::Global_Scale, 3 * AsConfig::Global_Scale, AsConfig::Global_Scale * 3);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Meltdown_State(HDC hdc, RECT &paint_area) //Рисуем платформу в расплавленном виде.
{
   int x, y;
   int y_offset;
   int area_width, area_height; 
 
   COLORREF pixel;
   COLORREF bg_pixel = RGB(AsConfig::BG_Color.R, AsConfig::BG_Color.G, AsConfig::BG_Color.B);
   RECT intersectRect;
   
   area_width = Width * AsConfig::Global_Scale;
   area_height = Height * AsConfig::Global_Scale + 1;

   if (! IntersectRect(&intersectRect, &paint_area, &Platform_Rect))
   {
      return;
   }

   for (int i = 0; i < area_width; i++)
   {
      y_offset = AsConfig::Rand(Meltdown_Speed) + 1;
      x = Platform_Rect.left + i;

      for(int j = 0; j < area_height; j++)
      {
         y = Meltdown_Platform_Y_Pos[i] - j;

         pixel = GetPixel(hdc, x, y);
         SetPixel(hdc, x, y + y_offset, pixel);
      }

      for (int j = 0; j < y_offset; j++)
      {
         y = Meltdown_Platform_Y_Pos[i] - area_height + 1 + j;
         SetPixel(hdc, x, y, bg_pixel);
      }
      Meltdown_Platform_Y_Pos[i] += y_offset;
   }
   
}
//------------------------------------------------------------------------------------------------------------