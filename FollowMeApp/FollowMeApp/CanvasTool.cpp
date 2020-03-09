#include "CanvasTool.h"
#include <time.h>
//make some relivant asserts 
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
	//pathsArr[r_Origin][c_Origin] = startTile;//set the origin
	currentPathArr[0].row = r_Origin;
	currentPathArr[0].column = c_Origin;
	for (int i = 0; i < (levelCounter+3); i++) {//loop through tile generator
	
		drawNewPath(r_Origin, c_Origin, currentPathArr, i);//code the next tile from the origin
	}
	onDraw();
	animatePathValid(currentPathArr);

	levelCounter++;
}

void CanvasTool::drawNewPath(int& rO, int& cO, orderArr cpa[], int i) {
	int tileChoice = rand() % 4; // chooses direction N=0, E=1, S=2, W=3
	bool invalidRnd = true;
	
	//todo: can i make this more efficient/ compact?
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


void CanvasTool::animatePathValid(orderArr cpa[]) {
	//take in CPA[] and find the point to have the asset overlay for examble if the origin is at (2,1) the location
	//of the asset would be (334,267) asset is 65x65 this assets x coord would incroment till it hits the next location
	//minus 65 (334,334) minus 65 because the asset is tracked from the top left 
	for (int i = 0; i < levelCounter + 3; i++) {//tile by tile

		const int rowCalc = cpa[i + 1].row - cpa[i].row;
		const int colCalc = cpa[i + 1].column - cpa[i].column;
		if (rowCalc == 0) {
			if (colCalc == -1) {
				animatePath(cpa[i].row, cpa[i].column, rowCalc, colCalc);//west
			}
			else if (colCalc == 1) {
				animatePath(cpa[i].row, cpa[i].column, rowCalc, colCalc);//east
			}
		}
		else if (colCalc == 0) {
			if (rowCalc == -1) {
				animatePath(cpa[i].row, cpa[i].column, rowCalc, colCalc);//north
			}
			else if (rowCalc == 1) {
				animatePath(cpa[i].row, cpa[i].column, rowCalc, colCalc);//south
			}
		}
	}
}

void CanvasTool::animatePath(int row,int column, int N_S, int W_E) {
		for (int j = 0; j < 65; j++) {
			drawBitmap(balloonTile.c_str(), (column * 67 + 200) + j * W_E , (row * 67 + 100) + j * N_S, 65, 65);
			//todo: delay stuff, may be related to timer so could do that first maybe?
			EasyGraphics::onDraw();
		}

	//might need like a position counter in cpa
	//todo: could i make this function work with the player also?
}

