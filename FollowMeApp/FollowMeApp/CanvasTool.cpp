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

	gridDrawer();
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
	
	for (int i = 0; i < 6; ++i) {
		for (int j = 0; j < 6; ++j) {
			pathsArr[i][j] = defaultTile;
		}
	}
}
void CanvasTool::gridDrawer() {

	if (levelCounter > 4) {
		gridSize = 5;
		gridDim = 52;
		if (levelCounter > 9) {
			gridDim = 43;
			gridSize = 6;
		}
	}
	
	for (int i = 0; i < gridSize; i++) {
		for (int j = 0; j < gridSize; j++) {
			drawBitmap(pathsArr[j][i].c_str(), i * (gridDim + 2) + 200, j * (gridDim + 2) + 100, gridDim, gridDim);
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
	point OriginGen; 
	 OriginGen.row = rand() % (4 + (gridSize - 4));//random number in range, as the grid increases in size this bit of maths adds extra to the range.
	 OriginGen.column = rand() % (4 + (gridSize - 4));
	vector<point> currentPathArr;
	currentPathArr.push_back(OriginGen);
	
	for (int i = 0; i < (levelCounter+3); i++) {//loop through tile generator
	
		drawNewPath(OriginGen, currentPathArr, i);//code the next tile from the origin
	}
	onDraw();
	animatePathValid(currentPathArr);

	levelCounter++;
}
void CanvasTool::drawNewPath(point& coords, vector<point>& cpa, int i) {//todo: enum
	
	int tileChoice = rand() % 4; // chooses direction N=0, E=1, S=2, W=3
	bool invalidRnd = true;
	
	//todo: can i make this more efficient/ compact?
	while (invalidRnd) {//validation loop checks if the direction is out of bounds
	/*		if (cpa.size() > 1) { 
								  !!!	this validation dosen't work it was meant to test the immediately after thing but i can't think how
										to get this to work without having an entire extra switch statement like the one below.		!!!  
				if (cpa[i - 1].row == cpa[i].row && cpa[i - 1].column == cpa[i].column) {
				tileChoice = rand() % 4;
				}
			}*/
			if ((coords.row - 1 < 0) && (tileChoice == 0)) {//if row -1 < 0 means that it is outside the bounds of the array so re-roll
				tileChoice = rand() % 4;
			}
			else if ((coords.column + 1 >= gridSize) && (tileChoice == 1)) {
				tileChoice = rand() % 4;
			}
			else if ((coords.row + 1 >= gridSize) && (tileChoice == 2)) {
				tileChoice = rand() % 4;
			}
			else if ((coords.column - 1 < 0) && (tileChoice == 3)) {
				tileChoice = rand() % 4;
			}
			else {
				invalidRnd = false;
			}
		
		}
	switch (tileChoice) {
	case north:
		coords.row--;
		pathsArr[coords.row][coords.column] = lightTile;
		cpa.push_back(coords);
		break;
	case east:
		coords.column++;
		pathsArr[coords.row][coords.column] = lightTile;
		cpa.push_back(coords);
		break;
	case south:
		coords.row++;
		pathsArr[coords.row][coords.column] = lightTile;
		cpa.push_back(coords);
		break;
	case west:
		coords.column--;
		pathsArr[coords.row][coords.column] = lightTile;
		cpa.push_back(coords);
		break;
	}
}
void CanvasTool::animatePathValid(vector<point> cpa) {
	//take in CPA[] and find the point to have the asset overlay for examble if the origin is at (2,1) the location
	//of the asset would be (334,267) asset is 65x65 this assets x coord would incroment till it hits the next location
	//minus 65 (334,334) minus 65 because the asset is tracked from the top left 
	for (int i = 0; i < levelCounter + 3; i++) {//tile by tile
		const int rowCalc = cpa[i + 1].row - cpa[i].row;
		const int colCalc = cpa[i + 1].column - cpa[i].column;
		if (rowCalc == 0) {
			if (colCalc == -1) {
				animatePath(cpa[i], rowCalc, colCalc);//west
			}
			else if (colCalc == 1) {
				animatePath(cpa[i], rowCalc, colCalc);//east
			}
		}
		else if (colCalc == 0) {
			if (rowCalc == -1) {
				animatePath(cpa[i], rowCalc, colCalc);//north
			}
			else if (rowCalc == 1) {
				animatePath(cpa[i], rowCalc, colCalc);//south
			}
		}
	}
}
void CanvasTool::animatePath(const point coords, int N_S, int W_E) {
		for (int j = 0; j < (gridDim +2); j++) {
			//clear asset
			drawBitmap(balloonTile.c_str(), (coords.column * (gridDim+2) + 200) + j * W_E , (coords.row * (gridDim+2) + 100) + j * N_S, gridDim, gridDim);
			//todo: delay stuff, may be related to timer so could do that first maybe?
			//remove
			EasyGraphics::onDraw();
		}

	//might need like a position counter in cpa
	//todo: could i make this function work with the player also?
}

