#include "Layers.h"

#include <imgui.h>

#include "Game/World/Chunk.h"

namespace Game
{
	using namespace Arcane;

	struct Transform
	{
		glm::vec3 Position = glm::vec3(0);
		glm::vec3 Rotation = glm::vec3(0);
		float Scale = 1.0f;
	};
	
	struct InstanceData {
		glm::mat4 Model = glm::mat4(1.0f);
		float TextureID = 0.0f;
	};

	Shared<CamControl> Camera;
	Shared<Shader> VoxelShader;

	Shared<VertexArray> Cube;
	Shared<Texture2D> Test, Test1;

	Shared<VertexArray> MakeCube();
	std::vector<InstanceData> instanceData;

	void TestLayer::OnAttach() 
	{
		auto& window = Application::Get().GetWindow();
		window->SetCaptureMouse(true);
		Camera = CreateShared<CamControl>((float) window->GetWidth(), (float) window->GetHeight());
		Camera->GetCamera()->SetPosition({0, 61, 0});
		Camera->SetSpeed(5.0f);

		Blocks::Init();

		Cube = MakeCube();
		Test = Texture2D::Create("assets/textures/test1.jpg");
		Test1 = Texture2D::Create("assets/textures/test.jpg");

		RenderCMD::EnableFaceCulling(true);
		RenderCMD::SetCullFace(CullFace::BACK);

		for (int k = 0; k < 60; k++)
		{
			for (int i = 0; i < 16; i++)
			{
				for (int j = 0; j < 16; j++)
				{
						Transform trans;
						trans.Position.x = i;
						trans.Position.y = k;
						trans.Position.z = j;

						trans.Rotation.y = 90.0f;

						InstanceData data;
						data.Model = glm::translate(data.Model, trans.Position);
						data.Model = glm::rotate(data.Model, glm::radians(trans.Rotation.x), { 1, 0, 0 });
						data.Model = glm::rotate(data.Model, glm::radians(trans.Rotation.y), { 0, 1, 0 });
						data.Model = glm::rotate(data.Model, glm::radians(trans.Rotation.z), { 0, 0, 1 });

						data.TextureID = (i + j) % 2;
						instanceData.push_back(data);
				}
			}
		}

		Shared<VertexBuffer> instanceVBO = VertexBuffer::Create(instanceData.data(), instanceData.size() * sizeof(InstanceData));
		instanceVBO->SetLayout({
			{ ShaderDataType::Mat4, "i_Transform", false, true, 4 },
			{ ShaderDataType::Float, "i_InstanceTexId", false, true, 8 },
			});
		Cube->AddVertexBuffer(instanceVBO);

		VoxelShader = Shader::Create("assets/shaders/Voxel.glsl");
		VoxelShader->Bind();

		uint32_t samplers[32];
		for (int i = 0; i < 32; ++i)
			samplers[i] = i;

		VoxelShader->SetIntArray("u_Textures", (int*)samplers, 32);
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

		Test->Bind(0);
		Test1->Bind(1);

		RenderCMD::DrawInstanced(Cube, instanceData.size());

		VoxelShader->Unbind();
	}

	void TestLayer::OnImGuiRender() 
	{
#if GAME_DEBUG
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

	Shared<VertexArray> MakeCube()
	{
		Shared<VertexArray> result = VertexArray::Create();

		float vertices[] = {
			// North
			 1.0f,  0.0f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			 0.0f,  1.0f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
			 0.0f,  0.0f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			// East
			 0.0f,  0.0f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
			 0.0f,  1.0f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			 0.0f,  1.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
			 0.0f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			// South
			 0.0f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
			 0.0f,  1.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			 1.0f,  1.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
			 1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			// West
			 1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
			 1.0f,  0.0f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			// Top
			 0.0f,  1.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
			 0.0f,  1.0f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
			 1.0f,  1.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			// Bottom
			 0.0f,  0.0f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
			 0.0f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			 1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
			 1.0f,  0.0f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		};

		Shared<VertexBuffer> vbo = VertexBuffer::Create(vertices, sizeof(vertices));
		vbo->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float2, "a_TexCoords" },
			{ ShaderDataType::Float,  "a_TextureId" },
		});
		result->AddVertexBuffer(vbo);

		uint32_t indices[] = {
			0, 1, 2,
			2, 3, 0,

			4, 5, 6,
			6, 7, 4,

			8, 9, 10,
			10, 11, 8,

			12, 13, 14,
			14, 15, 12,

			16, 17, 18,
			18, 19, 16,

			20, 21, 22,
			22, 23, 20
		};

		Shared<IndexBuffer> ebo = IndexBuffer::Create(indices, sizeof(indices));
		result->SetIndexBuffer(ebo);

		return result;
	}

}