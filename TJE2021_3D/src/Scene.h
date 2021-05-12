#ifndef SCENE_H
#define SCENE_H

#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "camera.h"
class Prefab;
using namespace std;

class Entity
{
public:
	Entity(); //Constructor
	virtual ~Entity(); //Destructor
	//some attributes
	int id; 
	string name; 
	Matrix44 model;
	/*Entity* parent; 
	vector<Entity*> children; */
	//methods overwritten by derived classes
	virtual void render() {}; //empty body 
	virtual void update(float elapsed_time) {}; //empty body 
	void addChild(Entity* ent);
	void removeChild(Entity* ent);
	void destroyLast();
	Matrix44 getGlobalMatrix(); //returns transform in world coordinates
	////some useful methods...
	//Vector3 getPosition(); 
};



class EntityMesh :public Entity
{
public:
	EntityMesh(); //constructor
	~EntityMesh();
	//Attrubutes of this class
	Mesh* mesh; 
	Texture* texture; 
	Shader* shader; 
	Vector4 color; 

	//methods overwritten
	virtual void render();
	virtual void update(float dt);
};

class EntitySound :public Entity
{
public:
	EntitySound(); //constructor
	~EntitySound();
	//methods overwritten
	virtual void render();
	virtual void update(float dt);
};


class EntityLight :public Entity
{
public:
	//function 
	EntityLight();
	~EntityLight();

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
	Scene();
	~Scene();

	vector<Entity*> entities;
	Vector3 ambient;
};


#endif 