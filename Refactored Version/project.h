#ifndef _PROJECT_
#define _PROJECT_


#include <SFML/Graphics.hpp>
#include <math.h>
#include <string>
#include <iostream>
#include <sstream>
#include <ctime>
#include <vector>
#include <iterator>

class Actor
{
public:
	Actor();
	void setHealth(int & x);
	void damage(int x);
	int getHealth();
	void heal(int);

private:
	int health_;
	//int movespeed_;
};

//class Player: public Actor
//{
//public:
//	
//};
//class Enemy : public Actor
//{
//public:
//	void updatePosition(sf::Sprite &s,std::vector<int> level);
//};



/**************************************************************/

class cScreen
{
public:
	virtual int Run(sf::RenderWindow &App) = 0;
};

// Direction enum
// Needs to correspond to the order put into the array for different direction sprites
enum class DIRECTION { RIGHT, UP, LEFT, DOWN };
enum class TILESETFILE { FOREST, CAVE };

class TileMap : public sf::Drawable, public sf::Transformable
{
public:
	bool load(const std::string& tileset, sf::Vector2u tileSize, std::vector<int> tiles, unsigned int width, unsigned int height);

	std::vector<int>& getTiles();
	std::string toString();

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	std::vector<int> m_tiles;
	sf::VertexArray m_vertices;
	sf::Texture m_tileset;
};

class GameObject : public sf::Drawable, public sf::Transformable
{
public:
	virtual sf::Vector2i& getGameCoordinates();
	virtual sf::Sprite& getObject();

	virtual void setGameCoordinates(sf::Vector2i);
	virtual void setObject(sf::Sprite&);
	void setSpritePosition(float x, float y);
	void setSpritePosition(const sf::Vector2f&);

	virtual std::string toString();
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
private:
	sf::Vector2i m_gameCoordinates;
	sf::Sprite m_obj; // Error if unique
};

class ActorObject : public GameObject, public Actor
{
public:
	DIRECTION getDirectionFacing();

	std::vector<sf::IntRect>& getRects();
	void setDirectionFacing(DIRECTION);

	void setSpritePosition(float x, float y);
	void setSpritePosition(const sf::Vector2f&);

	std::string toString();
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
private:
	std::vector<sf::IntRect> m_rects; // list of rectangular vertices of sprites in texture in DIRECTION order
	DIRECTION m_directionFacing;
};


class Level : public sf::Drawable, public sf::Transformable
{
public:
	bool load(const std::string& tileset, sf::Vector2u tileSize, std::vector<int> tiles, unsigned int width, unsigned int height);
	static void saveGame(Level& level);
	static void loadGame(Level& level);

	std::vector<GameObject>& getItems();
	std::vector<ActorObject>& getEnemies();
	GameObject& getStairs();
	ActorObject& getPlayer();
	int getTileNum(int x, int y);
	bool tileIsWalkable(sf::Vector2i);
	bool tileIsWalkable(int x, int y);
	void addItem(GameObject&);
	void addEnemy(ActorObject&);
	void setPlayer(ActorObject&);
	void setStairs(GameObject&);
	void enemy_AI_Movement();
	bool canMove_Enemy(int index, DIRECTION);
	bool canMove_Player(DIRECTION);
	bool canMove(GameObject&, DIRECTION);
	void move_Enemy(int index, DIRECTION);
	void move_Player(DIRECTION);
	void move(GameObject&, DIRECTION);
	//void moveToCoordinates(GameObject&, sf::Vector2i);
	//void moveToCoordinates(GameObject&, int x, int y);
	bool ifCanThenMove_Enemy(int index, DIRECTION);
	bool ifCanThenMove_Actor(ActorObject&, DIRECTION);
	bool ifCanThenMove_Player(DIRECTION);
	bool ifCanThenMove(GameObject&, DIRECTION);
	bool isPlayerWhereEnemiesAre();
	bool isPlayerWhereDoorIs();
	void playerAttack();
	TileMap& getTileMap();
	void setTileMap(TileMap&);
	sf::Vector2u getTileSize();
	//void Level::randPos(GameObject& obj);
	void randPos(ActorObject&);
	void levelGen();
	void moveToCoordinates(GameObject& obj, sf::Vector2i coordinates);
	void moveToCoordinates(GameObject& obj, int x, int y);

	std::string toString();

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
	std::vector<GameObject> m_Items;
	std::vector<ActorObject> m_Enemies;
	ActorObject m_Player;
	GameObject m_stairs;
	TileMap m_map;
	sf::Vector2u m_tileSize;
	sf::Vector2i m_sizeOfTileMap;
	TILESETFILE m_tileSetFile;
};

// Special Operators for directions
DIRECTION& operator-(DIRECTION&); //reverse DIRECTION
DIRECTION& operator++(DIRECTION&); // increment (counterclockwise)
DIRECTION operator++(DIRECTION&, int); // Postfix increment (counterclockwise)
DIRECTION& operator--(DIRECTION&); // decrement (clockwise)
DIRECTION operator--(DIRECTION&, int); //Postfix decrement (clockwise)
DIRECTION intToEnum(int);

const std::string enumToFileString(TILESETFILE);
TILESETFILE fileStringToEnum(const std::string);

/**************************************************************/

#endif