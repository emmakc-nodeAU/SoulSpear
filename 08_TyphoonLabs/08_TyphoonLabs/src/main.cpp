#include "TyphoonLabs.h"

int main() {
	
	auto app = new TyphoonLabs();
	app->run("AIE", 1280, 720, false);
	delete app;

	return 0;
}