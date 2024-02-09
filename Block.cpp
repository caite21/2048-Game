#include "Block.hpp"


Block::Block() {
	this->colorIndex = 0;
	this->value = 2;
	this->needMove = false;
}

Block::~Block() {

}

void Block::add(sf::RectangleShape * rect) {
	square = rect;
	square->setSize(sf::Vector2f(squareSize, squareSize));
	square->setOrigin(squareSize/2, squareSize/2);
    square->setFillColor(colorArr[colorIndex]);
    square->setPosition(squareSize/2, squareSize/2);
	return;
}

void Block::makeActive(int row, int col) {
	square->setPosition(squareSize/2 + squareSize*col, squareSize/2 + squareSize*row);
	this->active = true;
	this->value = 2;
	return;
}

void Block::makeInactive() {
	this->active = false;
	this->value = 0;
	this->colorIndex = 0;
	square->setFillColor(colorArr[colorIndex]);
	return;
}

void Block::combine() {
	this->value = value*2;
	colorIndex++;
	if(colorIndex >= 6) {
		colorIndex = 0;
	}
	square->setFillColor(colorArr[colorIndex]);
}