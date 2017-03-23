#include "ParticlesApp.h"

int main() {
	
	auto app = new ParticlesApp();
	app->run("AIE", 1280, 720, false);
	delete app;

	return 0;
}