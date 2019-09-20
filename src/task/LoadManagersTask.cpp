#include "LoadManagersTask.hpp"
#include "Director.hpp"
#include "gui/GuiManager.hpp"

void LoadManagersTask::run() {

  auto director = Director::getInstance();

  director->loadObject<GuiManager>();
}
