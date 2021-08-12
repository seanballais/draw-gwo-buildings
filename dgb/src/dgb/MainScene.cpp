#include <cstdio>

#include <entt/entt.hpp>
#include <imgui.h>
#include <SDL2/SDL.h>

#include <corex/core/AssetManager.hpp>
#include <corex/core/Camera.hpp>
#include <corex/core/Scene.hpp>
#include <corex/core/components/Position.hpp>
#include <corex/core/components/Renderable.hpp>
#include <corex/core/components/RenderableType.hpp>
#include <corex/core/components/RenderRectangle.hpp>
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
    , corex::core::Scene(registry, eventDispatcher, assetManager, camera) {}

  void MainScene::init()
  {
    this->eventDispatcher.sink<corex::core::WindowEvent>()
         .connect<&MainScene::handleWindowEvents>(this);
  }

  void MainScene::update(float timeDelta)
  {
    this->registry.clear();

    // Draw boundary.
    constexpr float boundaryCenterX = 1366.f / 2.f;
    constexpr float boundaryCenterY = 768.f / 2.f;
    auto boundaryEntity = this->registry.create();
    this->registry.emplace<corex::core::Position>(
      boundaryEntity,
      boundaryCenterX,
      boundaryCenterY,
      0.f,
      static_cast<int8_t>(1));
    this->registry.emplace<corex::core::Renderable>(
      boundaryEntity, corex::core::RenderableType::PRIMITIVE_RECTANGLE);
    this->registry.emplace<corex::core::RenderRectangle>(
      boundaryEntity,
      boundaryCenterX,
      boundaryCenterY,
      this->boundaryDims.x * this->drawScale,
      this->boundaryDims.y * this->drawScale,
      0.f,
      SDL_Color{ 23, 23, 23, 255 },
      false);

    // Draw buildings.
    for (const BuildingData& data : this->buildingData) {
      const float posX = (data.x * this->drawScale) + boundaryCenterX;

      // We're reversing the y-coordinate of the building since window space
      // has the +y coordinate going downwards, and we're translating the
      // origin to the middle of the window.
      const float posY = ((data.y * this->drawScale * -1.f) + boundaryCenterY);

      auto entity = this->registry.create();
      this->registry.emplace<corex::core::Position>(
        entity,
        posX,
        posY,
        0.f,
        static_cast<int8_t>(0));
      this->registry.emplace<corex::core::Renderable>(
        entity, corex::core::RenderableType::PRIMITIVE_RECTANGLE);
      this->registry.emplace<corex::core::RenderRectangle>(
        entity,
        posX,
        posY,
        data.width * this->drawScale,
        data.height * this->drawScale,
        data.angle,
        SDL_Color{ 22, 57, 112, 255 },
        false);
    }

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

      char widthInputText[12];
      char heightInputText[13];
      char angleInputText[12];
      char xInputText[8];
      char yInputText[8];

      sprintf(widthInputText, "Width##%d", i);
      sprintf(heightInputText, "Height##%d", i);
      sprintf(angleInputText, "Angle##%d", i);
      sprintf(xInputText, "X##%d", i);
      sprintf(yInputText, "Y##%d", i);

      ImGui::InputFloat(widthInputText, &b.width);
      ImGui::InputFloat(heightInputText, &b.height);
      ImGui::InputFloat(angleInputText, &b.angle);
      ImGui::InputFloat(xInputText, &b.x);
      ImGui::InputFloat(yInputText, &b.y);

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