#ifndef SCENE_H
#define SCENE_H

#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "camera.h"
#include "input.h"
#include "animation.h"
class Prefab;
using namespace std;

class Entity
{
public:

	//some attributes
	int id;
	bool isColision = true;
	bool isInteractive = false;
	Matrix44 model;

	int alpha;
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
	Texture* texture2;
	Shader* shader; 
	Vector4 color; 
	float tiling;
	//methods overwritten
	virtual void render();
	virtual void update(float dt);
};
class EntityPlayer :public Entity
{
public:
	EntityPlayer(); //constructor

	
//Attrubutes of this class

	float vel_factor = 0;
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
	Vector3 targetPos;
	Vector3 playerSpeed;
	//methods overwritten
	virtual void render();
	virtual void update(float dt);
	void collisionMesh(float dt);
	void Interaction();
	

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

	Shader* shader;
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
struct sMirrorParticle {
	float rotateParticle;
	EntityMesh* v_particles;
	float sizeParticle;
};
class Scene
{
public:
	Scene(); //constructor
	void CreatePlayer();
	vector<Entity*> entities;
	vector<Entity*> entities_mirror;
	vector<EntityPlayer*> characters;
	//particles
	vector<sMirrorParticle> mirrorParticle;
	float timeLive;
	vector<EntityLight*> lights;
	vector<EntityLight*> lightsMirror;
	Vector3 ambient;
};


#endif 