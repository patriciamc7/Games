
#ifndef STAGE_H
#define STAGE_H

#include "Scene.h"
#include "framework.h"


#define MAX_ENT_INTRO 3
#define MAX_CHARACTERS 1
class Stage {
public:
	virtual void createEntities() {};
	virtual void render() {}; //empty body 
	virtual void update(double seconds_elapsed) {}; //empty body 
	virtual void createPlayer() {};
};

class IntroStage : public Stage {
public:
	vector<EntityPlayer*> characters;
	vector<EntityMesh*> entities;
	
	virtual void createEntities();
	virtual void createPlayer();
	virtual void render();
	virtual void update(double seconds_elapsed);
};

class PlayStage : public Stage {
public:
	virtual void createPlayer();
	virtual void createEntities();
	virtual void render();
	virtual void update(double seconds_elapsed);
};



#endif 
