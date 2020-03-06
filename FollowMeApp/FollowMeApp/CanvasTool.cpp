#include "CanvasTool.h"
#include <time.h>

CanvasTool::CanvasTool()
{
	setImmediateDrawMode(false);
	defaultTileSetter();
	srand(time(NULL));
}

CanvasTool::~CanvasTool()
{
}
 


void CanvasTool::onDraw() {


	clearScreen(WHITE);

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
				drawBitmap(pathsArr[j][i].c_str(), i * 67 + 200, j * 67 + 100, 65, 65);
		}
	}
	
	startBtn();
	levelCounterText();
	drawText("0.000", 200, 405);
	EasyGraphics::onDraw();
}
void CanvasTool::startBtn() {
	setBackColour(startBtnC);
	drawRectangle(300, 400, 100, 50, true);
	setTextColour(BLACK);
	setFont(20, L"Tahoma");
	drawText("start", 315, 405);
}
void CanvasTool::levelCounterText(){
	setBackColour(LevelCTxtC);
	drawRectangle(300, 10, 120, 50, true);
	setTextColour(BLACK);
	setFont(20, L"Tahoma");
	string levelCWord = "Level: " + to_string(levelCounter);
	drawText(levelCWord.c_str(), 315, 15);
}
void CanvasTool::defaultTileSetter() {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			pathsArr[i][j] = defaultTile;
		}
	}
}
void CanvasTool::onTimer(UINT nIDEvent)
{
	if (nIDEvent == timerID)
	{
		timerCounter++;

		//logic to check if solved
	}
}
void CanvasTool::onLButtonDown(UINT nFlags, int x, int y)
{
	cx = x;
	cy = y;
	if (cx > 300 && cx < 400 && cy >400 && cy < 450) //if the clear area is clicked
	{

		//setTimer(timerID, 100);
		startBtnC = GREEN;
		defaultTileSetter();
		startGame();

	}
	 
}



void CanvasTool::startGame() {
	
	bool notGameOver = true;
	int c_Origin = rand() % 4;//random number in range
	int r_Origin = rand() % 4;
	orderArr currentPathArr[20];//todo: vector?***********************
	pathsArr[r_Origin][c_Origin] = startTile;//set the origin
	currentPathArr[0].row = r_Origin;
	currentPathArr[0].column = c_Origin;
	for (int i = 0; i < (levelCounter+3); i++) {//loop through tile generator
	
		drawNewPath(r_Origin, c_Origin, currentPathArr, i);//code the next tile from the origin
	}

	onDraw();		
	orderArr hi = currentPathArr[3];//so i can have a look at the content of the array***************

	levelCounter++;
}

void CanvasTool::drawNewPath(int& rO, int& cO, orderArr cpa[], int i) {
	int tileChoice = rand() % 4; // chooses direction N=0, E=1, S=2, W=3
	bool invalidRnd = true;
	
	while (invalidRnd) {//validation loop checks if the direction is out of bounds
		if (cpa[i-1].row == cpa[i].row && cpa[i-1].column == cpa[i].column) {
			tileChoice = rand() % 4;
		}
		else if ((rO - 1 < 0) && (tileChoice == 0)) {//if row -1 < 0 means that it is outside the bounds of the array so re-roll
			tileChoice = rand() % 4;
		}
		else if((cO + 1 > 3) && (tileChoice == 1)){
			tileChoice = rand() % 4;
		}
		else if ((rO + 1 > 3) && (tileChoice == 2)) {
			tileChoice = rand() % 4;
		}
		else if ((cO - 1 < 0) && (tileChoice == 3)) {
			tileChoice = rand() % 4;
		}
		else {
			invalidRnd = false;
		}
	}
	switch (tileChoice) {
	case 0:
		rO--;
		pathsArr[rO][cO] = lightTile;
		cpa[i].row = rO;
		cpa[i].column = cO;

		break;
	case 1:
		cO++;
		pathsArr[rO][cO] = lightTile;
		cpa[i].row = rO;
		cpa[i].column = cO;
		break;
	case 2:
		rO++;
		pathsArr[rO][cO] = lightTile;
		cpa[i].row = rO;
		cpa[i].column = cO;
		break;
	case 3:
		cO--;
		pathsArr[rO][cO] = lightTile;
		cpa[i].row = rO;
		cpa[i].column = cO;
		break;
	}
}