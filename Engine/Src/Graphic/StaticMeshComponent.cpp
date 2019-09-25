#include "Graphic/StaticMeshComponent.hpp"
#include <vector>


namespace Entropy {

	std::vector<std::shared_ptr<StaticMeshComponent>> StaticMeshComponent::AllStaticMeshComponents;

	StaticMeshComponent::StaticMeshComponent() : Component() {
	}

	StaticMeshComponent::~StaticMeshComponent() {
	}

	int StaticMeshComponent::Initialize() {
		AllStaticMeshComponents.push_back(std::dynamic_pointer_cast<StaticMeshComponent>(SharedPtr()));
		return 0;
	}

	void StaticMeshComponent::Finalize() {
		for (auto it = AllStaticMeshComponents.begin(); it != AllStaticMeshComponents.end(); ++it) {
			if (it->get() == this) {
				AllStaticMeshComponents.erase(it);
				return;
			}
		}
	}

}
