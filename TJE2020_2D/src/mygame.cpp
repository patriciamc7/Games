#include "mygame.h"
#include "game.h"
#include "input.h"


void IntroStage::render(Image& framebuffer){
	
	Game* game = Game::instance;
	framebuffer.drawImage(game->menu->sprite, 0, 0, 160, 120);
	framebuffer.drawText("SHAFT",160/2-20, 10, game->world->font);
	framebuffer.drawText("Start", 160 / 2 - 20, 40, game->world->font);	
	framebuffer.drawText("Tutorial", 160 / 2 - 30, 50, game->world->font);	
	framebuffer.drawText("Load", 160 / 2 - 18, 60, game->world->font);
	framebuffer.drawText("Exit", 160 / 2 - 18, 70, game->world->font);	
	if (game->world->button == 0) framebuffer.drawTriangle(160 / 2 - 27, 40, 160 / 2 - 22, 45, 160 / 2 - 27, 50, Color(0, 0, 0));
	if (game->world->button == 1) framebuffer.drawTriangle(160 / 2 - 35, 50, 160 / 2 - 30, 55, 160 / 2 - 35, 60, Color(0, 1, 0));	
	if (game->world->button == 2) framebuffer.drawTriangle(160 / 2 - 27, 60, 160 / 2 - 22, 65, 160 / 2 - 27, 70, Color(0, 0, 10));
	if (game->world->button == 3) framebuffer.drawTriangle(160 / 2 - 27, 70, 160 / 2 - 22, 75, 160 / 2 - 27, 80, Color(0, 0, 10));
	
}

void IntroStage::update(double seconds_elapsed) {
	Game* game = Game::instance;
	World* world = Game::instance->world;

	
	if (Input::wasKeyPressed(SDL_SCANCODE_DOWN)) //if key down //If only not collision
	{
		world->button += 1;
		if (world->button == 4) world->button = 0;
		
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_UP)) //if key down //If only not collision
	{
		world->button -= 1;
		if (world->button == -1) world->button = 0;
	}
	if (Input::isKeyPressed(SDL_SCANCODE_RETURN) &&  world->button == 0 ) //if key enter
	{
		game->current_stage = game->play_stage;
	}
	if (Input::isKeyPressed(SDL_SCANCODE_RETURN) && game->world->button == 1) //if key enter
	{
		game->current_stage = game->tutorial_stage;
	}
	if (Input::isKeyPressed(SDL_SCANCODE_RETURN) && game->world->button == 2) //if key enter
	{
		world->loadGameInfo();
		game->current_stage = game->play_stage;

	}
	if (world->button == 3 && Input::isKeyPressed(SDL_SCANCODE_RETURN) ) //if key enter
	{
		game->must_exit= true;
	}
	
};

void TutorialStage::render(Image& framebuffer) {

	Game* game = Game::instance;
	World* world = Game::instance->world;
	sPlayer* player = &Game::instance->world->myGame.players[0];
	sPlayer* player2 = &Game::instance->world->myGame.players[1];

	if (world->IsPlaytime == false) {
		world->Playtime = game->time;
		world->IsPlaytime = true;
	}
	Color bgcolor(13, 122, 138);
	framebuffer.fill(bgcolor);
	framebuffer.drawImage(game->mecanics->sprite, 20, 10, 0,0, 50, 20);
	framebuffer.drawText("Jump",30,30, game->world->font);
	framebuffer.drawImage(game->mecanics->sprite, 80, 10, 0, 20, 50, 50);
	framebuffer.drawText("Move", 90,35, game->world->font);
	world->camera.position = Vector2(20, 0);
	world->ShowGameMap(game->tutorial, framebuffer, world->tutorialTile);


	world->animation.current_animation = player->ismoving
		? (int(game->time * world->animation.velocity_animation) % world->animation.num_animations) : 0;
	framebuffer.drawImage(game->sprite->sprite,
		player->pos.x,
		player->pos.y,
		Area(13 * world->animation.current_animation, 40 * (int)player->dir, 13, 40));	//draws only a part of an image
	
	player->ismoving = 0;

	//Woman
	world->animation.current_animation = player2->ismoving
		? (int(game->time * world->animation.velocity_animation) % 5) : 0;
	framebuffer.drawImage(game->woman->sprite,
		player2->pos.x,
		player2->pos.y,
		Area(16 * world->animation.current_animation, 80 * (int)player2->dir, 16, 80));	//draws only a part of an image

	framebuffer.drawImage(game->pozo->sprite, 120,70, 40, 50);

};

