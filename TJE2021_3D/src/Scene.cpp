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
	//var for fog

	Vector4 fogColor = Vector4(0.5f, 0.5f, 0.5f, 1.f);
	float fogDensity = 0.025f;
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	Game* game = Game::instance;
	
	vector<EntityLight*> lights = Game::instance->CurrentScene->lights;
	//get the last camera thet was activated
	Camera* camera = Camera::current;
	if (game->current_stage == game->intro_stage)
		this->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/fog.fs");
	else
		this->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/phong.fs");

	//enable shader and pass uniforms
	this->shader->enable();

	this->shader->setUniform("u_fogColor", fogColor);
	this->shader->setUniform("u_fog_density", fogDensity);
	this->shader->setUniform("u_camera_position", camera->eye);
	this->shader->setUniform("u_fog_settings", Vector2(0.0f,200.0f));

	this->shader->setUniform("u_model", this->model);
	this->shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	this->shader->setUniform("u_color", this->color);
	this->shader->setUniform("u_texture_tiling", this->tiling);
	this->shader->setUniform("u_alpha", this->alpha);
	this->shader->setUniform("u_rand", Vector3(random(), random(), random()));
	this->shader->setUniform("u_texture", this->texture, 0);
	this->shader->setUniform("u_texture_noise", Texture::Get("data/imShader/noise2.tga") , 1);


	for (int i = 0; i < lights.size(); i++)
	{
		if (i != 0)
		{
			if (this->alpha == 1) {
				continue;
			}
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);
		}

		this->shader->setUniform("u_light_position", lights[i]->light_position);
		this->shader->setUniform("u_light_direction", lights[i]->light_vector);
		this->shader->setUniform("u_light_color", lights[i]->color);
		this->shader->setUniform("u_light_type", lights[i]->light_type);
		this->shader->setUniform("u_light_intensity", lights[i]->intensity);
		this->shader->setUniform("u_light_ambient", vec3(0.1, 0.1, 0.1));

			if (lights[i]->light_type == 1) { //si es una spot
				this->shader->setUniform("u_light_maxdist", lights[i]->max_distance);
				this->shader->setUniform("u_light_cutoffCos", lights[i]->spotCosineCutoff);
				this->shader->setUniform("u_light_exponent", lights[i]->spotExponent);
			}

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
	this->light_position =  Vector3(-50.0f, 50.0f, 0.0f);
	this->light_vector = Vector3(0.5, 0, -1);
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
	
	string text = "data/character.ASE";
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
			this->characters[i]->texture = Texture::Get("data/UV.tga");
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
	this->yaw = -90; 
	this->pos = Vector3(-20.0f, 0.0f, 0.0f);
	this->targetPos = this->pos;
	this->pitch = 0.0f;
}


