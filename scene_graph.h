#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <iostream>
#include <vector>
#include <math.h>
#include "material.h"

#define DEFAULT_SCALE glm::vec3(1.f)
#define DEFAULT_ROTATION glm::vec4(0.f, 0.f, 0.f, 1.f)
#define DEFAULT_TRANSLATION glm::vec3(0.f)
#define DEFAULT_MATRIX glm::mat4(1.f)

class SceneNode {
public:
	SceneNode(std::string name);
	SceneNode(std::string name, glm::vec3 translation, glm::vec3 scale, glm::vec4 rotation);
	SceneNode(std::string name, float translation[3], float scale[3], float rotation[4], std::string materialName);
	~SceneNode();
	void addChild(SceneNode* child);
	void release();
	void destroy();
	void destroyChild(int index);
	

	void setName(std::string name);
	void translate(glm::vec3 translation);
	void setMaterial(std::string materialName);
	void setParent(SceneNode* parent);
	void setThis(SceneNode* copyNode);

	glm::mat4 getMatrix();
	std::string getName();
	float* getTranslationf();
	glm::vec3 getTranslation();
	float* getScalef();
	glm::vec3 getScale();
	float* getRotationf();
	glm::vec4 getRotation();
	size_t getChildrenCount();
	std::vector<SceneNode*> getChildren();

	//temp
	Material m_material;
private:
	std::string m_name;
	SceneNode* m_parent;
	glm::mat4 m_matrix;
	glm::vec3 m_translation;
	glm::vec3 m_scale;
	glm::vec4 m_rotation;
	
	
	std::vector<SceneNode*> m_children;
};

