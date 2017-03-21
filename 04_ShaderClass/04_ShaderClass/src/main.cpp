#include "ShaderClassApp.h"

int main() {
	
	auto app = new ShaderClassApp();
	app->run("AIE", 1280, 720, false);
	delete app;

	return 0;
}