void TutorialStage::update(double seconds_elapsed) {
	Game* game = Game::instance;
	World* world = Game::instance->world;
	sPlayer* player = &Game::instance->world->myGame.players[0];
	sPlayer* player2 = &Game::instance->world->myGame.players[1];
	Vector2 target = player->pos;

	player2->ismoving = 0;
	//woman movement 
	if (game->time > world->Playtime + 20.0){
		game->current_stage = game->play_stage;
		game->current_stage->restart();
	}
			
	if (game->time > world->Playtime+10.0) {
		if (player2->pos.x < 130) {
			player2->ismoving = 1;
			player2->pos.x += world->player_velocity * seconds_elapsed;
		}
		else {
			player2->pos.y += world->player_velocity*4*seconds_elapsed;
		}

	}

	//player movement
	if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) {  //if key right
		target.x += world->player_velocity * seconds_elapsed;
		player->ismoving = 1;
		player->dir = eDirection::RIGHT;

	}

	if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) { //if key left
		target.x -= world->player_velocity * seconds_elapsed;
		player->ismoving = 1;
		player->dir = eDirection::LEFT;

	}

	if (Input::wasKeyPressed(SDL_SCANCODE_SPACE) && !player->isjumping) { //jump action
		player->isjumping = true;
		player->jumpAngle = 0;
		player->startY = player->pos.y;
		player->posYMax = 30;
		game->synth.playSample("data/jump.wav", 20, false);

	}

	if (player->isjumping) {
		if (player->jumpAngle == 180)
		{
			player->isjumping = false;
		}
		else {
			player->pos.y = int(player->startY - player->posYMax * sin(3.14159f * player->jumpAngle / 180));
			if (player->jumpAngle > 90)
				player->isjumping = !game->map->isValid(player->pos);
			player->pos.y += player->speed_fall;

		}
		if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) //if key right
		{
			target.x += world->player_velocity * seconds_elapsed;
			player->ismoving = 1;
			player->dir = eDirection::RIGHT;

		}
		if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) //if key left
		{
			target.x -= world->player_velocity * seconds_elapsed;
			player->ismoving = 1;
			player->dir = eDirection::LEFT;

		}
		player->jumpAngle += 1;

	}

	if (game->map->isValid(Vector2(target.x - 1, player->pos.y - 1)))
		player->pos.x = target.x;

	if (game->map->isValid(target)) //gravity
		player->pos.y += player->speed_fall;

}

