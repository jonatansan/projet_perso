#include "dungeonGenerator.h"
#include "map.h"

 
	DungeonGenerator::DungeonGenerator():
		Seed(std::random_device()()),
		XSize(80), YSize(80),
		MaxFeatures(1000),
		ChanceRoom(100) { }

	DungeonGenerator::DungeonGenerator(int XSize, int YSize, int MaxFeatures):
		Seed(std::random_device()()),
		XSize(XSize),
		YSize(YSize),
		MaxFeatures(MaxFeatures),
		ChanceRoom(100) { }
 
	Map DungeonGenerator::Generate()
	{
		// TODO: proper input validation.
		//assert(MaxFeatures > 0 && MaxFeatures <= 1000);
		//assert(XSize > 3 && XSize <= 80);
		//assert(YSize > 3 && YSize <= 80);
 
		auto rng = RngT(Seed);
		auto map = Map(XSize, YSize, Tile::Unused);
 
		MakeDungeon(map, rng);
 
		return map;
	}
 
	int DungeonGenerator::GetRandomInt(RngT& rng, int min, int max) const
	{
		return std::uniform_int_distribution<int>(min, max)(rng);
	}
 
	Direction DungeonGenerator::GetRandomDirection(RngT& rng) const
	{
		return Direction(std::uniform_int_distribution<int>(0, 3)(rng));
	}
 
	bool DungeonGenerator::MakeCorridor(Map& map, RngT& rng, int x, int y, int maxLength, Direction direction) const
	{
		assert(x >= 0 && x < XSize);
		assert(y >= 0 && y < YSize);
 
		assert(maxLength > 0 && maxLength <= std::max(XSize, YSize));
 
		auto length = GetRandomInt(rng, 2, maxLength);
 
		auto xStart = x;
		auto yStart = y;
 
		auto xEnd = x;
		auto yEnd = y;
 
		if (direction == Direction::North)
			yStart = y - length;
		else if (direction == Direction::East)
			xEnd = x + length;
		else if (direction == Direction::South)
			yEnd = y + length;
		else if (direction == Direction::West)
			xStart = x - length;
 
		if (!map.IsXInBounds(xStart) || !map.IsXInBounds(xEnd) || !map.IsYInBounds(yStart) || !map.IsYInBounds(yEnd))
			return false;
 
		if (!map.IsAreaUnused(xStart, yStart, xEnd, yEnd))
			return false;
 
		map.SetCells(xStart, yStart, xEnd, yEnd, Tile::Corridor);
 
		std::cout << "Corridor: ( " << xStart << ", " << yStart << " ) to ( " << xEnd << ", " << yEnd << " )" << std::endl;
 
		return true;
	}
 
	bool DungeonGenerator::MakeRoom(Map& map, RngT& rng, int x, int y, int xMaxLength, int yMaxLength, Direction direction) const
	{
		// Minimum room size of 4x4 tiles (2x2 for walking on, the rest is walls)
		auto xLength = GetRandomInt(rng, 4, xMaxLength);
		auto yLength = GetRandomInt(rng, 4, yMaxLength);
 
		auto xStart = x;
		auto yStart = y;
 
		auto xEnd = x;
		auto yEnd = y;
 
		if (direction == Direction::North)
		{
			yStart = y - yLength;
			xStart = x - xLength / 2;
			xEnd = x + (xLength + 1) / 2;
		}
		else if (direction == Direction::East)
		{
			yStart = y - yLength / 2;
			yEnd = y + (yLength + 1) / 2;
			xEnd = x + xLength;
		}
		else if (direction == Direction::South)
		{
			yEnd = y + yLength;
			xStart = x - xLength / 2;
			xEnd = x + (xLength + 1) / 2;
		}
		else if (direction == Direction::West)
		{
			yStart = y - yLength / 2;
			yEnd = y + (yLength + 1) / 2;
			xStart = x - xLength;
		}
 
		if (!map.IsXInBounds(xStart) || !map.IsXInBounds(xEnd) || !map.IsYInBounds(yStart) || !map.IsYInBounds(yEnd))
			return false;
 
		if (!map.IsAreaUnused(xStart, yStart, xEnd, yEnd))
			return false;
 
		map.SetCells(xStart, yStart, xEnd, yEnd, Tile::DirtWall);

		map.SetCell(xStart, yStart, Tile::Wall_UpLeft_Corner);
		map.SetCell(xStart, yEnd, Tile::Wall_DownLeft_Corner);
		map.SetCell(xEnd, yStart, Tile::Wall_UpRight_Corner);
		map.SetCell(xEnd, yEnd, Tile::Wall_DownRight_Corner);
		
		for(int i = xStart + 1; i < xEnd; ++i) { map.SetCell(i, yStart, Wall_Hor); }
		for(int i = xStart + 1; i < xEnd; ++i) { map.SetCell(i, yEnd, Wall_Hor); }
		for(int j = yStart + 1; j < yEnd; ++j) { map.SetCell(xStart, j, Wall_Ver); }
		for(int j = yStart + 1 ; j < yEnd; ++j) { map.SetCell(xEnd, j, Wall_Ver); }

		map.SetCells(xStart + 1, yStart + 1, xEnd - 1, yEnd - 1, Tile::DirtFloor);
 
		//std::cout << "Room: ( " << xStart << ", " << yStart << " ) to ( " << xEnd << ", " << yEnd << " )" << std::endl;
 
		return true;
	}
 
	bool DungeonGenerator::MakeFeature(Map& map, RngT& rng, int x, int y, int xmod, int ymod, Direction direction) const
	{
		// Choose what to build
		auto chance = GetRandomInt(rng, 0, 100);
 
		if (chance <= ChanceRoom)
		{
			if (MakeRoom(map, rng, x + xmod, y + ymod, 20, 20, direction))
			{
				map.SetCell(x, y, Tile::Door);
 
				// Remove wall next to the door.
				map.SetCell(x + xmod, y + ymod, Tile::DirtFloor);
 
				return true;
			}
 
			return false;
		}
		else
		{
			if (MakeCorridor(map, rng, x + xmod, y + ymod, 6, direction))
			{
				map.SetCell(x, y, Tile::Door);
 
				return true;
			}
 
			return false;
		}
	}
 
	bool DungeonGenerator::MakeFeature(Map& map, RngT& rng) const
	{
		auto tries = 0;
		auto maxTries = 1000;
 
		for( ; tries != maxTries; ++tries)
		{
			// Pick a random wall or corridor tile.
			// Make sure it has no adjacent doors (looks weird to have doors next to each other).
			// Find a direction from which it's reachable.
			// Attempt to make a feature (room or corridor) starting at this point.
 
			int x = GetRandomInt(rng, 1, XSize - 2);
			int y = GetRandomInt(rng, 1, YSize - 2);
 
			if (!map.isWall(map, x, y) && map.GetCell(x, y) != Tile::Corridor)
				continue;
 
			if (map.IsAdjacent(x, y, Tile::Door))
				continue;
 
			if (map.GetCell(x, y+1) == Tile::DirtFloor || map.GetCell(x, y+1) == Tile::Corridor)
			{
				if (MakeFeature(map, rng, x, y, 0, -1, Direction::North))
					return true;
			}
			else if (map.GetCell(x-1, y) == Tile::DirtFloor || map.GetCell(x-1, y) == Tile::Corridor)
			{
				if (MakeFeature(map, rng, x, y, 1, 0, Direction::East))
					return true;
			}
			else if (map.GetCell(x, y-1) == Tile::DirtFloor || map.GetCell(x, y-1) == Tile::Corridor)
			{
				if (MakeFeature(map, rng, x, y, 0, 1, Direction::South))
					return true;
			}
			else if (map.GetCell(x+1, y) == Tile::DirtFloor || map.GetCell(x+1, y) == Tile::Corridor)
			{
				if (MakeFeature(map, rng, x, y, -1, 0, Direction::West))
					return true;
			}
		}
 
		return false;
	}
 
	bool DungeonGenerator::MakeStairs(Map& map, RngT& rng, Tile tile) const
	{
		auto tries = 0;
		auto maxTries = 10000;
 
		for ( ; tries != maxTries; ++tries)
		{
			int x = GetRandomInt(rng, 1, XSize - 2);
			int y = GetRandomInt(rng, 1, YSize - 2);
 
			if (!map.IsAdjacent(x, y, Tile::DirtFloor) && !map.IsAdjacent(x, y, Tile::Corridor))
				continue;
 
			if (map.IsAdjacent(x, y, Tile::Door))
				continue;
 
			map.SetCell(x, y, tile);
 
			return true;
		}
 
		return false;
	}

	bool DungeonGenerator::MakeDungeon(Map& map, RngT& rng) const
	{
		// Make one room in the middle to start things off.
		MakeRoom(map, rng, XSize / 2, YSize / 2, 8, 6, GetRandomDirection(rng));
 
		for (auto features = 1; features != MaxFeatures; ++features)
		{
			if (!MakeFeature(map, rng))
			{
				std::cout << "Unable to place more features (placed " << features << ")." << std::endl;
				break;
			}
		}
 
		if (!MakeStairs(map, rng, Tile::UpStairs))
			std::cout << "Unable to place up stairs." << std::endl;
 
		if (!MakeStairs(map, rng, Tile::DownStairs))
			std::cout << "Unable to place down stairs." << std::endl;
 
		return true;
	}
