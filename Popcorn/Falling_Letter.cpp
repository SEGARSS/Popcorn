﻿#include"Falling_Letter.h"

//AFalling_Letter
//------------------------------------------------------------------------------------------------------------
int AFalling_Letter::All_Letters_Popularity;
int AFalling_Letter::Letters_Popularity[(int)ELetter_Type::MAX] = {7, 7, 7, 7, 7, 7, 7, 3, 3, 3, 1};
//------------------------------------------------------------------------------------------------------------
AFalling_Letter::AFalling_Letter(EBrick_Type brick_type, ELetter_Type letter_type, int x, int y)
: Brick_Type(brick_type), Letter_Type(letter_type), Falling_Letter_State(EFalling_Letter_State::Normal), X(x), Y(y), Rotation_Step(2), 
  Next_Rotation_Tick(AsConfig::Current_Timer_Tick + Tick_Per_Step)
{
   Letter_Cell.left = X;
   Letter_Cell.top = Y;
   Letter_Cell.right = Letter_Cell.left + AsConfig::Brick_Width * AsConfig::Global_Scale;
   Letter_Cell.bottom = Letter_Cell.top + AsConfig::Brick_Height * AsConfig::Global_Scale;

   Prev_Letter_Cell = Letter_Cell;
}
//------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Act()
{
   if (Falling_Letter_State != EFalling_Letter_State::Normal)
   {
      return;
   }

   if (Letter_Cell.top >= AsConfig::Max_Y_Pos * AsConfig::Global_Scale)
   {
      Finalize();
      return;
   }

   Prev_Letter_Cell = Letter_Cell;

   Y += AsConfig::Global_Scale;
   Letter_Cell.top += AsConfig::Global_Scale;
   Letter_Cell.bottom += AsConfig::Global_Scale;

   if (AsConfig::Current_Timer_Tick >= Next_Rotation_Tick)
   {
      ++Rotation_Step;
      Next_Rotation_Tick += Tick_Per_Step;
   }

   AsTools::Invalidate_Rect(Prev_Letter_Cell);
   AsTools::Invalidate_Rect(Letter_Cell);
}
//------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Clear(HDC hdc, RECT &paint_area)
{
	RECT intersection_rect;

	// Стираем предыдущее изображение
	if (!IntersectRect(&intersection_rect, &paint_area, &Prev_Letter_Cell))
		return;

	AsTools::Rect(hdc, Prev_Letter_Cell, AsConfig::BG_Color);
}
//------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Draw(HDC hdc, RECT &paint_area)
{
   RECT intersectRect;

   if (Falling_Letter_State == EFalling_Letter_State::Finalizing)
   {
      Falling_Letter_State = EFalling_Letter_State::Finished;
      return;
   }

   if (IntersectRect(&intersectRect, &paint_area, &Letter_Cell) )
   {
      Draw_Brick_Letter(hdc);
   }
	
}
//------------------------------------------------------------------------------------------------------------
bool AFalling_Letter::Is_Finished()
{
   if (Falling_Letter_State == EFalling_Letter_State::Finished)
   {
      return true;
   }
   else
   {
      return false;
   }
}
//------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Get_Letter_Cell(RECT &rect)
{
   rect = Letter_Cell;
}
//------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Finalize()
{
   Falling_Letter_State = EFalling_Letter_State::Finalizing;
   AsTools::Invalidate_Rect(Prev_Letter_Cell);
   AsTools::Invalidate_Rect(Letter_Cell);
}
//------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Test_Draw_All_Steps(HDC hdc)
{
   Rotation_Step = 0;
   int x_step = AsConfig::Cell_Width * AsConfig::Global_Scale;

   for (int i = 0; i < Max_Rotation_Step; i++)
   {
      Draw_Brick_Letter(hdc);
      ++Rotation_Step;
      X += x_step;
      Letter_Cell.left += x_step;
      Letter_Cell.right += x_step;
   }
}
//------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Init()
{
   All_Letters_Popularity = 0;

   for (int i = 0; i < (int)ELetter_Type::MAX; i++)
      All_Letters_Popularity += Letters_Popularity[i];
}
//------------------------------------------------------------------------------------------------------------
ELetter_Type AFalling_Letter::Get_Random_Letter_Type()
{
   int letters_popularity;
   letters_popularity = AsTools::Rand(All_Letters_Popularity);

   for (int i = 0; i < (int)ELetter_Type::MAX; i++)
   {
      if (letters_popularity < Letters_Popularity[i])
         return (ELetter_Type)i;

      letters_popularity -= Letters_Popularity[i];
   }
   return ELetter_Type::O;
}
//------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Set_Brick_Letter_Colors(bool is_switch_color, const AColor **front_color, const AColor **back_color)
{
   if (is_switch_color)
	{
      *front_color = &AsConfig::Red_Color;
      *back_color = &AsConfig::Blue_Color;
	}
	else
	{
      *front_color = &AsConfig::Blue_Color;
      *back_color = &AsConfig::Red_Color;
	}
}
//------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Draw_Brick_Letter(HDC hdc)
{//Вывод падающией буквы

   bool switch_color;
	double offset;
	double rotation_angle;  // Преобразование шага в угол поворота
   double y_ratio;
	int back_part_offset;
   const AColor *front_color, *back_color;
	XFORM xform, old_xform;

   if (!(Brick_Type == EBrick_Type::Blue || Brick_Type == EBrick_Type::Red)) 
      return;  // Падающие буквы могут быть только от кирпичей такого типа

   // Корректируем шаг вращения и угол поворота
   Rotation_Step = Rotation_Step % Max_Rotation_Step;

   if (Rotation_Step < 8)
      rotation_angle = 2.0 * M_PI / (double)Max_Rotation_Step * (double)Rotation_Step;
   else
      rotation_angle = 2.0 * M_PI / (double)Max_Rotation_Step * (double)(8 - Rotation_Step);
   
   if (Rotation_Step > 4 && Rotation_Step <= 12)
   {
      if (Brick_Type == EBrick_Type::Blue)
         switch_color = true;
      else
         switch_color = false;
   }
   else
   {
      if (Brick_Type == EBrick_Type::Red)
         switch_color = true;
      else
         switch_color = false;
   }

   Set_Brick_Letter_Colors(switch_color, &front_color, &back_color);

   if (Rotation_Step == 4 || Rotation_Step == 12)
   {
      // Выводим фон
      back_color->Select(hdc);

      Rectangle(hdc, X, Y + Brick_Half_Height - AsConfig::Global_Scale, X + AsConfig::Brick_Width * AsConfig::Global_Scale, 
                     Y + Brick_Half_Height);

      //Выводим передний план
      front_color->Select(hdc);

      Rectangle(hdc, X, Y + Brick_Half_Height, X + AsConfig::Brick_Width * AsConfig::Global_Scale, Y + Brick_Half_Height + AsConfig::Global_Scale - 1);
   }
   else
   {
      // Настраиваем матрицу "переворота" буквы
      y_ratio = cos(rotation_angle);

      xform.eM11 = 1.0f;
      xform.eM12 = 0.0f;
      xform.eM21 = 0.0f;
      xform.eM22 = (float)fabs(y_ratio);
      xform.eDx = (float)X;
      xform.eDy = (float)Y + (float)(Brick_Half_Height);
      GetWorldTransform(hdc, &old_xform);
      SetWorldTransform(hdc, &xform);

      // Выводим фон
      back_color->Select(hdc);

      offset = 3.0 * (1.0 - fabs(xform.eM22)) * AsConfig::D_Global_Scale;
      back_part_offset = (int)round(offset);

      if (y_ratio < 0.0)
         back_part_offset = -back_part_offset;
      
      Rectangle(hdc, 0, -Brick_Half_Height - back_part_offset, AsConfig::Brick_Width * AsConfig::Global_Scale - 1, Brick_Half_Height - back_part_offset);

      //Выводим передний план
      front_color->Select(hdc);

      Rectangle(hdc, 0, -Brick_Half_Height, AsConfig::Brick_Width * AsConfig::Global_Scale - 1, Brick_Half_Height);

      if (Rotation_Step > 4 && Rotation_Step <= 12)
      {
         AsConfig::Letter_Color.Select_Pen(hdc);

         switch (Letter_Type)
         {
         case ELetter_Type::O: // Отмена
            Ellipse(hdc, 0 + 5 * AsConfig::Global_Scale, 1 * AsConfig::Global_Scale - Brick_Half_Height, 0 + 10 * AsConfig::Global_Scale, 6 * AsConfig::Global_Scale - Brick_Half_Height - 1);
            break;

         case ELetter_Type::I: // Инверсия
            Draw_Line(hdc, 5, 1, 5, 6);
            Draw_Line_To(hdc, 9, 1);
            Draw_Line_To(hdc, 9, 6);
            break;

         case ELetter_Type::C: // Скорость
            Draw_C(hdc);
            break;

         case ELetter_Type::M: // Монстры
            Draw_Line(hdc, 5, 6, 5, 1);
            Draw_Line_To(hdc, 7, 3);
            Draw_Line_To(hdc, 9, 1);
            Draw_Line_To(hdc, 9, 6);
            break;

         case ELetter_Type::G: // Жизнь
            Draw_Line(hdc, 7, 1, 7, 6);
            Draw_Line(hdc, 5, 3, 9, 3);
            Draw_Line(hdc, 4, 1, 5, 3);
            Draw_Line(hdc, 5, 3, 4, 6);
            Draw_Line(hdc, 10, 1, 9, 3);
            Draw_Line(hdc, 9, 3, 10, 6);
            break;

         case ELetter_Type::K: // Клей
            Draw_Line(hdc, 5, 1, 5, 6);
            Draw_Line(hdc, 5, 5, 9, 1);
            Draw_Line(hdc, 7, 4, 9, 6);
            break;

         case ELetter_Type::W: // Шире
            Draw_Line(hdc, 4, 1, 4, 6);
            Draw_Line_To(hdc, 10, 6);
            Draw_Line_To(hdc, 10, 1);
            Draw_Line(hdc, 7, 1, 7, 6);
            break;

         case ELetter_Type::P: // Пол
            Draw_Line(hdc, 5, 6, 5, 1);
            Draw_Line_To(hdc, 9, 1);
            Draw_Line_To(hdc, 9, 6);
            break;

         case ELetter_Type::L: // Лазер
            Draw_Line(hdc, 5, 6, 7, 1);
            Draw_Line_To(hdc, 9, 6);
            break;

         case ELetter_Type::T: // Три
            Draw_Line(hdc, 5, 1, 9, 1);
            Draw_Line(hdc, 7, 1, 7, 6);
            break;

         case ELetter_Type::Plus: // Переход на следующий уровень 
            Draw_Line(hdc, 7, 1, 7, 5);
            Draw_Line(hdc, 5, 3, 9, 3);
            break;
         }
      }
      SetWorldTransform(hdc, &old_xform);
   }
}
//------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Draw_C(HDC hdc)
{
   int arc_x = 5 * AsConfig::Global_Scale;
   int arc_y = 1 * AsConfig::Global_Scale - Brick_Half_Height;
   int arc_size = 5 * AsConfig::Global_Scale - 1;

   Arc(hdc, arc_x, arc_y, arc_x + arc_size, arc_y + arc_size, arc_x + arc_size, arc_y, arc_x + arc_size, arc_y + arc_size);
}
//------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Draw_Line(HDC hdc, int x_1, int y_1, int x_2, int y_2)
{
   int start_y, end_y;

   start_y = y_1 * AsConfig::Global_Scale - Brick_Half_Height;

   if (y_1 == 6)
      --start_y;  

   end_y = y_2 * AsConfig::Global_Scale - Brick_Half_Height;

   if (y_2 == 6)
      --end_y;

	MoveToEx(hdc, x_1 * AsConfig::Global_Scale + 1, start_y, 0);
	LineTo(hdc, x_2 * AsConfig::Global_Scale + 1, end_y);
}
//------------------------------------------------------------------------------------------------------------
void AFalling_Letter::Draw_Line_To(HDC hdc, int x, int y)
{
   int end_y = y * AsConfig::Global_Scale - Brick_Half_Height;

   if (y == 6)
      --end_y;
   LineTo(hdc, x * AsConfig::Global_Scale + 1, end_y);
}
//------------------------------------------------------------------------------------------------------------

