#pragma once

#include <assert.h>
#include <iostream>
#include <vector>

enum Tile
{
	Unused = 8,
	DirtWall,
	Wall_Ver = 200,
	Wall_Hor = 181,
	DirtFloor = 88,
	Wall_DownLeft_Corner = 220,
	Wall_DownRight_Corner = 222,
	Wall_UpLeft_Corner = 180,
	Wall_UpRight_Corner = 182,
	Corridor,
	Door = 65,
	UpStairs = 3,
	DownStairs = 10
};
 
enum class Direction
{
	North, South, East, West,
};
 
class Map
{
public:
 
	Map();
	Map(int x, int y, Tile value);
 
	void SetCell(int x, int y, Tile celltype);

	void SetCells(int xStart, int yStart, int xEnd, int yEnd, Tile cellType);

	Tile GetCell(int x, int y) const;
 
	bool IsXInBounds(int x) const;

	bool IsYInBounds(int y) const;

	bool IsAreaUnused(int xStart, int yStart, int xEnd, int yEnd);

	bool IsAdjacent(int x, int y, Tile tile);
 
	void Print() const;

	bool isWall(Map& map, int x, int y) const;
 
private:
 
	int xSize, ySize;
 
	std::vector<Tile> data;
};