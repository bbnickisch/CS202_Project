#include "project.h"
#include <iostream>
#include <fstream>

Actor::Actor() {
	health_ = 10;
}

void Actor::setHealth(int &x) {
	health_ = x;
}

void Actor::damage(int x) {
	health_ -= x;
}

int Actor::getHealth() {
	return health_;
}

void Actor::heal(int x)
{
	health_ += x;
}

// Reverse DIRECTION
DIRECTION& operator-(DIRECTION& dir) //reverse DIRECTION
{
	switch (dir)
	{
	case DIRECTION::RIGHT:
		dir = DIRECTION::LEFT;
		break;
	case DIRECTION::UP:
		dir = DIRECTION::DOWN;
		break;
	case DIRECTION::LEFT:
		dir = DIRECTION::RIGHT;
		break;
	case DIRECTION::DOWN:
		dir = DIRECTION::UP;
		break;
		//default:
		//dir remains the same
	}

	return dir;
}

// Counterclockwise rotation of DIRECTION
DIRECTION& operator++(DIRECTION& dir) // increment (counterclockwise)
{
	switch (dir)
	{
	case DIRECTION::RIGHT:
		dir = DIRECTION::UP;
		break;
	case DIRECTION::UP:
		dir = DIRECTION::LEFT;
		break;
	case DIRECTION::LEFT:
		dir = DIRECTION::DOWN;
		break;
	case DIRECTION::DOWN:
		dir = DIRECTION::RIGHT;
		break;
		//default:
		//dir remains the same
	}

	return dir;
}

// Postfix Counterclockwise rotation of DIRECTION
DIRECTION operator++(DIRECTION& dir, int) // Postfix increment (counterclockwise)
{
	DIRECTION temp = dir;

	++dir;

	return temp;

}

// Clockwise rotation of DIRECTION
DIRECTION& operator--(DIRECTION& dir) // decrement (clockwise)
{
	switch (dir)
	{
	case DIRECTION::RIGHT:
		dir = DIRECTION::DOWN;
		break;
	case DIRECTION::UP:
		dir = DIRECTION::RIGHT;
		break;
	case DIRECTION::LEFT:
		dir = DIRECTION::UP;
		break;
	case DIRECTION::DOWN:
		dir = DIRECTION::LEFT;
		break;
		//default:
		//dir remains the same
	}

	return dir;
}

// Postfix Clockwise rotation of DIRECTION
DIRECTION operator--(DIRECTION& dir, int) //Postfix decrement (clockwise)
{
	DIRECTION temp = dir;

	--dir;

	return temp;
}

DIRECTION intToEnum(int ii)
{
	return static_cast<DIRECTION>(ii);
}

char * enumToString(TILESETFILE tile) {
	switch (tile) {
	case TILESETFILE::FOREST:
		return "tileset.png";

	case TILESETFILE::CAVE:
		return "tilese_cave.png";

	default:
		return "";
	}
}

// Convert TILESETFILE to const String
const std::string enumToFileString(TILESETFILE tile) {
	switch (tile) {
	case TILESETFILE::FOREST:
		return "tileset.png";

	case TILESETFILE::CAVE:
		return "tilese_cave.png";

	default:
		return "";
	}
}

bool TileMap::load(const std::string& tileset, sf::Vector2u tileSize, std::vector<int> tiles, unsigned int width, unsigned int height)
{
	// load the tileset texture
	if (!m_tileset.loadFromFile(tileset))
		return false;

	// resize the vertex array to fit the level size
	m_vertices.setPrimitiveType(sf::Quads);
	m_vertices.resize(width * height * 4);

	// populate the vertex array, with one quad per tile
	for (unsigned int i = 0; i < width; ++i)
		for (unsigned int j = 0; j < height; ++j)
		{
			// get the current tile number
			int tileNumber = tiles[i + j * width];

			// find its position in the tileset texture
			int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
			int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

			// get a pointer to the current tile's quad
			sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

			// define its 4 corners
			quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
			quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
			quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
			quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

			// define its 4 texture coordinates
			quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
			quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
			quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
			quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
		}

	m_tiles.resize(0);

	m_tiles.clear(); // this might not reallocate memory

	for (auto it = tiles.begin(); it != tiles.end(); ++it)
	{
		m_tiles.push_back(*it);
	}

	return true;
}

