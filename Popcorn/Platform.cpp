﻿#include"Platform.h"


//AsPlatform
const double AsPlatform::Max_Glue_Spot_Height_Ratio = 1.0;
const double AsPlatform::Min_Glue_Spot_Height_Ratio = 0.4;
const double AsPlatform::Glue_Spot_Height_Ratio_Step = 0.05;
//------------------------------------------------------------------------------------------------------------
AsPlatform::~AsPlatform()
{
   delete[] Normal_Platform_Imege;
}
//------------------------------------------------------------------------------------------------------------
AsPlatform::AsPlatform()
: X_Pos(AsConfig::Border_X_Offset), Platform_State(EPS_Missing), Platform_Substate_Meltdown(EPSM_Unknown),
  Platform_Substate_Rolling(EPSR_Unknown), Platform_Substate_Glue(EPSG_Unknown), Platform_Moving_State(EPMS_Stop), Left_Key_Down(false), 
  Right_Key_Down(false), Inner_Width(Normal_Platform_Inner_Width), Rolling_Step(0.0), Speed(0), Glue_Spot_Height_Ratio(0.0),
  Ball_Set(0), Normal_Platform_Imege_Width(0), Normal_Platform_Imege_Height(0), Normal_Platform_Imege(0), Width(Normal_Width), 
  Platform_Rect{}, Prev_Platform_Rect{}, Highlight_Color(255, 255, 255), Platform_Cercle_Color(151, 0 , 0), Platform_Inner_Color(0, 128, 192)
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
   double ball_x, ball_y;
	if (next_y_pos + ball->Radius < AsConfig::Platform_Y_Pos)
		return false;

   inner_top_y = (double)(AsConfig::Platform_Y_Pos + 1);
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

   if (Platform_State == EPS_Glue && Platform_Substate_Glue == EPSG_Active)
   {
      ball->Get_Center(ball_x, ball_y);
      ball->Set_State(EBS_On_Platform, ball_x, ball_y);
   }

   return true;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Begin_Movement()
{
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Finish_Movement()
{
   if (Platform_Moving_State == EPMS_Stop)
      return;

   Redraw_Platform();

   if (Platform_Moving_State == EPMS_Stopping)
      Platform_Moving_State = EPMS_Stop;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Advance(double max_speed)
{
   double max_platform_x, next_step;

   if (Platform_Moving_State == EPMS_Stopping || Platform_Moving_State == EPMS_Stop)
      return;

   max_platform_x = AsConfig::Max_X_Pos - Width + 1;
   next_step = Speed / max_speed * AsConfig::Moving_Step_Size;

	X_Pos += next_step;

	if (X_Pos <= AsConfig::Border_X_Offset)
   {
		X_Pos = AsConfig::Border_X_Offset;
      Speed = 0.0;
      Platform_Moving_State == EPMS_Stopping;
   }

	if (X_Pos >= max_platform_x)
   {
		X_Pos = max_platform_x;
      Speed = 0.0;
      Platform_Moving_State == EPMS_Stopping;
   }

   //Смещаем преклеинные шарики
   if (Platform_State == EPS_Ready || (Platform_State == EPS_Glue && Platform_Substate_Glue == EPSG_Active) )
   {
      if (Platform_Moving_State == EPMS_Moving_Left)
         Ball_Set->On_Platform_Advance(M_PI, fabs(Speed), max_speed);
      else
         if (Platform_Moving_State == EPMS_Moving_Right)
             Ball_Set->On_Platform_Advance(0.0, fabs(Speed), max_speed);
   }
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
      Act_For_Meltdown_State();
      break;

   case EPS_Rolling:
      Act_For_Rolling_State();
      break;

	case EPS_Glue:
      Act_For_Glue_State();
		break;
	}
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Clear(HDC hdc, RECT &paint_area)
{
   RECT intersectRect;

   if (! IntersectRect(&intersectRect, &paint_area, &Prev_Platform_Rect) )
      return;

   switch (Platform_State)
   {
   case EPS_Ready:
   case EPS_Normal:
   case EPS_Rolling:
   case EPS_Glue:
      //Очищаем фоном прежнее место
      AsConfig::BG_Color.Select(hdc);
      Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top, Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);
   }
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


   //case EPS_Pre_Meltdown:
   //   Draw_Normal_State(hdc, paint_area);
   //   Set_State(EPS_Meltdown);
   //   break;


   case EPS_Meltdown:
      if (Platform_Substate_Meltdown == EPSM_Active)
         Draw_Meltdown_State(hdc, paint_area);
      break;


   case EPS_Roll_In:
      Draw_Roll_In_State(hdc, paint_area);
      break;


   case EPS_Expand_Roll_In:
      Draw_Expanding_Roll_In_State(hdc, paint_area);
      break;


   case EPS_Glue:
      Draw_Glue_State(hdc, paint_area);
      break;
   }
}
//------------------------------------------------------------------------------------------------------------
bool AsPlatform::Is_Finished()
{
   return false; // Заглушка. Не используеться метод.
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Init(AsBall_Set *ball_set)
{
   Ball_Set = ball_set;
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
	case EPS_Normal:
		if (Platform_State == EPS_Glue)
		{
			Platform_Substate_Glue = EPSG_Finalize;

			while (Ball_Set->Release_Next_Ball())
			{
			}

         return;
		}
		break;


   //case EPS_Pre_Meltdown:
   //   Speed = 0.0;
   //   break;


   case EPS_Meltdown:
      Speed = 0.0;
      Platform_Substate_Meltdown = EPSM_Init;

      len = sizeof(Meltdown_Platform_Y_Pos) / sizeof(Meltdown_Platform_Y_Pos[0]);

      for (int i = 0; i < len; i++)
         Meltdown_Platform_Y_Pos[i] = Platform_Rect.top;
 
      break;


   case EPS_Roll_In:
      X_Pos = AsConfig::Max_X_Pos - 1;
      Rolling_Step = Max_Rolling_Step - 1;
      break;


   case EPS_Glue:
      if (Platform_Substate_Glue == EPSG_Finalize)
         return;
      else
      {
         Platform_Substate_Glue = EPSG_Init;
         Glue_Spot_Height_Ratio = Min_Glue_Spot_Height_Ratio;
      }
      break;
   } 
   
   Platform_State = new_state;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Redraw_Platform(bool update_rect)
{
   int platform_width;

	if (update_rect)
	{
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
	}

   AsConfig::Ivalidate_Rect(Prev_Platform_Rect);
   AsConfig::Ivalidate_Rect(Platform_Rect);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Move(bool to_left, bool key_down)
{
	if (! (Platform_State == EPS_Normal || Platform_State == EPS_Glue) )
		return;

   if (to_left)
      Left_Key_Down = key_down;
   else
      Right_Key_Down = key_down;

   if (Left_Key_Down && Right_Key_Down)
      return; // Игнорируем одновременное нажатие клавич

	if (! Left_Key_Down && ! Right_Key_Down)
		if (!key_down)
		{
			Speed = 0.0;
			Platform_Moving_State = EPMS_Stopping;
			return;
		}

	if (Left_Key_Down)
	{
      Platform_Moving_State = EPMS_Moving_Left;
      Speed = -X_Step;
	}

	if (Right_Key_Down)
	{
      Platform_Moving_State = EPMS_Moving_Right;
      Speed = X_Step;
	}
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::On_Space_Key(bool key_down)
{
	if (! key_down)
      return;

	switch (Get_State() )
	{
   case EPS_Ready:
		Ball_Set->Release_From_Platform(Get_Middle_Pos() );
		Set_State(EPS_Normal);
      break;


   case EPS_Glue:
      Ball_Set->Release_Next_Ball();
      break;
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
void AsPlatform::Act_For_Meltdown_State()
{
   switch (Platform_Substate_Meltdown)
   {
   case EPSM_Init:
      Platform_Substate_Meltdown = EPSM_Active;
      break;

   case EPSM_Active:
      Redraw_Platform();
      break;
   }
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Act_For_Rolling_State()
{
   Redraw_Platform();
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Act_For_Glue_State()
{
	switch (Platform_Substate_Glue)
	{
	case EPSG_Init:
		if (Glue_Spot_Height_Ratio < Max_Glue_Spot_Height_Ratio)
			Glue_Spot_Height_Ratio += Glue_Spot_Height_Ratio_Step;
		else
			Platform_Substate_Glue = EPSG_Active;

		Redraw_Platform(false);
		break;

	case EPSG_Finalize:
		if (Glue_Spot_Height_Ratio > Min_Glue_Spot_Height_Ratio)
			Glue_Spot_Height_Ratio -= Glue_Spot_Height_Ratio_Step;
		else
		{
			Platform_State = EPS_Normal;
			Platform_Substate_Glue = EPSG_Unknown;
		}

		Redraw_Platform(false);
		break;
	}
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
void AsPlatform::Draw_Glue_State(HDC hdc, RECT &paint_area)
{//Рисуем платформу с растикающемся клеем

   HRGN region;
   RECT glue_rect;

   Draw_Normal_State(hdc, paint_area);

   glue_rect.left = (int)( (X_Pos + 5.0) * AsConfig::D_Global_Scale);
   glue_rect.top = (AsConfig::Platform_Y_Pos + 1) * AsConfig::Global_Scale;
   glue_rect.right = glue_rect.left + Normal_Platform_Inner_Width * AsConfig::Global_Scale;
   glue_rect.bottom = glue_rect.top + (Height - 2) * AsConfig::Global_Scale;

   region = CreateRectRgnIndirect(&glue_rect);
   SelectClipRgn(hdc, region);

   AsConfig::BG_Color.Select(hdc);
   Draw_Glue_Spot(hdc, 0, 9, 5);
   Draw_Glue_Spot(hdc, 6, 6, 5);
   Draw_Glue_Spot(hdc, 9, 9, 6);

   AsConfig::White_Color.Select(hdc);   
   Draw_Glue_Spot(hdc, 0, 9, 4);
   Draw_Glue_Spot(hdc, 6, 6, 4);
   Draw_Glue_Spot(hdc, 9, 9, 5);

   SelectClipRgn(hdc, 0);
   DeleteObject(region);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Glue_Spot(HDC hdc, int x_offset, int width, int height)
{//Рисуем пятно клея

   RECT spot_rect;
   int platform_top = (AsConfig::Platform_Y_Pos + 1) * AsConfig::Global_Scale;
   int spot_height = (int)( (double)height * AsConfig::D_Global_Scale * Glue_Spot_Height_Ratio);

   //Рисуем полуэллипс как "пятно" клея
   spot_rect.left = (int)( (X_Pos + 5.0 + (double)x_offset) * AsConfig::D_Global_Scale);
   spot_rect.top = platform_top - spot_height;
   spot_rect.right = spot_rect.left + width * AsConfig::Global_Scale;
   spot_rect.bottom = platform_top + spot_height - AsConfig::Global_Scale;

   Chord(hdc, spot_rect.left, spot_rect.top, spot_rect.right - 1, spot_rect.bottom - 1, spot_rect.left, platform_top - 1, spot_rect.right - 1, platform_top - 1);
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

   if (distance + AsConfig::Moving_Step_Size < two_radiuses)
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

