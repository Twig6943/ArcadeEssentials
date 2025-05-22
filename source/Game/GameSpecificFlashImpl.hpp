#pragma once
#include "../Game/CLinearHash.hpp"

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
	GameSpecificFlashImpl();
	~GameSpecificFlashImpl();
	void SetFlashVariableFunc(const char*, enum class SetFlashVariDef::ArgumentType, class GameSpecificFlashFunction*);
};

static_assert(sizeof(GameSpecificFlashImpl) == 0x38);

class GameSpecificFlashFunction {
public:
	virtual void operator()(void* val, void* movie) = 0;
};

static_assert(sizeof(GameSpecificFlashFunction) == 4);