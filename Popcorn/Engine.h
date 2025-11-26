#pragma once

#include <Windows.h>

enum EKey_Type
{
    EKT_Left,
    EKT_Right,
    EKT_Space
};

const int Timer_ID = WM_USER + 1;

void Init_Engine(HWND hwnd);
void Draw_Ball(HDC hdc, RECT &paint_area);
void Draw_Border(HDC hdc, int x, int y, bool top_boder);
void Draw_Bounds(HDC hdc, RECT &paint_area, bool top_boder);
void Draw_Frame(HDC hdc, RECT &paint_area);
int On_Key_Down(EKey_Type key_type);
void Check_Level_Brick_Hit(int &next_y_pos);
void Move_Ball();
int On_Timer();
