#pragma once

inline auto ScaleformValueDestructor = (void* (__thiscall*)(void*))(0x005fb740);

namespace Scaleform {
	namespace GFx {
		enum class ValueType : unsigned int {
			Undefined = 0,
			Null,
			Boolean,
			Number,
			String,
			StringW,
			Object,
			Array,
			DisplayObject
		};

		struct Value {
			unsigned int unknown;
			ValueType type;
			union {
				const char* string;
				float number;
			};
			inline Value() : unknown(0), type(ValueType::Undefined), string(nullptr) {}
			inline Value(const char* str) : unknown(0), type(ValueType::String), string(str) {}
			inline Value(float num) : unknown(0), type(ValueType::Number), number(num) {}
			inline ~Value() {
				ScaleformValueDestructor(this);
			}
			Value(const Value&) = delete;
			Value& operator=(const Value&) = delete;
		};
	};
};