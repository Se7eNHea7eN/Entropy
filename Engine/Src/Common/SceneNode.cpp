#include "Common/SceneNode.hpp"
#include "Common/Transform.hpp"

Entropy::SceneNode::SceneNode() {
	m_Transform = std::make_shared<Transform>();
}

Entropy::SceneNode::SceneNode(const char* name) { m_strName = name; }

Entropy::SceneNode::SceneNode(const std::string& name) { m_strName = name; }

Entropy::SceneNode::SceneNode(const std::string&& name) { m_strName = std::move(name); }

Entropy::SceneNode::~SceneNode() {
	m_Children.clear();
	m_Components.clear();
}

void Entropy::SceneNode::AppendChild(std::shared_ptr<SceneNode>&& sub_node) {
	m_Children.push_back(sub_node);
	sub_node->setParent(std::shared_ptr<SceneNode>(this));
}

void Entropy::SceneNode::AppendComponent(std::shared_ptr<Component>&& c) {
	m_Components.push_back(c);
	c->SetNode(std::shared_ptr<SceneNode>(this));
}

void Entropy::SceneNode::Tick(float deltaTime) {
	for (auto c : m_Components) {
		c->Tick();
	}
}
