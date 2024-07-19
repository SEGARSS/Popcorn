#pragma once

#include"Config.h"


//------------------------------------------------------------------------------------------------------------
enum class ELaser_Beam_State : unsigned char
{
	Disabled,
	Active,
	Stopping,
	Cleanup
};
//------------------------------------------------------------------------------------------------------------


//ALaser_Beam
//------------------------------------------------------------------------------------------------------------
class ALaser_Beam : public AMover, public AGraphics_Object
{
public:
	ALaser_Beam();

	virtual void Begin_Movement();
	virtual void Finish_Movement();
	virtual void Advance(double max_speed);
	virtual double Get_Speed();

	virtual void Act();
	virtual void Clear(HDC hdc, RECT& paint_area);
	virtual void Draw(HDC hdc, RECT& paint_area);
	virtual bool Is_Finished();

	void Set_At(double x_pos, double y_pos);
	bool Is_Active();

	static void Add_Hit_Checker(AHit_Checker* hit_checker);

private:
	virtual void Disable();
	void Redraw_Beam();

	ELaser_Beam_State Laser_Beam_State;
	double X_Pos, Y_Pos;
	double Speed;
	RECT Beam_Rect, Prev_Beam_Rect;

	static const int Width = 1;
	static const int Height = 3;
	static int Hit_Checkers_Count;
	static AHit_Checker* Hit_Checkers[3];
};
//------------------------------------------------------------------------------------------------------------
