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

void CanvasTool::onKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {

	if (nChar == 67 && playerMode) {//67 "c" shows the user the path if they can't remember it
		gridDrawer();
	}
	else if (playerMode && (nChar <= 40 && nChar >= 37)) {//arrow key values are between 37 and 40, also playermode must be true which means that bot mode has already happened
		if ((levelCounter + 3) == playerPos) {//win condition: when player pos = the length of the vector the game has been won
			levelCounter++;//move to next level
			playerMode = false;//do back to bot mode
			winGameText();
			DWORD interval = 1000;
			wait(interval);//wait a few seconds on win screen
			pressBtn();

		}
		else {

			playerGame(nChar, currentPathVec);//start player game
		}
		if (gameOver) {//game over screen
			gameOverText();
			DWORD interval = 1000;
			wait(interval);//wait on gameover screen for a few seconds
			pressBtn();
			levelCounter = 1;//restart from level 1
			gridSize = 4;//reset grid size
			gridDim = 65;//reset grid dimentions
			playerMode = false;//change to bot game
		}			

	}
	else if (playerMode && !(nChar < 40 && nChar > 37)) {//invalid input during player game (not arrow keys)
			//do nothing
	}
	else {
			defaultTileSetter();//set all tiles to default grey
			startGame();//start bot game
			drawBitmap(balloonTile.c_str(), (currentPathVec[0].column * (gridDim + 2) + 200),(currentPathVec[0].row * (gridDim + 2) + 100), gridDim, gridDim);
			EasyGraphics::onDraw();//draw origin tile
			playerMode = true;//start player mode
	}
}
void CanvasTool::onDraw() {//default screen with grid(4x4 to 6x6) levelcounter and hint text
	clearScreen(WHITE);
	blackGridDrawer();
	levelCounterText();
	hintText();
	EasyGraphics::onDraw();

}
void CanvasTool::levelCounterText(){//displays what level the user is on
	setBackColour(YELLOW);
	drawRectangle(300, 10, 120, 50, true);
	setTextColour(BLACK);
	setFont(20, L"Tahoma");
	string levelCWord = "Level: " + to_string(levelCounter);
	drawText(levelCWord.c_str(), 315, 15);
}
void CanvasTool::hintText() {//tells about c hint key
	setPenColour(BLACK, 3);
	setFont(15, L"Tahoma");
	drawText("press c to view path:", 10, 200);
}
void CanvasTool::pressBtn(){//plays on win and game over screens
	setFont(20, L"Tahoma");
	drawText("Press any key to restart", 200, 400);
	EasyGraphics::onDraw();
}
void CanvasTool::wait(DWORD interval)//wait a given amount of time
{
	DWORD startTime = GetTickCount();

	while (GetTickCount() < (startTime + interval)) {
		//wait for a bit
	}
}
void CanvasTool::defaultTileSetter() {//sets all tiles to be the default grey
	
	for (int i = 0; i < 6; ++i) {
		for (int j = 0; j < 6; ++j) {
			pathsArr[i][j] = defaultTile;
		}
	}
}
void CanvasTool::blackGridDrawer() {//ignores the light grey tiles that are stored during the path generation
	if (levelCounter > 4) {//sets the grid size based of level
		gridSize = 5;
		gridDim = 52;
		if (levelCounter > 9) {
			gridSize = 6;
			gridDim = 43;
		}
	}

	for (int i = 0; i < gridSize; i++) {
		for (int j = 0; j < gridSize; j++) {
			drawBitmap(L"assets\\GrayTile.bmp", i * (gridDim + 2) + 200, j * (gridDim + 2) + 100, gridDim, gridDim);
		}
	}
}
//-------botSection-----------//
void CanvasTool::startGame() {
	srand((unsigned int)time(NULL));

	point originGen; //generate a new point value just for storing the origin
	originGen.row = rand() % (gridSize);//random number in range, range changes depending on the grid size
	originGen.column = rand() % (gridSize);
	originGen.direction = -1;//temp direction value
	pathsArr[originGen.row][originGen.column] = lightTile;
	currentPathVec.clear();//clear to make sure it dosen't rememeber anything between rounds
	currentPathVec.push_back(originGen);//push origin onto vector
	
	for (int i = 0; i < (levelCounter+3); i++) {//loop through tile generator
	
		createNewPath(originGen, currentPathVec, i);//code the next tile from the origin
	}
	onDraw();
	animatePathValid(currentPathVec);//draw path
	clearScreen(WHITE);
	onDraw();//after the ai path is drawn it is overwritten

	currentPathVec[0].direction = currentPathVec[1].direction;//when doing the player mode it is needed that the origin value has the same direction as the next
	playerPos = 0;//resets the index for players position in the vector before it is run
	gameOver = false;
}
void CanvasTool::createNewPath(point& coords, vector<point>& cpv, int i) {
	
	int tileChoice = rand() % 4; // chooses direction N=0, E=1, S=2, W=3
	bool invalidRnd = true;

	
	//todo: can i make this more efficient/ compact?
	while (invalidRnd) {//validation loop checks if the direction is out of bounds
			if ((oppositeDirection[cpv[i].direction]) == (tileChoice)) {//special validation to check the immediatly before while generating the path
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
	coords.direction = tileChoice;//save the direction 

	switch (tileChoice) {
	case north:
		coords.row--;//to go north you decrement the row value
		pathsArr[coords.row][coords.column] = lightTile;
		cpv.push_back(coords);//add new value to vector
		
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
	//take in cpv[] and find the point to have the asset overlay for example if the origin is at (2,1) the location
	//of the asset would be (334,267) asset is 65x65 this asset's x coord would incroment till it hits the next location
	//minus 65 (334,334) minus 65 because the asset is tracked from the top left 
	for (int i = 0; i < levelCounter + 3; i++) {//tile by tile
		const int rowCalc = cpv[i + 1].row - cpv[i].row;//the difference is used so its animated in the right direction
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
//-----------end--------------//
void CanvasTool::animatePath(const point coords, int N_S, int W_E) {
	int solutionTime = 3;
	DWORD interval = 40;

	for (int j = 0; j < (gridDim + 2); j = j + 4) {//keep drawing frames till you hit the next one (gridDim) the +2 accounts for the border between tiles		
		drawBitmap(balloonTile.c_str(), (coords.column * (gridDim+2) + 200) + j * W_E , (coords.row * (gridDim+2) + 100) + j * N_S, gridDim, gridDim);
		//^ animate tile by tile so take in a single vector element (coords) * it by the grid dimention (+2 margin) this will give it its size then add 200 so the grid is in the middle of the page 
		//^ j is the thing that incroments to give it its motion, the N_S W_E is gonna be -1, 0 or 1 this moves it in the right direction if there is no movement it multiplies by 0 which would make no change
		for (int timer = 0; timer < solutionTime; timer += interval) {
			wait(interval);
		}
		EasyGraphics::onDraw();
	}
} 

//-------playerSection--------//
void CanvasTool::gameOverText() {
	clearScreen(WHITE);
	setTextColour(BLACK);
	setFont(100, L"Tahoma");
	drawText("Game Over", 80, 200);

	EasyGraphics::onDraw();
}
void CanvasTool::playerGame(UINT nChar, const vector<point> cpv) {//todo: see if could be const
		switch (nChar) {//arrowkeys

		case upArrow:
			if (cpv[playerPos + 1].direction == north) {//checks that the arrow key pressed is equal to the direction of the next tile				
				animatePath(cpv[playerPos], -1, 0);//animate a tile movement
			}
			else {
				gameOver = true;//if arrow key is wrong gameover
			}
			break;
		case downArrow:
			if (cpv[playerPos + 1].direction == south) {
				animatePath(cpv[playerPos], 1, 0);
			}
			else {
				gameOver = true;
			}
			break;
		case rightArrow:
			if (cpv[playerPos + 1].direction == east) {
				animatePath(cpv[playerPos], 0, 1);
			}
			else {
				gameOver = true;
			}
			break;
		case leftArrow:
			if (cpv[playerPos + 1].direction == west) {
				animatePath(cpv[playerPos], 0, -1);
			}
			else {
				gameOver = true;
			}
			break;
		default:
			gameOver = true;
			break;
		}
		playerPos++;//incroment the postion that the player is in the vector as in they moved a tile after this function has been finished
}
void CanvasTool::winGameText() {
	clearScreen(WHITE);
	setTextColour(BLACK);
	setFont(50, L"Tahoma");
	drawText("Level Complete", 80, 200);

	EasyGraphics::onDraw();
}
void CanvasTool::gridDrawer() {//this is the true grid drawer that shows all the tiles grey and lightgrey

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
	drawBitmap(balloonTile.c_str(), (currentPathVec[0].column * (gridDim + 2) + 200), (currentPathVec[0].row * (gridDim + 2) + 100), gridDim, gridDim);
	EasyGraphics::onDraw();

}
//-----------end--------------//