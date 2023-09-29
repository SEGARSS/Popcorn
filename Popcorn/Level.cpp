#include "Level.h"

char ALevel::Level_01[AsConfig::Level_Height][AsConfig::Level_Width] =
{
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

char ALevel::Test_Level[AsConfig::Level_Height][AsConfig::Level_Width] =
{
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};


//ALevel
//------------------------------------------------------------------------------------------------------------
ALevel::ALevel()
: Brick_Red_Pen(0), Brick_Blue_Pen(0), Letter_Pen(0), Brick_Red_Brush(0), Brick_Blue_Brush(0), Level_Rect{},
  Active_Bricks_Count(0)
{
}
//------------------------------------------------------------------------------------------------------------
bool ALevel::Check_Hit(double next_x_pos, double next_y_pos, ABall *ball) // Проверка попадания по кирпичу
{
   double direction;
   double min_ball_x;
   double max_ball_x;
   double min_ball_y;
   double max_ball_y;
   int min_level_x;
   int max_level_x;
   int min_level_y;
   int max_level_y;
   bool got_horizontal_Hit, got_vertical_Hit;
   double horizontal_reflection_pos, vertical_reflection_pos;

   if (next_y_pos + ball->Radius > AsConfig::Level_Y_Offset + (AsConfig::Level_Height - 1) * AsConfig::Cell_Height + AsConfig::Brick_Height)
   {
      return false;
   }

   direction = ball->Get_Direction();

   min_ball_x = next_x_pos - ball->Radius;
   max_ball_x = next_x_pos + ball->Radius;
   min_ball_y = next_y_pos - ball->Radius;
   max_ball_y = next_y_pos + ball->Radius;

   min_level_x = (int)((min_ball_x - AsConfig::Level_X_Offset) / (double)AsConfig::Cell_Width);
   max_level_x = (int)((max_ball_x - AsConfig::Level_X_Offset) / (double)AsConfig::Cell_Width);
   min_level_y = (int)((min_ball_y - AsConfig::Level_Y_Offset) / (double)AsConfig::Cell_Height);
   max_level_y = (int)((max_ball_y - AsConfig::Level_Y_Offset) / (double)AsConfig::Cell_Height);

	// Корректируем позицию при отражении от кирпичей
	for (int i = max_level_y; i >= min_level_y; i--)
	{
		Current_Brick_Top_Y = AsConfig::Level_Y_Offset + i * AsConfig::Cell_Height;
		Current_Brick_Low_Y = Current_Brick_Top_Y + AsConfig::Brick_Height;

		for (int j = min_level_x; j <= max_level_x; j++)
		{
			if (Current_Level[i][j] == 0)
			{
				continue;
			}

			Current_Brick_Left_X = AsConfig::Level_X_Offset + j * AsConfig::Cell_Width;
			Current_Brick_Right_X = Current_Brick_Left_X + AsConfig::Brick_Width;

         got_horizontal_Hit = Check_Horizontal_Hit(next_x_pos, next_y_pos, j, i, ball, horizontal_reflection_pos);

         got_vertical_Hit = Check_Vertical_Hit(next_x_pos, next_y_pos, j, i, ball, vertical_reflection_pos);

         if (got_horizontal_Hit && got_vertical_Hit)
         {
            if (vertical_reflection_pos < horizontal_reflection_pos)
            {
               ball->Reflect(true);
            }
            else
            {
               ball->Reflect(false);
            }

            Add_Active_Brick(j, i);

            return true;
         }
         else
         {
            if (got_horizontal_Hit)
            {
               ball->Reflect(false);
               Add_Active_Brick(j, i);
               return true;
            }
            else
            {
               if (got_vertical_Hit)
               {
                  ball->Reflect(true);
                  Add_Active_Brick(j, i);
                  return true;
               }
            }
         }
		}
	}

	return false;
}
//------------------------------------------------------------------------------------------------------------
void ALevel::Init()
{
   Letter_Pen = CreatePen(PS_SOLID, AsConfig::Global_Scale, RGB(255, 255, 255));

   AsConfig::Create_Pen_Brush(AsConfig::Red_Brick_Color, Brick_Red_Pen, Brick_Red_Brush);
   AsConfig::Create_Pen_Brush(AsConfig::Blue_Brick_Color, Brick_Blue_Pen, Brick_Blue_Brush);

   Level_Rect.left = AsConfig::Level_X_Offset * AsConfig::Global_Scale;
   Level_Rect.top = AsConfig::Level_Y_Offset * AsConfig::Global_Scale;
   Level_Rect.right = Level_Rect.left + AsConfig::Cell_Width * AsConfig::Level_Width * AsConfig::Global_Scale;
   Level_Rect.bottom = Level_Rect.top + AsConfig::Cell_Height * AsConfig::Level_Height * AsConfig::Global_Scale;

   memset(Current_Level, 0, sizeof(Current_Level));
   memset(Active_Brick, 0, sizeof(Active_Brick));
}
//------------------------------------------------------------------------------------------------------------
void ALevel::Set_Current_Level(char level[AsConfig::Level_Height][AsConfig::Level_Width])
{
   memcpy(Current_Level, level, sizeof(Current_Level));
}
//------------------------------------------------------------------------------------------------------------
void ALevel::Act()
{
   for (int i = 0; i < Active_Bricks_Count; i++)
   {
      Active_Brick[i]->Act();
   }
}
//------------------------------------------------------------------------------------------------------------
void ALevel::Draw(HDC hdc, RECT &paint_area)//Вывод всех кирпичей
{
   RECT intersectRect;

   if (! IntersectRect(&intersectRect, &paint_area, &Level_Rect))
   {
      return;
   }
   for (int i = 0; i < AsConfig::Level_Height; i++)
   {
      for (int j = 0; j < AsConfig::Level_Width; j++)
      {
         Draw_Brick(hdc, AsConfig::Level_X_Offset + j * AsConfig::Cell_Width, AsConfig::Level_Y_Offset + i * AsConfig::Cell_Height, (EBrick_Type)Current_Level[i][j]);
      }
   }

   for (int i = 0; i < Active_Bricks_Count; i++)
   {
      Active_Brick[i]->Draw(hdc, paint_area);
   }
}
//------------------------------------------------------------------------------------------------------------
void ALevel::Add_Active_Brick(int brick_x, int brick_y)
{
   EBrick_Type brick_type;
   AActive_Brick *active_brick;

	if (Active_Bricks_Count >= sizeof(Active_Brick) / sizeof(Active_Brick[0]))
	{
		return; //Активных кирпичей слишком много
	}

   brick_type = (EBrick_Type)Current_Level[brick_y][brick_x];

   switch (brick_type)
   {
   case EBT_None:
      return;
   case EBT_Red:
   case EBT_Blue:
      active_brick = new AActive_Brick(brick_type, brick_x, brick_y);
      break;
   /*case EBT_Unbreakable:
      break;
   case EBT_Multihit_1:
      break;
   case EBT_Multihit_2:
      break;
   case EBT_Multihit_3:
      break;
   case EBT_Multihit_4:
      break;
   case EBT_Parachute:
      break;
   case EBT_Teleport:
      break;
   case EBT_Ad:
      break;*/
   default:
      return;
   }
  
   Active_Brick[Active_Bricks_Count++] = active_brick;
}
//------------------------------------------------------------------------------------------------------------
bool ALevel::Check_Vertical_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, ABall *ball, double &reflection_pos)
{
   double direction = ball->Get_Direction();

	if (ball->Is_Moving_Up() )
	{
      // Проверяет попадание в нижнюю грань.
		if (Hit_Circle_On_Line(next_y_pos - Current_Brick_Low_Y, next_x_pos, Current_Brick_Left_X, Current_Brick_Right_X, ball->Radius, reflection_pos))
		{
         //Проверка возможности отскока вниз
         if (level_y < AsConfig::Level_Height - 1 && Current_Level[level_y +1][level_x] == 0)
         {
			   return true;
         }
         else
         {
            return false;
         }
		}
	}
	else
	{
      // Проверяет попадание в верхнию грань.
		if (Hit_Circle_On_Line(next_y_pos - Current_Brick_Top_Y, next_x_pos, Current_Brick_Left_X, Current_Brick_Right_X, ball->Radius, reflection_pos))
		{
			//Проверка возможности отскока вверх
			if (level_y > 0 && Current_Level[level_y - 1][level_x] == 0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
   return false;
}
//------------------------------------------------------------------------------------------------------------
bool ALevel::Check_Horizontal_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, ABall *ball, double &reflection_pos)
{
   double direction = ball->Get_Direction();

	if (! ball->Is_Moving_Left())
	{
      // Проверяет попадание в левую грань.
		if (Hit_Circle_On_Line(Current_Brick_Left_X - next_x_pos, next_y_pos, Current_Brick_Top_Y, Current_Brick_Low_Y, ball->Radius, reflection_pos))
		{
          //Проверка возможности отскока влево
         if (level_x > 0 && Current_Level[level_y][level_x - 1] == 0)
         {
			   return true;
         }
         else
         {
            return false;
         }
		}
	}
	else
	{
		// Проверяет попадание в правую грань.
		if (Hit_Circle_On_Line(Current_Brick_Right_X - next_x_pos, next_y_pos, Current_Brick_Top_Y, Current_Brick_Low_Y, ball->Radius, reflection_pos))
		{
			//Проверка возможности отскока вправо
			if (level_x < AsConfig::Level_Width - 1 && Current_Level[level_y][level_x + 1] == 0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
   return false;
}
//------------------------------------------------------------------------------------------------------------
void ALevel::Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type)//Вывод кирпича
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
   RoundRect(hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + AsConfig::Brick_Width) * AsConfig::Global_Scale, 
                 (y + AsConfig::Brick_Height) * AsConfig::Global_Scale, 2 * AsConfig::Global_Scale, AsConfig::Global_Scale * 2);
}
//------------------------------------------------------------------------------------------------------------
void ALevel::Set_Brick_Letter_Colors(bool is_switch_color, HPEN &front_pen, HBRUSH &front_brush, 
   HPEN &back_pen, HBRUSH &back_brush)
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
void ALevel::Draw_Brick_Letter(HDC hdc, int x, int y,
                               EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step)//Вывод падающией буквы
{
   bool switch_color;
	double offset;
	double rotation_angle;  // Преобразование шага в угол поворота
	int brick_half_height = AsConfig::Brick_Height * AsConfig::Global_Scale / 2;
	int back_part_offset;
	HPEN front_pen, back_pen;
	HBRUSH front_brush, back_brush;
	XFORM xform, old_xform;

   if (!(brick_type == EBT_Blue || brick_type == EBT_Red))
   {
      return;  // Падающие буквы могут быть только от кирпичей такого типа
   }

   // Корректируем шаг вращения и угол поворота
   rotation_step = rotation_step % 16;

   if (rotation_step < 8)
   {
      rotation_angle = 2.0 * M_PI / 16.0 * (double)rotation_step;
   }
   else
   {
      rotation_angle = 2.0 * M_PI / 16.0 * (double)(8 - rotation_step);
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

      Rectangle(hdc, x, y + brick_half_height - AsConfig::Global_Scale, x + AsConfig::Brick_Width * AsConfig::Global_Scale, 
                     y + brick_half_height);

      //Выводим передний план
      SelectObject(hdc, front_pen);
      SelectObject(hdc, front_brush);

      Rectangle(hdc, x, y + brick_half_height, x + AsConfig::Brick_Width * AsConfig::Global_Scale, 
                     y + brick_half_height + AsConfig::Global_Scale - 1);
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
      xform.eDy = (float)y + (float)(brick_half_height);
      GetWorldTransform(hdc, &old_xform);
      SetWorldTransform(hdc, &xform);

      // Выводим фон
      SelectObject(hdc, back_pen);
      SelectObject(hdc, back_brush);

      offset = 3.0 * (1.0 - fabs(xform.eM22)) * (double)AsConfig::Global_Scale;
      back_part_offset = (int)round(offset);
      Rectangle(hdc, 0, -brick_half_height - back_part_offset, AsConfig::Brick_Width * AsConfig::Global_Scale, 
                     brick_half_height - back_part_offset);

      //Выводим передний план
      SelectObject(hdc, front_pen);
      SelectObject(hdc, front_brush);

      
      Rectangle(hdc, 0, -brick_half_height, AsConfig::Brick_Width * AsConfig::Global_Scale, brick_half_height);

      if (rotation_step > 4 && rotation_step <= 12)
      {
         if (letter_type == ELT_O)
         {
            SelectObject(hdc, Letter_Pen);
            Ellipse(hdc, 0 + 5 * AsConfig::Global_Scale, (-5 * AsConfig::Global_Scale) / 2, 
                         0 + 10 * AsConfig::Global_Scale, 5 * AsConfig::Global_Scale / 2);
         }
      }
      SetWorldTransform(hdc, &old_xform);
   }
}
//------------------------------------------------------------------------------------------------------------
// 29 минута 29 видео. доделать