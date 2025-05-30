#pragma once
#include <cstdint>

using AgentHandle = std::uintptr_t;
using ActorHandle = std::uintptr_t;
using AgentID = AgentHandle;

class CEntityType {
public:
	uint32_t d_field;
	uint32_t d_baseMask;
	uint32_t d_derivedMask;
	uint_fast32_t d_derivedShift;
public:
	inline virtual ~CEntityType() {
	}
};

class CStageEntity {
public:
	AgentHandle m_agentHandle;
	CEntityType d_instanceType;
public:
	inline virtual bool IsUnstableCollisionObject(void) {
		return false;
	}
	inline virtual ~CStageEntity() = 0;
	virtual void* BeginCollisionTest(CStageEntity* i_entity) = 0;
	virtual bool EndCollisionTest() = 0;
	virtual const char* GetClassName(void) = 0;
	virtual void AppendDescription(char* io_msg) = 0;
};