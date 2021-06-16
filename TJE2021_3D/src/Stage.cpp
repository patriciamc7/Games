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
			entities[i]->isColision = false;

		}
		else {
			init = found + 1;
			found = mesh.find(",", found + 1);
			cad = mesh.substr(init, found - init);
			entities[i]->mesh = Mesh::Get(cad.c_str());
		}
		entities[i]->id = i;
		scene->entities.push_back(entities[i]);

		if (i == 3) {
			entities[i]->isColision = false;
		}
		if (i == 2) {
			entities[i]->isColision = false;
			scene->entities[i + 1]->model.translate(0.0f, 0.0f, -15.5f);

		}
		if (i == 1)
			scene->entities[i+1]->model.translate(0.0f,0.0f,-31.0f);
		if (i == 5)
		{
			scene->entities[i + 1]->model.translate(11.0f, 0.0f, -12.0f);
			entities[i]->isColision = false;
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
		this->firstTime = true;
		game->CurrentScene->entities.clear();
		game->current_stage = game->body_stage;
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

void BodyStage::createTextures()
{
	Scene* scene = Game::instance->PlayScene;
	string texture = "data/imShader/water.tga,data/bathroom/ceiling.tga,data/bathroom/wall.tga,data/bathroom/bath.tga,data/bathroom/door.tga,data/bathroom/sink.tga,data/bathroom/sink.tga,data/bathroom/cabinet.tga,data/bathroom/cabinet.tga,data/bathroom/passage.tga,data/bathroom/passagePlane.tga,data/imShader/noise.tga";

	string cad;
	int found = -1;
	int init = 0;
	
	for (int i = 0; i < MAX_ENT_PLAY; i++)
	{
		init = found + 1;
		found = texture.find(",", found + 1);
		cad = texture.substr(init, found - init);

		if (this->entities[i]->id == 1){
			this->entities[i]->texture2 = Texture::Get("data/imShader/cloud.tga");
		}
		if (this->entities[i]->id == 12) {
			this->entities[i]->texture2 = Texture::Get("data/imShader/gray.tga");
		}
		this->entities[i]->texture = Texture::Get(cad.c_str());
		this->entities_mirror[i]->texture = Texture::Get(cad.c_str());

	}
}


//0 water 1celling 2 wall 3 bath 4 door 5 sink 6 sink1 7 cabin 8 cabin 1 9 passage 10 plano 11 mirror
void BodyStage::createEntities()
{
	Scene* scene = Game::instance->PlayScene;

	string mesh= "data/bathroom/ceiling.ASE,data/bathroom/wall.ASE,data/bathroom/bath.ASE,data/bathroom/door.ASE,data/bathroom/sink.ASE,data/bathroom/sink1.ASE,data/bathroom/cabinet.ASE,data/bathroom/cabinet1.ASE,data/bathroom/passage.ASE,data/glassBody.ASE";

	string cad;

	int found = -1;
	int init = 0;
	int playerNum = scene->entities.size(); 
	this->InitStageBody = true;
	for (int i = 0; i < MAX_ENT_PLAY; i++)
	{
		this->entities.push_back(new EntityMesh());
		this->entities_mirror.push_back(new EntityMesh());

		this->entities[i]->id = i + playerNum;
		this->entities_mirror[i]->id = i + playerNum;

		if (this->entities[i]->id != 1 && this->entities[i]->id != 11 && this->entities[i]->id != 13) {
			init = found + 1;
			found = mesh.find(",", found + 1);
			cad = mesh.substr(init, found - init);
			this->entities[i]->mesh = Mesh::Get(cad.c_str());
			this->entities_mirror[i]->mesh = Mesh::Get(cad.c_str());
		}
		/*if (this->entities[i]->id == 1) {
			this->entities[i]->model.rotate(PI / 2, Vector3(0, 1, 0));
			this->entities_mirror[i]->model.rotate(PI / 2, Vector3(0, 1, 0));
			this->entities_mirror[i]->model.translate(0.0f, 0.0f, 20.0f);
			this->entities_mirror[i]->model.rotate(PI, Vector3(0, 1, 0));
		}*/
		//if (this->entities[i]->id == 2) { //espejo
		//	this->entities[i]->mesh->createPlane(20);
		//	//this->entities_mirror[i] = this->entities[i];
		//	this->entities[i]->model.setRotation(90 * DEG2RAD, Vector3(0.0f, 0.0f, 1.0f));
		//	this->entities[i]->model.translate(-15,0,0);
		//	this->entities[i]->model.scale(0.9,1, 0.5);
		//}
		if (this->entities[i]->id == 1) {
			this->entities[i]->isColision = false;

			this->entities[i]->mesh->createPlane(30);
			this->entities[i]->model.translate(0, 0, 5);
			this->entities[i]->model.scale(1.3, 1, 0.9);
			this->entities_mirror[i]->mesh->createPlane(30);
			this->entities_mirror[i]->alpha = 1;
		}
		
		
		if (this->entities[i]->id == 2) //techo
		{
			this->entities[i]->model.translate(5, 0, -25);
			this->entities_mirror[i]->model.translate(5, 0, -25);
			this->entities[i]->isColision = false;

		}
		if (this->entities[i]->id == 3) //wall
		{
			this->entities[i]->isColision = false;
		}
		if (this->entities[i]->id == 4) //bath
		{
			this->entities[i]->model.translate(19, 0, 19);
			this->entities_mirror[i]->model.translate(19, 0, 19);
			this->entities[i]->isInteractive = true;

		}
		if (this->entities[i]->id == 5) //door
		{
			this->entities[i]->model.translate(2, 0, -25);
			this->entities_mirror[i]->model.translate(2, 0, -25);
		}
		if (this->entities[i]->id == 6) //sink1
		{
			this->entities[i]->model.translate(28, 0, 0);
			this->entities_mirror[i]->model.translate(28, 0, 0);
		}
		if (this->entities[i]->id == 7) //sink
		{
			this->entities[i]->model.translate(-15, 0, -15);
			this->entities_mirror[i]->model.translate(-15, 0, -15);
		}
		if (this->entities[i]->id == 8) //cabinet
		{
			this->entities[i]->model.translate(-10, 0, 19);
			this->entities[i]->model.rotate( -45*DEG2RAD, Vector3 (0, 1, 0));
			this->entities_mirror[i]->model.translate(-10, 0, 19);
			this->entities_mirror[i]->model.rotate(-45 * DEG2RAD, Vector3(0, 1, 0));


		}
		if (this->entities[i]->id == 9) //cabinet 1
		{
			this->entities[i]->model.translate(0, 0, -15);
			this->entities_mirror[i]->model.translate(0, 0, -15);
		}
		if (this->entities[i]->id < 8 && this->entities[i]->id > 0) {
			this->entities_mirror[i]->model = this->entities_mirror[i]->model.relfexion_x();
		}
		if (this->entities[i]->id > 7 && this->entities[i]->id < 12) {
			this->entities[i]->alpha = 1;
			this->entities_mirror[i]->model = this->entities_mirror[i]->model.relfexion_x();
		}
		if (this->entities[i]->id == 10) {
			this->entities_mirror[i]->alpha = 1;
			this->entities[i]->alpha = 0;
			this->entities[i]->isColision = false;
			this->entities[i]->model.translate(5, 0, -55);
			this->entities_mirror[i]->model.translate(5,0,-55);

		}
		if (this->entities[i]->id == 11) {
			this->entities[i]->mesh->createPlane(20);
			this->entities[i]->model.setRotation(90 * DEG2RAD, Vector3(0.0f, 0.0f, 1.0f));
			this->entities[i]->model.setRotation(90 * DEG2RAD, Vector3(1.0f, 0.0f,0.0f));
			this->entities_mirror[i]->alpha = 1;
			this->entities[i]->alpha = 0;
			this->entities[i]->model.translate(5, 80, 20);
			this->entities_mirror[i]->model.translate(5, 80, 20);
		}
		if (this->entities[i]->id == 12) { //trozo de espejo
			this->entities[i]->model.translate(-30, 0, 20);
			this->entities[i]->model.scale(0.2f, 0.2f, 0.2f);
			this->entities[i]->model.rotate(-45, Vector3(0,1,0));
			this->entities[i]->isInteractive = true;
		}
		if (this->entities[i]->id == 13) { //plano para antorchas
			this->entities[i]->mesh->createPlane(20);

			this->entities[i]->model.rotate(90 * DEG2RAD, Vector3(1.0f, 0.0f, 0.0f));
			this->entities[i]->model.rotate(90 * DEG2RAD, Vector3(0.0f, 0.0f, 1.0f));

			//this->entities[i]->model.scale(0.5f, 0.5f, 0.5f);

			this->entities[i]->model.translate(-90, -25, -1);

		}

		
		scene->entities.push_back(this->entities[i]);
		scene->entities_mirror.push_back(this->entities_mirror[i]);

	}
	createTextures();
}

void BodyStage::renderWater(int i)
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
	water->shader->setUniform("u_texture_tiling", 0.2f);

	////render the 
	water->mesh->render(GL_TRIANGLES);
	water->shader->disable();
	glDisable(GL_BLEND);
}
void BodyStage::renderTorch(int i)
{

	EntityMesh* torch;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	Camera* camera = Camera::current;
	torch = this->entities[i - 1];
	torch->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/torch.fs");
	torch->shader->enable();
	torch->shader->setUniform("u_model", torch->model);
	torch->shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	torch->shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	torch->shader->setUniform("u_time", Game::instance->time);
	//torch->shader->setUniform("u_resolution", Vector2(Game::instance->window_width, Game::instance->window_height));
	/*torch->shader->setUniform("u_texture2", torch->texture2, 1);
	torch->shader->setUniform("u_texture", torch->texture, 0);
	torch->shader->setUniform("u_texture_tiling", 1.0f);*/
	////render the 
	torch->mesh->render(GL_TRIANGLES);
	torch->shader->disable();
	glDisable(GL_BLEND);
}


