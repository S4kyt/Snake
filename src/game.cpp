#include <iostream>
#include <thread>
#include <Windows.h>

#include "map.h"
#include "game.h"

struct SnakeNode {
public:
	int pos_x{ 0 };
	int pos_y{ 0 };
};

struct SnakeHead : SnakeNode {
};

Map* map = new Map;
SnakeHead* sHead = new SnakeHead;

void Game::init() {
	map->initMap();
	map->initApple();

	// SNAKE:
	// head
	sHead->pos_x = map->width / 2;
	sHead->pos_y = map->height / 2;
	// render sHead
	map->Map::boardArray[sHead->pos_x][sHead->pos_y] = 'X';
}

enum Direction {
	NOTMOVING,
	UP,
	DOWN,
	LEFT,
	RIGHT
};

void Game::update() {
	Direction dir = NOTMOVING;
	while (true)
	{
		// buffering
		// TODO: could improve by implementing double buffering, which might solve the blinking issue so i wouldn't have to disable the cursor
		std::wstring buffer;

		// limiting framerate
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		// sets cursor position back to the "home" position
		std::wcout << "\x1b[H" << std::flush;

		// illusion of movement 1/2
		// also turned out to be a clever solution to enable the apple spawning to continue, since missing the 'O' character from the array would mean we dont have any spawned yet.
		map->Map::boardArray[sHead->pos_x][sHead->pos_y] = '.';

		// MOVEMENT:
		// FIXME: movement still needs attention
		// Example: If we go up, we cant go down aka "reversing" cant happen.

		// input handling part of the program
		if (GetAsyncKeyState(VK_UP)) {
			dir = UP;
		}
		else if (GetAsyncKeyState(VK_DOWN)) {
			dir = DOWN;
		}
		else if (GetAsyncKeyState(VK_LEFT)) {
			dir = LEFT;
		}
		else if (GetAsyncKeyState(VK_RIGHT)) {
			dir = RIGHT;
		}
		else {
			dir = dir; // dir stays the same
		}

		switch (dir) {
		case UP:
			sHead->pos_x--;
			break;
		case DOWN:
			sHead->pos_x++;
			break;
		case LEFT:
			sHead->pos_y--;
			break;
		case RIGHT:
			sHead->pos_y++;
			break;
		}

		/*
		TODO:
		1. the snek collision, which shouldn't be too challenging, given any cell's value is "#", we got collision
		2. implementing my method of how the snake should move, node objects which point to the previous node's position, kinda like a linked list actually (yes i rly like this word)
		3. making appleh spawn again when its value got set back to '.' so i'll need to find a way to figure out if one of the cell's value is set to 'O' or no
		4. polish!
		*/

		// COLLISION:
		// TODO: still needs snek collision for when it curls into itself
		// also probaly a decent way to deal with collision, because even corners are handled this way, since both if's will be true if snek would somehow manage to get on the corner tile.
		// top and left
		if (sHead->pos_x == (map->width / map->width) - 1 || sHead->pos_y == (map->height / map->height) - 1) {
			break;
		}

		// bottom and right
		if (sHead->pos_x == (map->width - 1) || sHead->pos_y == (map->height - 1)) {
			break;
		}

		// illusion of movement 2/2
		// render position of new 'X'
		map->Map::boardArray[sHead->pos_x][sHead->pos_y] = 'X';

		// update game map
		for (int x = 0; x < map->width; x++) {
			buffer += '\n';
			for (int y = 0; y < map->height; y++) {
				buffer += map->Map::boardArray[x][y];
			}
		}
		std::wcout << buffer;
	}

	std::wcout << "You've died." << std::endl;
}

void Game::deinit() {
	delete map;
}