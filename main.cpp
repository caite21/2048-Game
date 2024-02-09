/*
	2048 Project main file

to compile:
g++ -o my_program main.cpp -I"C:/Users/caite/Documents/Projects/Tetris/SFML-2.5.1/include" -L"C:/Users/caite/Documents/Projects/Tetris/SFML-2.5.1/lib" -lsfml-graphics -lsfml-window -lsfml-system
// g++ -Wall -g main.cpp -o main.exe

to run exe, also need:
sfml-system-2.dll and windows and graphics similarily 
*/
#include <iostream>
#include <string.h> // for strcmp, strcpy
#include <SFML/Graphics.hpp>
#include "Board.cpp"


int main() {
	printf("Hi there\n");
	int windowHeight = 800;
	int windowWidth = 800;
    int squareSize = 200.0f;
    int numSquares = 16;

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "My 2048", sf::Style::Close | sf::Style::Titlebar);
    window.setFramerateLimit(60);
    // sf::Clock clock;
    // sf::Time deltaTime;
    Board board(&window);

    // main game loop
    while(window.isOpen()) {
        sf::Event evnt;
        // look for event
        while(window.pollEvent(evnt)) {
        	switch (evnt.type) {
        		case sf::Event::Closed:
        			// if window exit is pressed
                	window.close();
                	break;
                case sf::Event::KeyPressed:
                    printf("key pressed\n");
                    if (board.moveAll()) {
                        board.spawn();
                    }
                    
                    board.printOccupied();
                    printf("\n");
                    board.printValues();
                    printf("\n");

        	}
        }

        // if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        //     // get mouse position
        //     sf::Vector2i mousePos = sf::Mouse::getPosition(window); // mouse.x and y
        //     // square.setPosition((float)mousePos.x, (float)mousePos.y);
        // }

        // https://www.youtube.com/watch?v=l2iCYCLi6MU&list=PL21OsoBLPpMOO6zyVlxZ4S4hwkY_SLRW9&index=13

        // window.clear();
        board.update();    
        // window.display();
    }

    return 0;
}