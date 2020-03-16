#include "CanvasTool.h"
#include <time.h>
//make some relivant asserts 
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

	gridDrawer();
	levelCounterText();
	drawText("0.000", 200, 405);
	startBtnC = YELLOW;
	EasyGraphics::onDraw();
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
void wait(DWORD interval)
{
	DWORD startTime = GetTickCount();

	while (GetTickCount() < (startTime + interval)) {
		//wait for a bit
	}
}
void CanvasTool::onKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (playerMode) {
		drawBitmap(balloonTile.c_str(), (currentPathVec[1].column * (gridDim + 2) + 200), (currentPathVec[1].row * (gridDim + 2) + 100), gridDim, gridDim);
		if (nChar == 38) {//arrow up
			setBackColour(BLACK);
			drawRectangle(100, 100, 100, 100, true);
			EasyGraphics::onDraw();
		}
		if (nChar == 40) {//arrow down
			setBackColour(GREEN);
			drawRectangle(100, 100, 100, 100, true);
			EasyGraphics::onDraw();
		}
		if (nChar == 37) {//arrow left
			setBackColour(WHITE);
			drawRectangle(100, 100, 100, 100, true);
			EasyGraphics::onDraw();
		}
		if (nChar == 39) {//arrow right
			setBackColour(BLUE);
			drawRectangle(100, 100, 100, 100, true);
			EasyGraphics::onDraw();
		}
	}
	else {
		defaultTileSetter();
		startGame();
		playerMode = true;
	}
}
void CanvasTool::startGame() {
	srand((unsigned int)time(NULL));

	bool notGameOver = true;
	point OriginGen; 
	OriginGen.row = rand() % (4 + (gridSize - 4));//random number in range, as the grid increases in size this bit of maths adds extra to the range.
	OriginGen.column = rand() % (4 + (gridSize - 4));
	OriginGen.direction = -1; //can you do this with enum?
	currentPathVec.clear();
	currentPathVec.push_back(OriginGen);
	
	for (int i = 0; i < (levelCounter+3); i++) {//loop through tile generator
	
		createNewPath(OriginGen, currentPathVec, i);//code the next tile from the origin
	}
	onDraw();
	animatePathValid(currentPathVec);
	clearScreen(WHITE);
	onDraw();

	levelCounter++;
}
void CanvasTool::createNewPath(point& coords, vector<point>& cpv, int i) {
	
	int tileChoice = rand() % 4; // chooses direction N=0, E=1, S=2, W=3
	bool invalidRnd = true;

	
	//todo: can i make this more efficient/ compact?
	while (invalidRnd) {//validation loop checks if the direction is out of bounds
			if ((oppositeDirection[cpv[i].direction]) == (tileChoice)) {
				tileChoice = rand() % 4;
			}
			else if ((coords.row - 1 < 0) && (tileChoice == 0)) {//if row -1 < 0 means that it is outside the bounds of the array so re-roll
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
	coords.direction = tileChoice;

	switch (tileChoice) {
	case north:
		coords.row--;
		pathsArr[coords.row][coords.column] = lightTile;
		cpv.push_back({ coords.row,coords.column, tileChoice });
		
		break;
	case east:
		coords.column++;
		pathsArr[coords.row][coords.column] = lightTile;
		cpv.push_back(coords);
		break;
	case south:
		coords.row++;
		pathsArr[coords.row][coords.column] = lightTile;
		cpv.push_back(coords);
		break;
	case west:
		coords.column--;
		pathsArr[coords.row][coords.column] = lightTile;
		cpv.push_back(coords);
		break;
	}
}
void CanvasTool::animatePathValid(vector<point> cpv) {
	//take in cpv[] and find the point to have the asset overlay for examble if the origin is at (2,1) the location
	//of the asset would be (334,267) asset is 65x65 this assets x coord would incroment till it hits the next location
	//minus 65 (334,334) minus 65 because the asset is tracked from the top left 
	for (int i = 0; i < levelCounter + 3; i++) {//tile by tile
		const int rowCalc = cpv[i + 1].row - cpv[i].row;
		const int colCalc = cpv[i + 1].column - cpv[i].column;
		if (rowCalc == 0) {
			if (colCalc == -1) {
				animatePath(cpv[i], rowCalc, colCalc);//west
			}
			else if (colCalc == 1) {
				animatePath(cpv[i], rowCalc, colCalc);//east
			}
		}
		else if (colCalc == 0) {
			if (rowCalc == -1) {
				animatePath(cpv[i], rowCalc, colCalc);//north
			}
			else if (rowCalc == 1) {
				animatePath(cpv[i], rowCalc, colCalc);//south
			}
		}
	}

}
void CanvasTool::animatePath(const point coords, int N_S, int W_E) {
	int solutionTime = 2;
	int timer;
	int counter = 0;
	DWORD interval = 20;
	DWORD start = GetTickCount();

	for (int j = 0; j < (gridDim + 2); j = j + 4) {
		//clear asset
		
		drawBitmap(balloonTile.c_str(), (coords.column * (gridDim+2) + 200) + j * W_E , (coords.row * (gridDim+2) + 100) + j * N_S, gridDim, gridDim);
		//todo: delay stuff, may be related to timer so could do that first maybe?
		//remove
		for (timer = 0; timer < solutionTime; timer += interval) {
			wait(interval);
		}
		EasyGraphics::onDraw();
	}

	//might need like a position counter in cpv
	//todo: could i make this function work with the player also?
}

