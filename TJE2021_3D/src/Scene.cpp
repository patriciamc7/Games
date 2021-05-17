#include "Scene.h"
#include "game.h"


EntityMesh::EntityMesh()
{
	this->mesh = new Mesh();
	this->texture = new Texture();
	this->shader = new Shader();
	this->color =  Vector4(1, 1, 1, 1);
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
	shader->setUniform("u_color", this->color);

	shader->setUniform("u_texture", this->texture,0);

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

void EntitySound::render()
{
}

void EntitySound::update(float dt)
{
}

EntityLight::EntityLight()
{
	//General light
	this->color = Vector3(0, 0, 0);
	this->intensity = 20.f;
	this->light_type = eLightType::SPOT;
	this->light_position = Vector3(0,0,0);
	this->light_vector = Vector3(0, 0, 0);

	//Spot
	this->spotCosineCutoff= 10.f;
	this->max_distance = 10.f;
	this->spotExponent = 20.f;
	this->flag = 0;

	//Shadows
	this->bias = 0.001f;
}


void EntityLight::render()
{
}

void EntityLight::update(float dt)
{
}

Scene::Scene()
{
	this->CreatePlayer();
}

void Scene::CreatePlayer()
{
	
	string text = "data/Character.ASE";
	string cad;
	int found = -1;
	int init = 0;
	for (int i = 0; i < MAX_CHARACTERS; i++)
	{
		this->characters.push_back(new EntityPlayer());
		init = found + 1;
		found = text.find(",", found + 1);
		cad = text.substr(init, found - init);
		this->characters[i]->mesh = Mesh::Get(cad.c_str());
		this->characters[i]->id = i;
		this->entities.push_back(this->characters[i]);
		if (i == 0) {
			this->characters[i]->model.translate(-20.0f, 0.0f, 0.0f);
			this->characters[i]->model.rotate(PI / 2, Vector3(0, 1, 0));
			this->characters[i]->texture = Texture::Get("data/texture.tga");

		}
	}
}

EntityPlayer::EntityPlayer()
{
	this->mesh = new Mesh();
	this->shader = new Shader();
	this->texture = new Texture();
	this->center_value_y = 20.0f;
	this->player_speed = 20.0f;
	this->player_speed_rot = 1.0f;
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

	shader->setUniform("u_texture", this->texture, 0);

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
				center_value_y +=  0.05f;
			if(Input::mouse_delta.y < 0)
				center_value_y -= 0.05f;
		}
		if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP))	this->model.translate(0.0f, 0.0f, -speed);
		if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN))  this->model.translate(0.0f, 0.0f, speed);
		if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT))  this->model.rotate(-PI / 2 * rotation_speed, Vector3(0.0f, 1.0f, 0.0f));;
		if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) this->model.rotate(PI / 2 * rotation_speed, Vector3(0.0f, 1.0f, 0.0f));;


		camera->eye = this->model * Vector3(0.0f, 20.0f, -2.0f);
		camera->center = this->model * Vector3(0.0f, center_value_y, -6.0f);
		camera->up = this->model.rotateVector(Vector3(0.0f, 1.0f, 0.0f));

		camera->lookAt(camera->eye, camera->center, camera->up);
		camera->setPerspective(100.f, game->window_width / (float)game->window_height, 0.1f, 10000.f); //set the projection, we want to be perspective

	}
}
