#include "WindowsUtil.hpp"

#ifdef _WIN32
#include <dwmapi.h>
#include <stdexcept>
#include <string>
#include <vector>
#include <Windows.h>
#endif

namespace utp::utils {
	/**
	 * https://stackoverflow.com/questions/51334674/how-to-detect-windows-10-light-dark-mode-in-win32-application
	 */
	bool WindowsUtil::isLightTheme() {
#ifdef _WIN32
		// The value is expected to be a REG_DWORD, which is a signed 32-bit little-endian
		auto buffer = std::vector<char>(4);
		auto cbData = static_cast<DWORD>(buffer.size() * sizeof(char));
		auto res = RegGetValueW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
								L"AppsUseLightTheme",
								RRF_RT_REG_DWORD, // expected value type
								nullptr, buffer.data(), &cbData);

		if (res != ERROR_SUCCESS) {
			throw std::runtime_error("Error: error_code=" + std::to_string(res));
		}

		// convert bytes written to our buffer to an int, assuming little-endian
		auto i = int(buffer[3] << 24 | buffer[2] << 16 | buffer[1] << 8 | buffer[0]);

		return i == 1;
#else
		return false;
#endif
	}

	void WindowsUtil::setDarkHeader() {
#ifdef _WIN32
		int darkMode = true;
		HWND window = GetActiveWindow();
		if (S_OK != DwmSetWindowAttribute(window, 19, &darkMode, sizeof(darkMode))) {
			DwmSetWindowAttribute(window, 20, &darkMode, sizeof(darkMode));
		}
		UpdateWindow(window);
#endif
	}
} // namespace utp::utils
