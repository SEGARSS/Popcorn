﻿#include "Level.h"


char AsLevel::Level_01[AsConfig::Level_Height][AsConfig::Level_Width] =
{
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

char AsLevel::Test_Level[AsConfig::Level_Height][AsConfig::Level_Width] =
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


//AsLevel
//------------------------------------------------------------------------------------------------------------
AsLevel::AsLevel()
: Level_Rect{}, Active_Bricks_Count(0), Falling_Letter_Count(0)
{
}
//------------------------------------------------------------------------------------------------------------
bool AsLevel::Check_Hit(double next_x_pos, double next_y_pos, ABall *ball) 
{// Проверка попадания по кирпичу

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

            On_Hit(j, i);
            return true;
         }
         else
         {
            if (got_horizontal_Hit)
            {
               ball->Reflect(false);
               On_Hit(j, i);
               return true;
            }
            else
            {
               if (got_vertical_Hit)
               {
                  ball->Reflect(true);
                  On_Hit(j, i);
                  return true;
               }
            }
         }
		}
	}
	return false;
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Init()
{
   Level_Rect.left = AsConfig::Level_X_Offset * AsConfig::Global_Scale;
   Level_Rect.top = AsConfig::Level_Y_Offset * AsConfig::Global_Scale;
   Level_Rect.right = Level_Rect.left + AsConfig::Cell_Width * AsConfig::Level_Width * AsConfig::Global_Scale;
   Level_Rect.bottom = Level_Rect.top + AsConfig::Cell_Height * AsConfig::Level_Height * AsConfig::Global_Scale;

   memset(Current_Level, 0, sizeof(Current_Level) );
   memset(Active_Brick, 0, sizeof(Active_Brick) );
   memset(Falling_Letter, 0, sizeof(Falling_Letter) );
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Set_Current_Level(char level[AsConfig::Level_Height][AsConfig::Level_Width])
{
   memcpy(Current_Level, level, sizeof(Current_Level));
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Act()
{
   Act_Objects( (AGraphics_Object **)&Active_Brick, AsConfig::Max_Active_Bricks_Count);
   Act_Objects( (AGraphics_Object **)&Falling_Letter, AsConfig::Max_Falling_Letter_Count);
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Draw(HDC hdc, RECT &paint_area)
{//Вывод всех кирпичей

   RECT intersectRect;
   RECT brick_rect;

   //AFalling_Letter falling_letter(EBT_Blue, ELT_Plus, 8 * AsConfig::Global_Scale, 150 * AsConfig::Global_Scale);
   //falling_letter.Test_Draw_All_Steps(hdc);

	if (IntersectRect(&intersectRect, &paint_area, &Level_Rect) )
	{
		for (int i = 0; i < AsConfig::Level_Height; i++)
		{
			for (int j = 0; j < AsConfig::Level_Width; j++)
			{
            brick_rect.left = (AsConfig::Level_X_Offset + j * AsConfig::Cell_Width) * AsConfig::Global_Scale;
            brick_rect.top = (AsConfig::Level_Y_Offset + i * AsConfig::Cell_Height) * AsConfig::Global_Scale;
            brick_rect.right = brick_rect.left + AsConfig::Brick_Width * AsConfig::Global_Scale;
            brick_rect.bottom = brick_rect.top + AsConfig::Brick_Height * AsConfig::Global_Scale;

            if (IntersectRect(&intersectRect, &paint_area, &brick_rect) )
            {
               Draw_Brick(hdc, brick_rect, (EBrick_Type)Current_Level[i][j]);
            }
			}
		}
      Draw_Objects(hdc, paint_area, (AGraphics_Object **)&Active_Brick, AsConfig::Max_Active_Bricks_Count);
	}
   Draw_Objects(hdc, paint_area, (AGraphics_Object **)&Falling_Letter, AsConfig::Max_Falling_Letter_Count);
}
//------------------------------------------------------------------------------------------------------------
bool AsLevel::Get_Next_Fallin_Letter(int &index, AFalling_Letter **falling_letter)
{
   AFalling_Letter *current_letter;

   if (Falling_Letter_Count == 0)
   {
      return false;
   }

   if (index < 0 || index >= AsConfig::Max_Falling_Letter_Count)
   {
      return false;
   }

   while (index < AsConfig::Max_Falling_Letter_Count)
   {
      current_letter = Falling_Letter[index++];

      if (current_letter != 0)
      {
         *falling_letter = current_letter;
         return true;
      }
   }

   return false;
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::On_Hit(int brick_x, int brick_y)
{
   EBrick_Type brick_type;
   brick_type = (EBrick_Type)Current_Level[brick_y][brick_x];

   if (Add_Falling_Letter(brick_x, brick_y, brick_type) )
      Current_Level[brick_y][brick_x] = EBT_None;
   else
      Add_Active_Brick(brick_x, brick_y, brick_type);
}
//------------------------------------------------------------------------------------------------------------
bool AsLevel::Add_Falling_Letter(int brick_x, int brick_y, EBrick_Type brick_type)
{//Создаём падающую букву, если можем

   int leter_x, leter_y;
   ELetter_Type letter_type;
	AFalling_Letter* falling_letter;

	if (! (brick_type == EBT_Red || brick_type == EBT_Blue) )
      return false;
   
	if (AsConfig::Rand(AsConfig::Hit_Per_Letter) != 0)
      return false;
	
	if (Falling_Letter_Count >= AsConfig::Max_Falling_Letter_Count)
      return false;
	
	for (int i = 0; i < AsConfig::Max_Falling_Letter_Count; i++)
	{
		if (Falling_Letter[i] == 0)
		{
			leter_x = (brick_x * AsConfig::Cell_Width + AsConfig::Level_X_Offset) * AsConfig::Global_Scale;
			leter_y = (brick_y * AsConfig::Cell_Height + AsConfig::Level_Y_Offset) * AsConfig::Global_Scale;

         /*letter_type = (ELetter_Type)AsConfig::Rand(ELT_MAX - 1);*/
         letter_type = AFalling_Letter::Get_Random_Letter_Type();

			falling_letter = new AFalling_Letter(brick_type, letter_type, leter_x, leter_y);
			Falling_Letter[i] = falling_letter;
			++Falling_Letter_Count;
			return true;
		}
	}
	return false;
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Add_Active_Brick(int brick_x, int brick_y, EBrick_Type brick_type)
{//Создаём активный кирпич, если можем

   AActive_Brick *active_brick;

	if (Active_Bricks_Count >= AsConfig::Max_Active_Bricks_Count)
	{
		return; //Активных кирпичей слишком много
	}

   switch (brick_type)
   {
   case EBT_None:
      return;

   case EBT_Red:
   case EBT_Blue:
      active_brick = new AActive_Brick_Red_Blue(brick_type, brick_x, brick_y);
      Current_Level[brick_y][brick_x] = EBT_None;
      break;

   case EBT_Unbreakable:
      active_brick = new AActive_Brick_Unbreakable(brick_x, brick_y);
      break;

   //case EBT_Multihit_1:
   //   break;
   //case EBT_Multihit_2:
   //   break;
   //case EBT_Multihit_3:
   //   break;
   //case EBT_Multihit_4:
   //   break;
   //case EBT_Parachute:
   //   break;
   //case EBT_Teleport:
   //   break;
   //case EBT_Ad:
   //   break;
   default:
      throw 13;
   }
  
   //Добавляем новый активный кирпич на первое свободное место
   for (int i = 0; i < AsConfig::Max_Active_Bricks_Count; i++)
   {
      if(Active_Brick[i] == 0)
      {
         Active_Brick[i] = active_brick;
         ++Active_Bricks_Count;
         break;
      }
   }
}
//------------------------------------------------------------------------------------------------------------
bool AsLevel::Check_Vertical_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, ABall *ball, double &reflection_pos)
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
bool AsLevel::Check_Horizontal_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, ABall *ball, double &reflection_pos)
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
void AsLevel::Draw_Brick(HDC hdc, RECT &brick_rect, EBrick_Type brick_type)
{//Вывод кирпича

   const AColor *color = 0;

   switch (brick_type)
   {
   case EBT_None:
   {
      color = &AsConfig::BG_Color;
      break;
   }

   case EBT_Red:
   {
      color = &AsConfig::Red_Brick_Color;
      break;
   }

   case EBT_Blue:
   {
      color = &AsConfig::Blue_Brick_Color;
      break;
   }

   case EBT_Unbreakable:
      color = &AsConfig::White_Color;
      break;

   default:
      throw 13;
   }

   if(color != 0)
      color->Select(hdc);


   RoundRect(hdc, brick_rect.left, brick_rect.top, brick_rect.right - 1, brick_rect.bottom - 1, 2 * AsConfig::Global_Scale, AsConfig::Global_Scale * 2);
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Draw_Objects(HDC hdc, RECT &paint_area, AGraphics_Object ** objects_array, int objectc_max_count)
{
   for (int i = 0; i < objectc_max_count; i++)
   {
      if (objects_array[i] != 0)
      {
         objects_array[i]->Draw(hdc, paint_area);
      }
   }
}
//------------------------------------------------------------------------------------------------------------
void AsLevel::Act_Objects(AGraphics_Object ** objects_array, int objectc_max_count)
{
   for (int i = 0; i < objectc_max_count; i++)
   {
      if ( objects_array[i] != 0)
      {
         objects_array[i]->Act();

         if (objects_array[i]->Is_Finished() )
         {
            delete objects_array[i];
            objects_array[i] = 0;
            --Falling_Letter_Count;
         }
      }
   }
}
//------------------------------------------------------------------------------------------------------------