void EntityPlayer::render()
{
	cout << this->pos.x <<" " <<this->pos.y <<" "<< this->pos.z << "\n ";
	Game* game = Game::instance;
	//get the last camera thet was activated
	Camera* camera = Camera::current;
	vector<EntityLight*> lights = Game::instance->CurrentScene->lights;
	
	glDepthFunc(GL_LEQUAL);
	if (game->current_stage == game->body_stage){
		if (game->body_stage->InitStage) {
			this->pos = Vector3(6, 0, -65);
			this->yaw = 0;
			game->body_stage->InitStage = false;
		}
	}
	if (game->current_stage == game->mind_stage) {
		if (game->mind_stage->InitStage) {
			this->yaw = 0;
			this->pos = Vector3(-20.0f, 0, -113.0f);
			game->mind_stage->InitStage = false;
		}
	}
	if (game->current_stage == game->corridor_stage) {
		if (game->corridor_stage->InitStage) {
			this->yaw = -90;
			this->pos = Vector3(4.0f, 0, 0.0f);
			game->corridor_stage->InitStage = false;
		}
	}
	if (game->current_stage == game->soul_stage) {
		if (game->soul_stage->InitStage) {
			this->yaw = -90;
			this->pos = Vector3(-140, 0, -24.0f);
			game->soul_stage->InitStage = false;
		}
	}
	if (game->current_stage == game->intro_stage)
		shader = Shader::Get("data/shaders/skinning.vs", "data/shaders/fog.fs");
	else
		shader = Shader::Get("data/shaders/skinning.vs", "data/shaders/phong.fs");

	this->model =  Matrix44(); //set position
	this->model.translate(this->pos.x, this->pos.y, this->pos.z);
	this->model.rotate(this->yaw * DEG2RAD, Vector3(0.0f, 1.0f, 0.0f));
	
	//First person
	if (!game->free_camera){
		Matrix44 pitch;
		pitch.rotate(this->pitch * DEG2RAD, Vector3(1.0f, 0.0f, 0.0f));

		Vector3 forward = pitch.rotateVector(Vector3(0.0f, 0.0f, 1.0f));
		forward = this->model.rotateVector(forward);

		camera->eye = this->model * Vector3(0.0f, 20.0f, 5.0f);

		camera->center = camera->eye + forward;
		camera->up = Vector3(0.0f, 1.0f, 0.0f);
		
		camera->lookAt(camera->eye, camera->center, camera->up);
		camera->setPerspective(100.f, game->window_width / (float)game->window_height, 2.0f, 10000.f); //set the projection, we want to be perspective

		Input::centerMouse();
		SDL_ShowCursor(false);
	
	}

	//enable shader and pass uniforms
	shader->enable();
	shader->setUniform("u_model", this->model);
	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));

	shader->setUniform("u_texture", this->texture, 0);
	shader->setUniform("u_texture_tiling", 1.0f);
	

	Animation* idle = Animation::Get("data/animation/animations_idle.skanim");
	float t = fmod(game->time, idle->duration) / idle->duration; //norm duracion de la animacion
	idle->assignTime(t * idle->duration);
	Animation* walk = Animation::Get("data/animation/animations_walking.skanim");
	walk->assignTime(t * walk->duration);

	Skeleton skeleton;

	if (this->playerSpeed.length() == 0) {
		vel_factor -= 0.01;
		if (vel_factor < 0)
			vel_factor = 0;
	}
	vel_factor += this->playerSpeed.length() * 0.1;
	blendSkeleton(&idle->skeleton, &walk->skeleton, vel_factor, &skeleton); //si el jugador se mueve aplicar walk, si no idle


	for (int i = 0; i < lights.size(); i++)
	{
		if (i != 0)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);
		}
		this->shader->setUniform("u_light_position", lights[i]->light_position);
		this->shader->setUniform("u_light_direction", lights[i]->light_vector);
		this->shader->setUniform("u_light_color", lights[i]->color);
		this->shader->setUniform("u_light_type", lights[i]->light_type);
		this->shader->setUniform("u_light_intensity", lights[i]->intensity);
		this->shader->setUniform("u_light_ambient", vec3(0.1, 0.1, 0.1));

		if (game->CurrentScene->lights[i]->light_type == 1) { //si es una spot
			this->shader->setUniform("u_light_maxdist", lights[i]->max_distance);
			this->shader->setUniform("u_light_cutoffCos", lights[i]->spotCosineCutoff);
			this->shader->setUniform("u_light_exponent", lights[i]->spotExponent);
		}

		////render the 
		Mesh::Get("data/animation/character.mesh")->renderAnimated(GL_TRIANGLES, &skeleton);

	}

	shader->disable();
	glDisable(GL_BLEND);


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
		playerRotate.setRotation(this->yaw * DEG2RAD, Vector3(0, 1, 0));

		Vector3 playerFront = playerRotate.rotateVector(Vector3(0.0f, 0.0f, 1.0f));
		Vector3 playerRight = playerRotate.rotateVector(Vector3(1.0f, 0.0f, 0.0f));
		playerSpeed = Vector3(0, 0, 0);


		if ((Input::mouse_state & SDL_BUTTON_LEFT) || game->mouse_locked) //is left button pressed?
		{
			if (this->pitch > 60.0f)
				this->pitch = 60.0f;
			else if (this->pitch < -51.0f)
				this->pitch = -51.0f;
			else
				this->pitch += Input::mouse_delta.y * rotation_speed;
			this->yaw -= Input::mouse_delta.x * rotation_speed;
		}


		//Player
		if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) playerSpeed = playerSpeed + (playerFront * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) playerSpeed = playerSpeed - (playerFront * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) playerSpeed = playerSpeed + (playerRight * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) playerSpeed = playerSpeed - (playerRight * speed);

		this->targetPos = this->pos + playerSpeed;
		this->pos = this->targetPos;


		if (scene == game->intro_scene) { //skybox
			scene->entities[4]->model.setIdentity();
			scene->entities[4]->model.translate(camera->eye.x, camera->eye.y - 10, camera->eye.z);
		}


		this->collisionMesh(dt); 	//Collision
		this->Interaction();

		if (game->current_stage == game->intro_stage) {  //animation intro
			if ((-25.0f < this->pos.z && this->pos.z < 1.0f && -11.0f < this->pos.x) || game->current_stage->Timeanimation != 0.0f)
			{
				game->current_stage->animation = true;
			}
			else game->current_stage->animation = false;
			if (-25.0f < this->pos.z && this->pos.z < 1.0f && this->pos.x > 19.0f)
			{
				game->CurrentScene->entities.clear();
				game->current_stage = game->body_stage;
				game->CurrentScene = game->BodyScene;
				game->current_stage->createEntities();
			}
		}

		if (game->current_stage == game->body_stage) {  //animation bodystage

			if ((-7.0f < this->pos.x && this->pos.x < 14.0f && -42.0f < this->pos.z && -20.f > this->pos.z) || game->current_stage->Timeanimation != 0.0f)
			{
				game->current_stage->animation = true;
			}
			if ((-7.0f < this->pos.x && this->pos.x < 14.0f && -11.f < this->pos.z && game->body_stage->animation2) && game->current_stage->glassCount == 0)
			{
				game->current_stage->animation = true;
				game->current_stage->firstTime = true;
				game->body_stage->animation2 = false;

			}
			if (-36.0f > this->pos.z && game->current_stage->changeGlass)
			{
				game->CurrentScene->entities.clear();
				game->CurrentScene->entities_mirror.clear();
				float aux = game->current_stage->glassCount;
				game->current_stage = game->corridor_stage;
				game->CurrentScene = game->corridor_scene;
				game->current_stage->glassCount = aux;
				game->current_stage->createEntities();
			}
		}
		if (game->current_stage == game->mind_stage) {  //animation mindstage

			if ((-90.0f < this->pos.z && -58.0f > this->pos.z) || game->current_stage->Timeanimation != 0.0f) //abrir
			{
				game->current_stage->animation = true;
			}
			if ((-30.0f < this->pos.z && game->mind_stage->animation2) && game->current_stage->isRa == false) //cerrar
			{
				game->current_stage->animation = true;
				game->current_stage->firstTime = true;
				game->current_stage->animation2 = false;

			}
			if (-91.0f > this->pos.z && game->current_stage->changeGlass)
			{
				game->CurrentScene->entities.clear();
				game->CurrentScene->entities_mirror.clear();
				float aux = game->current_stage->glassCount;
				game->current_stage = game->corridor_stage;
				game->CurrentScene = game->corridor_scene;
				game->current_stage->glassCount = aux;
				game->current_stage->createEntities();
			}
		}
		if (game->current_stage == game->soul_stage) {
			if ((-90.0f < this->pos.x && -58.0f > this->pos.x) || game->current_stage->Timeanimation != 0.0f) //abrir
			{
				game->current_stage->animation = true;
			}
			if ((-30.0f < this->pos.x && game->current_stage->animation2)) //cerrar
			{
				game->current_stage->animation = true;
				game->current_stage->firstTime = true;
				game->current_stage->animation2 = false;

			}
			if (-60.0f > this->pos.x && game->current_stage->changeGlass) //abrir puerta
			{
				game->CurrentScene->entities.clear();
				game->CurrentScene->entities_mirror.clear();
				float aux = game->current_stage->glassCount;
				game->current_stage = game->corridor_stage;
				game->CurrentScene = game->corridor_scene;
				game->current_stage->glassCount = aux;
				game->current_stage->createEntities();
			}
		}
		if (game->current_stage == game->corridor_stage) {  //animation mindstage
			if (this->pos.z < -60) {
				float aux = game->current_stage->glassCount;
				game->current_stage = game->body_stage;
				game->CurrentScene = game->BodyScene;
				game->current_stage->glassCount = aux;
				game->current_stage->createEntities();
			}
			if (this->pos.z > 50) {
				float aux = game->current_stage->glassCount;
				game->current_stage = game->soul_stage;
				game->CurrentScene = game->soul_scene;
				game->current_stage->glassCount = aux;
				game->current_stage->createEntities();
			}
			if (this->pos.x > 100) {
				float aux = game->current_stage->glassCount;
				game->current_stage = game->mind_stage;
				game->CurrentScene = game->mind_scene;
				game->current_stage->glassCount = aux;
				game->current_stage->createEntities();
			}
		}
	
	}

}

