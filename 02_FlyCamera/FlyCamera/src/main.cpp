#include "FlyCameraApp.h"

int main() {
	
	auto app = new FlyCameraApp();
	app->run("AIE", 1280, 720, false);
	delete app;

	return 0;
}