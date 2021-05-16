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
		entities.push_back(new EntityMesh());
		init = found + 1;
		found = text.find(",", found + 1);
		cad = text.substr(init, found - init);
		entities[i]->mesh = Mesh::Get(cad.c_str());
		entities[i]->id = i;
		scene->entities.push_back(entities[i]);
	}
}

void IntroStage::createPlayer() {
	Scene* scene = Game::instance->intro_scene;
	string text = "data/Character.ASE";
	string cad;
	int found = -1;
	int init = 0;
	for (int i = 0; i < MAX_CHARACTERS; i++)
	{
		characters.push_back(new EntityPlayer());
		init = found + 1;
		found = text.find(",", found + 1);
		cad = text.substr(init, found - init);
		characters[i]->mesh = Mesh::Get(cad.c_str());
		characters[i]->id = i;
		scene->entities.push_back(characters[i]);
		if (i == 0) {
			characters[i]->model.translate(-20.0f, 0.0f, 0.0f);
			characters[i]->model.rotate(PI / 2, Vector3(0, 1, 0));

		}
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
	Scene* scene = Game::instance->intro_scene;
	for (int i = 0; i < scene->entities.size(); i++)
	{
		scene->entities[i]->update(seconds_elapsed);
	}
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
