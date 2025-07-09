#include <cwctype>
#include <memory>
#include <Windows.h>
#include <SetupAPI.h>
#include <initguid.h>
#include <Usbiodef.h>
#include <cfgmgr32.h>
#include <usbspec.h>
#include <winusb.h>
#include "GameCubeAdapter.hpp"

GameCubeAdapter::GameCubeAdapter(const std::wstring& devicePath) : m_deviceHandle(INVALID_HANDLE_VALUE), m_winUsbHandle(INVALID_HANDLE_VALUE), m_pipeIn(0), m_pipeOut(0), m_adapterState() {
	m_deviceHandle = CreateFileW(
		devicePath.c_str(),
		GENERIC_WRITE | GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		nullptr,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
		nullptr
	);

	if (m_deviceHandle == INVALID_HANDLE_VALUE) {
		printf("Failed to open device: %lu\n", GetLastError());
		return;
	}

	if (!WinUsb_Initialize(m_deviceHandle, &m_winUsbHandle)) {
		printf("WinUsb_Initialize failed: %lu\n", GetLastError());
		CloseHandle(m_deviceHandle);
		m_deviceHandle = INVALID_HANDLE_VALUE;
		m_winUsbHandle = INVALID_HANDLE_VALUE;
		return;
	}

	USB_INTERFACE_DESCRIPTOR ifaceDesc;
	WinUsb_QueryInterfaceSettings(m_winUsbHandle, 0, &ifaceDesc);

	for (auto i = 0; i < ifaceDesc.bNumEndpoints; i++) {
		WINUSB_PIPE_INFORMATION pipeInfo;
		if (WinUsb_QueryPipe(m_winUsbHandle, 0, i, &pipeInfo)) {
			if (USB_ENDPOINT_DIRECTION_IN(pipeInfo.PipeId) && pipeInfo.PipeType == UsbdPipeTypeInterrupt) {
				m_pipeIn = pipeInfo.PipeId;
			}
			else if (USB_ENDPOINT_DIRECTION_OUT(pipeInfo.PipeId)) {
				m_pipeOut = pipeInfo.PipeId;
			}
		}
	}

	UCHAR initBuf[1] = { 0x13 };
	ULONG bytesWritten = 0;

	if (!WinUsb_WritePipe(m_winUsbHandle, m_pipeOut, initBuf, sizeof(initBuf), &bytesWritten, nullptr)) {
		printf("Failed to send init: %lu\n", GetLastError());
	}
}

GameCubeAdapter::~GameCubeAdapter() {
	if (m_winUsbHandle != INVALID_HANDLE_VALUE) {
		WinUsb_Free(m_winUsbHandle);
	}
	if (m_deviceHandle != INVALID_HANDLE_VALUE) {
		CloseHandle(m_deviceHandle);
	}
}

bool GameCubeAdapter::Update() {
	if (IsAlive()) {
		unsigned long bytesRead = 0;
		if (WinUsb_ReadPipe(m_winUsbHandle, m_pipeIn, reinterpret_cast<unsigned char*>(&m_adapterState), sizeof(m_adapterState), &bytesRead, nullptr) == 0) {
			unsigned long err = GetLastError();
			if (err == ERROR_DEVICE_NOT_CONNECTED || err == ERROR_GEN_FAILURE || err == ERROR_INVALID_HANDLE) {
				WinUsb_Free(m_winUsbHandle);
				CloseHandle(m_deviceHandle);
				m_deviceHandle = INVALID_HANDLE_VALUE;
				m_winUsbHandle = INVALID_HANDLE_VALUE;
			}
			return false;
		}
		if (bytesRead != sizeof(Packet)) {
			return false;
		}
		return true;
	}
	return false;
}

bool GameCubeAdapter::IsConnected(int port) const {
	if (IsAlive()) {
		return m_adapterState.controllers[port].state != ControllerStatus::Disconnected;
	}
	return false;
}

bool GameCubeAdapter::IsAlive() const {
	return m_deviceHandle != INVALID_HANDLE_VALUE && m_winUsbHandle != INVALID_HANDLE_VALUE;
}

GameCubeControllerState GameCubeAdapter::GetState(int port) const {
	return m_adapterState.controllers[port];
}

std::vector<std::wstring> SearchForAdapters() {
	HDEVINFO devInfoSetHndl = SetupDiGetClassDevsW(&GUID_DEVINTERFACE_USB_DEVICE, nullptr, nullptr, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
	SP_DEVICE_INTERFACE_DATA ifData = {};
	ifData.cbSize = sizeof(ifData);
	auto i = 0;
	std::vector<std::wstring> devicePaths{};
	while (SetupDiEnumDeviceInterfaces(devInfoSetHndl, nullptr, &GUID_DEVINTERFACE_USB_DEVICE, i, &ifData) != 0) {
		unsigned long len = 0;
		SetupDiGetDeviceInterfaceDetailW(devInfoSetHndl, &ifData, nullptr, 0, &len, nullptr);
		std::vector<unsigned char> detailDataBuffer(len);
		auto* detailData = reinterpret_cast<PSP_DEVICE_INTERFACE_DETAIL_DATA_W>(detailDataBuffer.data());
		detailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA_W);

		SP_DEVINFO_DATA deviceInfoData{};
		deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

		if (SetupDiGetDeviceInterfaceDetailW(devInfoSetHndl, &ifData, detailData, len, nullptr, &deviceInfoData)) {
			if (CM_Get_Device_ID_Size(&len, deviceInfoData.DevInst, 0) == CR_SUCCESS) {
				std::vector<char> temp(len + 1);
				if (CM_Get_Device_IDA(deviceInfoData.DevInst, temp.data(), temp.size(), 0) == CR_SUCCESS) {
					std::string_view view(temp.data(), temp.size());
					if (view.starts_with("USB\\VID_") && view.contains("&PID_")) {
						unsigned long vendorId = std::stoul(std::string(view.substr(8, 4)), nullptr, 16);
						unsigned long productId = std::stoul(std::string(view.substr(17, 4)), nullptr, 16);
						if (vendorId == NINTENDO_CO_LTD && productId == GAMECUBE_CONTROLLER_ADAPTER) {
							devicePaths.push_back(detailData->DevicePath);
						}
					}
				}
			}
		}
		i++;
	};
	SetupDiDestroyDeviceInfoList(devInfoSetHndl);
	return devicePaths;
}
