#pragma once

#include <random>

#include "map.h"

class DungeonGenerator
{
public:
 
	int Seed;
	int XSize, YSize;
	int MaxFeatures;
	int ChanceRoom;
 
	DungeonGenerator();
	DungeonGenerator(int XSize, int YSize, int MaxFeatures);
 
	Map Generate();
 
private:
 
	typedef std::mt19937 RngT;
 
	int GetRandomInt(RngT& rng, int min, int max) const;
 
	Direction GetRandomDirection(RngT& rng) const;
 
	bool MakeCorridor(Map& map, RngT& rng, int x, int y, int maxLength, Direction direction) const;
 
	bool MakeRoom(Map& map, RngT& rng, int x, int y, int xMaxLength, int yMaxLength, Direction direction) const;
 
	bool MakeFeature(Map& map, RngT& rng, int x, int y, int xmod, int ymod, Direction direction) const;
 
	bool MakeFeature(Map& map, RngT& rng) const;
 
	bool MakeStairs(Map& map, RngT& rng, Tile tile) const;

	bool MakeDungeon(Map& map, RngT& rng) const;
 
};