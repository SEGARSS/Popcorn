#pragma once
#include"Config.h"
#include "Level.h"


//Iterfase
//------------------------------------------------------------------------------------------------------------
class ABall;
class AHit_Checker
{
public:
   virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall *bal) = 0;
private:
};
//------------------------------------------------------------------------------------------------------------




//ABall
//------------------------------------------------------------------------------------------------------------
enum EBall_State
{
   EBS_Normal,
   EBS_Lost,
   EBS_On_Platform,
};
//------------------------------------------------------------------------------------------------------------
class ABall
{
public:
   ABall();

   void Init();

   void Draw(HDC hdc, RECT &paint_area);
   void Move(int platform_x_pos, int platform_width, ALevel *level, AHit_Checker *hit_checker);
   EBall_State Get_State();
   void Set_State(EBall_State new_state, double x_pos);

   double Ball_Direction;

   static const double Radius;

private:
   void Redraw_Ball();

   EBall_State Ball_State;
   HPEN Ball_Pen;
   HBRUSH Ball_Brush;

   double Ball_Speed;
   double Center_X_Pos;
   double Center_Y_Pos;
   double Rest_Distance;

   RECT Ball_Rect, Prev_Ball_Rect;

   static const double Start_Ball_Y_Pos;
};
//------------------------------------------------------------------------------------------------------------
