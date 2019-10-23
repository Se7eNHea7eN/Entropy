#include "Common/SceneNode.hpp"
#include "Common/Transform.hpp"

Entropy::SceneNode::SceneNode() {
	m_strName = "new node";
}

Entropy::SceneNode::SceneNode(const char* name) {
	m_strName = name;
	if (m_strName.empty()) {
		m_strName = "new node";
	}
}

Entropy::SceneNode::SceneNode(const std::string& name) {
	m_strName = name;
	if (m_strName.empty()) {
		m_strName = "new node";
	}
}

Entropy::SceneNode::SceneNode(const std::string&& name) {
	m_strName = std::move(name);
	if (m_strName.empty()) {
		m_strName = "new node";
	}
}

Entropy::SceneNode::~SceneNode() {
	for(auto c: m_Components) {
		c->Finalize();
	}
}

void Entropy::SceneNode::AddChild(std::shared_ptr<SceneNode>&& sub_node) {
	m_Children.push_back(sub_node);
	sub_node->setParent(SharedPtr());
}

void Entropy::SceneNode::AddComponent(std::shared_ptr<Component>&& c) {
	m_Components.push_back(c);
	c->SetNode(SharedPtr());
}

int Entropy::SceneNode::Initialize() {
	m_Transform = std::make_unique<Transform>();
	AddComponent(std::shared_ptr<Transform>(m_Transform.get()));
	return 0;
}

void Entropy::SceneNode::Finalize() {
}

void Entropy::SceneNode::Tick(float deltaTime) {
	for (auto c : m_Components) {
		c->Tick(deltaTime);
	}
}
