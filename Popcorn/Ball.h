#pragma once
#include"Config.h"


//Iterfase
//------------------------------------------------------------------------------------------------------------
class ABall;
class AHit_Checker
{
public:
   virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall *ball) = 0;
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
   void Move();
   void Set_For_test();
   EBall_State Get_State();
   void Set_State(EBall_State new_state, double x_pos);
   double Get_Direction();
   void Set_Direction(double new_direction);
   void Reflect(bool from_horizontal);

   static void Add_Hit_Checkers (AHit_Checker *hit_checker);

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
   double Ball_Direction;

   RECT Ball_Rect, Prev_Ball_Rect;

   static const double Start_Ball_Y_Pos;
   static int Hit_Checkers_Count;
   static AHit_Checker *Hit_Checkers[3];
};
//------------------------------------------------------------------------------------------------------------
