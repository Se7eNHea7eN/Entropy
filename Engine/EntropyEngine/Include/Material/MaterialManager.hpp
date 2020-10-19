#pragma once
class MaterialManager {
public:
	MaterialManager* Instance() {
		if (!s_instance)
		{
			s_instance = new MaterialManager;
		}
		return s_instance;
	}

private:
	MaterialManager();
	static MaterialManager* s_instance;
};