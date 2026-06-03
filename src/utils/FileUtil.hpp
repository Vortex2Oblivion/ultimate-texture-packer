#pragma once

#include "nfd.h"

#include <filesystem>
#include <vector>


namespace utp::utils {

	struct FileResult {
		std::filesystem::path outPath;
		nfdresult_t result;
	};

	class FileUtil {
	public:
		static std::filesystem::path lastPath;
		static FileResult openFileDialog(const std::vector<nfdu8filteritem_t> &filterList, const std::filesystem::path& defaultPath = lastPath);
	};
} // namespace utp::utils
