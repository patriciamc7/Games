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
		game->current_stage = game->body_stage; //tiene que ir a corridor
		game->CurrentScene = game->BodyScene;
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
	Scene* scene = Game::instance->CurrentScene;
	string texture = "data/imShader/water.tga,data/body/ceiling.tga,data/body/wall.tga,data/body/bath.tga,data/body/door.tga,data/body/sink.tga,data/body/sink.tga,data/body/cabinet.tga,data/body/cabinet.tga,data/body/passage.tga,data/body/passagePlane.tga,data/imShader/noise.tga,data/body/torch.tga,data/body/torch.tga,data/body/torch.tga,data/body/torch.tga";

	string cad;
	int found = -1;
	int init = 0;

	for (int i = 0; i < MAX_ENT_BODY; i++)
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
	Scene* scene = Game::instance->CurrentScene;

	string mesh = "data/body/ceiling.ASE,data/body/wall.ASE,data/body/bath.ASE,data/body/door.ASE,data/body/sink.ASE,data/body/sink1.ASE,data/body/cabinet.ASE,data/body/cabinet1.ASE,data/body/passage.ASE,data/glassBody.ASE,data/body/torch.ASE,data/body/torch.ASE,data/body/torch.ASE,data/body/torch.ASE";

	string cad;

	int found = -1;
	int init = 0;
	int playerNum = scene->entities.size();
	this->changeGlass = false; 
	this->doorOpen2 = true;
	this->InitStage= true;
	for (int i = 0; i < MAX_ENT_BODY; i++)
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

void Stage::renderTorch(int i, vector<EntityMesh*> entities)
{

	EntityMesh* torch;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	Camera* camera = Camera::current;
	torch = entities[i - 1];
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

void Stage::renderParticle(float timeParticle)
{
	Scene* scene = Game::instance->CurrentScene;
	Camera* camera = Camera::current;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(false);


	//particle->texture = Texture::Get("data/imShader/grayMirror.tga"); 
	for (int i = 0; i < scene->mirrorParticle.size(); i++)
	{
		scene->mirrorParticle[i].v_particles->mesh->vertices.resize(6);
		scene->mirrorParticle[i].v_particles->mesh->uvs.resize(6);

		Vector3& pos = Vector3(0, -0.1 * pow(timeParticle, 2), -timeParticle);
		//if(scene->v_particles[i])
		float sizeParticle = scene->mirrorParticle[i].sizeParticle;
		Vector3 camUp = camera->getLocalVector(Vector3(0, 1, 0)) * sizeParticle;
		Vector3 camRight = camera->getLocalVector(Vector3(1, 0, 0)) * sizeParticle;
		//encarar al ojo de la camara
		scene->mirrorParticle[i].v_particles->mesh->vertices[0] = Vector3(pos + camUp + camRight);
		scene->mirrorParticle[i].v_particles->mesh->uvs[0] = Vector2(1, 0);
		scene->mirrorParticle[i].v_particles->mesh->vertices[1] = Vector3(pos + camUp - camRight);
		scene->mirrorParticle[i].v_particles->mesh->uvs[1] = Vector2(0, 0);
		scene->mirrorParticle[i].v_particles->mesh->vertices[2] = Vector3(pos - camUp - camRight);
		scene->mirrorParticle[i].v_particles->mesh->uvs[2] = Vector2(0, 1);


		scene->mirrorParticle[i].v_particles->mesh->vertices[3] = Vector3(pos - camUp - camRight);
		scene->mirrorParticle[i].v_particles->mesh->uvs[3] = Vector2(0, 1);
		scene->mirrorParticle[i].v_particles->mesh->vertices[4] = Vector3(pos - camUp + camRight);
		scene->mirrorParticle[i].v_particles->mesh->uvs[4] = Vector2(1, 1);
		scene->mirrorParticle[i].v_particles->mesh->vertices[5] = Vector3(pos + camUp + camRight);
		scene->mirrorParticle[i].v_particles->mesh->uvs[5] = Vector2(1, 0);

		scene->mirrorParticle[i].v_particles->model.rotate(scene->mirrorParticle[i].rotateParticle, Vector3(0, 0, 1));
		scene->mirrorParticle[i].v_particles->render();
		scene->mirrorParticle[i].v_particles->model.rotate(-scene->mirrorParticle[i].rotateParticle, Vector3(0, 0, 1));
	}
	//Fin de encarar
	glDisable(GL_BLEND);
	glDepthMask(true);

}


void Stage::renderGui() {
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Game* game = Game::instance;

	Camera* cam2D = new Camera();
	cam2D->setOrthographic(0, game->window_width, game->window_height, 0, -1, 1);

	Mesh quad;
	quad.createQuad(100, 100, 100, 100, true);

	Mesh inventario;
	inventario.createQuad(100, 500, 100, 100, true);
	cam2D->enable();

	Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	shader->enable();

	shader->setUniform("u_model", Matrix44());
	shader->setUniform("u_viewprojection", cam2D->viewprojection_matrix);
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader->setUniform("u_texture_tiling", 1.0f);
	if (glassCount == 0)
		shader->setUniform("u_texture", Texture::Get("data/gui/GUI0.tga"), 0);
	if (this->glassCount == 1)
		shader->setUniform("u_texture", Texture::Get("data/gui/GUI1.tga"), 0);
	if (this->glassCount == 2)
		shader->setUniform("u_texture", Texture::Get("data/gui/GUI2.tga"), 0);
	if (this->glassCount == 3)
		shader->setUniform("u_texture", Texture::Get("data/gui/GUI3.tga"), 0);
	quad.render(GL_TRIANGLES);
	shader->disable();
	//inventario
	Shader* shader2 = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	shader2->enable();

	shader2->setUniform("u_model", Matrix44());
	shader2->setUniform("u_viewprojection", cam2D->viewprojection_matrix);
	shader2->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader2->setUniform("u_texture_tiling", 1.0f);
	if (this->amuleto)
		shader2->setUniform("u_texture", Texture::Get("data/gui/amuleto.tga"), 1);
	else  if (this->cruz)
		shader2->setUniform("u_texture", Texture::Get("data/gui/cruz.tga"), 2);
	else if (this->grail)
		shader2->setUniform("u_texture", Texture::Get("data/gui/grail.tga"), 3);
	else if (this->arrow)
		shader2->setUniform("u_texture", Texture::Get("data/gui/ouijaArrow.tga"), 4);
	else 
		shader2->setUniform("u_texture", Texture::Get("data/gui/void.tga"), 5);
	
	inventario.render(GL_TRIANGLES);
	shader2->disable();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}

void BodyStage::render()
{

	Camera* camera = Camera::current;
	Scene* scene = Game::instance->CurrentScene;
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
			renderTorch(i, this->entities);
		}
		if (scene->entities[i]->id == 12) {
			renderMirror(i,this->entities);
		}

		if (scene->entities[i]->id != 1 && scene->entities[i]->id != 12 && (scene->entities[i]->id < 13 || scene->entities[i]->id>16))
			scene->entities[i]->render();
	}
	renderGui();
}