void BodyStage::renderMirror(int i)
{
	Scene* scene = Game::instance->PlayScene;
	
	if (!animation2)
		glDisable(GL_DEPTH_TEST);
	EntityMesh* mirror;
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	Camera* camera = Camera::current;
	mirror = this->entities[i-1];
	mirror->mesh->renderBounding(mirror->model);
	mirror->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/mirror.fs");
	mirror->shader->enable();
	mirror->shader->setUniform("u_model", mirror->model);
	mirror->shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	mirror->shader->setUniform("u_color", Vector4(1, 1, 1, 1));

	mirror->shader->setUniform("u_texture_1", mirror->texture2, 5);
	mirror->shader->setUniform("u_texture", mirror->texture, 6);
	mirror->shader->setUniform("u_alpha", mirror->alpha);


	////render the 
	mirror->mesh->render(GL_TRIANGLES);
	mirror->shader->disable();
	glDisable(GL_BLEND);
	if (!animation2)
		glEnable(GL_DEPTH_TEST);
}

void BodyStage::render()
{

	Camera* camera = Camera::current;
	Scene* scene = Game::instance->PlayScene;
	for (int i = 0; i < scene->entities_mirror.size(); i++)
	{
		if ( scene->entities_mirror[i]->id != 1 && scene->entities_mirror[i]->id != 11 && scene->entities_mirror[i]->id != 12 && scene->entities_mirror[i]->id != 13)  //2 es el suelo water, no lo renderizamos en la realidad mirror
			scene->entities_mirror[i]->render();
		
	}
 	for (int i = 0; i < scene->entities.size(); i++)
	{
		if (scene->entities[i]->id == 1) {
			renderWater(i);
		}
		if (scene->entities[i]->id == 13) {
			renderTorch(i);
		}
		if (scene->entities[i]->id == 12) {
			renderMirror(i);
		}

		if (scene->entities[i]->id != 1 && scene->entities[i]->id != 12 && scene->entities[i]->id != 13 )
			scene->entities[i]->render();
	}
	renderGui(); 
}

