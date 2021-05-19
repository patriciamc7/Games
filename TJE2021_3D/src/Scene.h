#ifndef SCENE_H
#define SCENE_H

#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "camera.h"
#include "input.h"
class Prefab;
using namespace std;

class Entity
{
public:

	//some attributes
	int id; 
	Matrix44 model;

	//methods overwritten by derived classes
	virtual void render() {}; //empty body 
	virtual void update(float elapsed_time) {}; //empty body 
	////some useful methods...
	//Vector3 getPosition(); 
};
class EntityMesh :public Entity
{
public:
	EntityMesh(); //constructor

	//Attrubutes of this class
	Mesh* mesh; 
	Texture* texture; 
	Shader* shader; 
	Vector4 color; 
	bool alpha; 

	//methods overwritten
	virtual void render();
	virtual void update(float dt);
};
class EntityPlayer :public Entity
{
public:
	EntityPlayer(); //constructor

	//Attrubutes of this class
	float center_value_y;
	Mesh* mesh;
	Texture* texture;
	Shader* shader;
	Vector4 color;
	Vector3 pos;
	float yaw;
	float pitch; 
	Vector3 player_speed;
	float player_speed_rot;

	//methods overwritten
	virtual void render();
	virtual void update(float dt);
	void collisionMesh(float dt);

};
class EntitySound :public Entity
{
public:
	EntitySound(); //constructor

	//methods overwritten
	virtual void render();
	virtual void update(float dt);
};

class EntityLight :public Entity
{
public: 
	EntityLight(); //constructor

	//attributes
	Vector3 color;
	float intensity;
	enum eLightType {
		POINT,
		SPOT,
		DIRECTIONAL
	};

	eLightType light_type;
	Vector3 light_position;
	Vector3 light_vector;

	//Directional light
	float spotCosineCutoff;
	float max_distance;
	float spotExponent;
	int flag;

	//Shadowmap
	Camera* cameraLight;
	float bias;

	//methods overwritten
	virtual void render();
	virtual void update(float dt);
};

class Scene
{
public:
	Scene(); //constructor
	void CreatePlayer();
	vector<Entity*> entities;
	vector<Entity*> entities_mirror;
	vector<EntityPlayer*> characters;
	Vector3 ambient;
};


#endif 