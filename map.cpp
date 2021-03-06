#include "map.h"

	Map::Map():
		xSize(0), ySize(0),
		data() { }
 
	Map::Map(int x, int y, Tile value = Tile::Unused):
		xSize(x), ySize(y),
		data(x * y, value) { }
 
	void Map::SetCell(int x, int y, Tile celltype)
	{
		assert(IsXInBounds(x));
		assert(IsYInBounds(y));
 
		data[x + xSize * y] = celltype;
	}
 
	Tile Map::GetCell(int x, int y) const
	{
		assert(IsXInBounds(x));
		assert(IsYInBounds(y));
 
		return data[x + xSize * y];
	}
 
	void Map::SetCells(int xStart, int yStart, int xEnd, int yEnd, Tile cellType)
	{
		assert(IsXInBounds(xStart) && IsXInBounds(xEnd));
		assert(IsYInBounds(yStart) && IsYInBounds(yEnd));
 
		assert(xStart <= xEnd);
		assert(yStart <= yEnd);
 
		for (auto y = yStart; y != yEnd + 1; ++y)
			for (auto x = xStart; x != xEnd + 1; ++x)
				SetCell(x, y, cellType);
	}
 
	bool Map::IsXInBounds(int x) const
	{
		return x >= 0 && x < xSize;
	}
 
	bool Map::IsYInBounds(int y) const
	{
		return y >= 0 && y < ySize;
	}
 
	bool Map::IsAreaUnused(int xStart, int yStart, int xEnd, int yEnd)
	{
		assert(IsXInBounds(xStart) && IsXInBounds(xEnd));
		assert(IsYInBounds(yStart) && IsYInBounds(yEnd));
 
		assert(xStart <= xEnd);
		assert(yStart <= yEnd);
 
		for (auto y = yStart; y != yEnd + 1; ++y)
			for (auto x = xStart; x != xEnd + 1; ++x)
				if (GetCell(x, y) != Tile::Unused)
					return false;
 
		return true;
	}
 
	bool Map::IsAdjacent(int x, int y, Tile tile)
	{
		assert(IsXInBounds(x - 1) && IsXInBounds(x + 1));
		assert(IsYInBounds(y - 1) && IsYInBounds(y + 1));
 
		return 
			GetCell(x - 1, y) == tile || GetCell(x + 1, y) == tile ||
			GetCell(x, y - 1) == tile || GetCell(x, y + 1) == tile;
	}
 
	void Map::Print() const
	{
		// TODO: proper ostream iterator.
		// TODO: proper lookup of character from enum.
 
		for (auto y = 0; y != ySize; y++)
		{
			for (auto x = 0; x != xSize; x++)
			{

				std::cout<<GetCell(x,y);
				/*switch(GetCell(x, y))
				{
				case Tile::Unused:
					std::cout << "#";
					break;
				case Tile::DirtWall:
					std::cout << "#";
					break;
				case Tile::DirtFloor:
					std::cout << ".";
					break;
				case Tile::Corridor:
					std::cout << ".";
					break;
				case Tile::Door:
					std::cout << "D";
					break;
				case Tile::UpStairs:
					std::cout << "<";
					break;
				case Tile::DownStairs:
					std::cout << ">";
					break;
				};*/
			}
 
			std::cout << std::endl;
		}
 
		std::cout << std::endl;
	}

	bool Map::isWall(Map& map, int x, int y) const{
	
		return map.GetCell(x, y) == Tile::Wall_Hor || map.GetCell(x, y) == Tile::Wall_Ver;
	}
