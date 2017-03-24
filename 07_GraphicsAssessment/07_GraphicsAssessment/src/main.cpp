#include "GraphicsAssessment.h"

int main() {
	
	auto app = new GraphicsAssessment();
	app->run("AIE", 1280, 720, false);
	delete app;

	return 0;
}