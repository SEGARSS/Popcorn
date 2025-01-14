﻿#include "Border.h"

//AGate
//------------------------------------------------------------------------------------------------------------
AGate::AGate(int x_pos, int y_pos)
: X_Pos(x_pos), Y_Pos(y_pos)
{
}
//------------------------------------------------------------------------------------------------------------
void AGate::Act()
{
	//!!! Надо сделать!
}
//------------------------------------------------------------------------------------------------------------
void AGate::Clear(HDC hdc, RECT& paint_area)
{
	//!!! Надо сделать!
}
//------------------------------------------------------------------------------------------------------------
void AGate::Draw(HDC hdc, RECT &paint_area)
{
	//Draw_Cup(hdc, true);
	Draw_Cup(hdc, false);
}
//------------------------------------------------------------------------------------------------------------
bool AGate::Is_Finished()
{
	return false;//!!! Надо сделать!
}
//------------------------------------------------------------------------------------------------------------
void AGate::Draw_Cup(HDC hdc, bool top_cup)
{
	int x = 0, y = 0;
	const int scale = AsConfig::Global_Scale;
	const int half_scale = scale / 2;
	HRGN region;
	RECT rect;
	XFORM xform, old_xform;

	xform.eM11 = 1.0f;
	xform.eM12 = 0.0f;
	xform.eM21 = 0.0f;
	xform.eDx = (float)(X_Pos * scale);
	xform.eDy = (float)(Y_Pos * scale);

	if (top_cup)
		xform.eM22 = 1.0f;
	else
		xform.eM22 = -1.0f;

	GetWorldTransform(hdc, &old_xform);
	SetWorldTransform(hdc, &xform);

	//1. Полукруглая часть чаши
	rect.left = x * scale;
	rect.top = (y + 1) * scale;
	rect.right = rect.left + 6 * scale;
	rect.bottom = rect.top + 4 * scale;

	//1.1 основа
	AsConfig::Blue_Color.Select(hdc);
	AsConfig::Round_Rect(hdc, rect, 3);

	//1.2 блик слева
	rect.left = X_Pos * scale;
	rect.right = rect.left + 3 * scale;	

	if (top_cup)
	{
		rect.top = (Y_Pos + 1) * scale;
		rect.bottom = rect.top + 4 * scale;
	}
	else
	{
		rect.top = (Y_Pos - 1) * scale;
		rect.bottom = rect.top - 4 * scale;
	}

	region = CreateRectRgnIndirect(&rect);
	SelectClipRgn(hdc, region);

	AsConfig::Letter_Color.Select(hdc);

	rect.left = x * scale + half_scale;
	rect.top = (y + 1) * scale + half_scale;
	rect.right = rect.left + 5 * scale + half_scale;
	rect.bottom = rect.top + 5 * scale + half_scale;
	
	AsConfig::Round_Rect(hdc, rect, 3);

	SelectClipRgn(hdc, 0);
	DeleteObject(region);
	
	AsConfig::Rect(hdc, x, y + 4, 4, 1, AsConfig::White_Color); //Блик снизу	
	AsConfig::Rect(hdc, x + 4, y + 3, 2, 2, AsConfig::Blue_Color); //Заплатка в правом нижнем углу	
	AsConfig::Rect(hdc, x + 4, y + 3, 1, 1, AsConfig::BG_Color);//Фоновая перфорация
	AsConfig::Rect(hdc, x + 2, y, 2, 1, AsConfig::Blue_Color);//Перемычка перед чашей

	SetWorldTransform(hdc, &old_xform);
}
//------------------------------------------------------------------------------------------------------------




