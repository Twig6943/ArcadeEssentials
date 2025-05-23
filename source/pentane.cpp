#include "pentane.hpp"

void(*Pentane_LogUTF8)(PentaneCStringView*) = nullptr;

extern "C" PluginInformation Pentane_PluginInformation = PluginInformation {
	.name = "Arcade Essentials",
	.author = "itsmeft24",
	.uuid = { 0x00, 0xFA, 0x94, 0x8B, 0x24, 0xA7, 0x4B, 0x2E, 0xA6, 0xA6, 0x18, 0xE3, 0xA1, 0x02, 0x9A, 0xB5 },
	.version = SemVer{ 0, 1, 0 },
	.minimum_pentane_version = SemVer{ 0, 1, 0 },
};
extern "C" int Pentane_PluginDependencyCount = 0;
extern "C" PentaneUUID* Pentane_PluginDependencies = nullptr;
