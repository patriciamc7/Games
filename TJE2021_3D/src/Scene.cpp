#include "Scene.h"
#include "game.h"
#include "framework.h"

EntityMesh::EntityMesh()
{
	this->mesh = new Mesh();
	this->texture = new Texture();
	this->shader = new Shader();
	this->color =  Vector4(1, 1, 1, 1);
	this->alpha = false; 
	this->isColision = true;
}

void EntityMesh::render()
{
	Game* game = Game::instance; 
	//get the last camera thet was activated
	Camera* camera = Camera::current;
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	if(game->free_camera)
		this->mesh->renderBounding(this->model); 
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
		if (i == 0) 
			this->characters[i]->texture = Texture::Get("data/texture.tga");
		
	}
}

EntityPlayer::EntityPlayer()
{
	this->mesh = new Mesh();
	this->shader = new Shader();
	this->texture = new Texture();
	this->center_value_y = 20.0f;
	this->player_speed = Vector3(20.0f,0, 20.0f);
	this->player_speed_rot = 50.0f;
	this->yaw = 90; 
	this->pos = Vector3(-20.0f, 0.0f, 0.0f);
	this->targetPos = this->pos;
	this->pitch = 0.0f; 

}


void EntityPlayer::render()
{
	Game* game = Game::instance;
	//get the last camera thet was activated
	Camera* camera = Camera::current;
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	//this->player_speed = Vector3(20.0f, 0.0f, 20.0f);
	this->model =  Matrix44(); 
	this->model.translate(this->pos.x, this->pos.y, this->pos.z);
	this->model.rotate(this->yaw * DEG2RAD, Vector3(0.0f, 1.0f, 0.0f));
	
	if (game->free_camera)
		this->mesh->renderBounding(this->model);
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
	

	if (!game->free_camera) {
		float speed = this->player_speed.x * dt;
		float rotation_speed = this->player_speed_rot * dt;
		//center camera ar player pos
		Matrix44 playerRotate; 
		playerRotate.setRotation(this->yaw *DEG2RAD, Vector3(0,1,0)); 
		
		Vector3 playerFront = playerRotate.rotateVector(Vector3(0.0f, 0.0f, 1.0f));
		Vector3 playerRight = playerRotate.rotateVector(Vector3(1.0f,0.0f,0.0f));
		Vector3 playerSpeed; 
		if ((Input::mouse_state & SDL_BUTTON_LEFT) || game->mouse_locked) //is left button pressed?
		{
			this->model.rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f, -1.0f, 0.0f));
			if(Input::mouse_delta.y > 0){
				center_value_y += 0.05f;
				if (center_value_y > 23) center_value_y = 23.0f;
			}
				
			if (Input::mouse_delta.y < 0) {
				center_value_y -= 0.05f;
				if (center_value_y < 13) center_value_y = 13.0f;
			}

		}
		//PLayer
		if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) playerSpeed = playerSpeed - (playerFront*speed);
		if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) playerSpeed = playerSpeed + (playerFront * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) playerSpeed = playerSpeed - (playerRight * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) playerSpeed = playerSpeed + (playerRight * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_Q) ) this->yaw -= rotation_speed;
		if (Input::isKeyPressed(SDL_SCANCODE_E) )this->yaw += rotation_speed;

		this->targetPos = this->pos + playerSpeed; 
		this->pos = this->targetPos; 
		camera->eye = this->model * Vector3(0.0f, 20.0f, -2.0f);
		camera->center = this->model * Vector3(0.0f, center_value_y, -6.0f);
		camera->up = this->model.rotateVector(Vector3(0.0f, 1.0f, 0.0f));

		camera->lookAt(camera->eye, camera->center, camera->up);
		camera->setPerspective(100.f, game->window_width / (float)game->window_height, 1.0f, 10000.f); //set the projection, we want to be perspective
		
		//Collision
		this->collisionMesh(dt);
		
		if ((-25.0f < this->pos.z && this->pos.z < 1.0f && -11.0f < this->pos.x) || game->current_stage->Timeanimation != 0.0f)
		{
			if (game->current_stage == game->intro_stage)
				game->current_stage->animation = true;

		}
		else game->current_stage->animation = false;
		
		

	}
}

void EntityPlayer::collisionMesh(float dt)
{
	Scene* currentScene = Game::instance->CurrentScene; 
	//// calculamos el centro de la esfera de colisión del player elevandola hasta la cintura
	Vector3 character_center = this->pos + Vector3(0, 4, 0);
	Vector3 col_point; 	//temp var para guardar el punto de colision si lo hay
	Vector3 col_normal; 	//temp var para guardar la normal al punto de colision
	float max_ray_dist = 10;
	Vector3 ray_origin = this->pos;
	Vector3 ray_dir = this->player_speed;
	//cout << character_center.x << " " << character_center.y << " " << character_center.z<<"\n"; 
	////para cada objecto de la escena...
	for  (int i = 1; i < currentScene->entities.size(); i++)
	{
		if (currentScene->entities[i]->isColision){
			////comprobamos si colisiona el objeto con la esfera (radio 3)
			if (this->mesh->testSphereCollision(currentScene->entities[i]->model, character_center, 10, col_point, col_normal) == false) {
			//if (mesh->testRayCollision(currentScene->entities[i]->model, 	//the model of the entity to know where it is
			//	character_center,		//the origin of the ray we want to test
			//	ray_dir,		//the dir vector of the ray
			//	col_point,		//here we will h
			//	col_normal,		//a temp var to store the collision normal
			//	max_ray_dist,	//max ray distance to test
			//	false			//false if we want the col_point in world space (true if in object)
			//	) == false)
			//{
				//cout << "NO colisiona\n";
				this->player_speed = Vector3(20.0f, 0.0f, 20.0f);
				continue; //si no colisiona, pasamos al siguiente objeto
			}
			//else
				//cout << "Colisiona\n"; 
		/*	cout << col_normal.x <<" "<< col_normal.y << " " << col_normal.z << " " << "\n";*/

			//si la esfera está colisionando muevela a su posicion anterior alejandola del objeto
			Vector3 push_away = normalize(col_point - character_center) * dt;
			this->pos = this->pos - push_away; //move to previous pos but a little bit further

			////cuidado con la Y, si nuestro juego es 2D la ponemos a 0
			this->pos.y = 0;

			//reflejamos el vector velocidad para que de la sensacion de que rebota en la pared
			this->player_speed = reflect(this->player_speed, col_normal) * 0.96;
		}
	}

}
