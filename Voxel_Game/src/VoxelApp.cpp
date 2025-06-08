#include <Arcane.h>
#include <Arcane/Core/EntryPoint.h>

#include "Layers/Layers.h"

namespace Game
{
	class VoxelApp : public Arcane::Application
	{
	public:
		VoxelApp(const Arcane::WindowProps& props) :
			Application(props) {
			m_GameLayer = new TestLayer();
			PushLayer(m_GameLayer);
		}

		~VoxelApp()
		{
			Application::~Application();
			PopLayer(m_GameLayer);
		}

	private:
		TestLayer* m_GameLayer;
	};
}

Arcane::Application* Arcane::CreateApplication() {
	WindowProps props;
	props.Title = "Voxel Game";
	props.Width = 860;
	props.Height = 480;
	props.Resizable = true;

	return new Game::VoxelApp(props);
}