std::vector<int>& TileMap::getTiles()
{
	return m_tiles;
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// apply the transform
	states.transform *= getTransform();

	// apply the tileset texture
	states.texture = &m_tileset;

	// draw the vertex array
	target.draw(m_vertices, states);
}

std::string TileMap::toString()
{
	std::stringstream temp;

	temp << std::to_string(m_tiles.size()) + '\n'; // Size of vector m_tiles

	for (auto it = m_tiles.begin(); it != m_tiles.end() - 1; ++it)
	{
		temp << std::to_string(*it) << ' '; // every element but last element in m_tiles
	}

	temp << std::to_string(m_tiles.back()); // last element in m_tiles

	return temp.str();
}

sf::Vector2i& GameObject::getGameCoordinates()
{
	return m_gameCoordinates;
}

sf::Sprite& GameObject::getObject()
{
	return m_obj;
}

void GameObject::setGameCoordinates(sf::Vector2i gameCoordinates)
{
	m_gameCoordinates = gameCoordinates;
	return;
}

void GameObject::setObject(sf::Sprite& obj)
{
	m_obj = obj;
	return;
}

void GameObject::setSpritePosition(float x, float y)
{
	m_obj.setPosition(x, y);
}

void GameObject::setSpritePosition(const sf::Vector2f& position)
{
	m_obj.setPosition(position);
}

std::string GameObject::toString()
{
	return std::to_string(getGameCoordinates().x) + ' ' + std::to_string(getGameCoordinates().y);
}

void GameObject::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(m_obj, states);
	return;
}

void ActorObject::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	GameObject::draw(target, states);

	// TODO Draw different sprites here
}

DIRECTION ActorObject::getDirectionFacing()
{
	return m_directionFacing;
}

void ActorObject::setDirectionFacing(DIRECTION dir)
{
	if (m_rects.size() > (int)dir && getObject().getTextureRect() != m_rects[(int)dir])
	{
		// access the enum values int value which needs to correspond to their order in the vector m_rects
		getObject().setTextureRect(m_rects[(int)dir]); // TODO maybe might not work
	}

	m_directionFacing = dir;
}

void ActorObject::setSpritePosition(float x, float y)
{
	getObject().setPosition(x, y);
}

void ActorObject::setSpritePosition(const sf::Vector2f& position)
{
	getObject().setPosition(position);
}

std::string ActorObject::toString()
{
	return std::to_string(getGameCoordinates().x) + ' ' + std::to_string(getGameCoordinates().y) + ' ' + std::to_string(getHealth()) + ' ' + std::to_string((int)getDirectionFacing());
}

std::vector<sf::IntRect>& ActorObject::getRects()
{
	return m_rects;
}

bool Level::load(const std::string& tileset, sf::Vector2u tileSize, std::vector<int> tiles, unsigned int width, unsigned int height)
{
	if (m_map.load(tileset, tileSize, tiles, width, height))
	{
		m_tileSize.x = tileSize.x;
		m_tileSize.y = tileSize.y;
		m_sizeOfTileMap.x = width;
		m_sizeOfTileMap.y = height;
		m_tileSetFile = fileStringToEnum(tileset);
		return true;
	}

	std::cout << "Failed to load" << std::endl;
	return false;
}

TILESETFILE fileStringToEnum(const std::string str)
{
	if (str.compare(enumToFileString(TILESETFILE::FOREST)) == 0)
	{
		return TILESETFILE::FOREST;
	}
	else if (str.compare(enumToFileString(TILESETFILE::CAVE)) == 0)
	{
		return TILESETFILE::CAVE;
	}
	else
		std::cout << "Unable to covert string " << str << " to TILESETFILE" << std::endl;
		return (TILESETFILE) 0;
}

void Level::saveGame(Level& level) {
	std::ofstream file("save.txt");
	file << level.toString() << std::endl;
}


