﻿#include "Engin.h"

//AsEngine
//------------------------------------------------------------------------------------------------------------
AsEngine::AsEngine()
: Game_State (EGame_State::Lost_Ball), Rest_Distance(0.0), Life_Count(AsConfig::Initial_Life_Count), Movers {}, Modules {}
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
   Platform.Init(&Ball_Set, &Laser_Beam_Set);

   AFalling_Letter::Init();

   ABall::Hit_Checker_List.Add_Hit_Checker(&Border);
   ABall::Hit_Checker_List.Add_Hit_Checker(&Level);
   ABall::Hit_Checker_List.Add_Hit_Checker(&Platform);

   ALaser_Beam::Hit_Checker_List.Add_Hit_Checker(&Level);

   Level.Set_Current_Level(AsLevel::Level_01);

   //Ball.Set_State(EBall_State::Normal, Platform.X_Pos + Platform.Width / 2);
   //Platform.Set_State(EPS_Normal);
   //Platform.Set_State(EPlatform_State::Laser);

   Platform.Redraw_Platform();

   SetTimer(AsConfig::Hwnd, Timer_ID, 1000 / AsConfig::FPS, 0);

   // Movers
   memset(Movers, 0, sizeof(Movers) );
   Movers[0] = &Platform;
   Movers[1] = &Ball_Set;
   Movers[2] = &Laser_Beam_Set;

   // Modules
   memset(Modules, 0, sizeof(Modules) );
   Modules[0] = &Level;
   Modules[1] = &Border;
   Modules[2] = &Platform;
   Modules[3] = &Ball_Set;
   Modules[4] = &Laser_Beam_Set;
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Frame(HDC hdc, RECT &paint_area)
{// Отрисовка экрана игры

	SetGraphicsMode(hdc, GM_ADVANCED);

	for (int i = 0; i < AsConfig::Max_Modules_Count; i++)
		if (Modules[i] != 0)
			Modules[i]->Clear(hdc, paint_area);

	for (int i = 0; i < AsConfig::Max_Modules_Count; i++)
		if (Modules[i] != 0)
			Modules[i]->Draw(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------
int AsEngine::On_Key(EKey_Type key_type, bool key_down)
{
   switch (key_type)
	{
   case EKey_Type::Left:
      Platform.Move(true, key_down);
		break;


	case EKey_Type::Right:
      Platform.Move(false, key_down);
		break;


	case EKey_Type::Space:
      Platform.On_Space_Key(key_down);
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
   case EGame_State::Test_Ball:
      Ball_Set.Set_For_Test();
      Game_State = EGame_State::Play_Level;
      break;


   case EGame_State::Play_Level:
      Play_Level(); 
      break;


   case EGame_State::Lost_Ball:
       if (Platform.Has_State(EPlatform_Substate_Regular::Missing))
           Restart_Level();
      break;


   case EGame_State::Restart_Level:
      if (Platform.Has_State(EPlatform_Substate_Regular::Ready) )
      {
         Game_State = EGame_State::Play_Level;
         Ball_Set.Set_On_Platform(Platform.Get_Middle_Pos() );
         //Platform.Set_State(EPS_Glue_Init);
      }
      break;
   }
   Act();

   return 0;
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Restart_Level()
{
    Game_State = EGame_State::Restart_Level;
    Border.Open_Gate(7, true);
    Border.Open_Gate(5, false);
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Play_Level()
{
   Advance_Movers();

	if (Ball_Set.All_Balls_AreLost() )
	{
		Game_State = EGame_State::Lost_Ball;
		Level.Stop();
		Platform.Set_State(EPlatform_State::Meltdown);
	}
   else
      Ball_Set.Accelerate();
   

   if (Ball_Set.Is_Test_Finished() ) 
	   Game_State = EGame_State::Test_Ball;
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Advance_Movers()
{
   double curr_speed;
   double max_speed = 0.0;   

   //1. Получаем максимальную скорость.
	for (int i = 0; i < AsConfig::Max_Movers_Count; i++)
    {
      if (Movers[i] != 0)
      {
         Movers[i]->Begin_Movement();

         curr_speed = fabs(Movers[i]->Get_Speed() );

			if (curr_speed > max_speed)
            max_speed = curr_speed;
      }
    }

   //2. Смещаем все движущиеся объекты.
   Rest_Distance += max_speed;

   while (Rest_Distance > 0.0)
   {
      for (int i = 0; i < AsConfig::Max_Movers_Count; i++)
         if (Movers[i] != 0)
            Movers[i]->Advance(max_speed);

      /*Platform.Advance(max_speed);*/
      Rest_Distance -= AsConfig::Moving_Step_Size;
   }

   //3. Заканчивает все движения в кадре.
   for (int i = 0; i < AsConfig::Max_Movers_Count; i++)
      if (Movers[i] != 0)
         Movers[i]->Finish_Movement();
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Act()
{
   int index = 0;
   AFalling_Letter *falling_letter;

   // 1. Выполняем все действия
   for (int i = 0; i < AsConfig::Max_Modules_Count; i++)
       if (Modules[i] != 0)
           Modules[i]->Act();

   // 2. Ловим падающие буквы
   while (Level.Get_Next_Falling_Letter(index, &falling_letter) )
   {
      if (Platform.Hit_By(falling_letter) )
      {
         On_Falling_Letter(falling_letter);
      }
   }

   // 3. Рестарт уровня (если надо)
   if (Game_State == EGame_State::Restart_Level)
       if (Border.Is_Gate_Opened(AsConfig::Gates_Count - 1) )
           Platform.Set_State(EPlatform_State::Rolling);   
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::On_Falling_Letter(AFalling_Letter *falling_letter)
{
   switch (falling_letter->Letter_Type)
   {
   case ELetter_Type::O: // "Отмена"
      Platform.Set_State(EPlatform_Substate_Regular::Normal);
      break;//!!! Отмену клея

   
   case ELetter_Type::I: // "Инверсия"
      Ball_Set.Inverse_Balls();
      Platform.Set_State(EPlatform_Substate_Regular::Normal);
      break;
   
   case ELetter_Type::C: // "Скорость"
      Ball_Set.Reset_Speed();
      Platform.Set_State(EPlatform_Substate_Regular::Normal);
      break;

   //case ELetter_Type::M: // "Монстры"

   case ELetter_Type::G: // "Жизнь"
      if (Life_Count < AsConfig::Max_Life_Count)
         ++Life_Count; //!!! Отобразить на индикаторе!
      Platform.Set_State(EPlatform_Substate_Regular::Normal);
      break;

   case ELetter_Type::K: // "Клей"
      Platform.Set_State(EPlatform_State::Glue);
      break;
   
   case ELetter_Type::W: // "Шире"
      Platform.Set_State(EPlatform_State::Expanding);
      break;

   case ELetter_Type::T: // "Три"
      Platform.Set_State(EPlatform_Substate_Regular::Normal);
      Ball_Set.Triple_Balls();      
      break;
   
   case ELetter_Type::L: // "Лазер"
      Platform.Set_State(EPlatform_State::Laser);
      break;

   case ELetter_Type::P: // "Пол"
      AsConfig::Level_Has_Floor = true;
      Border.Redraw_Floor();
      //!!! Отобразить на индикаторе!
      Platform.Set_State(EPlatform_Substate_Regular::Normal);
      break;

   //case ELetter_Type::Plus: // "Переход на следующий уровень"
   
   //case ELetter_Type::MAX:
   
   default:
      AsConfig::Throw();
      break;
   }

   falling_letter->Finalize();
}
//------------------------------------------------------------------------------------------------------------





