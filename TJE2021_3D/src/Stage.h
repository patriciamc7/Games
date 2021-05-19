
#ifndef STAGE_H
#define STAGE_H

#include "Scene.h"
#include "framework.h"


#define MAX_ENT_INTRO 3
#define MAX_ENT_PLAY 1 //comun mirror and reality
#define MAX_ENT_PLAY_MIRR 0 //only mirror
#define MAX_CHARACTERS 1
class Stage {
public:

	virtual void createEntities() {};
	virtual void createTextures() {};
	virtual void render() {}; //empty body 
	virtual void update(double seconds_elapsed) {}; //empty body 
};

class IntroStage : public Stage {
public:

	vector<EntityMesh*> entities;

	virtual void createTextures();
	virtual void createEntities();
	virtual void render();
	virtual void update(double seconds_elapsed);
};

class PlayStage : public Stage {
public:
	vector<EntityMesh*> entities;
	vector<EntityMesh*> entities_mirror;

	virtual void createTextures();
	virtual void createEntities();
	virtual void render();
	virtual void update(double seconds_elapsed);
};



#endif 
