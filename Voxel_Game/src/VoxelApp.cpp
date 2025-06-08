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
			PushLayer(new TestLayer());
		}
	};
}

Arcane::Application* Arcane::CreateApplication() {
	return new Game::VoxelApp({ "" });
}