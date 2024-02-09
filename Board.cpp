
#include "Board.hpp"
#include <random>


Board::Board(sf::RenderWindow* window) {
	numBlocks = 0;
	numSpaces = 16;
	score = 0;
	this->window = window;
	this->speed = 30.0f;
	this->numNeedMove = 0;
	memset(doneMoving, false, numSquares * sizeof(bool));

	// init each block
	for (int i=0; i<numSquares; i++) {
		blocks[i].add(&rectangles[i]);
	}
	spawn();
	spawn();
}

Board::~Board() {

}

void Board::spawn() {
	int blockInd, randNum;

	// need system that searches thru list to find inactive blocks
	for (int i=0; i<numSquares; i++) {
		if (!blocks[i].active) {
			blockInd = i;
			break;
		}
	}
	// random num from 1 to numSpaces inclusive
	randNum = rand() % numSpaces + 1;
	// pick Num-th empty spot on grid
	int count = 0;
	for (int row=0; row<4; row++) {
		for (int col=0; col<4; col++) {
			if (occupied[row][col] == empty) {
				count++;
				if (count == randNum) {
					blocks[blockInd].makeActive(row, col);
					// blocks[blockInd].value = 2;
					numBlocks++;
					numSpaces--;
					occupied[row][col] = blockInd; 
					break;
				}
			}
		}
	}
	return;
}

void Board::printOccupied() {
	for (int row=0; row<4; row++) {
		printf("%d\t%d\t%d\t%d\t\n", occupied[row][0], occupied[row][1], occupied[row][2], occupied[row][3]);
	}
	return;
}

void Board::printValues() {
	int a, b, c, d;
	for (int row=0; row<4; row++) {
		a = occupied[row][0];
		b =	occupied[row][1];
		c = occupied[row][2];
		d = occupied[row][3];
		(a == empty) ? (a = 0) : (a = blocks[a].value);
		(b == empty) ? (b = 0) : (b = blocks[b].value);
		(c == empty) ? (c = 0) : (c = blocks[c].value);
		(d == empty) ? (d = 0) : (d = blocks[d].value);

		printf("%d\t%d\t%d\t%d\t\n", a,b,c,d);
	}
	return;
}

bool Board::isWall(int row, int col) {
	return (row < 0 || col < 0 || row > 3 || col > 3);
}

bool Board::move(int blockInd, int row, int col, int newRow, int newCol, sf::Vector2f newPos) {
	int newBlockInd;

	// check if left move is wall or another block
	if (isWall(newRow, newCol)) {
		// do not move
		return false;
	}
	else if (occupied[newRow][newCol] == empty) {
		blocks[blockInd].nextPos = newPos;
		blocks[blockInd].needMove = true;
		numNeedMove++;
		// animateMove(blockInd, newPos);
		occupied[newRow][newCol] = blockInd;
		occupied[row][col] = empty;

		return true;
	}
	else {
		// collision detected
		newBlockInd = occupied[newRow][newCol];

		if (blocks[blockInd].value != blocks[newBlockInd].value) {
			// if same value, do not move
		} else {
			// collide and combines 
			blocks[newBlockInd].combine();
			
			// remove block from board  
			// printf("making %d inactive\n", blockInd);
			blocks[blockInd].makeInactive();
			numSpaces++;
			numBlocks--;
			occupied[row][col] = empty;
			return true;
		}
	}
}

void Board::animateMove(int blockInd, sf::Vector2f newPos) {
	// moving animation
	sf::Clock clock;
	sf::Time deltaTime;

	while (true) {
		deltaTime = clock.restart();
		sf::Vector2f currPos = rectangles[blockInd].getPosition();

		sf::Vector2f direction = newPos - currPos;
        sf::Vector2f movement = speed * deltaTime.asSeconds() * direction;
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    	if (distance < 1.0f) {
    		printf("breaking\n");
        	rectangles[blockInd].setPosition(newPos);
        	break;
    	}
        rectangles[blockInd].move(movement);
        update();
	}
}

void Board::update() {
	window->clear();
	// draw each block that is in play
    for (int i=0; i<numSquares; i++) {
        if (blocks[i].active) {
            window->draw(*(blocks[i].square));
        }
    }
    window->display();
	return;
}


void Board::animateAll() {
	// moving animation
	sf::Clock clock;
	sf::Time deltaTime;
	int blockInd;
	float distance;

	// for each block that needs to be moved

	while (numNeedMove > 0) {
		for (int i=0; i<numBlocks; i++) {
			if (blocks[i].needMove) {
				blockInd = i;

				// small move single block
				deltaTime = clock.restart();
				sf::Vector2f currPos = rectangles[blockInd].getPosition();
				sf::Vector2f newPos = blocks[blockInd].nextPos;

				sf::Vector2f direction = newPos - currPos;
		        sf::Vector2f movement = speed * deltaTime.asSeconds() * direction;
		        distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
		    	if (distance < 1.0f) {
		    		printf("breaking in animate all\n");
		        	rectangles[blockInd].setPosition(newPos);
		        	blocks[blockInd].needMove = false;
		        	numNeedMove--;
		    	} else {
		    		rectangles[blockInd].move(movement);
		    		printf("move() called for %d\n", blockInd);
		    	}

			}		
		}
		// update
		update();
	}
}

