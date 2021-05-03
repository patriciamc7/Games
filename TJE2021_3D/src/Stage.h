
#ifndef STAGE_H
#define STAGE_H

#include "game.h"

class Stage {
public:
	virtual void render() {}; //empty body 
	virtual void update(double seconds_elapsed) {}; //empty body 
};

class IntroStage : public Stage {
public:
	virtual void render();
	virtual void update(double seconds_elapsed);
};

class PlayStage : public Stage {
public:
	virtual void render();
	virtual void update(double seconds_elapsed);
};


#endif 