//AsBorder
//------------------------------------------------------------------------------------------------------------
AsBorder::AsBorder()
: Gate(AsConfig::Max_X_Pos, 178)
{
	Floor_Rect.left = AsConfig::Level_X_Offset * AsConfig::Global_Scale;
	Floor_Rect.top = AsConfig::Floor_Y_Pos * AsConfig::Global_Scale;
	Floor_Rect.right = (AsConfig::Max_X_Pos - 1) * AsConfig::Global_Scale;
	Floor_Rect.bottom = AsConfig::Max_Y_Pos * AsConfig::Global_Scale;
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Redraw_Floor()
{
	AsConfig::Invalidate_Rect(Floor_Rect);
}
//------------------------------------------------------------------------------------------------------------
bool AsBorder::Check_Hit(double next_x_pos, double next_y_pos, ABall *ball)
{// Корректируем позицию при отражении от рамки

	bool got_hit = false;
	
	// 1. Левый край
	if (next_x_pos - ball->Radius < AsConfig::Border_X_Offset)
	{
		got_hit = true;
		ball->Reflect(false);
	}

	// 2. Верхний край
	if (next_y_pos - ball->Radius < AsConfig::Border_Y_Offset)
	{
		got_hit = true;
		ball->Reflect(true);
	}

	// 3. Правй край
	if (next_x_pos + ball->Radius > AsConfig::Max_X_Pos + 1)
	{
		got_hit = true;
		ball->Reflect(false);
	}

	// 3. Нижний край
	if (AsConfig::Level_Has_Floor && next_y_pos + ball->Radius > AsConfig::Floor_Y_Pos)
	{
		got_hit = true;
		ball->Reflect(true);
	}
	
	// Чтобы шарик смог улететь ниже пола, проверяем его max_y_pos ниже видимой границы
	if (next_y_pos + ball->Radius > (double)AsConfig::Max_Y_Pos + ball->Radius * 4.0)
		ball->Set_State(EBS_Lost);

	return got_hit;
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Act()
{
	// Заглушка, т.к. метод не используеться.
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Clear(HDC hdc, RECT &paint_area)
{
	RECT intersection_rect;

	if (! AsConfig::Level_Has_Floor)
		return;

	if (! IntersectRect(&intersection_rect, &paint_area, &Floor_Rect) )
		return;

	AsConfig::BG_Color.Select(hdc);

	Rectangle(hdc, Floor_Rect.left, Floor_Rect.top, Floor_Rect.right - 1, Floor_Rect.bottom - 1);
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Draw(HDC hdc, RECT &paint_area)//Рисуем полную рамку
{
   //1.Линия слева
   for (int i = 0; i < 50; i++)
		Draw_Element(hdc, paint_area, 2, 1 + i * 4, false);

   //2.Линия справа
  // for (int i = 0; i < 50; i++)
		//Draw_Element(hdc, paint_area, AsConfig::Max_X_Pos + 1, 1 + i * 4, false);

   //3.Линия сверху
   for (int i = 0; i < 50; i++)
		Draw_Element(hdc, paint_area, 3 + i * 4, 0, true);

	//4.По (если есть)
	if (AsConfig::Level_Has_Floor)
		Draw_Floor(hdc, paint_area);

	//5.Гейты
	Gate.Draw(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------
bool AsBorder::Is_Finished()
{
	return false; // Заглушка. Не используеться, т.к. мячик сам ничего не делает (не анимируеться)
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Draw_Element(HDC hdc, RECT &paint_area, int x, int y, bool top_border)// Рисует элемент рамки уровня
{
	RECT intersection_rect, rect;

	rect.left = x * AsConfig::Global_Scale;
	rect.top = y * AsConfig::Global_Scale;
	rect.right = (x + 4) * AsConfig::Global_Scale;
	rect.bottom = (y + 4) * AsConfig::Global_Scale;

	if (! IntersectRect(&intersection_rect, &paint_area, &rect) )
		return;

	// Основная линия
	AsConfig::Blue_Color.Select(hdc);

	if (top_border)
		Rectangle(hdc, x * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale, (x + 4) * AsConfig::Global_Scale - 1, (y + 4) * AsConfig::Global_Scale - 1);
	else
		Rectangle(hdc, (x + 1) * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + 4) * AsConfig::Global_Scale - 1, (y + 4) * AsConfig::Global_Scale - 1);

	// Белая кайма
	AsConfig::White_Color.Select(hdc);

	if (top_border)
		Rectangle(hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + 4) * AsConfig::Global_Scale - 1, (y + 1) * AsConfig::Global_Scale - 1);
	else
		Rectangle(hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + 1) * AsConfig::Global_Scale - 1, (y + 4) * AsConfig::Global_Scale - 1);

	// Перфорация
	AsConfig::BG_Color.Select(hdc);

	if (top_border)
		Rectangle(hdc, (x + 2) * AsConfig::Global_Scale, (y + 2) * AsConfig::Global_Scale, (x + 3) * AsConfig::Global_Scale - 1, (y + 3) * AsConfig::Global_Scale - 1);
	else
		Rectangle(hdc, (x + 2) * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale, (x + 3) * AsConfig::Global_Scale - 1, (y + 2) * AsConfig::Global_Scale - 1);
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Draw_Floor(HDC hdc, RECT &paint_area)
{
	int strokes_count;
	int x_pos, y_pos;
	int line_len = 4 * AsConfig::Global_Scale;
	int gap_len = 2 * AsConfig::Global_Scale;
	int stroke_len = line_len + gap_len;
	RECT intersection_rect;

	if (! IntersectRect(&intersection_rect, &paint_area, &Floor_Rect) )
		return;

	strokes_count = (Floor_Rect.right - Floor_Rect.left + AsConfig::Global_Scale) / stroke_len;
	x_pos = Floor_Rect.left + 1;
	y_pos = Floor_Rect.top + (Floor_Rect.bottom - Floor_Rect.top) / 2;

	AsConfig::Letter_Color.Select(hdc);

	for (int i = 0; i < strokes_count; i++)
	{
		MoveToEx(hdc, x_pos, y_pos, 0);
		LineTo(hdc, x_pos + line_len, y_pos);

		x_pos += stroke_len;
	}
}
//------------------------------------------------------------------------------------------------------------