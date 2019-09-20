#include "UnloadManagersTask.hpp"
#include "Director.hpp"
#include "gui/GuiManager.hpp"

void UnloadManagersTask::run() {

  auto director = Director::getInstance();

  director->unloadObject<GuiManager>();
}
