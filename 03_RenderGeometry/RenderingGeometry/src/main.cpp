#include "RenderingGeometryApp.h"

int main() {
	
	auto app = new RenderingGeometryApp();
	app->run("AIE", 1280, 720, false);
	delete app;

	return 0;
}