void Level::loadGame(Level& level) {
	//Create all needed variables
	std::string itemSizeString;
	std::string tempItemXCoorString;
	std::string tempItemYCoorString;
	std::string placeHolder;
	std::string enemySizeString;
	std::string tempEnemyXCoorString;
	std::string tempEnemyYCoorString;
	std::string tempEnemyHealthString;
	std::string tempEnemyDirectionFacingString;
	std::string playerXCoorString;
	std::string playerYCoorString;
	std::string playerHealthString;
	std::string playerDirectionFacingString;
	std::string stairsXCoorString;
	std::string stairsYCoorString;
	std::string tileListSizeString;
	std::string tileTypeString;
	std::string levelWidthString;
	std::string levelHeightString;
	std::string tileSetFileString;

	std::ifstream file;
	file.open("save.txt");
	if (file.is_open()) {

		file >> itemSizeString;
		int itemSizeInt = atoi(itemSizeString.c_str());

		//resize item vector
		level.m_Items.resize(itemSizeInt);

		for (int i = 0; i < itemSizeInt; ++i)
		{
			file >> tempItemXCoorString;
			file >> tempItemYCoorString;

			int tempItemXCoorInt = atoi(tempItemXCoorString.c_str());
			int tempItemYCoorInt = atoi(tempItemYCoorString.c_str());

			// Set Game Coordinates
			level.m_Items[i].setGameCoordinates(sf::Vector2i{ tempItemXCoorInt, tempItemYCoorInt });

			// Actually change the drawing coordinates of the Sprite to match the GameCoordinates
			level.m_Items[i].getObject().setPosition(sf::Vector2f{ 1.0f * tempItemXCoorInt * level.m_tileSize.x , 1.0f * tempItemYCoorInt * level.m_tileSize.y });
		}

		//read in enemy values
		file >> enemySizeString;
		int enemySizeInt = atoi(enemySizeString.c_str());

		//resize enemy vector
		level.m_Enemies.resize(enemySizeInt);

		for (int i = 0; i < enemySizeInt; ++i)
		{
			//read in enemy attributes as strings
			file >> tempEnemyXCoorString;
			file >> tempEnemyYCoorString;
			file >> tempEnemyHealthString;
			file >> tempEnemyDirectionFacingString;

			//Convert string enemy attributes to int values
			int tempEnemyXCoorInt = atoi(tempEnemyXCoorString.c_str());
			int tempEnemyYCoorInt = atoi(tempEnemyYCoorString.c_str());
			int tempEnemyHealthInt = atoi(tempEnemyHealthString.c_str());
			int tempEnemyDirectionFacingInt = atoi(tempEnemyDirectionFacingString.c_str());

			// Sets game attributes for m_Enemies[i]
			level.m_Enemies[i].setGameCoordinates(sf::Vector2i{ tempEnemyXCoorInt, tempEnemyYCoorInt });
			level.m_Enemies[i].setHealth(tempEnemyHealthInt);
			level.m_Enemies[i].setDirectionFacing(intToEnum(tempEnemyDirectionFacingInt));

			// Actually change the drawing coordinates of the Sprite to match the GameCoordinates
			level.m_Enemies[i].getObject().setPosition(sf::Vector2f{ 1.0f * tempEnemyXCoorInt * level.m_tileSize.x , 1.0f * tempEnemyYCoorInt * level.m_tileSize.y });
		}

		//read in Player values
		file >> playerXCoorString;
		file >> playerYCoorString;
		file >> playerHealthString;
		file >> playerDirectionFacingString;

		//Convert string Player attributes to int values
		int playerXCoorInt = atoi(playerXCoorString.c_str());
		int playerYCoorInt = atoi(playerYCoorString.c_str());
		int playerHealthInt = atoi(playerHealthString.c_str());
		int playerDirectionFacingInt = atoi(playerDirectionFacingString.c_str());

		//Set Player game attributes
		level.m_Player.setGameCoordinates(sf::Vector2i{ playerXCoorInt, playerYCoorInt });
		level.m_Player.setHealth(playerHealthInt);
		level.m_Player.setDirectionFacing(intToEnum(playerDirectionFacingInt));

		// Actually change the drawing coordinates of the Sprite to match the GameCoordinates
		level.m_Player.getObject().setPosition(sf::Vector2f{ 1.0f * playerXCoorInt * level.m_tileSize.x , 1.0f * playerYCoorInt * level.m_tileSize.y });

		//Read in Stair Coordinates
		file >> stairsXCoorString;
		file >> stairsYCoorString;

		//Set String Stair Coordinates as Ints
		int stairsXCoorInt = atoi(stairsXCoorString.c_str());
		int stairsYCoorInt = atoi(stairsXCoorString.c_str());

		//Set m_staris gameCoordinates
		level.m_stairs.setGameCoordinates(sf::Vector2i{ stairsXCoorInt, stairsYCoorInt });

		// Actually change the drawing coordinates of the Sprite to match the GameCoordinates
		level.m_stairs.getObject().setPosition(sf::Vector2f{ 1.0f * stairsXCoorInt * level.m_tileSize.x , 1.0f * stairsYCoorInt * level.m_tileSize.y });

		//Read in size of Vector for map
		file >> tileListSizeString;

		//sets tileSizeString to int value
		int tileListSizeInt = atoi(tileListSizeString.c_str());

		//Load vector with old map tiles
		std::vector<int> map;

		for (int i = 0; i < tileListSizeInt; ++i) {
			file >> tileTypeString;
			int tileTypeInt = atoi(tileTypeString.c_str());
			map.push_back(tileTypeInt);
		}

		file >> levelWidthString;
		file >> levelHeightString;

		int levelWidth = atoi(levelWidthString.c_str());
		int levelHeight = atoi(levelHeightString.c_str());

		file >> tileSetFileString;

		int tileSetFileInt = atoi(tileSetFileString.c_str());

		TileMap tempTileMap;
		level.getTileMap() = tempTileMap;
		level.load(enumToFileString((TILESETFILE) tileSetFileInt), sf::Vector2u{32, 32}, map, levelWidth, levelHeight);

		std::cout << "Load:" << std::endl << level.toString() << std::endl;
	}

}