void EntityPlayer::collisionMesh(float dt)
{
	
	Scene* currentScene = Game::instance->CurrentScene; 
	//// calculamos el centro de la esfera de colisi�n del player elevandola hasta la cintura
	Vector3 character_center = this->pos + Vector3(0, 2, 0);
	this->player_speed = Vector3(20.0f, 0.0f, 20.0f);
	////para cada objecto de la escena...
	for  (int i = 1; i < currentScene->entities.size(); i++)
	{
		Vector3 col_point; 	//temp var para guardar el punto de colision si lo hay
		Vector3 col_normal; 	//temp var para guardar la normal al punto de colision
		
		//comprobamos si colisiona el objeto con la esfera (radio 3)
		if (currentScene->entities[i]->isColision){
			if (this->mesh->testSphereCollision(currentScene->entities[i]->model, character_center, 7, col_point, col_normal) == false) {
				continue; //si no colisiona, pasamos al siguiente objeto
			}
			//cout << currentScene->entities[i]->id << "\n";
			//si la esfera est� colisionando muevela a su posicion anterior alejandola del objeto
			Vector3 push_away = normalize(col_point - character_center) * dt;
			this->pos = this->pos - push_away; //move to previous pos but a little bit further

			//cuidado con la Y, si nuestro juego es 2D la ponemos a 0, nostras no tenemos el player a niguna altura nunca
			this->pos.y = 0;

			//reflejamos el vector velocidad para que de la sensacion de que rebota en la pared
			this->player_speed = reflect(this->player_speed, col_normal) * 0.95;

		}
		
	}

}

