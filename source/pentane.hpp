#pragma once
#include <cstddef>
#include <cstdint>
#include <string>
#include <format>

constexpr uint8_t hex_char_to_byte(char c) {
	return (c >= '0' && c <= '9') ? (c - '0') : (c >= 'a' && c <= 'f') ? (c - 'a' + 10) : (c >= 'A' && c <= 'F') ? (c - 'A' + 10) : throw std::invalid_argument("Invalid Character");
}

struct PentaneCStringView {
	const char* const data;
	const std::size_t data_len;
};

struct SemVer {
	std::int16_t major;
	std::int16_t minor;
	std::int16_t patch;
};

struct PentaneUUID {
	std::uint8_t data[16];
};

struct PluginInformation {
	char name[256];
	char author[256];
	PentaneUUID uuid;
	SemVer version;
	SemVer minimum_pentane_version;
	char reserved[484];
};

// The size of PluginInformation should be kept at 1024 bytes throughout Pentane updates.
// This should theoretically allow for Pentane to update the layout of PluginInformation without breaking compatibility with older plugins.
static_assert(sizeof(PluginInformation) == 0x400);

extern void(*Pentane_LogUTF8)(PentaneCStringView*);

namespace logger {
	inline void log(const std::string& str) {
		PentaneCStringView c_str{ str.data(), str.length() };
		Pentane_LogUTF8(&c_str);
	}
	template<typename... Args>
	void log_format(std::format_string<Args...> fmt, Args&&... args) {
		auto str = std::vformat(fmt.get(), std::make_format_args(args...));
		log(str);
	}
};

#define PENTANE_GAME_TARGET_2TVGA