void PlayStage::render(Image& framebuffer) {

	Color bgcolor(8, 14, 13);
	Game* game = Game::instance;
	World* world = Game::instance->world;
	sPlayer* player = &Game::instance->world->myGame.players[0];

	framebuffer.fill(bgcolor);

	world->camera.position = Vector2(15, (game->time - world->Gametime) * 3 - 30);
	world->ShowGameMap(game->map, framebuffer, world->tileset );
	world->animation.current_animation = player->ismoving
		? (int(game->time * world->animation.velocity_animation) % world->animation.num_animations) : 0;
	framebuffer.drawImage(game->sprite->sprite,
		player->pos.x,
		player->pos.y ,
		Area(13 * world->animation.current_animation, 40 * (int)player->dir, 13, 40));	//draws only a part of an image
	player->ismoving = 0;

	if (player->health == 6) {
	framebuffer.drawImage(game->health->sprite, 0, 0, 0, 10, 12, 30); //full
	framebuffer.drawImage(game->health->sprite, 10, 0, 0, 10, 12, 30); //full
	framebuffer.drawImage(game->health->sprite, 20, 0, 0, 10, 12, 30); //full
	}

	if (player->health == 5) {
		framebuffer.drawImage(game->health->sprite, 0,  0,  0, 10, 12, 30); //full
		framebuffer.drawImage(game->health->sprite, 10, 0,  0, 10, 12, 30); //full
		framebuffer.drawImage(game->health->sprite, 20, 0, 20, 10, 12, 30); //half

	}
	if (player->health == 4) {
		framebuffer.drawImage(game->health->sprite, 0,  0, 0, 10, 12, 30); //full
		framebuffer.drawImage(game->health->sprite, 10, 0, 0, 10, 12, 30); //full
		framebuffer.drawImage(game->health->sprite, 20, 0, 10, 10, 12, 30); //void
	}
	if (player->health == 3) {

		framebuffer.drawImage(game->health->sprite, 0,  0, 0,  10, 12, 30); //full
		framebuffer.drawImage(game->health->sprite, 11, 0, 23, 10, 12, 30); //half
		framebuffer.drawImage(game->health->sprite, 20, 0, 12, 10, 12, 30); //void
	}
	if (player->health == 2) {
		framebuffer.drawImage(game->health->sprite, 0,  0, 1, 12,  12, 30); //full
		framebuffer.drawImage(game->health->sprite, 11, 0, 12, 12, 12, 30); //void
		framebuffer.drawImage(game->health->sprite, 20, 0, 12, 12, 12, 30); //void
	}
	if (player->health == 1) {

		framebuffer.drawImage(game->health->sprite, 0, 0, 23, 10, 12, 30); //half
		framebuffer.drawImage(game->health->sprite, 10, 0, 12, 10, 12, 30); //void
		framebuffer.drawImage(game->health->sprite, 20, 0, 12, 10, 12, 30); //void
	}
	if (player->health == 0) {
		game->current_stage = game->over_stage;
	}

	if (menu == true) {
		Game* game = Game::instance;
		framebuffer.drawImage(game->menu->sprite, 0, 0, 160, 120);
		framebuffer.drawText("Hello world", 160 / 2 - 40, 10, game->world->font);		//draws some text using a bitmap font in an image (assuming every char is 7x9)
		framebuffer.drawText("Save", 160 / 2 - 20, 40, game->world->font);				//draws some text using a bitmap font in an image (assuming every char is 7x9)
		framebuffer.drawText("Exit", 160 / 2 - 18, 50, game->world->font);				//draws some text using a bitmap font in an image (assuming every char is 7x9)
		if (game->world->button == 0) framebuffer.drawTriangle(160 / 2 - 27, 40, 160 / 2 - 22, 45, 160 / 2 - 27, 50, Color(0, 0, 0));
		if (game->world->button == 1) framebuffer.drawTriangle(160 / 2 - 27, 50, 160 / 2 - 22, 55, 160 / 2 - 27, 60, Color(0, 1, 0));

	}

	if (game->time > world->Playtime + 60.0) { //if game ends you have to choose one option
		menu = false;
		end = true;
		framebuffer.drawText("Would you like", 160 / 2 - 50, 120 / 2 - 30, world->fontWhite);
		framebuffer.drawText("to stay together?", 160 / 2 - 40, 120 / 2 - 15, world->fontWhite);
		framebuffer.drawText("Yes", 160 / 2 - 40, 70, world->fontWhite);
		framebuffer.drawText("No", 160 / 2 + 10, 70, world->fontWhite);
		if (world->button == 0) framebuffer.drawTriangle(160 / 2 - 50, 69, 160 / 2 - 45, 74, 160 / 2 - 50, 79, Color(255, 255, 255));
		if (world->button == 1) framebuffer.drawTriangle(160 / 2, 69, 160 / 2 + 5, 74, 160 / 2, 79, Color(255, 255, 255));

	}
	if (game->time > (world->Playtime + 55.0))
		framebuffer.drawImage(game->deadwoman->sprite, 60, 90);

}