std::vector<GameObject>& Level::getItems()
{
	return m_Items;
}

std::vector<ActorObject>& Level::getEnemies()
{
	return m_Enemies;
}

GameObject& Level::getStairs()
{
	return m_stairs;
}

ActorObject& Level::getPlayer()
{
	return m_Player;
}

int Level::getTileNum(int x, int y)
{
	return m_map.getTiles()[x + y*m_sizeOfTileMap.x];
}

bool Level::tileIsWalkable(sf::Vector2i coordinates)
{
	return tileIsWalkable(coordinates.x, coordinates.y);
}

// Change this function to change which tiles are walkable
bool Level::tileIsWalkable(int x, int y)
{
	return (getTileNum(x, y) != 0);
}

void Level::addItem(GameObject& obj)
{
	m_Items.push_back(obj);
}

void Level::addEnemy(ActorObject& obj)
{
	m_Enemies.push_back(obj);
}

void Level::setPlayer(ActorObject& obj)
{
	m_Player = obj;
}

void Level::setStairs(GameObject& stairs)
{
	m_stairs = stairs;
}

void Level::enemy_AI_Movement()
{
	// Move all enemies in the vector
	for (auto it = m_Enemies.begin(); it != m_Enemies.end(); ++it)
	{
		// You don't want zombies to always turn right when they hit a wall right?
		bool counterClockwiseRotation = (bool)rand() % 2;

		int direction = rand() % 5; // direction ranges from [0,5) with 0 as right, 3 as down and 4 as rest one turn

		if (direction == 4) continue; // continue the loop "this enemy does nothing this turn"

		for (int directionLoop = 0; directionLoop < 4; ++directionLoop)
		{
			if (direction == 0 && ifCanThenMove_Actor(*it, DIRECTION::RIGHT))
				break;
			if (direction == 1 && ifCanThenMove_Actor(*it, DIRECTION::UP))
				break;
			if (direction == 2 && ifCanThenMove_Actor(*it, DIRECTION::LEFT))
				break;
			if (direction == 3 && ifCanThenMove_Actor(*it, DIRECTION::DOWN))
				break;

			if (counterClockwiseRotation)
			{
				if (directionLoop != 3) // last valid direction counterclockwise from right (or zero)
					++direction;
				else
					direction = 0;
			}
			else // go clockwise
			{
				if (direction != 0) // first last valid direction clockwise from down (or three)
					--direction;
				else
					direction = 3;
			}
		}
	}
}

bool Level::canMove_Enemy(int index, DIRECTION dir)
{
	m_Enemies[index].setDirectionFacing(dir);
	return canMove(m_Enemies[index], dir);
}

bool Level::canMove_Player(DIRECTION dir)
{
	m_Player.setDirectionFacing(dir);
	return canMove(m_Player, dir);
}

