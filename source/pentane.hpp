#pragma once
#include <cstddef>
#include <cstdint>
#include <string>
#include <format>
#include <chrono>

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
extern int(*Pentane_IsWindowedModeEnabled)();

#ifdef _DEBUG
extern std::chrono::time_point<std::chrono::system_clock> start_time;
#endif

namespace logger {
	inline void log(const std::string& str) {
		PentaneCStringView c_str{ str.data(), str.length() };
		Pentane_LogUTF8(&c_str);
	}
	template<typename... Args>
	void log_format(std::format_string<Args...> fmt, Args&&... args) {
		std::string str = std::vformat(fmt.get(), std::make_format_args(args...));
#ifdef _DEBUG
		double seconds = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start_time).count() * 0.001;
		double minutes = std::floor(seconds / 60.0);
		std::string timestamp = std::format(" [{}:{:.3f}]", minutes, seconds - minutes * 60.0);
		log(str + timestamp);
#else
		log(str);
#endif
	}
};

#define PENTANE_GAME_TARGET_2TVGA