void PlayStage::update(double seconds_elapsed) { //movement of the character
	Game* game = Game::instance;
	World* world = Game::instance->world;
	sPlayer* player = &Game::instance->world->myGame.players[0];

	Vector2 target = player->pos;

	if (Input::wasKeyPressed(SDL_SCANCODE_M))
		player->health = player->health - 1;

	// Menu for save
	if (Input::wasKeyPressed(SDL_SCANCODE_X))
		menu = true;
	if (Input::wasKeyPressed(SDL_SCANCODE_DOWN) && menu == true || Input::wasKeyPressed(SDL_SCANCODE_RIGHT) && end == true){
		world->button += 1;
		if (world->button == 2) world->button = 0;
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_UP) && menu == true || Input::wasKeyPressed(SDL_SCANCODE_LEFT) && end == true) {
		world->button -= 1;
		if (world->button == -1) world->button = 0;
	}

	if (Input::isKeyPressed(SDL_SCANCODE_RETURN) && world->button == 0 && menu == true) {
		world->saveGameInfo();
		world->button = 0;
		menu = false;
	}
	if (Input::isKeyPressed(SDL_SCANCODE_RETURN) && world->button == 1 && menu == true)
		game->must_exit = true;

	if (Input::wasKeyPressed(SDL_SCANCODE_RETURN) && world->button == 0 && end == true){
		game->current_stage = game->over_stage;
		world->Playtime = game->time;
		world->button = 3;
	}
	
	if (Input::isKeyPressed(SDL_SCANCODE_RETURN) && world->button ==1 && end == true)
		game->current_stage->restart();

	if (player->pos.y < -40) //if player out of frame game over
		game->current_stage = game->over_stage;

	//movement
	if (Input::wasKeyPressed(SDL_SCANCODE_LEFT)) {  //if key right
		player->falldistance = player->pos.y;
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_RIGHT)) {  //if key right
		player->falldistance = player->pos.y;
	}

	if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)){  //if key right
		target.x += world->player_velocity * seconds_elapsed;
		player->ismoving = 1;
		player->dir = eDirection::RIGHT;

	}

	if (Input::isKeyPressed(SDL_SCANCODE_LEFT)){ //if key left
		target.x -= world->player_velocity * seconds_elapsed;
		player->ismoving = 1;
		player->dir = eDirection::LEFT;

	}

	if (Input::wasKeyPressed(SDL_SCANCODE_SPACE) && !player->isjumping){ //jump action
		player->isjumping = true;
		player->jumpAngle = 0;
		player->startY = player->pos.y;
		player->posYMax = 30;
		game->synth.playSample("data/jump.wav", 20, false);
	
	}

	if (player->isjumping) {
		if (player->jumpAngle == 180)
		{
			player->isjumping = false;
		}
		else{
			player->pos.y = int(player->startY - player->posYMax * sin(3.14159f * player->jumpAngle / 180));
			if (player->jumpAngle > 90)
				player->isjumping = !game->map->isValid(player->pos);
				player->pos.y += player->speed_fall;

		}
		if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) //if key right
		{
			target.x += world->player_velocity * seconds_elapsed;
			player->ismoving = 1;
			player->dir = eDirection::RIGHT;

		}
		if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) //if key left
		{
			target.x -= world->player_velocity * seconds_elapsed;
			player->ismoving = 1;
			player->dir = eDirection::LEFT;

		}
		player->jumpAngle += 1;

	}

	if (game->map->isValid(Vector2(target.x-1, player->pos.y-1)))
		player->pos.x = target.x;

	if (game->map->isValid(target))
		player->pos.y +=  player->speed_fall;

	if (game->time < (world->Playtime + 55.0))
		player->pos.y -= seconds_elapsed * 3;
	else
		player->pos = Vector2(40, 85); // player next to woman

		
	if (player->pos.y - player->falldistance > 40) {// life system
			player->health -= 1;
			player->falldistance = player->pos.y;
	}

	if (Input::wasKeyPressed(SDL_SCANCODE_R)) //if key enter
	{
		game->current_stage->restart();
	}
};

