
#ifndef STAGE_H
#define STAGE_H

#include "Scene.h"
#include "framework.h"


#define MAX_ENT_INTRO 6
#define MAX_ENT_PLAY 20 //comun mirror and reality
#define MAX_ENT_MIND 13 //comun mirror and reality
#define MAX_ENT_PLAY_MIRR 0 //only mirror
#define MAX_CHARACTERS 1
#define MAX_ENT_TITLE 1
#define MAX_LIGHTS 3

class Stage {
public:
	bool doorOpen2 = true;
	int glassCount = 0;
	bool animation = false;
	float Timeanimation = 0.0f;
	bool firstTime = true;
	bool animation2 = true;

	bool InitStageBody = false;
	virtual void createEntities() {};
	virtual void createTextures() {};
	virtual void render() {}; //empty body 
	virtual void update(double seconds_elapsed) {}; //empty body 
	void renderMirror(int i , vector<EntityMesh*> entities);

};

class TitleStage : public Stage {
public:
	enum eButton {
		START,
		SAFE,
		LOAD,
		CONTROLS,
		CONFIGURATION,
		EXIT
	};

	eButton button_type;
	EntityMesh* menu;
	virtual void createEntities();
	virtual void render();
	virtual void update(double seconds_elapsed);
};

class IntroStage : public Stage {
public:

	vector<EntityMesh*> entities;

	virtual void createTextures();
	virtual void createEntities();
	virtual void render();
	virtual void update(double seconds_elapsed);
};

class BodyStage : public Stage {
public:

	vector<EntityMesh*> entities;
	vector<EntityMesh*> entities_mirror;
	bool glass = false; 
	virtual void createTextures();
	virtual void createEntities();
	virtual void render();
	void renderWater(int i);
	void renderTorch(int i);
	void renderGui(); 
	virtual void update(double seconds_elapsed);
};

class MindStage : public Stage {
public:

	vector<EntityMesh*> entities;
	vector<EntityMesh*> entities_mirror;
	bool glass = false;
	int timeSpot = 0; 
	virtual void createTextures();
	virtual void createEntities();
	virtual void render();

	/*void renderTorch(int i);
	void renderMirror(int i);
	void renderGui();*/
	virtual void update(double seconds_elapsed);
	void ChangePosLight(); 
};

class EndStage : public Stage {
public:
	enum eButton {
		START,
		SAFE,
		LOAD,
		CONTROLS,
		CONFIGURATION,
		EXIT
	};

	eButton button_type;
	EntityMesh* menu;
	virtual void createEntities();
	virtual void render();
	virtual void update(double seconds_elapsed);
};



#endif 
