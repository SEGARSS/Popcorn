#pragma once

#include"Config.h"


//------------------------------------------------------------------------------------------------------------
enum EBrick_Type
{
   EBT_None,
   EBT_Red,
   EBT_Blue,
   EBT_Unbreakable,
   EBT_Multihit_1,
   EBT_Multihit_2,
   EBT_Multihit_3,
   EBT_Multihit_4,
   EBT_Parachute,
   EBT_Teleport,
   EBT_Ad
};
//------------------------------------------------------------------------------------------------------------




//AGraphics_Object
//------------------------------------------------------------------------------------------------------------
class AGraphics_Object
{
public:
   virtual ~AGraphics_Object();

   virtual void Act() = 0;
   virtual void Draw(HDC hdc, RECT &paint_area) = 0;
   virtual bool Is_Finished() = 0;
};
//------------------------------------------------------------------------------------------------------------



//AActive_Brick
//------------------------------------------------------------------------------------------------------------
class AActive_Brick: public AGraphics_Object
{
protected:
   virtual ~AActive_Brick();
   AActive_Brick(EBrick_Type brick_type, int level_x, int level_y);

   EBrick_Type Brick_Type;
   RECT Brick_Rect;
};
//------------------------------------------------------------------------------------------------------------




//AActive_Brick_Red_Blue
//------------------------------------------------------------------------------------------------------------
class AActive_Brick_Red_Blue: public AActive_Brick
{
public:
   ~AActive_Brick_Red_Blue();
   AActive_Brick_Red_Blue(EBrick_Type brick_type, int level_x, int level_y);

   virtual void Act();
   virtual void Draw(HDC hdc, RECT &paint_area);
   virtual bool Is_Finished();

   static void Setup_Color();

private:
   int Fade_Step;

   static unsigned char Get_Fading_Channel(unsigned char color, unsigned char bg_color, int step);
   static void Get_Fading_Color(const AColor &origin_color, int step, AColor &result_color);

   static const int Max_Fade_Step = AsConfig::FPS;

   static AColor Fading_Red_Colors[Max_Fade_Step];
   static AColor Fading_Blue_Colors[Max_Fade_Step];

};
//------------------------------------------------------------------------------------------------------------




//AActive_Brick_Unbreakable
//------------------------------------------------------------------------------------------------------------
class AActive_Brick_Unbreakable: public AActive_Brick
{
public:
   ~AActive_Brick_Unbreakable();
   AActive_Brick_Unbreakable(int level_x, int level_y);

   virtual void Act();
   virtual void Draw(HDC hdc, RECT &paint_area);
   virtual bool Is_Finished();

private:
   int Unreakable_Animation_step;

   static const int Max_Unreakable_Animation_step = 5;
};
//------------------------------------------------------------------------------------------------------------

