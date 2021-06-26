#include "Target.h"

namespace Engine {
	bool isDebuggerPresent() {
		return ::IsDebuggerPresent();
	}
}
