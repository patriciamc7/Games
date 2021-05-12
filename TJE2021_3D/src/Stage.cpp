#include "Stage.h"
#include "includes.h"
#include "camera.h"
#include "utils.h"
#include "game.h"

void IntroStage::createEntities()
{
	Scene* scene = Game::instance->intro_scene;
	string text = "data/RigtDoor.ASE,data/LeftDoor.ASE,data/ArcoDoor.ASE";
	string cad;
	int found = -1;
	int init = 0; 
	for (int i = 0; i < MAX_ENT_INTRO; i++)
	{
		aux.push_back(new EntityMesh());
		init = found + 1;
		found = text.find(",", found + 1);
		cad = text.substr(init, found - init);
		aux[i]->mesh = Mesh::Get(cad.c_str());
		scene->entities.push_back(aux[i]);
	}
}

void IntroStage::render()
{
	Scene* scene = Game::instance->intro_scene;
	for (int i = 0; i < scene->entities.size(); i++)
	{
		scene->entities[i]->render();
	}

}

void IntroStage::update(double seconds_elapsed)
{

}

void PlayStage::createEntities()
{
}

void PlayStage::render()
{
}

void PlayStage::update(double seconds_elapsed)
{
}
