#include "Application.hpp"
#include "Director.hpp"
//#include "task/LoadManagersTask.hpp"
//#include "task/UnloadManagersTask.hpp"

Application::Application(int &argc, char **argv) : QApplication(argc, argv) {

  auto director = Director::getInstance();

  // director->run(new LoadManagersTask);
}

Application::~Application() {

  auto director = Director::getInstance();

  // director->run(new UnloadManagersTask);
}
