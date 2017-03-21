#include "DiffuseLightingApp.h"

int main() {
	
	auto app = new DiffuseLightingApp();
	app->run("AIE", 1280, 720, false);
	delete app;

	return 0;
}