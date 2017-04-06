#include "ScriptingApplication.h"

int main() {
	
	auto app = new ScriptingApplication();
	app->run("AIE", 512, 256, false);
	delete app;

	return 0;
}
