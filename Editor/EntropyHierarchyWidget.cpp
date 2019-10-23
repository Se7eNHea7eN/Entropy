#include "EntropyHierarchyWidget.h"
#include "Common/Scene.hpp"
#include "Common/SceneNode.hpp"

using namespace Entropy;

EntropyHierarchyWidget::EntropyHierarchyWidget(QWidget* parent) {
}

void Entropy::EntropyHierarchyWidget::SetScene(Scene* scene)
{
	auto root = new QTreeWidgetItem(this);
	root->setText(0, QString::fromStdString(scene->GetRootNode()->GetName()));
	hierarchyMap[scene->GetRootNode()] = root;
	for(auto node : scene->GetRootNode()->GetChildren()) {
		AddNode(root, node.get());
	}
}

void EntropyHierarchyWidget::AddNode(QTreeWidgetItem* parent, SceneNode* sceneNode) {
	auto item = new QTreeWidgetItem(parent);
	item->setText(0, QString::fromStdString(sceneNode->GetName()));
	hierarchyMap[sceneNode] = item;
	for (auto node : sceneNode->GetChildren()) {
		AddNode(item, node.get());
	}
}
