#include "mygame.h"
#include "game.h"
#include "input.h"


void IntroStage::render(Image& framebuffer){
	Game* game = Game::instance;
	framebuffer.drawImage(game->menu->sprite, 0, 0, 160, 120);
	framebuffer.drawText("Hello world",160/2-45, 120/2-10, game->world->font);				//draws some text using a bitmap font in an image (assuming every char is 7x9)
}

void IntroStage::update(double seconds_elapsed) {
	Game* game = Game::instance;

	if (Input::isKeyPressed(SDL_SCANCODE_A)) //if key up
	{
		game->current_stage = game->play_stage;
	}
};

void PlayStage::render(Image& framebuffer) {
	Game* game = Game::instance;
	World* world = Game::instance->world;
	sPlayer* player = &Game::instance->world->myGame.players[0];
	

	//size in pixels of a cell, we assume every row has 16 cells so the cell size must be image.width / 16
	int cs = world->tileset.width / 16 ; //size of cellin tileset
	
	//for every cell
	for (int x = 0; x < game->map->width; ++x)
		for (int y = 0; y < game->map->height; ++y)
		{
			//get cell info
			sCell& cell = game->map->getCell(x, y);
			if (cell.type == 0) //skip empty
				continue;
			int type = (int)cell.type;
			//compute tile pos in tileset image
			int tilex = (type % 16) * cs; 	//x pos in tileset
			int tiley = floor(type / 16) * cs;	//y pos in tileset
			Area area(tilex , tiley , cs , cs); //tile area
			int screenx = (x * cs) + world->camera.position.x; //place offset here if you want
			int screeny = (y * cs) + world->camera.position.y;
			//avoid rendering out of screen stuff
			if (screenx < -cs || screenx > framebuffer.width ||
				screeny < -cs || screeny > framebuffer.height)
				continue;

			
			//draw region of tileset inside framebuffer
			framebuffer.drawImage(world->tileset,screenx, screeny, area);
			
		}
	
	world->animation.current_animation = player->ismoving 
	? (int(game->time*world->animation.velocity_animation) % world->animation.num_animations) : 0;
	framebuffer.drawImage(game->sprite->sprite, 
		player->pos.x,
		player->pos.y,
		Area(14  *world->animation.current_animation, 18*(int)player->dir, 14, 18));	//draws only a part of an image
	player->ismoving = 0;
	//framebuffer.drawImage(game->health->sprite, 0, 0, 1, 1);

}

void PlayStage::update(double seconds_elapsed) { //movement of the character
	Game* game = Game::instance;
	World* world = Game::instance->world;
	sPlayer* player = &Game::instance->world->myGame.players[0];

	//if (Input::isKeyPressed(SDL_SCANCODE_UP)) //if key up
	//{
	//	player->ismoving = 1;
	//	world->camera.position.y += world->camera.velocity * seconds_elapsed;
	//	player->pos.y -= world->player_velocity.y * seconds_elapsed;
	//	player->dir = eDirection::UP;
	//}
	if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) //if key down //If only not collision
	{
		player->ismoving = 1;
		world->camera.position.y -= world->camera.velocity * seconds_elapsed;
		player->pos.y += world->player_velocity * seconds_elapsed;
		player->dir = eDirection::DOWN;

	}
	if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) //if key right
	{
		player->ismoving = 1;
		world->camera.position.x -=world->camera.velocity * seconds_elapsed;
		player->pos.x += world->player_velocity * seconds_elapsed;
		player->dir = eDirection::RIGHT;

	}
	if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) //if key left
	{
		player->ismoving = 1;
		world->camera.position.x += world->camera.velocity * seconds_elapsed;
		world->myGame.players[0].pos.x -=world->player_velocity * seconds_elapsed;
		world->myGame.players[0].dir = eDirection::LEFT;
	}
	if (Input::isKeyPressed(SDL_SCANCODE_RETURN)) //if key left
	{	
		game->current_stage->restart();
		game->current_stage = game->intro_stage;
	}
	if (Input::isKeyPressed(SDL_SCANCODE_SPACE)) //jump action
	{
		player->ismoving = 0;
		world->camera.position.y += world->camera.velocity * seconds_elapsed;
		player->pos.y -= world->player_velocity* seconds_elapsed;
		player->dir = eDirection::UP;
	}


};

World::World() {

	this->camera.position = Vector2(0, 0);
	this->myGame.players[0].pos = Vector2(0, 100);
	this->myGame.players[0].dir = eDirection::RIGHT;
	this->myGame.players[0].health = 6;
};

GameMap::GameMap()
{
	this->width = height = 0;
	this->data = NULL;
};

GameMap::GameMap(int w, int h)
{
	this->width = w;
	this->height = h;
	this->data = new sCell[w * h];
};

sCell& GameMap::getCell(int x, int y)
{
	return data[x + y * width];
};

//example of parser of .map from rogued editor
GameMap* GameMap::loadGameMap(const char* filename)
{
	FILE* file = fopen(filename, "rb");
	if (file == NULL) //file not found
		return NULL;
	else
		std::cout << " + GameMap loaded: "  << filename << "\n ";

	sMapHeader header; //read header and store it in the struct
	fread(&header, sizeof(sMapHeader), 1, file);
	assert(header.bytes == 1); //always control bad cases!!


	//allocate memory for the cells data and read it
	unsigned char* cells = new unsigned char[header.w * header.h];
	fread(cells, header.bytes, header.w * header.h, file);
	fclose(file); //always close open files
	//create the map where we will store it
	GameMap* map = new GameMap(header.w, header.h);

	for (int x = 0; x < header.w; x++)
		for (int y = 0; y < header.h; y++)
			map->getCell(x, y).type = (eCellType)cells[x + y * header.w];

	delete[] cells; //always free any memory allocated!

	return map;
};


void PlayStage::restart() { //Restart the game
	World* world = Game::instance->world;
	sPlayer* player = &Game::instance->world->myGame.players[0];

	player->pos = Vector2(0, 100);	
	world->camera.position = Vector2(0, 0);
	player->dir = eDirection::RIGHT;
	player-> health = 6;
}; 
