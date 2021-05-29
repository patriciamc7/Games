#include "Stage.h"
#include "includes.h"
#include "camera.h"
#include "utils.h"
#include "game.h"

void IntroStage::createTextures()
{
	Scene* scene = Game::instance->intro_scene;
	string texture = "data/Door_BaseColor.tga,data/cielo.tga,data/ground.tga,data/intro.tga";
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

// 0 der puerta ,1  iz puerta ,2 arco puerta ,3 cielo ,4 suelo ,5 iglesia
void IntroStage::createEntities()
{
	Scene* scene = Game::instance->intro_scene;
	string mesh = "data/RightDoor.ase,data/LeftDoor.ase,data/ArcDoor.ase,data/cielo.ASE,data/intro.ase";
	string cad;
	int found = -1;
	int init = 0; 
	for (int i = 0; i < MAX_ENT_INTRO; i++) 
	{
		entities.push_back(new EntityMesh());

		if (i == 4) {
			entities[i]->mesh->createPlane(2000);
			entities[i]->tiling = 40.0f;
		}
		else {
			init = found + 1;
			found = mesh.find(",", found + 1);
			cad = mesh.substr(init, found - init);
			entities[i]->mesh = Mesh::Get(cad.c_str());
		}
		entities[i]->id = i;
		scene->entities.push_back(entities[i]);

		if (i == 1)
			scene->entities[i+1]->model.translate(0.0f,0.0f,-31.0f);
		if (i == 5)
		{
			scene->entities[i + 1]->model.translate(11.0f, 0.0f, -12.0f);
		}
	}
	createTextures();
}


void IntroStage::render()
{

	Camera* camera = Game::instance->camera;
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
		game->CurrentScene = game->PlayScene;
		game->current_stage->createEntities();
	}
	
	if (this->animation)
	{	
		float radLeftDoor = -90 * DEG2RAD * seconds_elapsed;
		float radRightDoor= 90 * DEG2RAD * seconds_elapsed;

		if (this->firstTime) {
			Timeanimation = game->time;
			this->firstTime = false;
		}
		if (game->time - Timeanimation < 1.2f){
			scene->entities[2]->model.rotate(radLeftDoor, Vector3(0.0f, 1.0f, 0.0f));
			scene->entities[1]->model.rotate(radRightDoor, Vector3(0.0f, 1.0f, 0.0f));
		}
	
	}
		
}

void PlayStage::createTextures()
{
	Scene* scene = Game::instance->PlayScene;
	string texture = "data/mirror.tga,data/imShader/water.tga";
	string cad;
	int found = -1;
	int init = 0;
	
	for (int i = 0; i < MAX_ENT_PLAY; i++)
	{
		init = found + 1;
		found = texture.find(",", found + 1);
		cad = texture.substr(init, found - init);

		if (this->entities[i]->id == 2){
			this->entities[i]->texture2 = Texture::Get("data/imShader/cloud.tga");
		}
		
		this->entities[i]->texture = Texture::Get(cad.c_str());
		this->entities_mirror[i]->texture = Texture::Get(cad.c_str());

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
	int playerNum = scene->entities.size(); 
	for (int i = 0; i < MAX_ENT_PLAY; i++)
	{
		this->entities.push_back(new EntityMesh());
		this->entities_mirror.push_back(new EntityMesh());

		init = found + 1;
		found = mesh.find(",", found + 1);
		cad = mesh.substr(init, found - init);

		this->entities[i]->id = i + playerNum;
		this->entities_mirror[i]->id = i+ playerNum;

		if (this->entities[i]->id != 2) {
			this->entities[i]->mesh = Mesh::Get(cad.c_str());
			this->entities_mirror[i]->mesh = Mesh::Get(cad.c_str());
		}
		if (this->entities[i]->id == 1) {
			this->entities[i]->model.rotate(PI / 2, Vector3(0, 1, 0));
			this->entities_mirror[i]->model.rotate(PI / 2, Vector3(0, 1, 0));
		}
		if (this->entities[i]->id == 2) {
			this->entities[i]->mesh->createPlane(50);
			this->entities_mirror[i]->mesh->createPlane(50);
			this->entities[i]->model.rotate(0 * DEG2RAD, Vector3(1.0f, 0.0f, 0.0f));
			this->entities_mirror[i]->model.rotate(0 * DEG2RAD, Vector3(1.0f, 0.0f, 0.0f));
		
		}
		scene->entities.push_back(this->entities[i]);
		this->entities_mirror[i]->model.translate(0.0f, 0.0f, 20.0f);
		this->entities_mirror[i]->model.rotate(PI, Vector3(0, 1, 0));
		scene->entities_mirror.push_back(this->entities_mirror[i]);

	}
	createTextures();
	

}

void PlayStage::renderWater(int i)
{

	EntityMesh* water;
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	Camera* camera = Camera::current;
	water = this->entities[i - 1];
	water->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/water.fs");
	water->shader->enable();
	water->shader->setUniform("u_model", water->model);
	water->shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	water->shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	water->shader->setUniform("u_time", Game::instance->time);

	water->shader->setUniform("u_texture2", water->texture, 1);
	water->shader->setUniform("u_texture", water->texture2, 0);
	water->shader->setUniform("u_texture_tiling", 1.0f);

	////render the 
	water->mesh->render(GL_TRIANGLES);
	water->shader->disable();
	glDisable(GL_BLEND);
}

void PlayStage::render()
{
	

	Scene* scene = Game::instance->PlayScene;
	for (int i = 0; i < scene->entities.size(); i++)
	{
		if (scene->entities[i]->id == 2) {
			renderWater(i);
		}
		else
			scene->entities[i]->render();
	}

	for (int i = 0; i < scene->entities_mirror.size(); i++)
	{
		if (scene->entities_mirror[i]->id != 2) //2 es el suelo water, no lo renderizamos en la realidad mirror
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



void TitleStage::createEntities()
{
	menu = new EntityMesh();
	menu->mesh->createPlane(100);
	menu->model.rotate(90 * DEG2RAD, Vector3(1.0f, 0.0f, 0.0f));
	menu->texture = Texture::Get("data/inspeculo.tga");
}


void TitleStage::render()
{
	Camera* camera = new Camera();
	
	menu->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/phong.fs");
	menu->shader->enable();
	menu->shader->setUniform("u_model", menu->model);
	menu->shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	menu->shader->setUniform("u_color", Vector4(1, 1, 1, 1));

	menu->shader->setUniform("u_texture", menu->texture, 0);
	menu->shader->setUniform("u_texture_tiling", 1.0f);

	////render the 
	menu->mesh->render(GL_TRIANGLES);
	menu->shader->disable();

}



void TitleStage::update(double seconds_elapsed)
{
	Game* game = Game::instance;

	if (Input::wasKeyPressed(SDL_SCANCODE_I)) {
		game->current_stage = game->intro_stage;
		game->current_stage->createEntities();
	}

}
