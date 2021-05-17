#include "Stage.h"
#include "includes.h"
#include "camera.h"
#include "utils.h"
#include "game.h"

void IntroStage::createTextures()
{
	Scene* scene = Game::instance->intro_scene;
	string texture = "data/Door_BaseColor.tga";
	string cad;
	int found = -1;
	int init = 0;
	for (int i = 0; i < MAX_ENT_INTRO; i++)
	{
		if (i==0 || i>2) {
			init = found + 1;
			found = texture.find(",", found + 1);
			cad = texture.substr(init, found - init);
		}
		
		entities[i]->texture = Texture::Get(cad.c_str());
	}
}

void IntroStage::createEntities()
{
	Scene* scene = Game::instance->intro_scene;
	string mesh = "data/RigtDoor.ASE,data/LeftDoor.ASE,data/ArcoDoor.ASE";
	string cad;
	int found = -1;
	int init = 0; 
	for (int i = 0; i < MAX_ENT_INTRO; i++)
	{
		entities.push_back(new EntityMesh());
		init = found + 1;
		found = mesh.find(",", found + 1);
		cad = mesh.substr(init, found - init);
		entities[i]->mesh = Mesh::Get(cad.c_str());
		entities[i]->id = i;
		scene->entities.push_back(entities[i]);
	}
	createTextures();
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
	Game* game = Game::instance;
	for (int i = 0; i < scene->entities.size(); i++)
	{
		scene->entities[i]->update(seconds_elapsed);
	}
	if (Input::isKeyPressed(SDL_SCANCODE_M))
	{
		game->current_stage = game->play_stage;
		game->current_stage->createEntities();
	}
		
}

void PlayStage::createTextures()
{
	Scene* scene = Game::instance->PlayScene;
	string texture = "data/mirror.tga";
	string cad;
	int found = -1;
	int init = 0;
	for (int i = 0; i < MAX_ENT_PLAY; i++)
	{
		if (i == 0 || i > 2) {
			init = found + 1;
			found = texture.find(",", found + 1);
			cad = texture.substr(init, found - init);
		}

		this->entities[i]->texture= Texture::Get(cad.c_str());
	}
}



void PlayStage::createEntities()
{
	Scene* scene = Game::instance->PlayScene;
	string mesh = "data/mirror.ASE";
	string mirror_mesh = "data/mirror.ASE";
	string cad;
	int found = -1;
	int init = 0;
	for (int i = 0; i < MAX_ENT_PLAY; i++)
	{
		this->entities.push_back(new EntityMesh());
		init = found + 1;
		found = mesh.find(",", found + 1);
		cad = mesh.substr(init, found - init);
		this->entities[i]->mesh = Mesh::Get(cad.c_str());
		this->entities[i]->id = i;
		scene->entities.push_back(this->entities[i]);
		this->entities_mirror.push_back(this->entities[i]);
		this->entities_mirror[i]->model.translate(20.0f,0.0f,0.0f);
	}

	createTextures();
	
	for (int i = MAX_ENT_PLAY; i < MAX_ENT_PLAY_MIRR; i++) //mirrored objects
	{
		this->entities_mirror.push_back(new EntityMesh());
		init = found + 1;
		found = mesh.find(",", found + 1);
		cad = mesh.substr(init, found - init);
		this->entities_mirror[i]->mesh = Mesh::Get(cad.c_str());
		this->entities_mirror[i]->id = i;
		scene->entities_mirror.push_back(this->entities_mirror[i]);
	}
//	createTextures();

}

void PlayStage::render()
{
	Scene* scene = Game::instance->PlayScene;

	for (int i = 0; i < scene->entities.size(); i++)
	{
		scene->entities[i]->render();
	}

	for (int i = 0; i < scene->entities_mirror.size(); i++)
	{
		scene->entities_mirror[i]->render();
	}

}

void PlayStage::update(double seconds_elapsed)
{

	Scene* scene = Game::instance->PlayScene;

	for (int i = 0; i < scene->entities.size(); i++)
	{
		scene->entities[i]->update(seconds_elapsed);
	}
	for (int i = 0; i < scene->entities_mirror.size(); i++)
	{
		scene->entities_mirror[i]->update(seconds_elapsed);
	}
}