void PlayStage::restart() { //Restart the game
	World* world = Game::instance->world;
	sPlayer* player = &Game::instance->world->myGame.players[0];
	Game* game = Game::instance;

	world->camera.position = Vector2(0, 0);
	player->pos = Vector2 (20, 0);
	player->dir = eDirection::RIGHT;
	player->health = 6;
	world->button = 0;
	player->speed_fall = 1;
	player->isjumping = false;
	player->jumpAngle = 0;
	player->posYMax = 0;
	world->Playtime = 0.0f;
	world->IsPlaytime = false;
	world->Gametime = game->time;
};

void OverStage::render(Image& framebuffer) {
	Game* game = Game::instance;
	World* world = Game::instance->world;
	
	framebuffer.fill(Color(1,0,0));
	framebuffer.drawText("Game Over", 160 / 2 - 32, 120 / 2 - 10, world->fontWhite);
	if (world->overAudio == true){
		game->synth.playSample("data/game-over.wav", 10, false);
		world->overAudio = false;
	}
	if (world->button!= 3){
		framebuffer.drawText("Retry", 160 / 2 - 40, 70, world->fontWhite);				
		framebuffer.drawText("Exit", 160 / 2 +10, 70,world->fontWhite);				
		if (world->button == 0 ) framebuffer.drawTriangle(160 / 2 - 50, 69, 160 / 2 - 45, 74, 160 / 2 - 50, 79, Color(255, 255, 255));
		if (world->button == 1) framebuffer.drawTriangle(160 / 2, 69, 160 / 2 + 5, 74, 160 / 2 , 79, Color(255, 255, 255));
	}
}

void OverStage::update(double seconds_elapsed) {
	World* world = Game::instance->world;
	Game* game = Game::instance;
	
	if (Input::isKeyPressed(SDL_SCANCODE_A)) //if key enter
	{
		game->current_stage = game->intro_stage;
	}

	if (Input::wasKeyPressed(SDL_SCANCODE_RIGHT)) //if key down 
	{
		world->button += 1;
		if (world->button == 2) world->button = 0;

	}
	if (Input::wasKeyPressed(SDL_SCANCODE_LEFT)) //if key left
	{
		world->button -= 1;
		if (world->button == -1) world->button = 0;
	}
	if (world->button == 0 && Input::isKeyPressed(SDL_SCANCODE_RETURN)) //if key enter
	{
		game->current_stage = game->play_stage;
		game->current_stage->restart();
	}
	if (world->button == 1 && Input::isKeyPressed(SDL_SCANCODE_RETURN)) //if key enter
	{
		game->must_exit = true;
	}

}


World::World() {
	Game* game = Game::instance;

	this->camera.position = Vector2(0, 0);
	this->myGame.players[0].pos = Vector2(20, 0);
	this->myGame.players[1].pos = Vector2(100,45);
	this->myGame.players[1].dir = eDirection::RIGHT;
	this->myGame.players[0].dir = eDirection::RIGHT;
	this->myGame.players[0].health = 6;
	this->button = 0;
	this->myGame.players[0].speed_fall = 1;
	this->myGame.players[0].isjumping = false;
	this->myGame.players[0].jumpAngle = 0;
	this->myGame.players[0].posYMax = 0;
	this->myGame.players[0].falldistance = 0;
	this->Playtime = 0.0f;
	this->IsPlaytime = false;
	this->Gametime = game->time;
	this->overAudio = true;

};

