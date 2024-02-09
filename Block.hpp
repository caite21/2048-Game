#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string.h> 

const int windowHeight = 800;
const int windowWidth = 800;
const int squareSize = 200.0f;


class Block {
public:
	Block();
	~Block();
	void add(sf::RectangleShape * rect);
	void makeActive(int row, int col);
	void makeInactive();
	void combine();
	
	sf::RectangleShape * square;
	static const int numSquares = 16;
	bool active = false;
    
	sf::Color colorArr[6] = {
		{249,38,114,255}, 
		{102,217,239,255},
		{166,226,46,255},
		{253,151,31,255},
		{230,219,116,255},
		{174,129,255,255}	
	};
	
	int colorIndex;
	int value;
	sf::Vector2f nextPos;
	bool needMove;

// pink       F92672 / 249,38,114
// blue       66D9EF / 102,217,239
// green      A6E22E / 166,226,46
// orange     FD971F / 253,151,31
// yellow     E6DB74 / 230,219,116
// purple     AE81FF / 174,129,255



private:


};