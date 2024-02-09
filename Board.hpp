#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string.h> 
#include "Block.cpp"

class Board {
public:
	Board(sf::RenderWindow* window);
	~Board();

	static const int numSquares = 16;
	sf::RectangleShape rectangles[numSquares];
	Block blocks[numSquares];
	sf::Vector2f positions[numSquares];

	int empty = -1;
	int occupied[4][4] = { { -1, -1, -1, -1 },
                           { -1, -1, -1, -1 },
                           { -1, -1, -1, -1 },
                           { -1, -1, -1, -1 } };

	int numBlocks;
	int numSpaces;
	int score;
	sf::RenderWindow* window;
	float speed;
	int numNeedMove;
	int doneCount;
	bool doneMoving[numSquares];

	bool isWall(int row, int col);
	bool move(int blockInd, int row, int col, int newRow, int newCol, sf::Vector2f newPos);
	bool moveAll();
	void printOccupied();
	void printValues();
	void spawn();
	void update();
	void animateMove(int blockInd, sf::Vector2f newPos);
	void animateAll();

	bool isWallCollision(int i);
	int getBlockCollision(int ind);
	bool blocksAreSame(int ind, int collInd);
	bool allDoneMoving();

	/*
	Should have:
	- when nothing moves, don't spawn
	- max of 1 combining 
	- spawn with value of 2 OR 4
	- animations on movement, combining, and spawning
	*/

private:

};