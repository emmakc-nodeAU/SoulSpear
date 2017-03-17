#include "_01_ProjectSetupApp.h"

int main() {
	
	auto app = new _01_ProjectSetupApp();
	app->run("AIE", 1280, 720, false);
	delete app;

	return 0;
}