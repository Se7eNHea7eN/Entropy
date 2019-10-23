#pragma once
#include <QTreeWidget>
namespace Entropy {
	class Scene;
	class SceneNode;
	class EntropyHierarchyWidget : public QTreeWidget {
		Q_OBJECT
	public:
		explicit EntropyHierarchyWidget(QWidget* parent = nullptr);
	public:
		void SetScene(Scene* scene);
		void AddNode(QTreeWidgetItem* parent,SceneNode* sceneNode);
	private:
		std::map<SceneNode*, QTreeWidgetItem*> hierarchyMap;
	};
}
