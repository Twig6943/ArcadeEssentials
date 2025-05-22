#include "pentane.hpp"

void(*Pentane_LogUTF8)(PentaneCStringView*) = nullptr;

extern "C" PluginInformation Pentane_PluginInformation = PluginInformation {
	.name = "Arcade Essentials",
	.author = "itsmeft24",
	.uuid = PentaneUUID::from_str("00fa948b-24a7-4b2e-a6a6-18e3a1029ab5"),
	.version = SemVer{ 0, 1, 0 },
	.minimum_pentane_version = SemVer{ 0, 1, 0 },
};
extern "C" int Pentane_PluginDependencyCount = 0;
extern "C" PentaneUUID* Pentane_PluginDependencies = nullptr;
