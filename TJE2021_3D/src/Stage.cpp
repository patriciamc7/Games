#include "Stage.h"
#include "includes.h"
#include "camera.h"
#include "utils.h"
#include "game.h"

void IntroStage::createTextures()
{
	Scene* scene = Game::instance->intro_scene;
	string texture = "data/intro/Door_BaseColor.tga,data/intro/cielo.tga,data/intro/ground.tga,data/intro/intro.tga";
	string cad;
	int found = -1;
	int init = 0;
	for (int i = 0; i < MAX_ENT_INTRO; i++)
	{
		if (i == 0 || i > 2) {
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
	string mesh = "data/intro/RightDoor.ase,data/intro/LeftDoor.ase,data/intro/ArcDoor.ase,data/intro/cielo.ASE,data/intro/intro.ase";
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
			scene->entities[i + 1]->model.translate(0.0f, 0.0f, -31.0f);
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
		float radRightDoor = 90 * DEG2RAD * seconds_elapsed;

		if (this->firstTime) {
			Timeanimation = game->time;
			this->firstTime = false;
		}
		if (game->time - Timeanimation < 1.2f) {
			scene->entities[2]->model.rotate(radLeftDoor, Vector3(0.0f, 1.0f, 0.0f));
			scene->entities[1]->model.rotate(radRightDoor, Vector3(0.0f, 1.0f, 0.0f));
		}

	}

}

void BodyStage::createTextures()
{
	Scene* scene = Game::instance->PlayScene;
	string texture = "data/imShader/water.tga,data/body/ceiling.tga,data/body/wall.tga,data/body/bath.tga,data/body/door.tga,data/body/sink.tga,data/body/sink.tga,data/body/cabinet.tga,data/body/cabinet.tga,data/body/passage.tga,data/body/passagePlane.tga,data/imShader/noise.tga,data/body/torch.tga,data/body/torch.tga,data/body/torch.tga,data/body/torch.tga";

	string cad;
	int found = -1;
	int init = 0;

	for (int i = 0; i < MAX_ENT_PLAY; i++)
	{
		if (this->entities[i]->id == 1) {
			this->entities[i]->texture2 = Texture::Get("data/imShader/cloud.tga");
		}
		if (this->entities[i]->id == 12) {
			this->entities[i]->texture2 = Texture::Get("data/imShader/gray.tga");
		}
		if (this->entities[i]->id > 16 || this->entities[i]->id < 13) {
			init = found + 1;
			found = texture.find(",", found + 1);
			cad = texture.substr(init, found - init);
			this->entities[i]->texture = Texture::Get(cad.c_str());
			this->entities_mirror[i]->texture = Texture::Get(cad.c_str());
		}
		
	}
}

//0 water 1celling 2 wall 3 bath 4 door 5 sink 6 sink1 7 cabin 8 cabin 1 9 passage 10 plano 11 mirror
void BodyStage::createEntities()
{
	Scene* scene = Game::instance->PlayScene;

	string mesh = "data/body/ceiling.ASE,data/body/wall.ASE,data/body/bath.ASE,data/body/door.ASE,data/body/sink.ASE,data/body/sink1.ASE,data/body/cabinet.ASE,data/body/cabinet1.ASE,data/body/passage.ASE,data/glassBody.ASE,data/body/torch.ASE,data/body/torch.ASE,data/body/torch.ASE,data/body/torch.ASE";

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

		if (this->entities[i]->id != 1 && this->entities[i]->id != 11 &&( this->entities[i]->id < 13 || this->entities[i]->id > 16)) {
			init = found + 1;
			found = mesh.find(",", found + 1);
			cad = mesh.substr(init, found - init);
			this->entities[i]->mesh = Mesh::Get(cad.c_str());
			this->entities_mirror[i]->mesh = Mesh::Get(cad.c_str());
		}
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
			this->entities[i]->model.rotate(-45 * DEG2RAD, Vector3(0, 1, 0));
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
			this->entities[i]->model.translate(9, 0, -55);
			this->entities_mirror[i]->model.translate(9, 0, -55);

		}
		if (this->entities[i]->id == 11) {
			this->entities[i]->mesh->createPlane(20);
			this->entities[i]->model.setRotation(90 * DEG2RAD, Vector3(0.0f, 0.0f, 1.0f));
			this->entities[i]->model.setRotation(90 * DEG2RAD, Vector3(1.0f, 0.0f, 0.0f));
			this->entities_mirror[i]->alpha = 1;
			this->entities[i]->alpha = 0;
			this->entities[i]->model.translate(5, 80, 20);
			this->entities_mirror[i]->model.translate(5, 80, 20);
		}
		if (this->entities[i]->id == 12) { //trozo de espejo
			this->entities[i]->model.translate(-30, 0, 20);
			this->entities[i]->model.scale(0.2f, 0.2f, 0.2f);
			this->entities[i]->model.rotate(-45, Vector3(0, 1, 0));
			this->entities[i]->isInteractive = true;
		}
		if (this->entities[i]->id == 13) { //plano para antorchas
			this->entities[i]->mesh->createPlane(20);

			this->entities[i]->model.rotate(90 * DEG2RAD, Vector3(1.0f, 0.0f, 0.0f));
			this->entities[i]->model.rotate(90 * DEG2RAD, Vector3(0.0f, 0.0f, 1.0f));
			this->entities[i]->model.translate(-50, 19, 30);
			this->entities[i]->model.scale(0.5f, 0.5f, 0.5f);

		}
		if (this->entities[i]->id == 14) { //plano para antorchas
			this->entities[i]->mesh->createPlane(20);

			this->entities[i]->model.rotate(90 * DEG2RAD, Vector3(1.0f, 0.0f, 0.0f));
			this->entities[i]->model.rotate(90 * DEG2RAD, Vector3(0.0f, 0.0f, 1.0f));
			this->entities[i]->model.translate(-35, 19, 30);
			this->entities[i]->model.scale(0.5f, 0.5f, 0.5f);

		}
		if (this->entities[i]->id == 15) { //plano para antorchas
			this->entities[i]->mesh->createPlane(20);

			this->entities[i]->model.rotate(90 * DEG2RAD, Vector3(1.0f, 0.0f, 0.0f));
			this->entities[i]->model.rotate(90 * DEG2RAD, Vector3(0.0f, 0.0f, 1.0f));
			this->entities[i]->model.translate(-50, 0, 30);
			this->entities[i]->model.scale(0.5f, 0.5f, 0.5f);

		}
		if (this->entities[i]->id == 16) { //plano para antorchas
			this->entities[i]->mesh->createPlane(20);
			this->entities[i]->model.rotate(90 * DEG2RAD, Vector3(1.0f, 0.0f, 0.0f));
			this->entities[i]->model.rotate(90 * DEG2RAD, Vector3(0.0f, 0.0f, 1.0f));
			this->entities[i]->model.translate(-35, 0, 30);
			this->entities[i]->model.scale(0.5f, 0.5f, 0.5f);
		}

		if (this->entities[i]->id == 17) { //plano para antorchas
			this->entities[i]->model.translate(-2, 10, -35);
		}
		if (this->entities[i]->id == 20) { //plano para antorchas
			this->entities[i]->model.translate(-2, 10, -50);
		}

		if (this->entities[i]->id == 18) { //plano para antorchas
			this->entities[i]->model.rotate(180 * DEG2RAD, Vector3(0.0f, 1.0f, 0.0f));
			this->entities[i]->model.translate(-20, 10, 35);

		}
		if (this->entities[i]->id == 19) { //plano para antorchas
			this->entities[i]->model.rotate(180 * DEG2RAD, Vector3(0.0f, 1.0f, 0.0f));
			this->entities[i]->model.translate(-20, 10, 50);
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
	////render the 
	torch->mesh->render(GL_TRIANGLES);
	torch->shader->disable();
	glDisable(GL_BLEND);
}

void Stage::renderMirror(int i, vector<EntityMesh*> entities )
{
	Scene* scene = Game::instance->CurrentScene;
	Stage* stage = Game::instance->current_stage;
	Game* game = Game::instance;

	EntityMesh* mirror;
	if (stage == game->body_stage) {
		if (!game->body_stage->animation2)
			glDisable(GL_DEPTH_TEST);
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	Camera* camera = Camera::current;
	mirror = entities[i - 1];

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
		if (scene->entities_mirror[i]->id != 1 && scene->entities_mirror[i]->id != 11 && scene->entities_mirror[i]->id != 12 && scene->entities_mirror[i]->id < 13)  //2 es el suelo water, no lo renderizamos en la realidad mirror
			scene->entities_mirror[i]->render();

	}
	for (int i = 0; i < scene->entities.size(); i++)
	{
		if (scene->entities[i]->id == 1) {
			renderWater(i);
		}
		if (scene->entities[i]->id > 12 && scene->entities[i]->id < 17) {
			renderTorch(i);
		}
		if (scene->entities[i]->id == 12) {
			renderMirror(i,this->entities);
		}

		if (scene->entities[i]->id != 1 && scene->entities[i]->id != 12 && (scene->entities[i]->id < 13 || scene->entities[i]->id>16))
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
	quad.createQuad(100, 100, 100, 100, true);
	cam2D->enable();

	Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	shader->enable();

	shader->setUniform("u_model", Matrix44());
	shader->setUniform("u_viewprojection", cam2D->viewprojection_matrix);
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader->setUniform("u_texture_tiling", 1.0f);
	if (this->glassCount == 0)
		shader->setUniform("u_texture", Texture::Get("data/gui/GUI0.tga"), 0);
	if (this->glassCount == 1)
		shader->setUniform("u_texture", Texture::Get("data/gui/GUI1.tga"), 0);
	if (this->glassCount == 2)
		shader->setUniform("u_texture", Texture::Get("data/gui/GUI2.tga"), 0);
	if (this->glassCount == 3)
		shader->setUniform("u_texture", Texture::Get("data/gui/GUI3.tga"), 0);

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
		game->current_stage = game->mind_stage;
		game->CurrentScene = game->mind_scene;
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
	if (this->glassCount == 3)
		game->current_stage = game->end_stage;
	if (this->glassCount == 1 && this->doorOpen2) {

		this->doorOpen2 = false;
		game->current_stage->animation2 = true;
		this->animation = true;
		this->firstTime = true;
	}
}

void MindStage::createTextures()
{
	Scene* scene = Game::instance->PlayScene;
	string texture = "data/mind/sala.tga,data/mind/suelo.tga,data/mind/suelo.tga,data/mind/trinidad.tga,data/mind/grail.tga,data/mind/cruz.tga,data/mind/cuadro1.tga,data/mind/cuadro2.tga,data/mind/amuleto.tga,data/imShader/noise.tga,data/mirror.tga,data/mind/puerta.tga,data/imShader/noise.tga,data/mind/altar.tga";

	string cad;
	int found = -1;
	int init = 0;

	for (int i = 0; i < MAX_ENT_MIND; i++)
	{
		if (this->entities[i]->id == 10 || this->entities[i]->id == 13) {
			this->entities[i]->texture2 = Texture::Get("data/imShader/gray.tga");
		}
		init = found + 1;
		found = texture.find(",", found + 1);
		cad = texture.substr(init, found - init);
		this->entities[i]->texture = Texture::Get(cad.c_str());
		this->entities_mirror[i]->texture = Texture::Get(cad.c_str());
		
	}
}

void MindStage::createEntities() {
	Scene* scene = Game::instance->mind_scene;

	string mesh = "data/mind/sala.ASE,data/mind/trinidad.ASE,data/mind/grail.ASE,data/mind/cruz.ASE,data/mind/cuadro1.ASE,data/mind/cuadro2.ASE,data/mind/amuleto.ASE,data/glassMind.ASE,data/mirror.ASE,data/mind/puerta.ASE,data/mind/altar.ASE";

	string cad;

	int found = -1;
	int init = 0;
	int playerNum = scene->entities.size();

	for (int i = 0; i < MAX_ENT_MIND; i++)
	{
		this->entities.push_back(new EntityMesh());
		this->entities_mirror.push_back(new EntityMesh());

		this->entities[i]->id = i + playerNum;
		this->entities_mirror[i]->id = i + playerNum;

		if (this->entities[i]->id != 2 && this->entities[i]->id != 3 && this->entities[i]->id != 13)
		{
			init = found + 1;
			found = mesh.find(",", found + 1);
			cad = mesh.substr(init, found - init);
			this->entities[i]->mesh = Mesh::Get(cad.c_str());
			this->entities_mirror[i]->mesh = Mesh::Get(cad.c_str());

		}
		if (this->entities[i]->id == 1) //SALA
		{
			this->entities[i]->isColision = false;
			this->entities[i]->model.translate(0, 10, 0);
			this->entities_mirror[i]->model.translate(0, 10, 0);
		}
		if (this->entities[i]->id == 2) // suelo
		{
			this->entities[i]->mesh->createPlane(100);
			this->entities_mirror[i]->mesh->createPlane(100);
			this->entities_mirror[i]->model.translate(-30, 0, 0);
		}
		if (this->entities[i]->id == 3) //techo
		{
			this->entities[i]->mesh->createPlane(100);
			this->entities[i]->model.translate(0, 35, 0);
			this->entities_mirror[i]->mesh->createPlane(100);
			this->entities_mirror[i]->model.translate(0,35,0);
		}
		if (this->entities[i]->id == 4)//cuadro trinidad
		{
			this->entities[i]->model.translate(42, 18, -30);
			this->entities_mirror[i]->model.translate(-42, 18, -30);
		}
		if (this->entities[i]->id == 5) //amuleto grail
		{
			this->entities[i]->alpha = 1;
			this->entities[i]->model.scale(0.3, 0.3, 0.3);
			this->entities[i]->model.translate(-200, 10, 0);
			this->entities_mirror[i]->model.scale(0.3, 0.3, 0.3);
			this->entities_mirror[i]->model.translate(-200, 10, 0);
		}
		if (this->entities[i]->id == 6) //amuelto cruz 
		{
			this->entities[i]->alpha = 1;
			this->entities[i]->model.translate(-60, 0, 20);
			this->entities[i]->model.rotate(90 * DEG2RAD, Vector3(0, 1, 0));
			this->entities_mirror[i]->model.translate(-110, 0, 190);
			this->entities_mirror[i]->model.rotate(90 * DEG2RAD, Vector3(0, 1, 0));
		}
		if (this->entities[i]->id == 7) //cuadro1 
		{
			this->entities[i]->model.translate(42, 12, 20);
			this->entities_mirror[i]->model.translate(-42, 12, 20);
		}
		if (this->entities[i]->id == 8) //cuadro2 
		{
			this->entities[i]->model.translate(42, 8, 70);
			this->entities[i]->model.scale(0.8,0.8,0.8);
			this->entities_mirror[i]->model.translate(-77, 8, 70);
			this->entities_mirror[i]->model.scale(0.8, 0.8, 0.8);
		}
		if (this->entities[i]->id == 9) //amuleto ra (bueno)  
		{
			this->entities[i]->alpha = 1;
			this->entities[i]->model.translate(-60, 0, 50);
			this->entities[i]->model.scale(0.4, 0.4, 0.4);
			this->entities[i]->model.rotate(90 * DEG2RAD, Vector3(0, 1, 0));
			this->entities_mirror[i]->model.translate(-110, 0, 120);
			this->entities_mirror[i]->model.scale(0.4, 0.4, 0.4);
			this->entities_mirror[i]->model.rotate(90 * DEG2RAD, Vector3(0, 1, 0));
		}
		if (this->entities[i]->id == 10) //trozo espejo
		{
			this->entities[i]->alpha = 1;
			this->entities[i]->isColision = false;
			this->entities_mirror[i]->alpha = 1;
			this->entities[i]->model.translate(0, 4, -10);
			this->entities[i]->model.scale(0.2, 0.2, 0.2);
			this->entities[i]->model.rotate(-90*DEG2RAD , Vector3(1, 0, 0));
		}
		if (this->entities[i]->id == 11) //espejo marco
		{
			this->entities[i]->model.translate(-80,15.5f, -24);
			this->entities[i]->model.rotate(90 * DEG2RAD, Vector3(0, 1, 0));
			this->entities[i]->model.rotate(90 * DEG2RAD, Vector3(0, 0, 1));
			this->entities[i]->model.scale(1, 2.8, 1);
		}
		if (this->entities[i]->id == 12) //puerta
		{
			this->entities[i]->model.translate(-19.5, 0, -50);
			this->entities_mirror[i]->model.translate(19.5, 0, -50);
		}
		if (this->entities[i]->id == 13) //mirror espejo 
		{
			this->entities[i]->mesh->createPlane(20);
			this->entities[i]->model.translate(-84, 15.5f, 20);
			this->entities[i]->model.rotate(90 * DEG2RAD, Vector3(0, 0, 1));
			this->entities[i]->model.scale(0.5, 1, 2);
		}
		if (this->entities[i]->id == 14) //mirror espejo 
		{
			this->entities[i]->model.translate(-15, 0, 70);
			this->entities[i]->model.scale(0.5, 0.5, 0.5);

		}
		//reflexion
		this->entities_mirror[i]->model = this->entities_mirror[i]->model.relfexion_y();
		this->entities_mirror[i]->model.translate(170,0,0);
		scene->entities.push_back(this->entities[i]);
		scene->entities_mirror.push_back(this->entities_mirror[i]);
	}

	createTextures();
}
void MindStage::render()
{
	Camera* camera = Camera::current;
	Scene* scene = Game::instance->mind_scene;
	for (int i = 0; i < scene->entities_mirror.size(); i++) 
	{
		//no renerizamos el epejo/ el marco ni el trozo de espejo
		if (scene->entities[i]->id != 9 && scene->entities[i]->id != 10 && scene->entities[i]->id != 12 && scene->entities[i]->id != 13)
			scene->entities_mirror[i]->render();
	}
	for (int i = 0; i < scene->entities.size(); i++)
	{
		if (scene->entities[i]->id == 10 || scene->entities[i]->id == 13)
			renderMirror(i, this->entities);

		if (scene->entities[i]->id != 10 && scene->entities[i]->id != 13)
			scene->entities[i]->render();
		
	}
	//renderGui();
}
void MindStage::update(double seconds_elapsed)
{
	Camera* camera = Camera::current;
	Scene* scene = Game::instance->mind_scene;

	for (int i = 0; i < scene->entities.size(); i++)
	{
		scene->entities[i]->update(seconds_elapsed);
	}
	
	for (int i = 0; i < scene->entities_mirror.size(); i++)
	{
		scene->entities_mirror[i]->update(seconds_elapsed);
	}
	ChangePosLight();

}
void MindStage::ChangePosLight()
{
	Scene* scene = Game::instance->mind_scene;
	Game* game = Game::instance; 
	 
	if (!isRa) {
		if (((int)game->time % 3) == 0 && timeSpot != (int)game->time) {
			int x = rand();
			int z= rand();
			scene->lights[0]->light_position = Vector3( x% 110 - 90, 25, z % 110 - 30); //spot reality world
			int pos = abs((x % 110 - 90) - (-100));
			scene->lights[1]->light_position = Vector3(-100-pos, 25, z % 110 - 30); //spot reality world

			timeSpot = (int)game->time;
		}
	}
	else //cuando has encontrado el amuleto correcto 
	{
		scene->lights[0]->light_position = Vector3(-50, 50, 0); //spot reality world
		scene->lights[1]->light_position = Vector3(-50, 25, 0); //spot mirror world
		scene->entities[10]->alpha = 0; //trozo de espejo se hace visible
		scene->entities_mirror[10]->alpha = 0;
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