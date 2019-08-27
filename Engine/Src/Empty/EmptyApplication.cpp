#include "Framework/Common/BaseApplication.hpp"

namespace Entropy {
	BaseApplication g_App;
	IApplication* g_pApp = &g_App;
}
