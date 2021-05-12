#include "Scene.h"
#include "game.h"

Entity::Entity()
{
	
}

Entity::~Entity()
{
}

//void Entity::render()
//{
//	//if this was an EntityMesh...
//	//Matrix44 model = getGlobalMatrix();
//	//render code...
//
//	for (int i = 0; i < children.size(); i++)
//		children[i]->render();  //repeat for every child

//}

//void Entity::addChild(Entity* ent)
//{
//	children.push_back(ent);
//}
//
//void Entity::removeChild(Entity* ent)
//{
//	 
//	for (int i = 0 ; i < children.size(); i++)
//	{
//		if (ent->id == children[i]->id)
//			children.erase(children.begin() + i);
//	}
//}
//
//void Entity::destroyLast()
//{
//	//for every children call destroy()
//	while (!children.empty())
//	{
//		children.pop_back();
//	}
//}
//
////get the global transformation of this object (not the relative to the parent)
////this function uses recursivity to crawl the tree upwards
//Matrix44 Entity::getGlobalMatrix()
//{
//	if (parent) //if I have a parent, ask his global and concatenate
//		return model * parent->getGlobalMatrix();
//	return model; //otherwise just return my model as global
//}


EntityMesh::EntityMesh()
{

	this->texture = new Texture();
	//texture->load("data/Door_BaseColor.tga");
}

EntityMesh::~EntityMesh()
{
}

void EntityMesh::render()
{

	//get the last camera thet was activated
	Camera* camera = Camera::current; /*
	Matrix44 model = getGlobalMatrix();*/
	Matrix44 model ;
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	
	//enable shader and pass uniforms
	shader->enable(); 
	shader->setUniform("u_model", model);
	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader->setUniform("u_color", Vector4(1,1,1,1));

	shader->setUniform("u_texture", Texture::Get("data/Door_BaseColor.tga"),0);

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

EntitySound::~EntitySound()
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
}

EntityLight::~EntityLight()
{
}

void EntityLight::render()
{
}

void EntityLight::update(float dt)
{
}

Scene::Scene()
{
}

Scene::~Scene()
{
}
