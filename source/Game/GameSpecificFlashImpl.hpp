#pragma once
#include "../Game/CLinearHash.hpp"

inline auto GameSpecificFlashImpl_SetFlashVariableFunc = (void(__thiscall*)(class GameSpecificFlashImpl*, const char*, char, void*))(0x01164570);

struct SetFlashVariDef {
	enum class ArgumentType : char {
		Null = 0,
		Boolean = 'b',
		Number = 'n',
		String = 's',
		ScaleformGFXValue = 'v',
	};
	const char* method_name;
	ArgumentType argType;
	class GameSpecificFlashFunction* set_function;
};

static_assert(sizeof(SetFlashVariDef) == 12);

class GameSpecificFlashImpl {
protected:
	CLinearHash<SetFlashVariDef*> set_variable_actions;
	void* parent_movie;
	void* unk;
public:
	inline GameSpecificFlashImpl() {
	}
	inline ~GameSpecificFlashImpl() {
	}
	inline void SetFlashVariableFunc(const char* name, SetFlashVariDef::ArgumentType arg_type, class GameSpecificFlashFunction* callback) {
		GameSpecificFlashImpl_SetFlashVariableFunc(this, name, std::to_underlying(arg_type), callback);
	}
};

static_assert(sizeof(GameSpecificFlashImpl) == 0x38);

class GameSpecificFlashFunction {
public:
	virtual void operator()(void* val, void* movie) = 0;
};

static_assert(sizeof(GameSpecificFlashFunction) == 4);