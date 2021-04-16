#include "mygame.h"
#include "game.h"
#include "input.h"


void IntroStage::render(Image& framebuffer) {
	framebuffer.drawLine(0, 0, 160, 120, Color::RED);
	
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

	//size in pixels of a cell, we assume every row has 16 cells so the cell size must be image.width / 16
	int cs = game->world->tileset.width / 16 ; //size of cellin tileset
	
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
			int screenx = (x * cs) + game->world->camera.position.x; //place offset here if you want
			int screeny = (y * cs) + game->world->camera.position.y;
			//avoid rendering out of screen stuff
			if (screenx < -cs || screenx > framebuffer.width ||
				screeny < -cs || screeny > framebuffer.height)
				continue;

			//draw region of tileset inside framebuffer
			
			framebuffer.drawImage(game->world->tileset,screenx, screeny,area);
			
		}
	
	framebuffer.drawImage(game->world->sprite, game->world->myGame.players[0].pos.x, game->world->myGame.players[0].pos.y, Area(0, 0, 14, 18));	//draws only a part of an image

}

void PlayStage::update(double seconds_elapsed) { //movement of the character
	Game* game = Game::instance;

	if (Input::isKeyPressed(SDL_SCANCODE_UP)) //if key up
	{
		game->world->camera.position.y += game->world->camera.velocity * seconds_elapsed;
		game->world->myGame.players[0].pos.y -= game->world->myGame.players[0].player_velocity * seconds_elapsed;
	}
	if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) //if key down
	{
		game->world->camera.position.y -= game->world->camera.velocity * seconds_elapsed;
		game->world->myGame.players[0].pos.y += game->world->myGame.players[0].player_velocity * seconds_elapsed;
	}
	if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) //if key right
	{
		game->world->camera.position.x -= game->world->camera.velocity * seconds_elapsed;
		game->world->myGame.players[0].pos.x += game->world->myGame.players[0].player_velocity * seconds_elapsed;
	}
	if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) //if key left
	{
		game->world->camera.position.x += game->world->camera.velocity * seconds_elapsed;
		game->world->myGame.players[0].pos.x -= game->world->myGame.players[0].player_velocity * seconds_elapsed;
	}
	if (Input::isKeyPressed(SDL_SCANCODE_A)) //if key left
	{
		game->current_stage = game->intro_stage;
	}

};

World::World() {
	this->camera.velocity = 100;
	this->camera.position.x = myGame.players[0].pos.x;
	this->camera.position.y = myGame.players[0].pos.y;
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

