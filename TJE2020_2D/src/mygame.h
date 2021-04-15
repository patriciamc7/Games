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
	SWORD
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

	struct sPlayer {
		Vector2 pos;
		float player_velocity = 50;
	};
	sPlayer player;
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

//class Sprite : public Image {
//	Vector2 pos;
//	Image* sprite; //ang, escala, opacidad...
//};