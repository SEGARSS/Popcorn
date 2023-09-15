#include "Engin.h"

//AsEngine
//------------------------------------------------------------------------------------------------------------
AsEngine::AsEngine()
: Game_State (EGS_Play_Level) 
{
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Init_Engine(HWND hwnd)//Настройка игры при старте
{
   AsConfig::Hwnd = hwnd;   

   AActive_Brick::Setup_Color();

   Level.Init();
   Platform.Init();
   Ball.Init(Platform.X_Pos + Platform.Width / 2);
   Border.Init();

   Platform.Set_State(EPS_Normal);

   Platform.Redraw_Platform();

   SetTimer(AsConfig::Hwnd, Timer_ID, 1000 / AsConfig::FPS, 0);

}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Frame(HDC hdc, RECT &paint_area)//отрисовка экрана игры
{
   Level.Draw(hdc, paint_area);   

   /*for (int i = 0; i < 16; i++)
   {
      Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale, 100, EBT_Blue, ELT_O, i);
      Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale, 130, EBT_Red, ELT_O, i);
   }*/

   Ball.Draw(hdc, paint_area);

   Border.Draw(hdc, paint_area);

   Platform.Draw(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------
int AsEngine::On_Key_Down(EKey_Type key_type)
{
   switch (key_type)
   {
   case EKT_Left:
      Platform.X_Pos -= Platform.X_Step;

      if (Platform.X_Pos <= AsConfig::Border_X_offset)
      {
         Platform.X_Pos = AsConfig::Border_X_offset;
      }

      Platform.Redraw_Platform();
      break;

   case EKT_Right:
      Platform.X_Pos += Platform.X_Step;

      if (Platform.X_Pos >= AsConfig::Max_X_Pos - Platform.Width + 1)
      {
         Platform.X_Pos = AsConfig::Max_X_Pos - Platform.Width + 1;
      }
      Platform.Redraw_Platform();
      break;

   case EKT_Space:
      break;
   }
   return 0;
}
//------------------------------------------------------------------------------------------------------------
int AsEngine::On_Timer() // Смещение по таймеру
{
   ++AsConfig::Current_Timer_Tick;

   switch (Game_State)
   {
   case EGS_Play_Level:
      Ball.Move(&Level, Platform.X_Pos, Platform.Width);

      if (Ball.Ball_State == EBS_Lost)
      {
         Game_State = EGS_Lost_Ball;
         Platform.Set_State(EPS_Meltdown); 
      }
      break;


   case EGS_Lost_Ball:
      break;


   case EGS_Restart_Level:
      break;
   }

   Platform.Act();
  /* Level.Active_Brick.Act();*/

   /*if (++AsConfig::Current_Timer_Tick % 3 == 0)*/
      
       
   
   
   return 0;
}
//------------------------------------------------------------------------------------------------------------



