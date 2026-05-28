#pragma once

#include "nfd.h"

#include <filesystem>
#include <string>
#include <vector>


namespace utp::utils {

	struct FileResult {
		std::string outPath;
		nfdresult_t result;
	};

	class FileUtil {
	public:
		static FileResult openFileDialog(const std::vector<nfdu8filteritem_t> &filterList,
										 const std::string &defaultPath = std::filesystem::current_path());
	};
} // namespace utp::utils
