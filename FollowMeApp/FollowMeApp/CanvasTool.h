#pragma once
#include "EasyGraphics.h"
#include <string>
using namespace std;

class CanvasTool :
	public EasyGraphics
{
public:
	CanvasTool();
	~CanvasTool();
	int cx = 0, cy = 0;
	int timerID = 0;
	int timerCounter = 0;
	int levelCounter = 0;
	const wstring defaultTile = L"assets\\GrayTile.bmp";  //todo: save as 0 and 1s and have an if statement to draw the paths based on which
	const wstring lightTile = L"assets\\LightTile.bmp";
	int startBtnC = YELLOW;
	wstring pathsArr[4][4];
	typedef struct orderArr {
		int row;
		int column;
	};



	virtual void onLButtonDown(UINT nFlags, int x, int y);
	virtual void onDraw();
	virtual void onTimer(UINT nIDEvent);
	virtual void startBtn();
	virtual void startGame();
	virtual void drawNewPath(int& rO, int& cO);
	virtual void defaultTileSetter();
};



