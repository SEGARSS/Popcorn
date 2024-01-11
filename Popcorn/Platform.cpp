#include"Platform.h"


//AMover
//------------------------------------------------------------------------------------------------------------
AMover::~AMover()
{
}
//------------------------------------------------------------------------------------------------------------




//AsPlatform
//------------------------------------------------------------------------------------------------------------
AsPlatform::~AsPlatform()
{
   delete[] Normal_Platform_Imege;
}
//------------------------------------------------------------------------------------------------------------
AsPlatform::AsPlatform()
: X_Pos(AsConfig::Border_X_Offset), Platform_State(EPS_Missing), Platform_Moving_State(EPMS_Stop), 
  Inner_Width(Normal_Platform_Inner_Width), Rolling_Step(0), Normal_Platform_Imege_Width(0), 
  Normal_Platform_Imege_Height(0), Normal_Platform_Imege(0), Width(Normal_Width), Platform_Rect{}, Prev_Platform_Rect{}, 
  Highlight_Color(255, 255, 255), Platform_Cercle_Color(151, 0 , 0), Platform_Inner_Color(0, 128, 192)
{
   X_Pos = (AsConfig::Max_X_Pos - Width) / 2;
}
//------------------------------------------------------------------------------------------------------------
bool AsPlatform::Check_Hit(double next_x_pos, double next_y_pos, ABall* ball)
{
   double inner_left_x, inner_right_x;
   double inner_top_y, inner_low_y;
   double reflection_pos;
   double inner_y;
	if (next_y_pos + ball->Radius < AsConfig::Platform_Y_Pos)
		return false;

   inner_top_y = (double)(AsConfig::Platform_Y_Pos - 1);
   inner_low_y = (double)(AsConfig::Platform_Y_Pos + Height - 1);
   inner_left_x = (double)(X_Pos + Circle_Size - 1);
   inner_right_x = (double)(X_Pos + Width - (Circle_Size - 1) );

   // 1. Проверяем отражение от боковых шариков
   // От левого
   if (Reflect_On_Circle(next_x_pos, next_y_pos, 0.0, ball) )
      goto _on_hit;

   // 2. Проверяем отражение от боковых шариков
   // От правого
   if (Reflect_On_Circle(next_x_pos, next_y_pos, Width - Circle_Size, ball) )
      goto _on_hit;

   // 3. Проверяме отражение от центральной части
   if (ball->Is_Moving_Up() )
      inner_y = inner_low_y; //От нижней грани
   else
      inner_y = inner_top_y; //От верхней грани
	if (Hit_Circle_On_Line(next_y_pos - inner_y, next_x_pos, inner_left_x, inner_right_x, ball->Radius, reflection_pos) )
	{
		ball->Reflect(true);
		goto _on_hit;
	}
	return false;

_on_hit:
   if (ball->Get_State() == EBS_On_Parachute)
      ball->Set_State(EBS_Off_Parachute);
   return true;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Advance(double max_speed)
{
   double max_platform_x = AsConfig::Max_X_Pos - Width + 1;
   double next_step = Speed / max_speed * AsConfig::Moving_Step_Size;

	X_Pos += next_step;

	if (X_Pos <= AsConfig::Border_X_Offset)
		X_Pos = AsConfig::Border_X_Offset;

	if (X_Pos >= max_platform_x)
		X_Pos = max_platform_x;
}
//------------------------------------------------------------------------------------------------------------
double AsPlatform::Get_Speed()
{
   return Speed;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Act()
{
   switch (Platform_State)
   {
   case EPS_Meltdown:
   case EPS_Roll_In:
   case EPS_Expand_Roll_In:
      Redraw_Platform();
   }
}
//------------------------------------------------------------------------------------------------------------
EPlatform_State AsPlatform::Get_State()
{
   return Platform_State;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Set_State(EPlatform_State new_state)
{
   int len;

   if (Platform_State == new_state)
      return;

   switch (new_state)
   {
   case EPS_Meltdown:
      len = sizeof(Meltdown_Platform_Y_Pos) / sizeof(Meltdown_Platform_Y_Pos[0]);
      for (int i = 0; i < len; i++)
      {
         Meltdown_Platform_Y_Pos[i] = Platform_Rect.top;
      }
      break;
   case EPS_Roll_In:
      X_Pos = AsConfig::Max_X_Pos - 1;
      Rolling_Step = Max_Rolling_Step - 1;
      break;
   } 
   
   Platform_State = new_state;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Redraw_Platform()
{
   int platform_width;

   Prev_Platform_Rect = Platform_Rect;

   if (Platform_State == EPS_Roll_In)
      platform_width = Circle_Size;
   else
      platform_width = Width;
   
   Platform_Rect.left = (int)(X_Pos * AsConfig::D_Global_Scale);
   Platform_Rect.top = AsConfig::Platform_Y_Pos * AsConfig::Global_Scale;
   Platform_Rect.right = Platform_Rect.left + platform_width * AsConfig::Global_Scale;
   Platform_Rect.bottom = Platform_Rect.top + Height * AsConfig::Global_Scale;
  
   if (Platform_State == EPS_Meltdown)
      Prev_Platform_Rect.bottom = (AsConfig::Max_X_Pos + 1) * AsConfig::Global_Scale;

   InvalidateRect(AsConfig::Hwnd, &Prev_Platform_Rect, FALSE);
   InvalidateRect(AsConfig::Hwnd, &Platform_Rect, FALSE);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw(HDC hdc, RECT &paint_area)
{
   RECT intersectRect;

   if (! IntersectRect(&intersectRect, &paint_area, &Platform_Rect))
      return;

   switch (Platform_State)
   {
   case EPS_Ready:
   case EPS_Normal:
      Draw_Normal_State(hdc, paint_area);
      break;


   case EPS_Meltdown:
      Draw_Meltdown_State(hdc, paint_area);
      break;


   case EPS_Roll_In:
      Draw_Roll_In_State(hdc, paint_area);
      break;


   case EPS_Expand_Roll_In:
      Draw_Expanding_Roll_In_State(hdc, paint_area);
      break;
   }
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Move(bool to_left, bool key_down)
{
	if (Platform_State != EPS_Normal)
		return;

	if (to_left)
	{
		if (Platform_Moving_State == EPMS_Moving_Left)
		{
			if (! key_down)
			{
            Speed = 0.0;
				Platform_Moving_State = EPMS_Stop;
				return;
			}
		}
		else
			Platform_Moving_State = EPMS_Moving_Left;

      Speed = -X_Step;
	}
	else
	{
		if (Platform_Moving_State == EPMS_Moving_Right)
		{
			if (!key_down)
			{
            Speed = 0.0;
				Platform_Moving_State = EPMS_Stop;
				return;
			}
		}
		else
			Platform_Moving_State = EPMS_Moving_Right;

      Speed = X_Step;
	}
}
//------------------------------------------------------------------------------------------------------------
bool AsPlatform::Hit_By(AFalling_Letter *falling_letter)
{
   RECT intersectRect, falling_letter_rect;

   falling_letter->Get_Letter_Cell(falling_letter_rect);

   if (IntersectRect(&intersectRect, &falling_letter_rect, &Platform_Rect) )
      return true;
   else
      return false;
}
//------------------------------------------------------------------------------------------------------------
double AsPlatform::Get_Middle_Pos()
{
   return X_Pos + (double)Width / 2.0;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Circle_BG(HDC hdc)//Очистка фона
{
   //Очищение фоном прежнее место
   AsConfig::BG_Color.Select(hdc);
   Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top, Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Circle_Highlight(HDC hdc, int x, int y)//Юлики на мячике
{
   //Рисуем блик
   Highlight_Color.Select_Pen(hdc);

   Arc(hdc, x + AsConfig::Global_Scale, y + AsConfig::Global_Scale, x + (Circle_Size - 1) * AsConfig::Global_Scale - 1, 
            y + (Circle_Size - 1) * AsConfig::Global_Scale - 1,x + 2 * AsConfig::Global_Scale, y + AsConfig::Global_Scale, 
            x + AsConfig::Global_Scale, y + 3 * AsConfig::Global_Scale);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Normal_State(HDC hdc, RECT &paint_area) //Рисуем платфолрму в нормальном состоянии
{
   double x = X_Pos;
   int y = AsConfig::Platform_Y_Pos;
   const int scale = AsConfig::Global_Scale;
   const double d_scale = AsConfig::D_Global_Scale;
   RECT inner_rect, rect;

   //Очистка фона
   Circle_BG(hdc);

   //1.Рисуем боковые шарики
   Platform_Cercle_Color.Select(hdc);

   rect.left = (int)(x * d_scale);
   rect.top = y * scale;
   rect.right = (int)( (x + (double)Circle_Size) * d_scale);
   rect.bottom = (y + Circle_Size) * scale;

   Ellipse(hdc, rect.left, rect.top, rect.right - 1, rect.bottom - 1);

   rect.left = (int)( (x + Inner_Width) * d_scale);
   rect.top = y * scale;
   rect.right = (int)( (x + (double)Circle_Size + Inner_Width) * d_scale);
   rect.bottom = (y + Circle_Size) * scale;

   Ellipse(hdc, rect.left, rect.top, rect.right - 1, rect.bottom - 1);

   //2.Рисуем блики
   Draw_Circle_Highlight(hdc, (int)(x * d_scale), y * scale);

   
   //3.Рисуем среднюю часть.
   Platform_Inner_Color.Select(hdc);

   inner_rect.left = (int)( (x + 4) * d_scale);
   inner_rect.top = (y + 1) * scale;
   inner_rect.right = (int)( (x + 4 + Inner_Width - 1) * d_scale);
   inner_rect.bottom = (y + 1 + 5) * scale;

   AsConfig::Round_Rect(hdc, inner_rect, 3);

   if (Normal_Platform_Imege == 0 && Platform_State == EPS_Ready)
      Get_Normal_Platform_Imege(hdc);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Meltdown_State(HDC hdc, RECT &paint_area) //Рисуем платформу в расплавленном виде.
{
   int x, y;
   int y_offset;
   int stroke_len;
   int moved_colums_count = 0;
   int max_platform_y;
   const AColor *color;
  
   Normal_Platform_Imege_Width = Width * AsConfig::Global_Scale;

   max_platform_y = (AsConfig::Max_Y_Pos + 1) * AsConfig::Global_Scale;

   for (int i = 0; i < Normal_Platform_Imege_Width; i++)
   {
      if (Meltdown_Platform_Y_Pos[i] > max_platform_y)
         continue;

      ++moved_colums_count;

      y_offset = AsConfig::Rand(Meltdown_Speed) + 1;
      x = Platform_Rect.left + i;

      int j = 0;
      y = Meltdown_Platform_Y_Pos[i];

      MoveToEx(hdc, x, y, 0);

      //Рисуем последовательность вертикальных штрихов разного цвета (согласно прообразу, сохранённому в Normal_Platform_Imege)
      while (Get_Platform_Image_Stroke_Color(i, j, &color, stroke_len) )
      {
         color->Select_Pen(hdc);
         LineTo(hdc, x, y + stroke_len);

         y += stroke_len;
         j += stroke_len;
      }

      // Стираем фоном пиксели над штрихом
      y = Meltdown_Platform_Y_Pos[i];
      MoveToEx(hdc, x, y, 0);
      AsConfig::BG_Color.Select_Pen(hdc);
      LineTo(hdc, x, y + y_offset);

      Meltdown_Platform_Y_Pos[i] += y_offset;
   }

   if (moved_colums_count == 0)
      Platform_State = EPS_Missing;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Roll_In_State(HDC hdc, RECT &paint_area)//Рисуем выкатывающуюся платформу
{
   int x = (int)(X_Pos * AsConfig::D_Global_Scale);
   int y = AsConfig::Platform_Y_Pos * AsConfig::Global_Scale;
   int roller_size = Circle_Size * AsConfig::Global_Scale;
   double alpha;
   XFORM xform, old_xform;
   
   Circle_BG(hdc);

   //1.Шарик.
   Platform_Cercle_Color.Select(hdc);

   Ellipse(hdc, x, y, x + roller_size - 1, y + roller_size - 1);

   //2.Разделительная линия.
   alpha = -2.0 * M_PI / (double)Max_Rolling_Step * (double)Rolling_Step;

   xform.eM11 = (float)cos(alpha);
   xform.eM12 = (float)sin(alpha);
   xform.eM21 = (float)-sin(alpha);
   xform.eM22 = (float)cos(alpha);
   xform.eDx  = (float)(x + roller_size / 2);
   xform.eDy  = (float)(y + roller_size / 2);
   GetWorldTransform(hdc, &old_xform);
   SetWorldTransform(hdc, &xform);

   AsConfig::BG_Color.Select(hdc);

   Rectangle(hdc, - AsConfig::Global_Scale / 2, -roller_size / 2, AsConfig::Global_Scale / 2, roller_size / 2);

   SetWorldTransform(hdc, &old_xform);
   
   //3.Блики.
   Draw_Circle_Highlight(hdc, x, y);

   ++Rolling_Step;

   if (Rolling_Step >= Max_Rolling_Step)
      Rolling_Step -= Max_Rolling_Step;

   X_Pos -= Rolling_Platform_Speed;

   if (X_Pos <= Roll_In_Platform_End_X_Pos)
   {
      X_Pos += Rolling_Platform_Speed;
      Platform_State = EPS_Expand_Roll_In;
      Inner_Width = 1;
   }
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Expanding_Roll_In_State(HDC hdc, RECT &paint_area)//Рисуем расширяющуюся после выкатывания платформы
{
   Draw_Normal_State(hdc, paint_area);

   --X_Pos;
   Inner_Width += 2;

   if (Inner_Width >= Normal_Platform_Inner_Width)
   {
      Inner_Width = Normal_Platform_Inner_Width;
      Platform_State = EPS_Ready;
      Redraw_Platform();
   }
}
//------------------------------------------------------------------------------------------------------------
bool AsPlatform::Reflect_On_Circle(double next_x_pos, double next_y_pos, double platform_ball_x_offset, ABall* ball)
{
   double dx,dy;
   double platform_ball_x, platform_ball_y, platform_ball_radius;
   double distance, two_radiuses;
   double gamma, beta, alpha;
   double related_ball_direction;
   const double pi_2 = 2.0 * M_PI;


   platform_ball_radius = (double)Circle_Size / 2.0;
   platform_ball_x = (double)X_Pos + platform_ball_radius + platform_ball_x_offset;
   platform_ball_y = (double)AsConfig::Platform_Y_Pos + platform_ball_radius;

   dx = next_x_pos - platform_ball_x;
   dy = next_y_pos - platform_ball_y;

   distance = sqrt(dx * dx + dy * dy);
   two_radiuses = platform_ball_radius + ball->Radius;

   if (fabs(distance - two_radiuses) < AsConfig::Moving_Step_Size)
   {
      // Мячик коснулся бокового шарика
      beta = atan2(-dy, dx);

      related_ball_direction = ball->Get_Direction();
      related_ball_direction -= beta;

      if (related_ball_direction > pi_2)                       
         related_ball_direction -= pi_2;

      if (related_ball_direction < 0.0)                        
         related_ball_direction += pi_2;

		if (related_ball_direction > M_PI_2 && related_ball_direction < M_PI + M_PI_2)
		{
			alpha = beta + M_PI - ball->Get_Direction();
			gamma = beta + alpha;

			ball->Set_Direction(gamma);

         return true;
		}
   }
   return false;
}
//------------------------------------------------------------------------------------------------------------
bool AsPlatform::Get_Platform_Image_Stroke_Color(int x, int y, const AColor **color, int &stroke_len)
{//Вычисляем длинну оредного вертикального штриха

   int offset = y * Normal_Platform_Imege_Width + x; // Позиция в массиве Normal_Platform_Imege, соответствующая смещению (х, у)
   int color_value;
   stroke_len = 0;

   if (y >= Normal_Platform_Imege_Height)
      return false;

   for (int i = y; i < Normal_Platform_Imege_Height; i++)
   {
      if (i == y)
      {
         color_value = Normal_Platform_Imege[offset];
         stroke_len = 1;
      }
      else
      {
         if (color_value == Normal_Platform_Imege[offset])
            ++stroke_len;
         else
            break;
      }
      offset += Normal_Platform_Imege_Width; //Переходим на строку ниже
   }

   if (color_value == Highlight_Color.Get_RGB() )
      *color = &Highlight_Color;
   else if (color_value == Platform_Cercle_Color.Get_RGB() )
      *color = &Platform_Cercle_Color;
   else if (color_value == Platform_Inner_Color.Get_RGB() )
      *color = &Platform_Inner_Color;
   else if (color_value == AsConfig::BG_Color.Get_RGB() )
      *color = &AsConfig::BG_Color;
   else
      AsConfig::Throw();
   return true;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Get_Normal_Platform_Imege(HDC hdc)
{
   int x = (int)(X_Pos * AsConfig::D_Global_Scale);
   int y = AsConfig::Platform_Y_Pos * AsConfig::Global_Scale;
   int offset = 0;

	Normal_Platform_Imege_Width = Width * AsConfig::Global_Scale;
	Normal_Platform_Imege_Height = Height * AsConfig::Global_Scale;

	Normal_Platform_Imege = new int[Normal_Platform_Imege_Width * Normal_Platform_Imege_Height];

	for (int i = 0; i < Normal_Platform_Imege_Height; i++)
		for (int j = 0; j < Normal_Platform_Imege_Width; j++)
			Normal_Platform_Imege[offset++] = GetPixel(hdc, x + j, y + i);
}
//------------------------------------------------------------------------------------------------------------

