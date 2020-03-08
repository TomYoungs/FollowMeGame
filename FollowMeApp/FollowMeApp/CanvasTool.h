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
	int levelCounter = 1;
	const wstring defaultTile = L"assets\\GrayTile.bmp";  //todo: save as 0 and 1s and have an if statement to draw the paths based on which
	const wstring lightTile = L"assets\\LightTile.bmp";
	const wstring startTile = L"assets\\start.bmp";
	const wstring balloonTile = L"assets\\hotAirBalloon.bmp";
	int startBtnC = YELLOW;
	int LevelCTxtC = YELLOW;
	wstring pathsArr[4][4];


	typedef struct orderArr {
		int row;
		int column;
	};



	virtual void onLButtonDown(UINT nFlags, int x, int y);
	virtual void onDraw();
	virtual void onTimer(UINT nIDEvent);
	virtual void startBtn();
	virtual void levelCounterText();
	virtual void startGame();
	virtual void drawNewPath(int& rO, int& cO, orderArr cpa[], int i);
	virtual void animatePath(orderArr cpa[]);
	virtual void defaultTileSetter();
};



