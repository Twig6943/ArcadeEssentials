#pragma once
#include "Types.hpp"
#include "DynamicArray.hpp"

namespace Renderer {
	struct R_CapturePoint;
	class Component : public Types::ReferenceCountable {
	public:
		enum CallbackType {
			None,
			Never,
			Visible,
			Always,
		};
		enum ComponentFlags {
			DeepClone = 1,
			AggregateClient = 2,
		};
	protected:
		DynamicArray<struct ComponentNode*> m_Owners;
		u32 m_UpdateKey;
		u8 m_SelectedCallback;
		u8 m_DesiredSelectedCallback;
		u8 m_Flags;
	public:
		virtual void SceneCullCallback(u32 callback, ComponentNode* relevantOwner);
		// Original signature: virtual Types::SmartPointer<Component> Clone(); Likely changed due to RVO.
		virtual void* Clone(Types::SmartPointer<Component>& ret);
		virtual void AddOwner(ComponentNode* node);
		virtual void RemoveOwner(ComponentNode* node);
	};
	static_assert(sizeof(Component) == 0x34);

};