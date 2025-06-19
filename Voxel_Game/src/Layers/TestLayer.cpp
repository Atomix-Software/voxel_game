#include "Layers.h"

#include <random>
#include <imgui.h>

#include "Game/World/Chunk.h"

namespace Game
{
	using namespace Arcane;

	struct Transform
	{
		glm::vec3 Position = glm::vec3(0.0f);
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		float Scale = 1.0f;
	};
	
	struct alignas(16) InstanceData {
		glm::mat4 Transform = glm::mat4(1.0f);
		int BlockId = 0;
		int padding[3];
	};

	Shared<CamControl> Camera;
	Shared<Shader> VoxelShader;

	Shared<VertexArray> Cube;

	Shared<ShaderStorageBuffer> instanceSSBO,uvSSBO;
	std::vector<InstanceData> instanceData;
	std::vector<BlockUVData> allBlockUVs;
	
	bool paused = false;

	Shared<VertexArray> MakeCube();

	void TestLayer::OnAttach() 
	{
		auto& window = Application::Get().GetWindow();
		window->SetCaptureMouse(true);
		Camera = CreateShared<CamControl>((float) window->GetWidth(), (float) window->GetHeight());
		Camera->GetCamera()->SetPosition({0, 0, 0});
		Camera->SetSpeed(5.0f);

		BlockDatabase::Init();

		Cube = MakeCube();

		RenderCMD::EnableFaceCulling(true);
		RenderCMD::SetCullFace(CullFace::BACK);

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> random(0, 10);

		float size = 16.0f;
		for (auto k = 0.0f; k < size; k++)
		{
			for (auto i = 0.0f; i < size; i++)
			{
				for (auto j = 0.0f; j < size; j++)
				{
						Transform trans;
						InstanceData data;


						if (random(gen) > 8)
							continue;

						if ((int) (i + j) % 2 == 0) data.BlockId = BlockId::GRASS;
						else data.BlockId = BlockId::DIRT;

						if (random(gen) >= 7)
							data.BlockId = BlockId::STONE;

						if ((data.BlockId == BlockId::DIRT || data.BlockId == BlockId::STONE) && k >= size - 1)
							data.BlockId = BlockId::GRASS;

						if ((data.BlockId == BlockId::GRASS || data.BlockId == BlockId::STONE) && k < size - 1)
						{
							data.BlockId = BlockId::DIRT;

							if (k <= size - 6)
							{
								if (random(gen) > 2)
									data.BlockId = data.BlockId != BlockId::STONE ? BlockId::STONE : data.BlockId;
							}
						}

						trans.Position.x = i;
						trans.Position.y = k;
						trans.Position.z = j;

						if (data.BlockId == BlockId::STONE || data.BlockId == BlockId::DIRT && random(gen) > 5)
						{
							trans.Rotation.y -= 90.0f;
							trans.Rotation.z += 90.0f;
						}

						data.Transform  = glm::translate(glm::mat4(1.0), trans.Position);
						data.Transform *= glm::rotate(glm::mat4(1.0), glm::radians(trans.Rotation.x), { 1.0f, 0.0f, 0.0f });
						data.Transform *= glm::rotate(glm::mat4(1.0), glm::radians(trans.Rotation.y), { 0.0f, 1.0f, 0.0f });
						data.Transform *= glm::rotate(glm::mat4(1.0), glm::radians(trans.Rotation.z), { 0.0f, 0.0f, 1.0f });
						data.Transform *= glm::scale(glm::mat4(1.0), glm::vec3(trans.Scale));

						instanceData.push_back(data);
				}
			}
		}

		instanceSSBO = ShaderStorageBuffer::Create((uint32_t) instanceData.size() * sizeof(InstanceData));

		std::vector<FaceUV> faceUVs = BlockDatabase::GenerateSSBOData();
		for (auto i = 0; i < faceUVs.size() / 6; ++i)
		{
			BlockUVData data;

			for (auto f = 0; f < 6; ++f)
				data.Faces[f] = faceUVs[i * 6 + f];

			allBlockUVs.push_back(data);
		}

		uvSSBO = ShaderStorageBuffer::Create((uint32_t) allBlockUVs.size() * sizeof(BlockUVData), allBlockUVs.data(), 1);

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
		if (paused && Input::KeyJustPressed(Key::Escape))
			Application::Get().Stop();

		if (Input::KeyJustPressed(Key::P))
		{
			paused = !paused;
			Application::Get().GetWindow()->SetCaptureMouse(!paused);
		}

		Camera->OnUpdate(ts, paused);
		RenderCMD::Clear(true);

		VoxelShader->Bind();
		VoxelShader->SetMat4("u_ProjectionView", Camera->GetCamera()->GetProjectionView());

		BlockDatabase::GetAtlas()->Bind(0);

		instanceSSBO->Bind(0); 
		instanceSSBO->SetData(instanceData.data(), (uint32_t) instanceData.size() * sizeof(InstanceData));

		uvSSBO->Bind(1);

		RenderCMD::DrawInstanced(Cube, (uint32_t) instanceData.size());

		VoxelShader->Unbind();
	}

	void TestLayer::OnImGuiRender() 
	{
#if GAME_DEBUG
		//ImVec2 pos(0, 0);
		//ImGui::SetNextWindowPos(pos);

		ImGui::Begin("Cam Data");
			auto& cam = Camera->GetCamera();
			glm::vec3 position = cam->GetPosition();
			glm::vec3 rotation = cam->GetRotation();
			ImGui::Text("Position: %.1f, %.1f, %.1f", position.x, position.y, position.z);
			ImGui::Text("Rotation: %.1f, %.1f, %.1f", rotation.x, rotation.y, rotation.z);
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
			// Front - 0
			  0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, // BL
			 -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // BR
			 -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // TR
			  0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // TL
			// Right - 1
			 -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // BL
			 -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // BR
			 -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, // TR
			 -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, // TL
			// Back - 2
			 -0.5f, -0.5f,  -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 2.0f, // BL
			  0.5f, -0.5f,  -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 2.0f, // BR
			  0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 2.0f, // TR
			 -0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 2.0f, // TL
			// Left - 3
			  0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 3.0f, // BL
			  0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 3.0f, // BR
			  0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 3.0f, // TR
			  0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 3.0f, // TL
			// Top - 4
			 -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 4.0f, // BL
			  0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 4.0f, // BR
			  0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 4.0f, // TR
			 -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 4.0f, // TL
			// Bottom - 5
			 -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 5.0f, // BL
			  0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 5.0f, // BR
			  0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 5.0f, // TR
			 -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 5.0f, // TL
		};

		Shared<VertexBuffer> vbo = VertexBuffer::Create(vertices, sizeof(vertices));
		vbo->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float2, "a_TexCoords" },
			{ ShaderDataType::Float,  "a_TextureId" },
			{ ShaderDataType::Float,  "a_FaceIndex" },
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