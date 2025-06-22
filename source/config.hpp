#pragma once
#include <memory>

class AEConfig {
public:
	int window_width = 1280;
	int window_height = 720;
	bool vsync = true;
public:
	AEConfig() = default;
	AEConfig(const AEConfig&) = delete;
	AEConfig& operator=(const AEConfig&) = delete;

	auto read() -> bool;
	auto finalize() const -> bool;
};

// Since we're only ever modifying this from a single thread post-boot, keeping this unlocked should be fine...
extern std::unique_ptr<AEConfig> GLOBAL_CONFIG;
