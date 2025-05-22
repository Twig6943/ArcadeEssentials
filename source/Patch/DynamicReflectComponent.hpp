#pragma once
#include "../Game/Renderer.hpp"

namespace Effects {
	class DynamicReflectComponent : public Renderer::Component {
	private:
		u32 m_BufferNum;
		Renderer::R_CapturePoint* m_CapturePoint;
	public:
		virtual ~DynamicReflectComponent() override;
		virtual void SceneCullCallback(u32 callback, Renderer::ComponentNode* relevantOwner) override;
		virtual void AddOwner(Renderer::ComponentNode* node) override;
	};
	static_assert(sizeof(DynamicReflectComponent) == 0x3C);
};