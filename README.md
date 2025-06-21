# ArcadeEssentials
A [Pentane](https://github.com/high-octane-dev/pentane) plugin for Cars 2: Arcade (v1.42) that restores a significant amount of critical base game functionality. Made with love by [itsmeft24](https://github.com/itsmeft24)!

# Features
ArcadeEssentials comes with a whole host of features and bug fixes, including (but not limited to!):
- Support for Windows 11-style Dark Mode
- Restored `FrontEnd` Menus
- Functional Audio Settings Menu
- Restored Per-MissionMode + In-Game Console UI
- Fixed UI SRGB/Linear Color-Space Mismatch
- Proper UI Scaling for Resolutions > 720p
- Basic Save File Functionality
- Removed Security Dongle Checks + "Maintenance Reboots" (lol, lmao even)
- Replaced Input Driver with its XInput/DirectInput Equivalent
- Removed AutoPilot
- Removed `CarsCameraInfo.lua` Distance Clamping
- Removed Global FOV Multiplier
- Implemented `SpinOut` and `SideBash` Handling
- Restored Manual Backwards Driving
- Removed Global Timers (3m Race Timer/ 15s `FrontEnd` Timer)
- and many, many more!

# Downloads
Head to the [Releases](https://github.com/itsmeft24/ArcadeEssentials/releases/latest) page to get the latest build!
## Installation and Usage
Note that you **will need [Pentane](https://github.com/high-octane-dev/pentane) installed and properly configured** to install ArcadeEssentials. Full installation and setup tutorials for both *Pentane* and *ArcadeEssentials* can be found on [the high-octane-dev organization website](https://high-octane-dev.github.io/). 

## Requisite Asset Changes
It is **highly recommended** that you either modify the following files *or* install an overhaul mod (such as *Cars 2: Arcade Restorations* or *Cars 2: Deluxe*) that includes these changes to obtain the most functional game experience.
- `assets/startup.zip`
  - Replace Arcade's `all.values.aurora` file with one from either the official PC, Xbox 360, or PlayStation 3 releases, to restore the majority of the original game's physics and speed.
- `assets/startup3.zip`
  - Replace Arcade's `xml/GameStructure.xml` file with one from either the official PC, Xbox 360, or PlayStation 3 releases, to allow for base game missions, maps, free play options, achievements, and AI changes to appear in FrontEnd.
- `assets/xml/xml.zip`
  - Replace Arcade's entire `xml.zip` package with one from either the official PC, Xbox 360, or PlayStation 3 releases, to restore PC/Console controller button mappings.
- `assets/flash/flash.zip`
  - Replace Arcade's entire `flash.zip` package with one from either the Xbox 360 or PlayStation 3 releases, to fix free play localization, missing Auto-Drift UI, and other minor issues.
  - Add PC's `pc_` related flash files into the main game's `flash.zip` package. Do not overwrite any existing Console files.

# Disclaimer
ArcadeEssentials *does not* aim to faithfully recreate or attempt to effectively replace the original Xbox 360 or PlayStation 3 versions of Cars 2: The Video Game. Many core gameplay aspects, such as physics, UI speed, gamemode scores, story progression, etc., may significantly differ from the base game, **or may not function at all**. Namely, Multiplayer **is not supported** and **there are absolutely zero plans to address this in the future.** Additionally, ArcadeEssentials makes **absolutely no guarantees** that its featureset will not change over time, so please keep this in mind if you seek to speedrun Arcade with ArcadeEssentials over the official releases. 

# Credits
- [Me](https://github.com/itsmeft24) (wrote it lol)
- [RiskiVR](https://github.com/RiskiVR) (Testing/QA)
- [MythicalBry](https://github.com/MythicalBry) (Testing/QA)
- [maximilian](https://github.com/DJmax0955) (Testing/QA)
