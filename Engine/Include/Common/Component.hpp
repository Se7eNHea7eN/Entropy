#pragma once
#include <memory>
#include "Interface/Interface.hpp"
#include "Interface/IRuntimeModule.hpp"

namespace Entropy {
	class SceneNode;
	class Component : public std::enable_shared_from_this<Component> , implements IRuntimeModule{
public:
		virtual ~Component() {};

		virtual int Initialize() {
			return 0;
		};
		virtual void Finalize() {
			// node.reset();
		};

		virtual void Tick(){};


		void SetNode(std::shared_ptr<SceneNode> n) {
			node = n;
		}

		std::shared_ptr<SceneNode> GetNode() {
			return node;
		}

		std::shared_ptr<Component> SharedPtr() {
			return shared_from_this();
		}
	protected:

		std::shared_ptr<SceneNode> node;
	};

}