bool Level::canMove(GameObject& obj, DIRECTION dir)
{
	int x = obj.getGameCoordinates().x;
	int y = obj.getGameCoordinates().y;
	int tileMapWidth = m_sizeOfTileMap.x;
	int tileMapHeight = m_sizeOfTileMap.y;

	switch (dir)
	{
	case DIRECTION::RIGHT:
		return (x < tileMapWidth - 1) && tileIsWalkable(x + 1, y); // Not on right side
	case DIRECTION::UP:
		return (y > 0) && tileIsWalkable(x, y - 1); // Not on top
	case DIRECTION::LEFT:
		return (x > 0) && tileIsWalkable(x - 1, y); // Not on left side
	case DIRECTION::DOWN:
		return (y < tileMapHeight - 1) && tileIsWalkable(x, y + 1); // Not on bottom
	default:
		std::cout << "Invalid direction." << std::endl;
		return false;
	}
}

void Level::move_Enemy(int index, DIRECTION dir)
{
	move(m_Enemies[index], dir);
}

void Level::move_Player(DIRECTION dir)
{
	m_Player.setDirectionFacing(dir);
	move(m_Player, dir);
}

void Level::move(GameObject& obj, DIRECTION dir)
{
	sf::Vector2i moveGameCoordinates{ 0, 0 };
	int tileWidth = m_tileSize.x;
	int tileHeight = m_tileSize.y;
	sf::Vector2f position;
	sf::Vector2f move{ 0, 0 };

	switch (dir)
	{
	case DIRECTION::LEFT:
		moveGameCoordinates.x = -1;
		move.x = -tileWidth;
		break;
	case DIRECTION::RIGHT:
		moveGameCoordinates.x = 1;
		move.x = tileWidth;
		break;
	case DIRECTION::DOWN:
		moveGameCoordinates.y = 1;
		move.y = tileHeight;
		break;
	case DIRECTION::UP:
		moveGameCoordinates.y = -1;
		move.y = -tileHeight;
		break;
	}

	obj.getGameCoordinates() += moveGameCoordinates;
	obj.getObject().setPosition(obj.getObject().getPosition().x + move.x, obj.getObject().getPosition().y + move.y);
}

bool Level::ifCanThenMove_Enemy(int index, DIRECTION dir)
{
	m_Enemies[index].setDirectionFacing(dir);
	return ifCanThenMove(m_Enemies[index], dir);
}

bool Level::ifCanThenMove_Player(DIRECTION dir)
{
	return ifCanThenMove(m_Player, dir);
}

bool Level::ifCanThenMove_Actor(ActorObject& obj, DIRECTION dir)
{
	obj.setDirectionFacing(dir);
	return ifCanThenMove(obj, dir);
}

bool Level::ifCanThenMove(GameObject& obj, DIRECTION dir)
{
	if (canMove(obj, dir))
	{
		move(obj, dir);
		return true;
	}

	return false;
}

bool Level::isPlayerWhereEnemiesAre()
{
	for (auto it = m_Enemies.begin(); it != m_Enemies.end(); ++it)
	{
		if (m_Player.getGameCoordinates() == it->getGameCoordinates())
			return true;
	}
	return false;
}

bool Level::isPlayerWhereDoorIs()
{
	return m_Player.getGameCoordinates() == m_stairs.getGameCoordinates();
}

void Level::playerAttack()
{
	std::vector<sf::Vector2i> setOfTargetedGameCoordinates;

	// Add cleave stuff and such here
	switch (m_Player.getDirectionFacing())
	{
	case::DIRECTION::RIGHT:
		std::cout << "ATTACK RIGHT" << std::endl;
		setOfTargetedGameCoordinates.push_back(m_Player.getGameCoordinates() + sf::Vector2i{ 1, 0 });
		break;
	case::DIRECTION::UP:
		std::cout << "ATTACK UP" << std::endl;
		setOfTargetedGameCoordinates.push_back(m_Player.getGameCoordinates() + sf::Vector2i{ 0, -1 });
		break;
	case::DIRECTION::LEFT:
		std::cout << "ATTACK LEFT" << std::endl;
		setOfTargetedGameCoordinates.push_back(m_Player.getGameCoordinates() + sf::Vector2i{ -1, 0 });
		break;
	case::DIRECTION::DOWN:
		std::cout << "ATTACK DOWN" << std::endl;
		setOfTargetedGameCoordinates.push_back(m_Player.getGameCoordinates() + sf::Vector2i{ 0, 1 });
		break;
	default:
		return;
	}

	// Iterate through all targeted Game Coordinates
	for (auto outerIT = setOfTargetedGameCoordinates.begin(); outerIT != setOfTargetedGameCoordinates.end(); ++outerIT)
	{
		// Check all target coordinates to see if any Enemies are at those Game Coordinates
		if (outerIT->x >= 0 && outerIT->x < m_sizeOfTileMap.x && outerIT->y >= 0 && outerIT->y < m_sizeOfTileMap.y)
		{
			// Iterate through the list of Enemies
			for (auto it = m_Enemies.begin(); it != m_Enemies.end(); ++it)
			{
				// If the Enemies coordinates are equal to the current target coordinate
				if (it->getGameCoordinates() == *outerIT)
				{
					it->damage(1); // Only one damage for now  // TODO Variable damage
					break; // Only one enemy per game coordinate (in theory)
				}
			}
		}
	}
}

