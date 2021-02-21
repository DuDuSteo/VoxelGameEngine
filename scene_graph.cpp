#include "scene_graph.h"


SceneNode::SceneNode(std::string name) {
	m_name = name;
	m_matrix = DEFAULT_MATRIX;
	m_translation = DEFAULT_TRANSLATION;
	m_scale = DEFAULT_SCALE;
	m_rotation = DEFAULT_ROTATION;
	m_parent = nullptr;
}

SceneNode::SceneNode(std::string name, glm::vec3 translation = DEFAULT_TRANSLATION,
	glm::vec3 scale = DEFAULT_SCALE, glm::vec4 rotation = DEFAULT_ROTATION)
{
	m_matrix = glm::mat4(1.f);
	glm::quat quatRotation(rotation.w, rotation.x, rotation.y, rotation.z);

	m_matrix = glm::translate(m_matrix, translation);
	m_matrix = m_matrix * glm::mat4_cast(quatRotation);
	m_matrix = glm::scale(m_matrix, scale);

	m_name = name;
	m_parent = nullptr;
}

SceneNode::SceneNode(std::string name, float translation[3], float scale[3], float rotation[4], std::string materialName) {
	m_translation = glm::make_vec3(translation);
	m_scale = glm::make_vec3(scale);
	m_rotation = glm::make_vec4(rotation);

	loadMaterial(m_material, materialName);
	m_matrix = glm::mat4(1.f);
	glm::quat quatRotation(m_rotation[3], m_rotation[0], m_rotation[1], m_rotation[2]);

	m_matrix = glm::translate(m_matrix, m_translation);
	m_matrix = m_matrix * glm::mat4_cast(quatRotation);
	m_matrix = glm::scale(m_matrix, m_scale);

	m_name = name;
	m_parent = nullptr;
}

SceneNode::~SceneNode() {
	for (size_t i = 0; i < m_children.size(); i++)
	{

	}
}


void SceneNode::addChild(SceneNode* child)
{	
	if (child && (child != this)) {
		child->setParent(this);
		m_children.push_back(child);
	}
}


void SceneNode::setName(std::string name) {
	m_name = name;
}

void SceneNode::translate(glm::vec3 translation) {
	m_matrix = glm::translate(m_matrix, translation);
}

void SceneNode::setMaterial(std::string materialName) {
	loadMaterial(m_material, materialName);
}

void SceneNode::setParent(SceneNode* parent) {
	m_parent = parent;
}

void SceneNode::setThis(SceneNode* copyNode) {
	m_material = copyNode->m_material;

	m_name = copyNode->getName();
	m_matrix = copyNode->getMatrix();
	m_translation = copyNode->getTranslation();
	m_scale = copyNode->getScale();
	m_rotation = copyNode->getRotation();
}


glm::mat4 SceneNode::getMatrix()
{
	return m_matrix;
}

std::string SceneNode::getName()
{
	return m_name;
}

float* SceneNode::getTranslationf() {
	return (float*)&m_translation;
}

glm::vec3 SceneNode::getTranslation(){
	return m_translation;
}

float* SceneNode::getScalef() {
	return (float*)&m_scale;
}

glm::vec3 SceneNode::getScale() {
	return m_scale;
}

float* SceneNode::getRotationf() {
	return (float*)&m_rotation;
}

glm::vec4 SceneNode::getRotation() {
	return m_rotation;
}

size_t SceneNode::getChildrenCount() {
	return m_children.size();
}

std::vector<SceneNode*> SceneNode::getChildren()
{
	return m_children;
}