void BodyStage::renderGui() {
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Game* game = Game::instance;

	Camera* cam2D = new Camera();
	cam2D->setOrthographic(0, game->window_width, game->window_height, 0, -1, 1);

	Mesh quad;
	quad.createQuad(100, 100, 100, 100, false);
	cam2D->enable();

	Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	shader->enable();

	shader->setUniform("u_model", Matrix44());
	shader->setUniform("u_viewprojection", cam2D->viewprojection_matrix);
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader->setUniform("u_texture_tiling", 1.0f);
	shader->setUniform("u_texture", Texture::Get("data/inspeculo.tga"), 0);

	quad.render(GL_TRIANGLES);

	shader->disable();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}

void BodyStage::update(double seconds_elapsed)
{

	Scene* scene = Game::instance->PlayScene;
	Game* game = Game::instance;

	for (int i = 0; i < scene->entities.size(); i++)
	{
		scene->entities[i]->update(seconds_elapsed);
	}
	for (int i = 0; i < scene->entities_mirror.size(); i++)
	{
		scene->entities_mirror[i]->update(seconds_elapsed);
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_I)) {
		game->CurrentScene->entities.clear();
		game->current_stage = game->end_stage;
		game->CurrentScene = game->EndScene;
		game->current_stage->createEntities();
	}
	if (this->animation)
	{
		float radDoor = 90 * DEG2RAD * seconds_elapsed;

		if (this->firstTime) {
			Timeanimation = game->time;
			this->firstTime = false;
		}
		if (game->time - Timeanimation < 1.2f && game->current_stage->animation2) {

			game->CurrentScene->entities[5]->isColision = false;
			scene->entities[5]->model.rotate(radDoor, Vector3(0.0f, 1.0f, 0.0f));
			scene->entities_mirror[4]->model.rotate(radDoor, Vector3(0.0f, 1.0f, 0.0f));
		
		}
		if (game->time - Timeanimation < 1.2f && !game->current_stage->animation2) {

			game->CurrentScene->entities[5]->isColision = true;
			scene->entities[5]->model.rotate(-radDoor, Vector3(0.0f, 1.0f, 0.0f));
			scene->entities_mirror[4]->model.rotate(-radDoor, Vector3(0.0f, 1.0f, 0.0f));
		}

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
	
	menu->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
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

void EndStage::createEntities()
{
	menu = new EntityMesh();
	menu->mesh->createPlane(100);
	menu->model.rotate(90 * DEG2RAD, Vector3(1.0f, 0.0f, 0.0f));
	menu->texture = Texture::Get("data/end.tga");
}

void EndStage::render()
{
	Camera* camera = new Camera();

	menu->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
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

void EndStage::update(double seconds_elapsed)
{
	Game* game = Game::instance;

	if (Input::wasKeyPressed(SDL_SCANCODE_M)) {
		game->current_stage = game->title_stage;
		game->current_stage->createEntities();
	}
}
