#pragma once
#include <array>
#include <string>
#include <vector>
#include <Windows.h>

constexpr unsigned short NINTENDO_CO_LTD = 0x057E;
constexpr unsigned short GAMECUBE_CONTROLLER_ADAPTER = 0x0337;

enum class ControllerStatus : unsigned char {
	Disconnected = 0,
	Normal = 16,
	WaveBird = 32,
};

constexpr unsigned short GAMECUBE_A = 0x1;
constexpr unsigned short GAMECUBE_B = 0x2;
constexpr unsigned short GAMECUBE_X = 0x4;
constexpr unsigned short GAMECUBE_Y = 0x8;
constexpr unsigned short GAMECUBE_LEFT = 0x10;
constexpr unsigned short GAMECUBE_RIGHT = 0x20;
constexpr unsigned short GAMECUBE_DOWN = 0x40;
constexpr unsigned short GAMECUBE_UP = 0x80;
constexpr unsigned short GAMECUBE_START = 0x100;
constexpr unsigned short GAMECUBE_Z = 0x200;
constexpr unsigned short GAMECUBE_RIGHT_TRIGGER = 0x400;
constexpr unsigned short GAMECUBE_LEFT_TRIGGER = 0x800;

#pragma pack(1)
struct GameCubeControllerState {
	ControllerStatus state;
	unsigned short buttons;
	unsigned char leftStickX;
	unsigned char leftStickY;
	unsigned char cStickX;
	unsigned char cStickY;
	unsigned char leftTrigger;
	unsigned char rightTrigger;
};

static_assert(sizeof(GameCubeControllerState) == 9);

struct Packet {
	unsigned char reportId;
	std::array<GameCubeControllerState, 4> controllers;
};

static_assert(sizeof(Packet) == 37);

class GameCubeAdapter {
private:
	HANDLE m_deviceHandle;
	HANDLE m_winUsbHandle;
	unsigned char m_pipeIn;
	unsigned char m_pipeOut;
	Packet m_adapterState;
public:
	GameCubeAdapter(const std::wstring& devicePath);
	GameCubeAdapter(const GameCubeAdapter&) = delete;
	GameCubeAdapter& operator=(const GameCubeAdapter&) = delete;
	~GameCubeAdapter();

	bool Update();
	bool IsConnected(int port) const;
	bool IsAlive() const;
	GameCubeControllerState GetState(int port) const;
};

std::vector<std::wstring> SearchForAdapters();