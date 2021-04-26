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
	RIGHT,
	LEFT
};
struct sPlayer {
	Vector2 pos;
	int health ;
	eDirection dir;
	bool ismoving = false;
	int speed_fall;
	bool isjumping = false;
	int jumpAngle;
	int startY ;
	int posYMax ;
	int falldistance;

};
struct sAnimation {
	int current_animation;
	const int num_animations = 6;
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
struct sCharacterInfo {
	Vector2 pos;
	int health;
	eDirection dir;
};

struct sGameInfo {
	sCharacterInfo player;
	Vector2 PosCam;
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
class World {
	public:
	World();

	int button;
	Image font;
	Image minifont;
	Image fontWhite;
	Image tileset;
	Image tutorialTile;
	float Playtime;
	float Gametime;
	bool IsPlaytime;
	bool overAudio;

	const int player_velocity = 30;
	
	sAnimation animation;
	sCamera camera;
	sGameData myGame; //instance of the whole game
	void saveGameInfo();
	bool loadGameInfo();
	void ShowGameMap(GameMap* map, Image& framebuffer, Image& tileset);
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
	bool menu;
	bool end;
};

class TutorialStage : public Stage {
public:
	virtual void render(Image& framebuffer);
	virtual void update(double seconds_elapsed);

};

class OverStage : public Stage {
	virtual void render(Image& framebuffer);
	virtual void update(double seconds_elapsed);
};
struct sMapHeader {
	int w; //width of map
	int h; //height of map
	unsigned char bytes; //num bytes per cell
	unsigned char extra[7]; //filling bytes, not used
};


class Sprite {
public:
	Image sprite;
};