#include "Scene.h"
#include "game.h"

Entity::Entity()
{
	
}

Entity::~Entity()
{
}

EntityMesh::EntityMesh()
{
	this->texture = new Texture();
}

EntityMesh::~EntityMesh()
{
}

void EntityMesh::render()
{

	//get the last camera thet was activated
	Camera* camera = Camera::current;
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	
	//enable shader and pass uniforms
	shader->enable(); 
	shader->setUniform("u_model", this->model);
	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader->setUniform("u_color", Vector4(1,1,1,1));

	shader->setUniform("u_texture", Texture::Get("data/Door_BaseColor.tga"),0);

	////render the 
	mesh->render(GL_TRIANGLES);
	shader->disable(); 
}

void EntityMesh::update(float dt)
{
}

EntitySound::EntitySound()
{
}

EntitySound::~EntitySound()
{
}

void EntitySound::render()
{
}

void EntitySound::update(float dt)
{
}

EntityLight::EntityLight()
{
}

EntityLight::~EntityLight()
{
}

void EntityLight::render()
{
}

void EntityLight::update(float dt)
{
}

Scene::Scene()
{
}

Scene::~Scene()
{
}

EntityPlayer::EntityPlayer()
{
	valor = 20.0f;
}

EntityPlayer::~EntityPlayer()
{
	
}

void EntityPlayer::render()
{
	//get the last camera thet was activated
	Camera* camera = Camera::current;
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");


	//enable shader and pass uniforms
	shader->enable();
	shader->setUniform("u_model", this->model);
	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));

	shader->setUniform("u_texture", Texture::Get("data/Door_BaseColor.tga"), 0);

	////render the 
	mesh->render(GL_TRIANGLES);
	shader->disable();
}

void EntityPlayer::update(float dt)
{
	Game* game = Game::instance;
	Camera* camera = Camera::current;
	
	float speed = this->player_speed * dt;
	float rotation_speed = this->player_speed_rot * dt;
	if (!game->free_camera) {
		//center camera ar player pos
		
		
		if ((Input::mouse_state & SDL_BUTTON_LEFT) || game->mouse_locked) //is left button pressed?
		{
			this->model.rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f, -1.0f, 0.0f));
			if(Input::mouse_delta.y > 0)
				valor +=  0.05f;
			if(Input::mouse_delta.y < 0)
				valor -= 0.05f;
		}
		if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP))	this->model.translate(0.0f, 0.0f, -speed);
		if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN))  this->model.translate(0.0f, 0.0f, speed);
		if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT))  this->model.rotate(-PI / 2 * rotation_speed, Vector3(0.0f, 1.0f, 0.0f));;
		if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) this->model.rotate(PI / 2 * rotation_speed, Vector3(0.0f, 1.0f, 0.0f));;


		camera->eye = this->model * Vector3(0.0f, 20.0f, -2.0f);
		camera->center = this->model * Vector3(0.0f, valor, -6.0f);
		camera->up = this->model.rotateVector(Vector3(0.0f, 1.0f, 0.0f));

		camera->lookAt(camera->eye, camera->center, camera->up);
		camera->setPerspective(100.f, game->window_width / (float)game->window_height, 0.1f, 10000.f); //set the projection, we want to be perspective

	}
}