void BodyStage::update(double seconds_elapsed)
{

	Scene* scene = Game::instance->CurrentScene;
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
		float radDoor = -90 * DEG2RAD * seconds_elapsed;

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
	/*if (this->glassCount == 3) // en el pasillo cuando el player deje los tres trozos en el portal, que se acabae el juego
		game->current_stage = game->end_stage;*/
	if (this->changeGlass && this->doorOpen2) {

		this->doorOpen2 = false;
		game->current_stage->animation2 = true;
		this->animation = true;
		this->firstTime = true;
	}
}

void MindStage::createTextures()
{
	Scene* scene = Game::instance->CurrentScene;
	string texture = "data/mind/sala.tga,data/mind/suelo.tga,data/mind/suelo.tga,data/mind/trinidad.tga,data/mind/grail.tga,data/mind/cruz.tga,data/mind/cuadro1.tga,data/mind/cuadro2.tga,data/mind/amuleto.tga,data/imShader/noise.tga,data/mirror.tga,data/mind/puerta.tga,data/imShader/noise.tga,data/mind/altar.tga,data/body/passage.tga,data/body/torch.tga,data/body/torch.tga,data/body/torch.tga,data/body/torch.tga,data/body/passagePlane.tga";

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
	Scene* scene = Game::instance->CurrentScene;

	string mesh = "data/mind/sala.ASE,data/mind/trinidad.ASE,data/mind/grail.ASE,data/mind/cruz.ASE,data/mind/cuadro1.ASE,data/mind/cuadro2.ASE,data/mind/amuleto.ASE,data/glassMind.ASE,data/mirror.ASE,data/mind/puerta.ASE,data/mind/altar.ASE,data/body/passage.ASE,data/body/torch.ASE,data/body/torch.ASE,data/body/torch.ASE,data/body/torch.ASE";
	this->changeGlass = false;

	string cad;
	this->InitStage = true;
	this->doorOpen2 = true;
	int found = -1;
	int init = 0;
	int playerNum = scene->entities.size();

	for (int i = 0; i < MAX_ENT_MIND; i++)
	{
		this->entities.push_back(new EntityMesh());
		this->entities_mirror.push_back(new EntityMesh());

		this->entities[i]->id = i + playerNum;
		this->entities_mirror[i]->id = i + playerNum;

		if (this->entities[i]->id != 2 && this->entities[i]->id != 3 && this->entities[i]->id != 13 && this->entities[i]->id < 20)
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
			this->entities[i]->isColision = false; 
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
			this->entities[i]->alpha = 0;
			this->entities[i]->model.scale(0.3, 0.3, 0.3);
			this->entities[i]->model.translate(-200, 10, 0);
			this->entities_mirror[i]->model.scale(0.3, 0.3, 0.3);
			this->entities_mirror[i]->model.translate(-200, 10, 0);
		}
		if (this->entities[i]->id == 6) //amuelto cruz 
		{
			this->entities[i]->alpha = 0;
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
			this->entities[i]->alpha = 0;
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
		if (this->entities[i]->id == 15) //pasillo 
		{
			this->entities[i]->isColision = false;
			this->entities[i]->model.translate(-15, 0, -85);
			this->entities[i]->model.scale(1.5, 1, 1);
		

		}
		if (this->entities[i]->id == 16) //antorchas prefab 
		{
			this->entities[i]->model.translate(2, 10, -104);
			this->entities[i]->model.rotate(180 * DEG2RAD, Vector3(0, 1, 0));
			


		}
		if (this->entities[i]->id == 17) //antorchas prefab 
		{
			this->entities[i]->model.translate(-35, 10, -108);


		}
		if (this->entities[i]->id == 18) //antorchas prefab 
		{
			this->entities[i]->model.translate(-35, 10, -93);


		}
		if (this->entities[i]->id == 19) //antorchas prefab 
		{
			this->entities[i]->model.translate(2, 10, -93);
			this->entities[i]->model.rotate(180 * DEG2RAD, Vector3(0, 1, 0));
			

		}
		if (this->entities[i]->id == 20) //plano profundidad 
		{
			this->entities[i]->mesh->createPlane(20);
			this->entities[i]->model.rotate(90 * DEG2RAD, Vector3(1.0f, 0.0f, 0.0f));
			this->entities[i]->model.translate(-15, 120, 20);
		}
		if (this->entities[i]->id == 21) //antorchas fuego
		{
			this->entities[i]->mesh->createPlane(10);
			this->entities[i]->model.rotate(90 * DEG2RAD, Vector3(1.0f, 0.0f, 0.0f));
			this->entities[i]->model.translate(0, 93, 30);
			
		}
		if (this->entities[i]->id == 22) //antorchas fuego
		{
			this->entities[i]->mesh->createPlane(10);
			this->entities[i]->model.rotate(90 * DEG2RAD, Vector3(1.0f, 0.0f, 0.0f));
			this->entities[i]->model.translate(-33, 93, 30);
		}
		if (this->entities[i]->id == 23) //antorchas fuego
		{
			this->entities[i]->mesh->createPlane(10);
			this->entities[i]->model.rotate(90 * DEG2RAD, Vector3(1.0f, 0.0f, 0.0f));
			this->entities[i]->model.translate(-33, 108, 30);
		}
		if (this->entities[i]->id == 24) //antorchas fuego
		{
			this->entities[i]->mesh->createPlane(10);
			this->entities[i]->model.rotate(90 * DEG2RAD, Vector3(1.0f, 0.0f, 0.0f));
			this->entities[i]->model.translate(0, 104, 30);
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
		if (scene->entities_mirror[i]->id != 10 && scene->entities_mirror[i]->id != 12 && scene->entities_mirror[i]->id != 13 && scene->entities_mirror[i]->id < 15)
			scene->entities_mirror[i]->render();
	}
	for (int i = 0; i < scene->entities.size(); i++)
	{
		if (scene->entities[i]->id == 10 || scene->entities[i]->id == 13)
			renderMirror(i, this->entities);

		if (scene->entities[i]->id > 20)
			renderTorch(i, this->entities);

		if (scene->entities[i]->id != 10 && scene->entities[i]->id != 13 && scene->entities[i]->id < 21)
			scene->entities[i]->render();
		
	}
	renderGui();
}

void MindStage::update(double seconds_elapsed)
{
	Camera* camera = Camera::current;
	Scene* scene = Game::instance->mind_scene;
	Game* game = Game::instance; 
	for (int i = 0; i < scene->entities.size(); i++)
	{
		scene->entities[i]->update(seconds_elapsed);
	}
	
	for (int i = 0; i < scene->entities_mirror.size(); i++)
	{
		scene->entities_mirror[i]->update(seconds_elapsed);
	}
	if (this->animation)
	{
		float x = 10 * seconds_elapsed;

		if (this->firstTime) {
			Timeanimation = game->time;
			this->firstTime = false;
		}
		if (game->time - Timeanimation < 1.2f && game->current_stage->animation2) {

			game->CurrentScene->entities[12]->isColision = false;			
			scene->entities[12]->model.translate(x, 0.0f, 0.0f);
		}
		if (game->time - Timeanimation < 1.2f && !game->current_stage->animation2) {
			game->CurrentScene->entities[12]->isColision = true;
			scene->entities[12]->model.translate(-x, 0.0f, 0.0f);
		}

	}
	if (this->changeGlass && this->doorOpen2) {
		this->doorOpen2 = false;
		game->current_stage->animation2 = true;
		this->animation = true;
		this->firstTime = true;
	}
	ChangePosLight();

}

void MindStage::ChangePosLight()
{
	Scene* scene = Game::instance->mind_scene;
	Game* game = Game::instance; 
	 
	if (!isRa) {
		if (((int)game->time % 3) == 0 && timeSpot != (int)game->time) {
			
			this->contObject += 1; 
			if (this->contObject == 3)
				this->contObject = 0; 
			scene->lights[0]->light_position = Vector3(-80, 25, contObject*25); //spot reality world
			scene->lights[1]->light_position = Vector3(-125, 25, contObject * 25); //spot reality world

			timeSpot = (int)game->time;
		}
	}
	else //cuando has encontrado el amuleto correcto 
	{
		scene->lights[0]->light_position = Vector3(-50, 50, 0); //spot reality world
		scene->lights[1]->light_position = Vector3(-50, 25, 0); //spot mirror world
		scene->entities[10]->alpha = 0; //trozo de espejo se hace visible
		scene->entities_mirror[10]->alpha = 0;
		scene->entities_mirror[10]->isColision = true;
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

void SoulStage::createTextures()
{

	Scene* scene = Game::instance->soul_scene;

	string texture = "data/soul/ouija_mirror.tga,data/soul/OuijaArrow.tga,data/soul/Altar_9_mirror.tga,data/soul/Altar_C_mirror.tga,data/soul/Altar_M.tga,data/soul/Floor.tga,data/soul/wall.tga,data/soul/pilar.tga,data/soul/window.tga,data/soul/Floor.tga,data/soul/OuijaArrow.tga,data/imShader/noise.tga,data/soul/wall.tga,data/soul/door.tga,data/imShader/noise.tga,data/body/passage.tga,data/body/torch.tga,data/body/torch.tga,data/body/torch.tga,data/body/torch.tga,data/body/passagePlane.tga";

	string cad;
	int found = -1;
	int init = 0;

	for (int i = 0; i < MAX_ENT_SOUL; i++)
	{
		init = found + 1;
		found = texture.find(",", found + 1);
		cad = texture.substr(init, found - init);
		this->entities[i]->texture = Texture::Get(cad.c_str());
	
		if (this->entities_mirror[i]->id == 1 ) {
			this->entities_mirror[i]->texture = Texture::Get("data/soul/ouija.tga");
		}
		else if (this->entities_mirror[i]->id == 3) {
			this->entities_mirror[i]->texture = Texture::Get("data/soul/Altar_9.tga");
		}
		else if (this->entities_mirror[i]->id == 4) {
			this->entities_mirror[i]->texture = Texture::Get("data/soul/Altar_C.tga"); 
		}
		else
			this->entities_mirror[i]->texture = Texture::Get(cad.c_str());
		
		if (this->entities[i]->id == 12 || this->entities[i]->id == 15) {
			this->entities[i]->texture2 = Texture::Get("data/imShader/gray.tga");
		}

	}
	for (int j = 0; j < scene->mirrorParticle.size(); j++) {
		scene->mirrorParticle[j].v_particles->texture = Texture::Get("data/imShader/grayMirror.tga");
	}
}

void SoulStage::createEntities()
{
	Scene* scene = Game::instance->soul_scene;
	Game* game = Game::instance;
	string mesh = "data/soul/Ouija.ASE,data/soul/OuijaArrow.ASE,data/mind/altar.ASE,data/mind/altar.ASE,data/mind/altar.ASE,data/soul/floor.ASE,data/soul/wall.ASE,data/soul/pilar.ASE,data/soul/window.ASE,data/soul/floor.ASE,data/soul/mirror.ASE,data/soul/wallMirror.ASE,data/soul/door.ASE,data/glassSpirit.ASE,data/body/passage.ASE,data/body/torch.ASE,data/body/torch.ASE,data/body/torch.ASE,data/body/torch.ASE";

	this->changeGlass = false;
	this->PuzzleCorrect = true; 
	scene->mirrorParticle.clear();
	scene->mirrorParticle.resize(NumParticle);
	string cad;
	this->InitStage = true;
	this->doorOpen2 = true;
	int found = -1;
	int init = 0;
	int playerNum = scene->entities.size();
	for (int i = 0; i < MAX_ENT_SOUL; i++)
	{
		this->entities.push_back(new EntityMesh());
		this->entities_mirror.push_back(new EntityMesh());

		this->entities[i]->id = i + playerNum;
		this->entities_mirror[i]->id = i + playerNum;

		if (this->entities[i]->id != 12 && this->entities[i]->id <21) {

			init = found + 1;
			found = mesh.find(",", found + 1);
			cad = mesh.substr(init, found - init);
			this->entities[i]->mesh = Mesh::Get(cad.c_str());
			this->entities_mirror[i]->mesh = Mesh::Get(cad.c_str());
		}
		if (this->entities_mirror[i]->id == 1) { //ouija
			this->entities[i]->model.rotate(180*DEG2RAD, Vector3(0, 1, 0));
			this->entities[i]->model.translate(0, 25, 50);
			this->entities[i]->model.scale(0.7, 0.7, 0.7);

			this->entities_mirror[i]->model.translate(0,25, 100);
			this->entities_mirror[i]->model.scale(0.7, 0.7, 0.7);
		}

		if (this->entities_mirror[i]->id == 2) { //arrow ouija
			this->entities[i]->model.scale(0.2 ,0.2, 0.2);
			this->entities[i]->model.rotate(90 * DEG2RAD, Vector3(1, 0, 0));
			this->entities[i]->model.translate(50, -100, 10);
			this->entities[i]->model.rotate(35 * DEG2RAD, Vector3(0, 0, 1));
		}
		if (this->entities_mirror[i]->id == 3) { //altar 9
			this->entities[i]->model.translate(-35, 0, 20);
			this->entities_mirror[i]->model.translate(-35, 0, 10);


			this->entities[i]->model.scale(0.5, 0.5, 0.5);
			this->entities_mirror[i]->model.scale(0.5, 0.5, 0.5);

			this->entities[i]->model.rotate(90 * DEG2RAD, Vector3(0, 1, 0));
			this->entities_mirror[i]->model.rotate(270 * DEG2RAD, Vector3(0, 1, 0));
			this->entities_mirror[i]->model.translate(-200, 0, -170);

		}
		if (this->entities_mirror[i]->id == 4) { //altar c
			this->entities[i]->model.translate(0, 0, 20);
			this->entities_mirror[i]->model.translate(0, 0, 10);
			this->entities[i]->model.scale(0.5, 0.5, 0.5);
			this->entities_mirror[i]->model.scale(0.5, 0.5, 0.5);


			this->entities[i]->model.rotate(90 * DEG2RAD, Vector3(0, 1, 0));
			this->entities_mirror[i]->model.rotate(270 * DEG2RAD, Vector3(0, 1, 0));
			this->entities_mirror[i]->model.translate(-200, 0, -170);
		}
		if (this->entities_mirror[i]->id == 5) { //altar m
			this->entities[i]->model.translate(35, 0, 20);
			this->entities_mirror[i]->model.translate(35, 0, 10);
			this->entities[i]->model.scale(0.5, 0.5, 0.5);
			this->entities_mirror[i]->model.scale(0.5, 0.5, 0.5);

			this->entities[i]->model.rotate(90 * DEG2RAD, Vector3(0, 1, 0));
			this->entities_mirror[i]->model.rotate(270 * DEG2RAD, Vector3(0, 1, 0));
			this->entities_mirror[i]->model.translate(-200, 0, -160);
		}
		if (this->entities_mirror[i]->id == 6)  //floor
			this->entities[i]->isColision = false;

		if (this->entities_mirror[i]->id == 10) { //techo
			this->entities[i]->model.translate(0, 50, 0);
			this->entities_mirror[i]->model.translate(0, 50, 0);

		}
		if (this->entities[i]->id > 6 && this->entities[i]->id <10) {
			this->entities[i]->model.rotate(180 * DEG2RAD, Vector3(0, 1, 0));
			this->entities[i]->model.translate(0, 0, -60);
			this->entities[i]->isColision = false;
		}
		if (this->entities_mirror[i]->id == 11) { //mirror
			this->entities[i]->model.translate(0, 0, 75);

		}
		if (this->entities_mirror[i]->id == 12) { //mirror
			this->entities[i]->mesh->createPlane(20);
			this->entities[i]->model.rotate(90 * DEG2RAD, Vector3(1, 0, 0));
			this->entities[i]->model.translate(0, -75, 20);
			this->entities[i]->model.scale(1.2, 1.0, 0.8);
		}
		
		if (this->entities_mirror[i]->id == 13) { //pared
			this->entities[i]->model.translate(0, -10, 80);

		}
		if (this->entities_mirror[i]->id == 14) { //door
			this->entities[i]->model.translate(-64, 0, -39);

		}
		if (this->entities_mirror[i]->id == 15) { //trozo cristal
			this->entities[i]->model.translate(0, 0, 100);
			this->entities[i]->model.scale(0.2, 0.2, 0.2);
			this->entities[i]->model.rotate(90 * DEG2RAD, Vector3(0, 0, 1));
			this->entities[i]->alpha = 1;

		}
		//passage
		if (this->entities[i]->id == 16) 
		{
			this->entities[i]->model.rotate(270 * DEG2RAD, Vector3(0, 1, 0));
			this->entities[i]->model.translate(28, 0, -105); 
			this->entities[i]->model.scale(1.4, 1.4, 1.4); 
			this->entities[i]->isColision = false; 

		}
		//torch
		if (this->entities[i]->id == 17)
		{
			this->entities[i]->model.rotate(90 * DEG2RAD, Vector3(0, 1, 0));
			this->entities[i]->model.translate(-45, 10, 95);


		}
		if (this->entities[i]->id == 18)
		{
			this->entities[i]->model.rotate(90 * DEG2RAD, Vector3(0, 1, 0));
			this->entities[i]->model.translate(-45, 10, 125);


		}
		if (this->entities[i]->id == 19)
		{
			this->entities[i]->model.rotate(-90 * DEG2RAD, Vector3(0, 1, 0));
			this->entities[i]->model.translate(10, 10, -95);

		}
		if (this->entities[i]->id == 20)
		{
			this->entities[i]->model.rotate(-90 * DEG2RAD, Vector3(0, 1, 0));
			this->entities[i]->model.translate(10, 10, -125);

		}
		if (this->entities[i]->id == 21) //plano profundidad 
		{
			this->entities[i]->mesh->createPlane(35);
			this->entities[i]->model.rotate(90 * DEG2RAD, Vector3(1.0f, 0.0f, 0.0f));
			this->entities[i]->model.rotate(90 * DEG2RAD, Vector3(0.0f, 0.0f, 1.0f));
			this->entities[i]->model.translate(-30, -150, 20);
		}



		if (this->entities[i]->id == 22) //antorchas fuego
		{
			this->entities[i]->mesh->createPlane(10);
			this->entities[i]->model.rotate(90 * DEG2RAD, Vector3(1.0f, 0.0f, 0.0f));
			this->entities[i]->model.rotate(90 * DEG2RAD, Vector3(0.0f, 0.0f, 1.0f));

			this->entities[i]->model.translate(-12, -95, 30);

		}
		if (this->entities[i]->id == 23) //antorchas fuego
		{
			this->entities[i]->mesh->createPlane(10);
			this->entities[i]->model.rotate(90 * DEG2RAD, Vector3(1.0f, 0.0f, 0.0f));
			this->entities[i]->model.rotate(90 * DEG2RAD, Vector3(0.0f, 0.0f, 1.0f));

			this->entities[i]->model.translate(-42, -95, 30);
		}
		if (this->entities[i]->id == 24) //antorchas fuego
		{
			this->entities[i]->mesh->createPlane(10);
			this->entities[i]->model.rotate(90 * DEG2RAD, Vector3(1.0f, 0.0f, 0.0f));
			this->entities[i]->model.rotate(90 * DEG2RAD, Vector3(0.0f, 0.0f, 1.0f));

			this->entities[i]->model.translate(-42, -125, 30);
		}
		if (this->entities[i]->id == 25) //antorchas fuego
		{
			this->entities[i]->mesh->createPlane(10);
			this->entities[i]->model.rotate(90 * DEG2RAD, Vector3(1.0f, 0.0f, 0.0f));
			this->entities[i]->model.rotate(90 * DEG2RAD, Vector3(0.0f, 0.0f, 1.0f));

			this->entities[i]->model.translate(-12, -125, 30);
		}
		this->entities_mirror[i]->model.translate(0, 0, 170);
		scene->entities.push_back(this->entities[i]);
		scene->entities_mirror.push_back(this->entities_mirror[i]);
	}
	for (int j = 0; j < scene->mirrorParticle.size(); j++) {
		scene->mirrorParticle[j].v_particles = new EntityMesh();
		//scene->v_particles[j]->model.rotate(rand()*90 * DEG2RAD, Vector3(0, 0, 1));
		scene->mirrorParticle[j].v_particles->model.translate(random() * 40 - 20, random() * 30 + 5, random() * 2 + 80);
		scene->mirrorParticle[j].sizeParticle = rand() % 25;
		scene->mirrorParticle[j].rotateParticle = rand() % 90;
	}
	createTextures();
}

void SoulStage::render()
{
	Scene* scene = Game::instance->soul_scene;
	Game* game = Game::instance;
	int timeAnimation = 15; 
	for (int i = 0; i < scene->entities_mirror.size(); i++)
	{
		if(scene->entities_mirror[i]->id != 2 && scene->entities_mirror[i]->id != 11 && scene->entities_mirror[i]->id < 12)
			scene->entities_mirror[i]->render();
	}	
	for (int i = 0; i < scene->entities.size(); i++)
	{
		if (scene->entities[i]->id == 12 || scene->entities[i]->id == 15)
			renderMirror(i, this->entities);
		if (scene->entities[i]->id > 21)
			renderTorch(i, this->entities);
		if (scene->entities[i]->id !=12 && scene->entities[i]->id != 15 && scene->entities[i]->id <22){
			scene->entities[i]->render();
		}
		if (scene->entities[15]->alpha == 0) 
		{
			if (scene->entities[i]->id == 12)// espejo
			{
				scene->entities[i]->isColision = false;
				scene->entities[i]->alpha = 1;
			}
			if (PuzzleCorrect == true) {
				scene->timeLive = game->time;
				PuzzleCorrect = false;
			}

			if (abs(game->time - scene->timeLive) > timeAnimation) {
				if (scene->entities[i]->id == 11) // marco
				{
					scene->entities[i]->isColision = false;
					scene->entities[i]->alpha = 1;
				}
				if(scene->entities[i]->id ==13)
					scene->entities[i]->isColision = false;
			}
			if (abs(game->time - scene->timeLive) > timeAnimation) {
				scene->mirrorParticle.clear(); //Quitar prefab de mirror y marco de mirror

			}
		}
		else
			scene->timeLive = 0.0f;
	}
	if (scene->entities[15]->alpha == 0 && abs(game->time - scene->timeLive) < timeAnimation)
		renderParticle(abs(game->time - scene->timeLive)+5);
	
	renderGui();
}

void SoulStage::update(double seconds_elapsed)
{
	Scene* scene = Game::instance->soul_scene;
	Game* game = Game::instance;

	for (int i = 0; i < scene->entities_mirror.size(); i++)
	{
		scene->entities_mirror[i]->update(seconds_elapsed);
	}

	for (int i = 0; i < scene->entities.size(); i++)
	{
		scene->entities[i]->update(seconds_elapsed);
	}
	if (this->animation)
	{
		float radDoor = -90 * DEG2RAD * seconds_elapsed;

		if (this->firstTime) {
			Timeanimation = game->time;
			this->firstTime = false;
		}
		if (game->time - Timeanimation < 1.2f && game->current_stage->animation2) {

			game->CurrentScene->entities[14]->isColision = false;
			scene->entities[14]->model.rotate(radDoor, Vector3(0.0f, 1.0f, 0.0f));

		}
		if (game->time - Timeanimation < 1.2f && !game->current_stage->animation2) {

			game->CurrentScene->entities[14]->isColision = true;
			scene->entities[14]->model.rotate(-radDoor, Vector3(0.0f, 1.0f, 0.0f));
		}
	}
	if (this->changeGlass && this->doorOpen2) {
		this->doorOpen2 = false;
		game->current_stage->animation2 = true;
		this->animation = true;
		this->firstTime = true;
	}
}

void CorridorStage::createTextures()
{

	Scene* scene = Game::instance->corridor_scene;

	string texture = "data/passage/sala.tga,data/intro/intro.tga,data/passage/portal.tga,data/body/passagePlane.tga,data/body/passagePlane.tga,data/body/passagePlane.tga";

	string cad;
	int found = -1;
	int init = 0;

	for (int i = 0; i < MAX_ENT_CORRIDOR; i++)
	{
		init = found + 1;
		found = texture.find(",", found + 1);
		cad = texture.substr(init, found - init);
		this->entities[i]->texture = Texture::Get(cad.c_str());

	}
}

void CorridorStage::createEntities()
{
	Scene* scene = Game::instance->corridor_scene; 

	string mesh = "data/passage/sala.ASE,data/passage/salaIntro.ASE,data/passage/portal.ASE";
	this->changeGlass = false;

	string cad;
	this->InitStage = true;
	this->doorOpen2 = true;
	int found = -1;
	int init = 0;
	int playerNum = scene->entities.size();

	for (int i = 0; i < MAX_ENT_CORRIDOR; i++)
	{
		this->entities.push_back(new EntityMesh());
		this->entities[i]->id = i + playerNum;

		if (this->entities[i]->id < 4) {
			init = found + 1;
			found = mesh.find(",", found + 1);
			cad = mesh.substr(init, found - init);
			this->entities[i]->mesh = Mesh::Get(cad.c_str());
		}
		
		if (this->entities[i]->id == 3) {
			this->entities[i]->model.translate(65, 0, 5);
			this->entities[i]->model.rotate(90*DEG2RAD, Vector3(0, 1, 0));
		}
		if (this->entities[i]->id == 4) {
			this->entities[i]->mesh->createPlane(20);
			this->entities[i]->model.translate(200, 20, 0);
			this->entities[i]->model.scale(1, 2, 2);
			this->entities[i]->model.rotate(90 * DEG2RAD, Vector3(0, 0, 1));
			this->entities[i]->model.rotate(90 * DEG2RAD, Vector3(0, 1, 0));
		}
		if (this->entities[i]->id == 5) {
			this->entities[i]->mesh->createPlane(20);
			this->entities[i]->model.translate(60, 7, -100);
			this->entities[i]->model.scale(1,2,1);

			this->entities[i]->model.rotate(90 * DEG2RAD, Vector3(1, 0, 0));
		}
		if (this->entities[i]->id == 6) {
			this->entities[i]->mesh->createPlane(20);
			this->entities[i]->model.translate(60, 20, 100);
			this->entities[i]->model.scale(2, 2, 1);

			this->entities[i]->model.rotate(90 * DEG2RAD, Vector3(1, 0, 0));
		}
		
		scene->entities.push_back(this->entities[i]);
	}
	createTextures();
}

void CorridorStage::render()
{
	Scene* scene = Game::instance->corridor_scene;


	for (int i = 0; i < scene->entities.size(); i++)
	{
		scene->entities[i]->render();
	}
	renderGui();
}

void CorridorStage::update(double seconds_elapsed)
{
	Scene* scene = Game::instance->corridor_scene;

	for (int i = 0; i < scene->entities.size(); i++)
	{
		scene->entities[i]->update(seconds_elapsed);
	}
}
