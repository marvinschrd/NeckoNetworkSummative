#include "02_rollback_simulation/rollback_engine.h"
#include "mathematics/transform.h"
#include "imgui.h"

namespace neko::net
{
AsteroidDebugApp::AsteroidDebugApp() : server_(clients_)
{
	for(int i = 0; i < clients_.size();i++)
	{
		clients_[i] = std::make_unique<SimulationClient>(server_);
	}
}

void AsteroidDebugApp::OnEvent(const SDL_Event& event)
{
}

void AsteroidDebugApp::Init()
{
	const auto& config = BasicEngine::GetInstance()->config;
	quad_.Init();
	//TODO client shader init
	clientShader_.LoadFromFile(config.dataRootPath + "shaders/comp_net/client.vert",
		config.dataRootPath + "shaders/comp_net/client.frag");
	for (auto& client : clients_)
	{
		client->Init();
	}
	server_.Init();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void AsteroidDebugApp::Update(seconds dt)
{
	//Checking if keys are down
	const Uint8* keys = SDL_GetKeyboardState(nullptr);
	net::PlayerInput clientInput1 = 0;
	clientInput1 = clientInput1 | (keys[SDL_SCANCODE_LEFT] ? asteroid::PlayerInput::LEFT : 0u);
	clientInput1 = clientInput1 | (keys[SDL_SCANCODE_RIGHT] ? asteroid::PlayerInput::RIGHT : 0u);
	clientInput1 = clientInput1 | (keys[SDL_SCANCODE_UP] ? asteroid::PlayerInput::UP : 0u);
	clientInput1 = clientInput1 | (keys[SDL_SCANCODE_DOWN] ? asteroid::PlayerInput::DOWN : 0u);
	clientInput1 = clientInput1 | (keys[SDL_SCANCODE_LCTRL] ? asteroid::PlayerInput::SHOOT : 0u);

	clients_[0]->SetPlayerInput(clientInput1);

	net::PlayerInput clientInput2 = 0;
	clientInput2 = clientInput2 | (keys[SDL_SCANCODE_A] ? asteroid::PlayerInput::LEFT : 0u);
	clientInput2 = clientInput2 | (keys[SDL_SCANCODE_D] ? asteroid::PlayerInput::RIGHT : 0u);
	clientInput2 = clientInput2 | (keys[SDL_SCANCODE_W] ? asteroid::PlayerInput::UP : 0u);
	clientInput2 = clientInput2 | (keys[SDL_SCANCODE_S] ? asteroid::PlayerInput::DOWN : 0u);
	clientInput2 = clientInput2 | (keys[SDL_SCANCODE_SPACE] ? asteroid::PlayerInput::SHOOT : 0u);

	clients_[1]->SetPlayerInput(clientInput2);
	for (auto& client: clients_)
	{
		client->Update(dt);
	}
	server_.Update(dt);
}

void AsteroidDebugApp::Destroy()
{
	for (auto& client : clients_)
	{
		client->Destroy();
	}
	server_.Destroy();
	glDisable(GL_BLEND);
}

void AsteroidDebugApp::DrawImGui()
{
    ImGui::Begin("Rollback Simulation");
    ImGui::End();
    for(auto& client : clients_)
    {
        client->DrawImGui();
    }
}

void AsteroidDebugApp::Render()
{
	const auto& config = BasicEngine::GetInstance()->config;
	for (auto& client : clients_)
	{
		client->Render();
	}

	glViewport(0, 0, config.windowSize.x, config.windowSize.y);
	clientShader_.Bind();
	auto transform = Mat4f::Identity;
	transform = Transform3d::Scale(transform, Vec3f(0.5f, 1.0f, 1.0f));
	transform = Transform3d::Translate(transform, Vec3f(-0.5f, 0.0f, 0.0f));
	clientShader_.SetMat4("transform", transform);
	clientShader_.SetTexture("texture", clients_[0]->GetFramebuffer().GetColorTexture());
	quad_.Draw();

	transform = Mat4f::Identity;
	transform = Transform3d::Scale(transform, Vec3f(0.5f, 1.0f, 1.0f));
	transform = Transform3d::Translate(transform, Vec3f(0.5f, 0.0f, 0.0f));
	clientShader_.SetMat4("transform", transform);
	clientShader_.SetTexture("texture", clients_[1]->GetFramebuffer().GetColorTexture());
	quad_.Draw();
	
}
}
