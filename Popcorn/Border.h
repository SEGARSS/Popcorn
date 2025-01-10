#pragma once

#include "Ball.h"

//AGate
//------------------------------------------------------------------------------------------------------------
class AGate: public AGraphics_Object
{
public:
	AGate(int x_pos, int y_pos);

	virtual void Act();
	virtual void Clear(HDC hdc, RECT& paint_area);
	virtual void Draw(HDC hdc, RECT& paint_area);
	virtual bool Is_Finished();

private:
	int X_Pos, Y_Pos;
};
//------------------------------------------------------------------------------------------------------------


//AsBorder
//------------------------------------------------------------------------------------------------------------
class AsBorder: public AHit_Checker, public AGraphics_Object
{
public:
	AsBorder();

	virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall *ball);

	virtual void Act();
	virtual void Clear(HDC hdc, RECT &paint_area);
	virtual void Draw(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

	void Redraw_Floor();

private:
	void Draw_Element(HDC hdc, RECT &paint_area, int x, int y, bool top_border);
	void Draw_Floor(HDC hdc, RECT &paint_area);

	RECT Floor_Rect;
	AGate Gate;
};
//------------------------------------------------------------------------------------------------------------
//Отлажен