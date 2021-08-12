#ifndef DGB_MAIN_SCENE_HPP
#define DGB_MAIN_SCENE_HPP

#include <EASTL/vector.h>
#include <entt/entt.hpp>

#include <corex/core/AssetManager.hpp>
#include <corex/core/Camera.hpp>
#include <corex/core/Scene.hpp>
#include <corex/core/ds/Vec2.hpp>

#include <dgb/BuildingData.hpp>

namespace dgb
{
  class MainScene : public corex::core::Scene
  {
  public:
    MainScene(entt::registry& registry,
              entt::dispatcher& eventDispatcher,
              corex::core::AssetManager& assetManager,
              corex::core::Camera& camera);

    void init() override;
    void update(float timeDelta) override;
    void dispose() override;

  private:
    void buildConfigWindow();
    void handleWindowEvents(const corex::core::WindowEvent& e);

    corex::core::Vec2 boundaryDims;
    float drawScale;
    eastl::vector<BuildingData> buildingData;
    eastl::vector<entt::entity> buildingEntities;
    entt::entity boundaryEntity;
  };
}

#endif
