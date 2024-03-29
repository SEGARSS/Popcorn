#pragma once

#include"Falling_Letter.h"
#include "Ball_Set.h"


//AsPlatform
//------------------------------------------------------------------------------------------------------------
enum EPlatform_State
{
   EPS_Regular,
   EPS_Meltdown,
   EPS_Rolling,   
   EPS_Glue
};
//------------------------------------------------------------------------------------------------------------
enum class EPlatform_Substate_Regular: unsigned char
{
   Unknown,

   Missing,
   Ready,
   Normal
};
//------------------------------------------------------------------------------------------------------------
enum EPlatform_Substate_Meltdown
{
   EPSM_Unknown,
      
   EPSM_Init,
   EPSM_Active
};
//------------------------------------------------------------------------------------------------------------
enum EPlatform_Substate_Rolling
{
   EPSR_Unknown,

   EPSR_Roll_In,
   EPSR_Expand_Roll_In
};
//------------------------------------------------------------------------------------------------------------
enum EPlatform_Substate_Glue
{
   EPSG_Unknown,

   EPSG_Init,
   EPSG_Active,
   EPSG_Finalize
};
//------------------------------------------------------------------------------------------------------------
enum EPlatform_Moving_State
{
   EPMS_Stopping,
   EPMS_Stop,
   EPMS_Moving_Left,
   EPMS_Moving_Right
};
//------------------------------------------------------------------------------------------------------------
class AsPlatform: public AHit_Checker, public AMover, public AGraphics_Object
{
public:
   ~AsPlatform();
   AsPlatform();

   virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall *ball);

   virtual void Begin_Movement();
   virtual void Finish_Movement();
   virtual void Advance(double max_speed);
   virtual double Get_Speed();

   virtual void Act();
	virtual void Clear(HDC hdc, RECT &paint_area);
	virtual void Draw(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

   void Init(AsBall_Set *ball_set);
   EPlatform_State Get_State();
   void Set_State(EPlatform_State new_state);
   void Set_State(EPlatform_Substate_Regular new_regular_state);
   bool Has_State(EPlatform_Substate_Regular regular_state);
   void Redraw_Platform(bool update_rect = true);
   void Move(bool to_left, bool key_down);
   void On_Space_Key(bool key_down);
   bool Hit_By(AFalling_Letter *falling_letter);
   double Get_Middle_Pos();

   int Width;
   double X_Pos;

private:
   void Act_For_Meltdown_State();
   void Act_For_Rolling_State();
   void Act_For_Glue_State();
   void Draw_Circle_Highlight(HDC hdc, int x, int y);
   void Draw_Normal_State(HDC hdc, RECT &paint_area);
   void Draw_Meltdown_State(HDC hdc, RECT &paint_area);
   void Draw_Rolling_State(HDC hdc, RECT &paint_area);
   void Draw_Roll_In_State(HDC hdc, RECT &paint_area);
   void Draw_Glue_State(HDC hdc, RECT &paint_area);
   void Draw_Glue_Spot(HDC hdc, int x_offset, int width, int height);
   bool Reflect_On_Circle(double next_x_pos, double next_y_pos, double platform_ball_x_offset, ABall* ball);
   bool Get_Platform_Image_Stroke_Color(int x, int y, const AColor **color, int &stroke_len);
   void Get_Normal_Platform_Imege(HDC hdc);

   EPlatform_State Platform_State;
   EPlatform_Substate_Regular Platform_Substate_Regular;
   EPlatform_Substate_Meltdown Platform_Substate_Meltdown;
   EPlatform_Substate_Rolling Platform_Substate_Rolling;
   EPlatform_Substate_Glue Platform_Substate_Glue;
   EPlatform_Moving_State Platform_Moving_State;
   bool Left_Key_Down, Right_Key_Down;
   int Inner_Width;
   int Rolling_Step;
   double Speed;
   double Glue_Spot_Height_Ratio;
   AsBall_Set *Ball_Set;


   int Normal_Platform_Imege_Width;
   int Normal_Platform_Imege_Height;
   int *Normal_Platform_Imege; // Пиксели изображения платформы на фоне

   static const int Normal_Width = 28;

   int Meltdown_Platform_Y_Pos[Normal_Width * AsConfig::Global_Scale];

   RECT Platform_Rect, Prev_Platform_Rect;

   AColor Highlight_Color, Platform_Cercle_Color, Platform_Inner_Color;

   static const double Max_Glue_Spot_Height_Ratio, Min_Glue_Spot_Height_Ratio, Glue_Spot_Height_Ratio_Step;
   static const int Height = 7;
   static const int Circle_Size = 7;
   static const int Normal_Platform_Inner_Width = Normal_Width - Circle_Size;
   static const int Meltdown_Speed = 3;
   static const int Max_Rolling_Step = 16;
   static const int Roll_In_Platform_End_X_Pos = 99;
   static const int Rolling_Platform_Speed = 3;
   static const int X_Step = 6;
};
//------------------------------------------------------------------------------------------------------------