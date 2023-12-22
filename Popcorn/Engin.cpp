#include "Engin.h"

//AsEngine
//------------------------------------------------------------------------------------------------------------
AsEngine::AsEngine()
: Game_State (EGS_Lost_Ball) 
{
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Init_Engine(HWND hwnd)//Настройка игры при старте
{
   SYSTEMTIME sys_time;
   FILETIME file_time;

   GetSystemTime(&sys_time);
   SystemTimeToFileTime(&sys_time, &file_time);

   srand(file_time.dwLowDateTime);

   AsConfig::Hwnd = hwnd;   

   AActive_Brick_Red_Blue::Setup_Colors();

   Level.Init();

   AFalling_Letter::Init();

   ABall::Add_Hit_Checker(&Border);
   ABall::Add_Hit_Checker(&Level);
   ABall::Add_Hit_Checker(&Platform);
;
   Level.Set_Current_Level(AsLevel::Level_01);

   //Ball.Set_State(EBS_Normal, Platform.X_Pos + Platform.Width / 2);
   //Platform.Set_State(EPS_Normal);

   Platform.Redraw_Platform();

   SetTimer(AsConfig::Hwnd, Timer_ID, 1000 / AsConfig::FPS, 0);

}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Frame(HDC hdc, RECT &paint_area)
{//отрисовка экрана игры
   SetGraphicsMode(hdc, GM_ADVANCED);

   Level.Draw(hdc, paint_area);
   Border.Draw(hdc, paint_area);
   Platform.Draw(hdc, paint_area);

   for (int i = 0; i < AsConfig::Max_Balls_Count; i++)
      Balls[i].Draw(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------
int AsEngine::On_Key_Down(EKey_Type key_type)
{
   switch (key_type)
	{
	case EKT_Left:
      Platform.Move(true);
		break;


	case EKT_Right:
      Platform.Move(false);
		break;


	case EKT_Space:
		if (Platform.Get_State() == EPS_Ready)
		{
         for (int i = 0; i < AsConfig::Max_Balls_Count; i++)
            if (Balls[i].Get_State() == EBS_On_Platform)
			      Balls[i].Set_State(EBS_Normal, Platform.X_Pos + Platform.Width / 2, AsConfig::Start_Ball_Y_Pos);

			Platform.Set_State(EPS_Normal);
		}
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
   case EGS_Test_Ball:
      Balls[0].Set_For_Test(); // В повторяющихся тестах участвует только 0-й мячик
      Game_State = EGS_Play_Level;
      break;


   case EGS_Play_Level:
      Play_Level(); 
      break;


   case EGS_Lost_Ball:
      if (Platform.Get_State() == EPS_Missing)
      {
         Game_State = EGS_Restart_Level;
         Platform.Set_State(EPS_Roll_In); 
      }
      break;


   case EGS_Restart_Level:
      if (Platform.Get_State() == EPS_Ready)
         Restart_Level();
      break;
   }
   Act();

   return 0;
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Restart_Level()
{
   int i;

	Game_State = EGS_Play_Level;

	for (i = 0; i < 3; i++)
		Balls[i].Set_State(EBS_On_Platform, Platform.X_Pos + Platform.Width / 2, AsConfig::Start_Ball_Y_Pos);

	for (; i < AsConfig::Max_Balls_Count; i++)
		Balls[i].Set_State(EBS_Disabled);
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Play_Level()
{
   int active_balls_count = 0;
   int lost_balls_count = 0;

	for (int i = 0; i < AsConfig::Max_Balls_Count; i++)
	{
      if(Balls[i].Get_State() == EBS_Disabled)
         continue;

      ++active_balls_count;

		if (Balls[i].Get_State() == EBS_Lost)
		{
         ++lost_balls_count;
         continue;
		}

      Balls[i].Move();
	}

   if (active_balls_count == lost_balls_count)
	{// Потеряли все мячики

		Game_State = EGS_Lost_Ball;
      Level.Stop();
		Platform.Set_State(EPS_Meltdown);
	}

   if (active_balls_count == 1)
	   if (Balls[0].Is_Test_Finished()) // В повторяющихся тестах участвует только 0-й мячик
		   Game_State = EGS_Test_Ball;
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Act()
{
   int index = 0;
   AFalling_Letter *falling_letter;

   Platform.Act();
   Level.Act();

   while (Level.Get_Next_Falling_Letter(index, &falling_letter) )
   {
      if (Platform.Hit_By(falling_letter) )
      {
         On_Falling_Letter(falling_letter);
      }
   }
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::On_Falling_Letter(AFalling_Letter *falling_letter)
{
   switch (falling_letter->Letter_Type)
   {
   //case ELT_O:
   //case ELT_I:
   //case ELT_C:
   //case ELT_M:
   //case ELT_G:
   //case ELT_K:
   //case ELT_W:


   case ELT_T:
      break;


   //case ELT_L:
   //case ELT_P:
   //case ELT_Plus:
   //case ELT_MAX:
   default:
      AsConfig::Throw();
      break;
   }

   falling_letter->Finalize();
}
//------------------------------------------------------------------------------------------------------------