void World::saveGameInfo()
{

	World* world = Game::instance->world;
	sPlayer* player = &Game::instance->world->myGame.players[0];

	sGameInfo game_info;
	//fill here game_info with all game data
	//...
	game_info.player.pos = player->pos;
	game_info.player.health = player->health;
	game_info.player.dir = player->dir;
	game_info.PosCam = world->camera.position;
	//save to file
	FILE* fp = fopen("savegame.bin", "wb");
	fwrite(&game_info, sizeof(sGameInfo), 1, fp);
	fclose(fp);
}

 bool World::loadGameInfo()
{
	World* world = Game::instance->world;
	sPlayer* player = &Game::instance->world->myGame.players[0];

	sGameInfo game_info;

	//load file
	FILE* fp = fopen("savegame.bin", "rb");
	if (fp == NULL) //no savegame found
		return false;
	else 
		std::cout << " + Game loaded: " << "savegame.bin" << "\n ";

	fread(&game_info, sizeof(sGameInfo), 1, fp);
	fclose(fp);

	//transfer data from game_info to Game
	//…
	player->pos = game_info.player.pos;
	player->health = game_info.player.health;
	player->dir = game_info.player.dir;
	world->camera.position = game_info.PosCam;
	return true;


}

void World::ShowGameMap(GameMap* map, Image& framebuffer, Image& tileset) {
	Game* game = Game::instance;
	World* world = Game::instance->world;
	sPlayer* player = &Game::instance->world->myGame.players[0];

	//size in pixels of a cell, we assume every row has 16 cells so the cell size must be image.width / 16
	int cs = tileset.width / 16; //size of cellin tileset
	int screeny = 0;
	if (world->IsPlaytime == false) {
		world->Playtime = game->time;
		world->IsPlaytime = true;
	}

	for (int x = 0; x < map->width; ++x)
		for (int y = 0; y < map->height; ++y)
		{
			//get cell info
			sCell& cell = map->getCell(x, y);
			if (cell.type == 0) //skip empty
				continue;
			int type = (int)cell.type;
			//compute tile pos in tileset image
			int tilex = (type % 16) * cs; 	//x pos in tileset
			int tiley = floor(type / 16) * cs;	//y pos in tileset
			Area area(tilex, tiley, cs, cs); //tile area
			int screenx = (x * cs) + world->camera.position.x; //place offset here if you want

			if (game->time > (world->Playtime + 55.0) && game->current_stage == game->play_stage)
				screeny = (y * cs) - (world->Playtime + 55.0)*3 + 30;

			else
				screeny = (y * cs) - world->camera.position.y;
			

			//avoid rendering out of screen stuff
			if (screenx < -cs || screenx > framebuffer.width ||
				screeny < -cs || screeny > framebuffer.height)
				continue;


			//draw region of tileset inside framebuffer
			framebuffer.drawImage(tileset, screenx, screeny, area);

		}

}
  
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
		std::cout << " + GameMap loaded: "  << filename << "\n";

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

bool GameMap::isValid(Vector2 target) {

	Game* game = Game::instance;
	World* world = Game::instance->world;
	sPlayer* player = &Game::instance->world->myGame.players[0];

	int cs = world->tileset.width / 16; //size of cellin tileset
	int celdax[2];
	int celday;

	celday = (target.y + (20) + world->camera.position.y) / cs;
	for (int i = 0; i < 2; i++) {
		celdax[i] = (target.x + (13 * i)- world->camera.position.x) / cs;
	}

	for (int i = 0; i < 2; i++)
	{
		if (game->current_stage == game->play_stage) { //PlayStage map
			sCell aux = game->map->getCell(celdax[i], celday);
			if (0 < aux.type && aux.type < 10) //floor
				return false;
			else if (10 < aux.type && aux.type < 22)//wall
				return false;
		}
		else { //tutorial map
			sCell aux = game->tutorial->getCell(celdax[i], celday);
			if (aux.type == 1 || aux.type == 2) //floor
				return false;
		
		}
			

	}
	return true;

}