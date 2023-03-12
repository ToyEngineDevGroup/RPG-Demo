#include "game_layer.h"

void GameLayer::onAttach() {
    Toy2D::Application::get().getResourceMngr()->add<Toy2D::ResourceType::TileSheet>(
        Toy2D::Application::get().getConfigMngr()->getGameAssetFolder() / "desc/tilesheet/player.json");
    Toy2D::Application::get().getResourceMngr()->add<Toy2D::ResourceType::Texture>(
        Toy2D::Application::get().getConfigMngr()->getGameAssetFolder() / "desc/tex/house_desc.json");
    Toy2D::Application::get().getResourceMngr()->add<Toy2D::ResourceType::Texture>(
        Toy2D::Application::get().getConfigMngr()->getGameAssetFolder() / "desc/tex/grass_desc.json");
    Toy2D::Application::get().getResourceMngr()->add<Toy2D::ResourceType::Texture>(
        Toy2D::Application::get().getConfigMngr()->getGameAssetFolder() / "desc/tex/ground_desc.json");
    Toy2D::Application::get().getResourceMngr()->add<Toy2D::ResourceType::Texture>(
        Toy2D::Application::get().getConfigMngr()->getGameAssetFolder() / "desc/tex/small_shadow_desc.json");

    m_world.addScene("main").setActiveScene("main").onResize(
        Toy2D::Application::get().getWindow()->getWidth(),
        Toy2D::Application::get().getWindow()->getHeight());

    m_world.getActiveScene()->setGravity({0.0f, 0.0f});

    auto&& ground = m_world.getActiveScene()->createEntity("ground");
    ground.addComponent<Toy2D::SpriteComponent>().tex_index =
        Toy2D::Application::get().getResourceMngr()->index<Toy2D::ResourceType::Texture>("ground");
    ground.getComponent<Toy2D::TransformComponent>().scale      = Vector3{100.0f, 100.0f, 1.0f};
    ground.getComponent<Toy2D::SpriteComponent>().tiling_factor = 100.0f;

    auto&& house = m_world.getActiveScene()->createEntity("house");
    house.addComponent<Toy2D::SpriteComponent>().tex_index =
        Toy2D::Application::get().getResourceMngr()->index<Toy2D::ResourceType::Texture>("house");
    house.getComponent<Toy2D::TransformComponent>().scale = Vector3{2.0f, 2.0f, 1.0f};
    auto& house_rbc =
        house.addComponent<Toy2D::Rigidbody2DComponent>();
    house_rbc.type              = Toy2D::Rigidbody2DComponent::BodyType::Static;
    house_rbc.is_fixed_rotation = true;
    house_rbc.show_box          = false;
    house_rbc.collider.createBox(2.0f, 2.0f);

    auto&& door =
        m_world.getActiveScene()->createEntity("door");
    door.getComponent<Toy2D::TransformComponent>().translation = Vector3{0.0f, -0.77f, 0.0f};
    auto& door_rbc =
        door.addComponent<Toy2D::Rigidbody2DComponent>();
    door_rbc.type              = Toy2D::Rigidbody2DComponent::BodyType::Static;
    door_rbc.is_fixed_rotation = true;
    door_rbc.show_box          = true;
    door_rbc.collider.createBox(0.4f, 0.5f);

    auto&& player = m_world.getActiveScene()->createEntity("player");
    player.addComponent<Toy2D::CameraComponent>().camera.setOrthographicSize(3.0f);
    player.addComponent<Toy2D::TileComponent>("player");
    player.getComponent<Toy2D::TransformComponent>().translation.x = -2.5f;
    player.addComponent<Toy2D::LuaScriptComponent>(
        Toy2D::Application::get().getConfigMngr()->getGameScriptFolder() / "player_controller.lua");
    auto& player_rbc =
        player.addComponent<Toy2D::Rigidbody2DComponent>();
    player_rbc.type              = Toy2D::Rigidbody2DComponent::BodyType::Dynamic;
    player_rbc.is_fixed_rotation = true;
    player_rbc.show_box          = false;
    player_rbc.collider.createBox(0.35f, 0.4f);

    auto&& player_shadow =
        m_world.getActiveScene()->createEntity("player_shadow");
    player_shadow.getComponent<Toy2D::TransformComponent>().scale = Vector3{0.25f, 0.18f, 1.0f};
    player_shadow.addComponent<Toy2D::SpriteComponent>().tex_index =
        Toy2D::Application::get().getResourceMngr()->index<Toy2D::ResourceType::Texture>("shadow");

    auto create_grass = [&](const Vector2& pos) {
        auto&& grass = m_world.getActiveScene()->createEntity();
        grass.addComponent<Toy2D::SpriteComponent>().tex_index =
            Toy2D::Application::get().getResourceMngr()->index<Toy2D::ResourceType::Texture>("grass");
        grass.getComponent<Toy2D::TransformComponent>().translation = Vector3{pos.x, pos.y, 0.0f};
        grass.getComponent<Toy2D::TransformComponent>().scale       = Vector3{0.3f, 0.3f, 1.0f};
        grass.addComponent<Toy2D::LuaScriptComponent>(
            Toy2D::Application::get().getConfigMngr()->getGameScriptFolder() / "grass_moving.lua");
    };

    create_grass(Vector2{-1.414f, 0.0f});
    create_grass(Vector2{1.414f, 0.0f});
    // create_grass(Vector2{0.0f, 1.414f});
    // create_grass(Vector2{0.0f, -1.414f});

    m_world.onRuntimeStart();
}

void GameLayer::onDetach() {
}

void GameLayer::onUpdate(Toy2D::TimeStep timestep) {
    m_world.onUpdateRuntime(timestep);
}

void GameLayer::onImGuiRender() {
    Toy2D::Widgets::drawButton(
        "RefreshAll",
        Vector2{200.0f, 100.0f},
        Color{0.5f, 0.5f, 0.5f, 0.8f},
        Color{0.8f, 0.8f, 0.8f, 1.0f},
        Color{0.4f, 0.4f, 0.4f, 1.0f},
        []() {
        Toy2D::Application::get().getLuaInterpreter()->refreshAll();
        });
}

void GameLayer::onEvent(Toy2D::Event& event) {
    Toy2D::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<Toy2D::WindowResizeEvent>(BIND_EVENT_FN(GameLayer::onWindowResizeEvent));
}

bool GameLayer::onWindowResizeEvent(Toy2D::WindowResizeEvent& event) {
    m_world.onResize(event.getWidth(), event.getHeight());
    return false;
}