void EntityPlayer::Interaction()
{

	Scene* currentScene = Game::instance->CurrentScene;
	Stage* currentStage = Game::instance->current_stage;
	Game* game = Game::instance;

	//// calculamos el centro de la esfera de colisi�n del player elevandola hasta la cintura
	Camera* camera = Camera::current;
	Vector3 character_center = this->pos + Vector3(0, 2, 0);

	Vector3 ray_origin = camera->eye;
	Vector3 ray_dir = camera->center;
	////para cada objecto de la escena...
	for (int i = 1; i < currentScene->entities.size(); i++)
	{
		Vector3 col_point; 	//temp var para guardar el punto de colision si lo hay
		Vector3 col_normal; 	//temp var para guardar la normal al punto de colision

		float max_ray_dist = 100;
		if (currentScene->entities[i]->isInteractive){

			if (currentStage == game->body_stage) {
				if (currentScene->entities[i]->id == 12) {
					if (this->pos.x > -32 && this->pos.x < -25 && this->pos.z > 2 && this->pos.x < 7) { //estoy mirando si el player esta cerca de el cristal lo pongo en alpha 1 si aprieto shift
						if (Input::wasKeyPressed(SDL_SCANCODE_LSHIFT)) {
							game->body_stage->glassCount += 1;
							currentStage->changeGlass = true;
							currentScene->entities[i]->alpha = 1;
						}
					}
				}
			}
		}
		if (currentStage == game->mind_stage) {
			if (!game->mind_stage->isAmulet) {
				if (currentScene->entities[i]->id == 9 || currentScene->entities[i]->id == 6 || currentScene->entities[i]->id == 5) {
					if (this->mesh->testSphereCollision(currentScene->entities[i]->model, character_center,20, col_point, col_normal) == true) {
						
						if (Input::wasKeyPressed(SDL_SCANCODE_LSHIFT)) {
							game->mind_stage->id = i-1;
							game->mind_stage->isAmulet = true;
							currentScene->entities_mirror[i-1]->alpha = 1;
							if (currentScene->entities[i]->id == 9)
								currentStage->amuleto = true;
							if (currentScene->entities[i]->id == 6)
								currentStage->cruz = true;
							if (currentScene->entities[i]->id == 5)
								currentStage->grail = true;
						}
					}
				}
			}
			else
			{
				if (currentScene->entities[i]->id == 14) { //altar
					
					if (this->mesh->testSphereCollision(currentScene->entities[i]->model, character_center, 20, col_point, col_normal) == true) {
						if (Input::wasKeyPressed(SDL_SCANCODE_LSHIFT)) {
							if (game->mind_stage->id == 8) //si objeto obtenido es el amuleto Ra (bueno) seguimos con el puzzle, si no se vuelve a intentar
							{
								game->mind_stage->isRa = true;
								currentStage->amuleto = false;

							}
							else //bajar intensidad de la spot
							{
								game->mind_stage->isAmulet = false;
								game->mind_stage->isRa = false;
								currentScene->entities_mirror[game->mind_stage->id]->alpha = 0;
								currentScene->lights[0]->intensity -= 0.3f;
								if (currentScene->lights[0]->intensity < 0)
									currentScene->lights[0]->intensity = 0;
								if (game->mind_stage->id == 5)
									currentStage->cruz = false;
								if (game->mind_stage->id == 4)
									currentStage->grail = false;
							}
						}
					}
				}
				if (currentScene->entities[i]->id == 10) { //trozo espejo

					if (this->mesh->testSphereCollision(currentScene->entities[i]->model, character_center, 40, col_point, col_normal) == true) {
						if (Input::wasKeyPressed(SDL_SCANCODE_LSHIFT)) {
							currentStage->glassCount += 1;
							currentStage->changeGlass = true;
							currentScene->entities[i]->alpha = 1;
							game->mind_stage->isRa = false;

						}
					}
				}
			}
		}
		if (currentStage == game->soul_stage) {
			if (this->mesh->testSphereCollision(currentScene->entities[i]->model, character_center, 100, col_point, col_normal) == true) {
				if (Input::wasKeyPressed(SDL_SCANCODE_LSHIFT)) {
					if (currentScene->entities[i]->id == 2) //si objeto obtenido arrow seguimos con el puzzle, si no se vuelve a intentar
					{
						currentStage->arrow = true;
						currentScene->entities[i]->alpha = 1;

					}
				}
			}
			if (currentScene->entities[i]->id == 3 || currentScene->entities[i]->id == 4 || currentScene->entities[i]->id == 5) { //algun altar

				if (this->mesh->testSphereCollision(currentScene->entities[i]->model, character_center, 40, col_point, col_normal) == true) {
					if (Input::wasKeyPressed(SDL_SCANCODE_LSHIFT)) {
						
						if (currentScene->entities[i]->id == 5 && currentStage->arrow) {
							//mostrar trozo espejo y romper el espejo grande
							currentScene->entities[15]->alpha = 0;
							currentStage->arrow = false;
						}
						
					}
				}
			}
			if (currentScene->entities[i]->id == 15) { //trozo espejo

				if (this->mesh->testSphereCollision(currentScene->entities[i]->model, character_center, 40, col_point, col_normal) == true) {
					if (Input::wasKeyPressed(SDL_SCANCODE_LSHIFT)) {
						currentStage->glassCount += 1;
						currentStage->changeGlass = true;
						currentScene->entities[i]->alpha = 1;
					}
				}
			}
		}
		if (currentStage == game->corridor_stage) {
			if (currentScene->entities[i]->id == 7) { //portal

				if (this->mesh->testSphereCollision(currentScene->entities[i]->model, character_center, 40, col_point, col_normal) == true)
				{
					if (currentStage->glassCount == 3) //si tenemos los 3 trozos y interactuamos se genera el espejo
					{
						if (Input::wasKeyPressed(SDL_SCANCODE_LSHIFT)) {
							currentScene->entities[i]->alpha = 0;
						}
					}
				}
			}
		}
	}
}
