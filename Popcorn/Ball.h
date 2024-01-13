﻿#pragma once
#include"Config.h"


//------------------------------------------------------------------------------------------------------------
enum EBall_State
{
	EBS_Disabled, // Отключён (не рисуеться, не перемещаеться и не взаимодействует)

   EBS_Normal,
	EBS_Lost,
	EBS_On_Platform,
	EBS_On_Parachute,
	EBS_Off_Parachute,
	EBS_Teleporting
};
//------------------------------------------------------------------------------------------------------------




//Iterfase
//------------------------------------------------------------------------------------------------------------
class ABall;
class AHit_Checker
{
public:
   virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall *ball) = 0;
   bool Hit_Circle_On_Line(double y, double next_x_pos, double left_x, double right_x, double radius, double &x);
private:
};
//------------------------------------------------------------------------------------------------------------




//AMover
//------------------------------------------------------------------------------------------------------------
class AMover
{
public:
   virtual ~AMover();

   virtual void Begin_Movement() = 0;
   virtual void Finish_Movement() = 0;
   virtual void Advance(double max_speed) = 0;
   virtual double Get_Speed() = 0;
};
//------------------------------------------------------------------------------------------------------------




//ABall
//------------------------------------------------------------------------------------------------------------
class ABall: public AMover
{
public:
	ABall();

	virtual void Begin_Movement();
   virtual void Finish_Movement();
	virtual void Advance(double max_speed);
	virtual double Get_Speed();

	void Draw(HDC hdc, RECT &paint_area);
	void Draw_Teleporting(HDC hdc, int step);
	void Set_For_Test();
	bool Is_Test_Finished();
	EBall_State Get_State();
	void Set_State(EBall_State new_state, double x_pos = 0, double y_pos = 0);
	void Get_Center(double &x_pos, double &y_pos);
	double Get_Direction();
	void Set_Direction(double new_direction);
	void Reflect(bool from_horizontal);
	bool Is_Moving_Up();
	bool Is_Moving_Left();
	void Set_On_Parachute(int brick_x, int brick_y);

	static void Add_Hit_Checker(AHit_Checker *hit_checker);

	static const double Radius;

private:
	void Redraw_Ball();
	void Redraw_Parachute();
	void Draw_Parachute(HDC hdc, RECT &paint_area);
	void Clear_Parachute(HDC hdc);

	EBall_State Ball_State, Prev_Ball_State;
	//double Rest_Distance;
	double Ball_Direction;
	double Ball_Speed;

	bool Testing_Is_Active;
	int Test_Iteration;
	double Rest_Test_Distance;

	double Center_X_Pos, Center_Y_Pos;

	RECT Ball_Rect, Prev_Ball_Rect;
	RECT Parachute_Rect, Prev_Parachute_Rect;

	static const int Parachute_Size = 15;
	static int Hit_Checkers_Count;
	static AHit_Checker *Hit_Checkers[3];
};
//------------------------------------------------------------------------------------------------------------
//Отлажен
