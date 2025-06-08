#pragma once

#define GAME_DEBUG 0

#include <Arcane.h>
#include "Game/CamControl.h"

namespace Game
{
	class TestLayer : public Arcane::Layer
	{
	public:
		TestLayer() : Layer("Test_Layer") {}
		virtual ~TestLayer() override {}

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		virtual void OnUpdate(Arcane::Timestep ts) override;
		virtual void OnEvent(Arcane::Event& event) override;
	};
}