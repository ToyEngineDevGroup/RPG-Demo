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

    m_world.addScene("main").setActiveScene("main").onResize(
        Toy2D::Application::get().getWindow()->getWidth(),
        Toy2D::Application::get().getWindow()->getHeight());

    auto&& ground = m_world.getActiveScene()->createEntity("ground");
    ground.addComponent<Toy2D::SpriteComponent>().tex_index =
        Toy2D::Application::get().getResourceMngr()->index<Toy2D::ResourceType::Texture>("ground");
    ground.getComponent<Toy2D::TransformComponent>().scale      = Vector3{10.0f, 10.0f, 1.0f};
    ground.getComponent<Toy2D::SpriteComponent>().tiling_factor = 10.0f;

    auto&& house = m_world.getActiveScene()->createEntity("house");
    house.addComponent<Toy2D::SpriteComponent>().tex_index =
        Toy2D::Application::get().getResourceMngr()->index<Toy2D::ResourceType::Texture>("house");
    house.getComponent<Toy2D::TransformComponent>().scale = Vector3{2.0f, 2.0f, 1.0f};

    auto&& player = m_world.getActiveScene()->createEntity("player");
    player.addComponent<Toy2D::CameraComponent>().camera.setOrthographicSize(3.0f);
    player.addComponent<Toy2D::TileComponent>("player");
    player.addComponent<Toy2D::LuaScriptComponent>(
        Toy2D::Application::get().getConfigMngr()->getGameScriptFolder() / "player_controller.lua");

    auto create_grass = [&](const Vector2& pos) {
        auto&& grass = m_world.getActiveScene()->createEntity();
        grass.addComponent<Toy2D::SpriteComponent>().tex_index =
            Toy2D::Application::get().getResourceMngr()->index<Toy2D::ResourceType::Texture>("grass");
        grass.getComponent<Toy2D::TransformComponent>().translation = Vector3{pos.x, pos.y, 0.0f};
        grass.getComponent<Toy2D::TransformComponent>().scale       = Vector3{0.3f, 0.3f, 1.0f};
    };

    create_grass(Vector2{-1.5f, 1.5f});
    create_grass(Vector2{1.5f, 1.5f});
    create_grass(Vector2{1.5f, -1.5f});
    create_grass(Vector2{-1.5f, -1.5f});

    m_world.onRuntimeStart();
}

void GameLayer::onDetach() {
}

void GameLayer::onUpdate(Toy2D::TimeStep timestep) {
    m_world.onUpdateRuntime(timestep);
}

void GameLayer::onImGuiRender() {
    Toy2D::Widgets::drawText("2D RPG Game Demo\nAuthor: yichenwu11");
}

void GameLayer::onEvent(Toy2D::Event& event) {
    Toy2D::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<Toy2D::WindowResizeEvent>(BIND_EVENT_FN(GameLayer::onWindowResizeEvent));
}

bool GameLayer::onWindowResizeEvent(Toy2D::WindowResizeEvent& event) {
    m_world.onResize(event.getWidth(), event.getHeight());
    return false;
}
