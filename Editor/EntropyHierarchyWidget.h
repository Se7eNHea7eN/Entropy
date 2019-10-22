#pragma once
#include <QTreeWidget>

namespace Entropy {
	class EntropyHierarchyWidget : public QTreeWidget {
		Q_OBJECT
	public:
		explicit EntropyHierarchyWidget(QWidget* parent = nullptr);
	};
}
