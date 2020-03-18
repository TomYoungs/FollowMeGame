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
	bool playerMode = false;
	int timerID = 0;
	int timerCounter = 0;
	int levelCounter = 1;
	int gridSize = 4;
	bool gameOver = false;
	int playerPos = 0;
	const wstring defaultTile = L"assets\\GrayTile.bmp";  //todo: save as 0 and 1s and have an if statement to draw the paths based on which
	const wstring lightTile = L"assets\\LightTile.bmp";
	const wstring startTile = L"assets\\start.bmp";
	const wstring balloonTile = L"assets\\hotAirBalloon.bmp";
	int startBtnC = YELLOW;
	int LevelCTxtC = YELLOW;
	int gridDim = 65;
	wstring pathsArr[6][6];
	enum direction { north, east, south, west };
	enum arrowKeys { upArrow = 38, downArrow = 40, rightArrow = 39, leftArrow = 37};
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
	virtual void hintText();
	virtual void defaultTileSetter();
	virtual void wait(DWORD interval);
	virtual void animatePathValid(vector<point> cpa);
	virtual void blackGridDrawer();

	//-------botSection-----------//

	virtual void startGame();
	virtual void createNewPath(point& coords, vector<point>& cpa, int i);
	virtual void animatePath(point coords, int N_S, int W_E);
	//-----------end--------------//

	//-------playerSection--------//
	virtual void gridDrawer();
	virtual void gameOverText();
	virtual void playerGame(UINT nChar, const vector<point> cpv);
	virtual void pressBtn();
	virtual void winGameText();
	//------------end-------------//
};



