#pragma once

#include "Config.h"

//AGate
//------------------------------------------------------------------------------------------------------------
enum class EGate_State : unsigned char
{
	Closed,
	Short_Open,
	Long_Open
};
//------------------------------------------------------------------------------------------------------------
enum class EGate_Transformation : unsigned char
{
	Unknown,

	Init,
	Active,
	Finalize
};
//------------------------------------------------------------------------------------------------------------
class AGate : public AGraphics_Object
{
public:
	AGate(int x_pos, int y_pos);

	virtual void Act();
	virtual void Clear(HDC hdc, RECT& paint_area);
	virtual void Draw(HDC hdc, RECT& paint_area);
	virtual bool Is_Finished();

	void Open_Gate(bool short_open);
	bool Is_Opened();
	void Get_Y_Size(int &gate_top_y, int &gate_low_y);

private:
	bool Act_For_Open(bool short_open, bool &correct_pos);
	void Draw_Cup(HDC hdc, bool top_cup);
	void Draw_Edges(HDC hdc);
	void Draw_Short_Opening_Edges(HDC hdc);
	void Draw_Long_Opening_Edges(HDC hdc);
	void Draw_One_Edge(HDC hdc, int edge_y_offset, bool long_edge);
	void Draw_Red_Edge(HDC hdc, int edge_y_offset, bool long_edge, bool has_heghlight);
	void Draw_Charge(HDC hdc);
	void Redraw_Gate();

	EGate_State Gate_State;
	EGate_Transformation Gate_Transformation;
	int X_Pos;
	double Y_Pos, Origin_Y_Pos;
	int Edges_Count;
	int Gate_Close_Tick;
	double Gap_Height;
	RECT Gate_Rect;

	static const double Max_Gap_Short_Height, Max_Gap_Long_Height;
	static const double Gap_Height_Short_Step, Gap_Height_Long_Step;
	static const int Width = 6;
	static const int Height = 19;
	static const int Short_Opening_timeout = AsConfig::FPS; // 1 секунда
	static const int Long_Opening_timeout = AsConfig::FPS * 3; // 3 секунды
};
//------------------------------------------------------------------------------------------------------------
