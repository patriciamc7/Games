
#ifndef STAGE_H
#define STAGE_H

#include "game.h"
#define MAX_ENT_INTRO 3
class Stage {
public:
	virtual void createEntities() {};
	virtual void render() {}; //empty body 
	virtual void update(double seconds_elapsed) {}; //empty body 
};

class IntroStage : public Stage {
public:
	//vector<EntityMesh*> aux;
	//EntityMesh* RightDoor;
	//EntityMesh* LeftDoor;
	//EntityMesh* ArcDoor;

	virtual void createEntities();
	virtual void render();
	virtual void update(double seconds_elapsed);
};

class PlayStage : public Stage {
public:
	virtual void createEntities();
	virtual void render();
	virtual void update(double seconds_elapsed);
};


#endif 
