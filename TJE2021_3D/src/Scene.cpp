#include "Scene.h"
#include "game.h"
#include "framework.h"

EntityMesh::EntityMesh()
{
	this->mesh = new Mesh();
	this->texture = new Texture();
	this->shader = new Shader();
	this->color =  Vector4(1, 1, 1, 1);
	this->alpha = 0; 
	this->isColision = true;
	this->tiling = 1.0f;
}

void EntityMesh::render()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	Game* game = Game::instance;
	vector<EntityLight*> lights = Game::instance->CurrentScene->lights;
	//get the last camera thet was activated
	Camera* camera = Camera::current;
	this->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	//if(game->free_camera)
	//	this->mesh->renderBounding(this->model); 

	//enable shader and pass uniforms
	this->shader->enable();
	this->shader->setUniform("u_model", this->model);
	this->shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	this->shader->setUniform("u_color", this->color);
	this->shader->setUniform("u_texture_tiling", this->tiling);
	this->shader->setUniform("u_alpha", this->alpha);
	this->shader->setUniform("u_texture", this->texture,0);
	for (int i = 0; i < lights.size(); i++)
	{
		

		this->shader->setUniform("u_light_position", lights[i]->light_position);
		this->shader->setUniform("u_light_direction", lights[i]->light_vector);
		this->shader->setUniform("u_light_color", lights[i]->color);
		this->shader->setUniform("u_light_type", lights[i]->light_type);
		this->shader->setUniform("u_light_intensity", lights[i]->intensity);
		this->shader->setUniform("u_light_ambient", vec3(0.1, 0.1, 0.1));

		//if (game->current_stage == game->play_stage) { //si estamos en segunda sala, render de spot
			if (game->CurrentScene->lights[i]->light_type == 1) { //si es una spot
				this->shader->setUniform("u_light_maxdist", lights[i]->max_distance);
				this->shader->setUniform("u_light_cutoffCos", lights[i]->spotCosineCutoff);
				this->shader->setUniform("u_light_exponent", lights[i]->spotExponent);
			}
		//}
		
		////render the 
		this->mesh->render(GL_TRIANGLES);
	}
	
	this->shader->disable();
	glDisable(GL_BLEND);
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
	this->color = Vector3(1.0f, 1.0f, 1.0f);
	this->intensity = 1.0f;
	this->light_type = eLightType::DIRECTIONAL;
	this->light_position = Vector3(0,0,0);
	this->light_vector = Vector3(0.5, 0, -1);

	////Shadows
	//this->bias = 0.001f;
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
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/phong.fs");

	this->model =  Matrix44(); //set position
	this->model.translate(this->pos.x, this->pos.y, this->pos.z);
	this->model.rotate(this->yaw * DEG2RAD, Vector3(0.0f, 1.0f, 0.0f));
	
	//First person
	if (!game->free_camera){
		Matrix44 pitch;
		pitch.rotate(this->pitch * DEG2RAD, Vector3(1.0f, 0.0f, 0.0f));

		Vector3 forward = pitch.rotateVector(Vector3(0.0f, 0.0f, -1.0f));
		forward = this->model.rotateVector(forward);

		camera->eye = this->model * Vector3(0.0f, 20.0f, -5.0f);

		camera->center = camera->eye + forward;
		camera->up = Vector3(0.0f, 1.0f, 0.0f);
		
		camera->lookAt(camera->eye, camera->center, camera->up);
		camera->setPerspective(100.f, game->window_width / (float)game->window_height, 2.0f, 10000.f); //set the projection, we want to be perspective

		Input::centerMouse();
		SDL_ShowCursor(false);
	
	}
	/*else
		this->mesh->renderBounding(this->model);*/

	//enable shader and pass uniforms
	shader->enable();
	shader->setUniform("u_model", this->model);
	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));

	shader->setUniform("u_texture", this->texture, 0);
	shader->setUniform("u_texture_tiling", 1.0f);

	////render the 
	mesh->render(GL_TRIANGLES);
	shader->disable();
}

void EntityPlayer::update(float dt)
{
	Game* game = Game::instance;
	Scene* scene = Game::instance->CurrentScene;
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
			if (this->pitch > 60.0f)
				this->pitch = 60.0f;
			else if (this->pitch < -51.0f)
				this->pitch = -51.0f;
			else
				this->pitch -= Input::mouse_delta.y * rotation_speed;
			this->yaw -= Input::mouse_delta.x * rotation_speed;
		}
		
		
		//Player
		if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) playerSpeed = playerSpeed - (playerFront*speed);
		if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) playerSpeed = playerSpeed + (playerFront * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) playerSpeed = playerSpeed - (playerRight * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) playerSpeed = playerSpeed + (playerRight * speed);

		this->targetPos = this->pos + playerSpeed; 
		this->pos = this->targetPos;

		
		if (scene == game->intro_scene) { //skybox
			scene->entities[4]->model.setIdentity();
			scene->entities[4]->model.translate(camera->eye.x, camera->eye.y-10, camera->eye.z);
		}

		this->collisionMesh(dt); 	//Collision
		
		if (game->current_stage == game->intro_stage) {  //animation intro
			if ((-25.0f < this->pos.z && this->pos.z < 1.0f && -11.0f < this->pos.x) || game->current_stage->Timeanimation != 0.0f)
			{
				game->current_stage->animation = true;
			}
			else game->current_stage->animation = false;
			if ( -25.0f < this->pos.z && this->pos.z < 1.0f && this->pos.x > 19.0f )
			{
				game->CurrentScene->entities.clear();
				game->current_stage = game->play_stage;
				game->CurrentScene = game->PlayScene;
				game->current_stage->createEntities();
			}
		}

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
	////para cada objecto de la escena...
	for  (int i = 1; i < currentScene->entities.size(); i++)
	{
		
		////comprobamos si colisiona el objeto con la esfera (radio 3)
		if (this->mesh->testSphereCollision(currentScene->entities[i]->model, character_center, 10, col_point, col_normal) == false) {
			this->player_speed = Vector3(20.0f, 0.0f, 20.0f);
			continue; //si no colisiona, pasamos al siguiente objeto

		}
		//si la esfera está colisionando muevela a su posicion anterior alejandola del objeto
		Vector3 push_away = normalize(col_point - character_center) * dt;
		this->pos = this->pos - push_away; //move to previous pos but a little bit further

		////cuidado con la Y, si nuestro juego es 2D la ponemos a 0
		this->pos.y = 0 ;

		//reflejamos el vector velocidad para que de la sensacion de que rebota en la pared
		this->player_speed = reflect(this->player_speed, col_normal) * 0.96;
		
	}

}
