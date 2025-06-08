#include "Layers.h"

#include <imgui.h>

namespace Game
{
	using namespace Arcane;

	Shared<CamControl> Camera;
	Shared<Shader> VoxelShader;

	Shared<VertexArray> Shape;
	glm::vec3 Position;
	float Scale;

	void TestLayer::OnAttach() 
	{
		VoxelShader = Shader::Create("assets/shaders/Voxel.glsl");
		auto& window = Application::Get().GetWindow();
		window->SetCaptureMouse(true);
		Camera = CreateShared<CamControl>(window->GetWidth(), window->GetHeight());
		Camera->SetSpeed(5.0f);

		Shape = VertexArray::Create();

		float vertices[] = {
			-0.5f, -0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
		};
		Shared<VertexBuffer> vbo = VertexBuffer::Create(vertices, sizeof(vertices));
		vbo->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
		});
		Shape->AddVertexBuffer(vbo);

		uint32_t indices[] = {
			0, 1, 2,
			2, 3, 0
		};
		Shared<IndexBuffer> ebo = IndexBuffer::Create(indices, sizeof(indices));
		Shape->SetIndexBuffer(ebo);

		Position = { 0, 0, 1 };
		Scale = 1.0f;

		RenderCMD::EnableFaceCulling(true);
		RenderCMD::SetCullFace(CullFace::BACK);
	}

	void TestLayer::OnDetach() 
	{

	}

	void TestLayer::OnUpdate(Timestep ts)
	{
		RenderCMD::Clear(true);
		glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		Camera->OnUpdate(ts);

		if (Input::IsKeyPressed(Key::D1))
		{
			color.g = 0.0f;
			color.b = 0.0f;
		}

		if (Input::IsKeyPressed(Key::D2))
		{
			color.r = 0.0f;
			color.b = 0.0f;
		}

		if (Input::IsKeyPressed(Key::D3))
		{
			color.r = 0.0f;
			color.g = 0.0f;
		}

		VoxelShader->Bind();
		VoxelShader->SetMat4("u_ProjectionView", Camera->GetCamera()->GetProjectionView());
		VoxelShader->SetFloat4("u_Color", color);

		glm::mat4 model = glm::translate(glm::mat4(1.0f), Position) *
			glm::scale(glm::mat4(1.0f), glm::vec3(Scale));

		VoxelShader->SetMat4("u_Model", model);
		RenderCMD::DrawIndexed(Shape);

		VoxelShader->Unbind();
	}

	void TestLayer::OnImGuiRender() 
	{

#if GAME_DEBUG 1
		if (ImGui::Begin("Cam Data"))
		{
			auto& cam = Camera->GetCamera();
			glm::vec3 position = cam->GetPosition();
			glm::vec3 rotation = cam->GetRotation();
			ImGui::Text("Position: %.2f, %.2f, %.2f", position.x, position.y, position.z);
			ImGui::Text("Rotation: %.2f, %.2f, %.2f", rotation.x, rotation.y, rotation.z);
		}
		ImGui::End();
#endif
	}

	void TestLayer::OnEvent(Event& event) 
	{
		Camera->OnEvent(event);
	}

}