TileMap& Level::getTileMap()
{
	return m_map;
}

void Level::setTileMap(TileMap& map)
{
	m_map = map;
}

sf::Vector2u Level::getTileSize()
{
	return m_tileSize;
}

void Level::levelGen()
{
	std::vector<int> level(128);
	int sum = 0;
	while (sum<140) {
		sum = 0;
		for (int i = 1; i<128; i++) {
			level[i] = (rand() % 3);
		}
		for (int i = 0; i<128; i++) {
			sum += level[i];
		}
	}
	level[0] = 1;
	level[1] = 1;
	level[2] = 1;
	level[17] = 1;

	TileMap newMap;
	m_map = newMap;
	load(enumToFileString(TILESETFILE::CAVE), sf::Vector2u{ 32, 32 }, level, 16, 8);

	// TODO player and enemy rand pos
	randPos(m_Player);

	for (auto it = m_Enemies.begin(); it != m_Enemies.end(); ++it)
	{
		do
		{
			randPos(*it);
		} while (m_Player.getGameCoordinates() == it->getGameCoordinates());
	}
}

void Level::randPos(ActorObject & obj)
{
	int xCoor;
	int yCoor;
	
	while (true) {//This prevents the new position to be on an impassible space	
		xCoor = rand() % m_sizeOfTileMap.x;
		yCoor = rand() % m_sizeOfTileMap.y;
	
		if (m_map.getTiles()[xCoor + yCoor * m_sizeOfTileMap.x] != 0)
		{
			moveToCoordinates(obj, sf::Vector2i{xCoor, yCoor});
			return;
		}
	}
}

void Level::moveToCoordinates(GameObject& obj, sf::Vector2i coordinates)
{
	obj.setGameCoordinates(coordinates);
	obj.getObject().setPosition(coordinates.x*m_tileSize.x, coordinates.y*m_tileSize.y);
}

void Level::moveToCoordinates(GameObject& obj,int x, int y)
{
	moveToCoordinates(obj, sf::Vector2i{ x, y });
}

void Level::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform = getTransform();
	// draw tile map here
	target.draw(m_map, states);

	target.draw(m_stairs);

	for (auto it = m_Items.begin(); it != m_Items.end(); ++it)
	{
		target.draw(*it, states);
	}

	for (auto it = m_Enemies.begin(); it != m_Enemies.end(); ++it)
	{
		target.draw(*it, states);
	}

	target.draw(m_Player);
	return;
}

std::string Level::toString()
{
	std::stringstream sStream;

	sStream << m_Items.size() << '\n';

	if (m_Items.size() != 0)
	{
		for (auto it = m_Items.begin(); it != m_Items.end() - 1; ++it)
			sStream << it->toString() << ' ';

		sStream << m_Items.back().toString();
	}

	sStream << '\n';

	sStream << m_Enemies.size() << '\n';

	if (m_Enemies.size() != 0)
	{
		for (auto it = m_Enemies.begin(); it != m_Enemies.end() - 1; ++it)
			sStream << it->toString() << ' ';

		sStream << m_Enemies.back().toString();
	}

	sStream << '\n';

	sStream << m_Player.toString() << '\n';

	sStream << m_stairs.toString() << '\n';

	sStream << m_map.toString() << '\n';

	sStream << m_sizeOfTileMap.x << ' ' << m_sizeOfTileMap.y << '\n';

	sStream << (int) m_tileSetFile << '\n';

	return sStream.str();
}