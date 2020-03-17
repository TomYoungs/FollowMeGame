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
	const int downArrow = 40;
	const int rightArrow = 39;
	const int upArrow = 38;
	const int leftArrow = 37;
	int cx = 0, cy = 0;
	bool playerMode = false;
	int timerID = 0;
	int timerCounter = 0;
	int levelCounter = 1;
	int gridSize = 4;
	const wstring defaultTile = L"assets\\GrayTile.bmp";  //todo: save as 0 and 1s and have an if statement to draw the paths based on which
	const wstring lightTile = L"assets\\LightTile.bmp";
	const wstring startTile = L"assets\\start.bmp";
	const wstring balloonTile = L"assets\\hotAirBalloon.bmp";
	int startBtnC = YELLOW;
	int LevelCTxtC = YELLOW;
	int gridDim = 65;
	wstring pathsArr[6][6];
	enum direction { north, east, south, west };
	enum arrowKeys { UP = 38, DOWN = 40, RIGHT = 39, LEFT = 37};
	int oppositeDirection[4] = {2,3,0,1};

	struct point {
		int row;
		int column;
		int direction;
	};
	vector<point> currentPathVec;
	
	virtual void onKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void onDraw();
	virtual void levelCounterText();
	virtual void defaultTileSetter();
	virtual void gridDrawer();

	//-------botSection-----------//
	virtual void startGame();
	virtual void createNewPath(point& coords, vector<point>& cpa, int i);
	virtual void animatePath(point coords, int N_S, int W_E);
	virtual void animatePathValid(vector<point> cpa);
	//-----------end--------------//

	//-------playerSection--------//
	virtual void playerGame(UINT nChar, vector<point>& cpv);
	//------------end-------------//
};



