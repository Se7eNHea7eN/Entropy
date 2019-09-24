#pragma once
#include <memory>
#include "Interface/Interface.hpp"
#include "Interface/IRuntimeModule.hpp"

namespace Entropy {
	class SceneNode;
	class Component : implements IRuntimeModule{
public:
		virtual ~Component() {};

		virtual int Initialize() {
			return 0;
		};
		virtual void Finalize() {
			node.reset();
		};

		virtual void Tick(){};


		void SetNode(std::shared_ptr<SceneNode> n) {
			node = n;
		}

		std::shared_ptr<SceneNode> GetNode() {
			return node;
		}
	protected:
		std::shared_ptr<SceneNode> node;
	};

}
