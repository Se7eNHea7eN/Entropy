#include "Graphic/StaticMeshComponent.hpp"
#include <vector>


namespace Entropy {

	std::vector<std::shared_ptr<StaticMeshComponent>> StaticMeshComponent::AllStaticMeshComponents;

	StaticMeshComponent::StaticMeshComponent() {
		Initialize();
	}

	StaticMeshComponent::~StaticMeshComponent() {
		Finalize();
	}

	int StaticMeshComponent::Initialize() {
		AllStaticMeshComponents.push_back(std::shared_ptr<StaticMeshComponent>(this));
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
