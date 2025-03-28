﻿#pragma once

#include"Config.h"

//------------------------------------------------------------------------------------------------------------
enum class EPlatform_State : unsigned char
{
	Unknown,

	Regular,
	Meltdown,
	Rolling,
	Glue,
	Expanding,
	Laser
};
//------------------------------------------------------------------------------------------------------------
enum class EPlatform_Substate_Regular : unsigned char
{
	Unknown,

	Missing,
	Ready,
	Normal
};
//------------------------------------------------------------------------------------------------------------
enum class EPlatform_Substate_Meltdown : unsigned char
{
	Unknown,

	Init,
	Active
};
//------------------------------------------------------------------------------------------------------------
enum class EPlatform_Substate_Rolling : unsigned char
{
	Unknown,

	Roll_In,
	Expand_Roll_In
};
//------------------------------------------------------------------------------------------------------------
enum class EPlatform_Transformation : unsigned char
{
	Unknown,

	Init,
	Active,
	Finalize
};
//------------------------------------------------------------------------------------------------------------
enum class EPlatform_Moving_State : unsigned char
{
	Stopping,
	Stop,
	Moving_Left,
	Moving_Right
};
//------------------------------------------------------------------------------------------------------------
enum class EFigure_Type : unsigned char
{
	Ellipse,
	Rectangle,
	Round_Rect_3x
};
//------------------------------------------------------------------------------------------------------------


//AsPlatform_State
//------------------------------------------------------------------------------------------------------------
class AsPlatform_State
{
public:
	AsPlatform_State();

	operator EPlatform_State() const;
	void operator = (EPlatform_State new_state);

	void Set_Next_State(EPlatform_State next_state);
	EPlatform_State Get_Next_State();
	EPlatform_State Set_State(EPlatform_Substate_Regular new_regular_state);
	EPlatform_State Set_Next_Or_Regular_State(EPlatform_Substate_Regular new_regular_state);

	EPlatform_Substate_Regular Regular;
	EPlatform_Substate_Meltdown Meltdown;
	EPlatform_Substate_Rolling Rolling;
	EPlatform_Transformation Glue;
	EPlatform_Transformation Expanding;
	EPlatform_Transformation Laser;

	EPlatform_Moving_State Moving;

private:
	EPlatform_State Current_State;
	EPlatform_State Next_State; // В этом состоянии переходим из void AsPlatform::Set_State(EPlatform_Substate_Regular new_regular_state)
};
//------------------------------------------------------------------------------------------------------------
