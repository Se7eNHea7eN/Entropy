#pragma once
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <vector>
namespace Entropy {
	class SceneNode {
	public:
		SceneNode() {};
		SceneNode(const char* name) { m_strName = name; };
		SceneNode(const std::string& name) { m_strName = name; };
		SceneNode(const std::string&& name) { m_strName = std::move(name); };
		virtual ~SceneNode() {};
	protected:
		std::string m_strName;
		std::list<std::unique_ptr<SceneNode>> m_Children;
	};
}