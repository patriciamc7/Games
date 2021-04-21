#pragma once 
#include "image.h"

class mygame {}; 
enum eCellType {
	EMPTY,
	WALL,
	FLOOR
};

enum eItemType : uint8 {
	NOTHING,
	SWORD,
	POTION
};

struct sCell {
	eCellType type;
	eItemType item;
};

enum eDirection : uint8{
	DOWN,
	RIGHT,
	LEFT,
	UP
};
struct sPlayer {
	Vector2 pos;
	int health;
	eDirection dir;
	bool ismoving;

};
struct sAnimation {
	int current_animation;
	const int num_animations = 4;
	const int velocity_animation = 5;
};
struct sCamera {
	Vector2 position;
	const int velocity = 50;
};

struct sItem {
	int type; //which item type
	int extra; //to store extra info
};


struct sGameData {
	sItem items; //available 
	sPlayer players[2];
};
class World {
	public:
	World();

	int button;
	Image font;
	Image minifont;
	Image tileset;

	const int player_velocity = 30;
	
	sAnimation animation;
	sCamera camera;
	sGameData myGame; //instance of the whole game

};

class Stage { 
	public: 
	virtual void render(Image& framebuffer) {}; //empty body 
	virtual void update(double seconds_elapsed) {}; //empty body 
	virtual void restart() {};
}; 
					
class IntroStage : public Stage { 
	public: 
	
	
	virtual void render(Image& framebuffer);
	virtual void update(double seconds_elapsed);
};

class PlayStage : public Stage { 
	public: 
	virtual void render(Image& framebuffer);
	virtual void update(double seconds_elapsed);
	virtual void restart();
};

class OverStage : public Stage {
	virtual void render(Image& framebuffer);
	virtual void update(double seconds_elapsed);
	virtual void restart();
};
struct sMapHeader {
	int w; //width of map
	int h; //height of map
	unsigned char bytes; //num bytes per cell
	unsigned char extra[7]; //filling bytes, not used
};

class GameMap {
public:
	int width;
	int height;
	sCell* data;

	GameMap();

	GameMap(int w, int h);

	sCell& getCell(int x, int y);
	
	//example of parser of .map from rogued editor
	GameMap* loadGameMap(const char* filename);
	bool isValid(Vector2 target);
};

class Sprite {
public:
	Image sprite;
};