bool Board::isWallCollision(int i) {
	sf::Vector2f pos = rectangles[i].getPosition();
	return (pos.x < squareSize/2 || pos.y < squareSize/2 || pos.x > windowWidth - squareSize/2 || pos.y > windowHeight - squareSize/2);
}

int Board::getBlockCollision(int ind) {
	return -1;
	sf::Vector2f pos = rectangles[ind].getPosition();
	sf::Vector2f diff;

    for (int i=0; i<numBlocks; i++) {
    	diff = rectangles[i].getPosition() - pos;
        if (i == ind) {
        	// skip
        }
        else if (std::abs(diff.x) < squareSize || std::abs(diff.y) < squareSize) {
        	return i;
        }
    }

	return -1;
}

bool Board::blocksAreSame(int ind, int collInd) {
	return (blocks[ind].value == blocks[collInd].value);
}

bool Board::allDoneMoving() {
	for (int i=0; i<numBlocks; i++) {
    	if (!doneMoving[i]) {
    		return false;
    	}
    }
    return true;
}

bool Board::moveAll() {
	int newRow, newCol, blockInd, repCol, repRow;
	int collInd;
	bool moved = false;
	// move(+right, +down)
	memset(doneMoving, false, numSquares * sizeof(bool));

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
		while (!allDoneMoving()) {
			// move every box delta to the left
			for (int ind=0; ind<numBlocks; ind++) {
				if (!doneMoving[ind]) {
					rectangles[ind].move(-speed, 0.0f);

					if (isWallCollision(ind)) {
						printf("%d: wall collision\n", ind);
						// if wall is detected, move back, incr doneCount
						rectangles[ind].move(speed, 0.0f);
						doneMoving[ind] = true;
					}
					else {
						collInd = getBlockCollision(ind);
						printf("got %d and %d colliding\n", ind, collInd);
						if (collInd == -1) {
							printf("%d: no collision\n", ind);
							// no collision, accept move
							moved = true;
						}
						else if (blocksAreSame(ind, collInd)) {
							printf("%d: block combining\n", ind);
							// collide and combines 
							blocks[collInd].combine();
							// remove block from board
							blocks[ind].makeInactive();
							numSpaces++;
							numBlocks--;

							moved = true;
						}
						else {
							printf("%d: block collision\n", ind);
							// if different value, move back, incr doneCount
							rectangles[ind].move(speed, 0.0f);
							doneMoving[ind] = true;
						}
					}
				}
			}
			update();
		}
		


		// // start at left, each col, call move on each block
		// for (int col=1; col<4; col++) {
		// 	for (repCol = col; repCol>=0; repCol--) {
		// 		for (int row=0; row<4; row++) {
		// 			if (occupied[row][repCol] != empty) { 
		// 				// if there is a block here
		// 				blockInd = occupied[row][repCol];
		// 				newRow = row;
		// 				newCol = repCol-1;
		// 				sf::Vector2f newPos = rectangles[blockInd].getPosition();
		// 				newPos.x -= squareSize;
		// 				if (move(blockInd, row, repCol, newRow, newCol, newPos)) {
		// 					moved = true;
		// 				}
		// 				// update();
		// 			}
		// 			printf("starting update all\n");
		// 			animateAll();
		// 			printf("ending update all\n");
		// 		}
		// 	}
			
		// }
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        // start at right
        for (int col=2; col>=0; col--) {
        	for (repCol = col; repCol<4; repCol++) {
				for (int row=0; row<4; row++) {
					if (occupied[row][repCol] != empty) {
						// if there is a block here
						blockInd = occupied[row][repCol];
						newRow = row;
						newCol = repCol+1;
						sf::Vector2f newPos = rectangles[blockInd].getPosition();
						newPos.x += squareSize;
						if (move(blockInd, row, repCol, newRow, newCol, newPos)) {
							moved = true;
						}
					}
				}
			}
		}
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
    	// start at top
		for (int row=1; row<4; row++) {
			for (repRow = row; repRow>=0; repRow--) {
				for (int col=0; col<4; col++) {
					if (occupied[repRow][col] != empty) {
						// if there is a block here
						blockInd = occupied[repRow][col];
						newRow = repRow-1;
						newCol = col;
						sf::Vector2f newPos = rectangles[blockInd].getPosition();
						newPos.y -= squareSize;
						if (move(blockInd, repRow, col, newRow, newCol, newPos)) {
							moved = true;
						}
					}
				}
			}
		}
    }                	
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
        // start at bottom
        for (int row=2; row>=0; row--) {
			for (repRow = row; repRow<4; repRow++) {
				for (int col=0; col<4; col++) {
					if (occupied[repRow][col] != empty) {
						// if there is a block here
						blockInd = occupied[repRow][col];
						newRow = repRow+1;
						newCol = col;
						sf::Vector2f newPos = rectangles[blockInd].getPosition();
						newPos.y += squareSize;
						if (move(blockInd, repRow, col, newRow, newCol, newPos)) {
							moved = true;
						}
					}
				}
			}
		}
    }
    return moved;
}
