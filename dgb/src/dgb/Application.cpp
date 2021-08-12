#include <iostream>

#include <EASTL/string.h>
#include <EASTL/unique_ptr.h>

#include <corex/core/Application.hpp>
#include <corex/core/Scene.hpp>

#include <dgb/Application.hpp>
#include <dgb/MainScene.hpp>

namespace dgb
{
  Application::Application(const eastl::string& windowTitle)
    : corex::core::Application(windowTitle) {}

  void Application::init()
  {
    std::cout << "Initializing DGB... Bleep, bloop, bleep." << std::endl;
    auto& mainScene = this->sceneManager->addScene<dgb::MainScene>();
    this->sceneManager->setRootScene(mainScene);
  }

  void Application::dispose()
  {
    std::cout << "Disposing DGB... Bleep, bloop, zzzzz." << std::endl;
  }
}

namespace corex
{
  eastl::unique_ptr<corex::core::Application> createApplication()
  {
    return eastl::make_unique<dgb::Application>(
      "Draw GWO Buildings");
  }
}
