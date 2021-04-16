#pragma once 
#include "image.h"

class mygame {}; 
enum eCellType : uint8 {
	EMPTY, START,
	WALL,
	DOOR,
	CHEST
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

class World {
	public:
	World();

	Image font;
	Image minifont;
	Image sprite;
	Image tileset;
	struct Camera {
		Vector2 position;
		int velocity;
	};
	struct sPlayer {
		Vector2 pos;
		float player_velocity = 50;
		int health ;
	};
	struct sItem {
		int type; //which item type
		int extra; //to store extra info
	};

	struct sCell { //every cell of the world
		int type; //which type (rock,water,etc)
		int item; //which item is here
		int inventory[16]; //max 16 items

	};
	struct sGameData {
		sItem items; //available 
		sPlayer players[2];
	};
	Camera camera;
	sGameData myGame; //instance of the whole game

};

class Stage { 
	public: 
	virtual void render(Image& framebuffer) {}; //empty body 
	virtual void update(double seconds_elapsed) {}; //empty body 
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

};
struct sMapHeader {
	int w; //width of map
	int h; //height of map
	unsigned char bytes; //num bytes per cell
	unsigned char extra[7]; //filling bytes, not used
};
