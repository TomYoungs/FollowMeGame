#include "CanvasTool.h"
#include <time.h>

CanvasTool::CanvasTool()
{
	setImmediateDrawMode(false);
	defaultTileSetter();
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
	srand(time(NULL));
	bool notGameOver = true;
	while (notGameOver) {//loops through levels 
		int c_Origin = rand() % 4;//random number in range
		int r_Origin = rand() % 4;
		levelCounter = 0;
		orderArr *currentPathArr = new ordeArr[4];
		pathsArr[r_Origin][c_Origin] = lightTile;//set the origin (todo: use a different asset)
		for (int i = 0; i < (levelCounter+2); i++) {//loop through tile generator
		
			drawNewPath(r_Origin, c_Origin);//code the next tile from the origin
		}
		levelCounter++;
		onDraw();		
		notGameOver = false;
	}
}

void CanvasTool::drawNewPath(int& rO, int& cO) {
	int tileChoice = rand() % 4; // chooses direction N=0, E=1, S=2, W=3
	bool invalidRnd = true;
	
	while (invalidRnd) {//validation loop checks if the direction is out of bounds
		
		if ((rO - 1 < 0) && (tileChoice == 0)) {//if row -1 < 0 means that it is outside the bounds of the array so re-roll
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
		
		break;
	case 1:
		cO++;
		pathsArr[rO][cO] = lightTile;
		break;
	case 2:
		rO++;
		pathsArr[rO][cO] = lightTile;
		break;
	case 3:
		cO--;
		pathsArr[rO][cO] = lightTile;
		break;
	}
}