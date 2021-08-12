#include <cstdio>

#include <EASTL/vector.h>
#include <entt/entt.hpp>
#include <imgui.h>

#include <corex/core/AssetManager.hpp>
#include <corex/core/Camera.hpp>
#include <corex/core/Scene.hpp>
#include <corex/core/events/sys_events.hpp>

#include <dgb/MainScene.hpp>

namespace dgb
{
  MainScene::MainScene(entt::registry& registry,
                       entt::dispatcher& eventDispatcher,
                       corex::core::AssetManager& assetManager,
                       corex::core::Camera& camera)
    : boundaryDims(0.f, 0.f)
    , drawScale(1.f)
    , buildingData()
    , buildingEntities()
    , boundaryEntity(entt::null)
    , corex::core::Scene(registry, eventDispatcher, assetManager, camera) {}

  void MainScene::init()
  {
    this->eventDispatcher.sink<corex::core::WindowEvent>()
         .connect<&MainScene::handleWindowEvents>(this);
  }

  void MainScene::update(float timeDelta)
  {
    this->buildConfigWindow();
  }

  void MainScene::dispose()
  {
  }

  void MainScene::buildConfigWindow()
  {
    ImGui::Begin("Configuration");

    static float boundaryDims[2] = { 0.f, 0.f };
    ImGui::InputFloat2("Bounding Region (W:H)", boundaryDims);

    this->boundaryDims.x = boundaryDims[0]; // Width
    this->boundaryDims.y = boundaryDims[1]; // Height

    ImGui::InputFloat("Draw Scale", &this->drawScale);

    ImGui::Separator();

    if (ImGui::Button("Add Building")) {
      this->buildingData.push_back();
    }

    if (!this->buildingData.empty()) {
      ImGui::Separator();
    }

    int32_t i = 0;
    int32_t removedBuildingIndex = -1;
    for (BuildingData& b : this->buildingData) {
      ImGui::Text("Building #%d", i);
      ImGui::InputFloat("Width", &b.width);
      ImGui::InputFloat("Height", &b.height);
      ImGui::InputFloat("Angle", &b.angle);
      ImGui::InputFloat("X", &b.x);
      ImGui::InputFloat("Y", &b.y);

      char removeBtnText[13];
      sprintf(removeBtnText, "Remove##%d", i);

      if (ImGui::Button(removeBtnText)) {
        removedBuildingIndex = i;
      }

      ImGui::Separator();

      i++;
    }

    // Remove building from vector.
    if (removedBuildingIndex != -1) {
      this->buildingData
           .erase(this->buildingData.begin() + removedBuildingIndex);
    }

    ImGui::End();
  }

  void MainScene::handleWindowEvents(const corex::core::WindowEvent& e)
  {
    if (e.event.window.event == SDL_WINDOWEVENT_CLOSE) {
      this->setSceneStatus(corex::core::SceneStatus::DONE);
    }
  }
}