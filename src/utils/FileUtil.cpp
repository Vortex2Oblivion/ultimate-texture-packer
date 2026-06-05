#include "FileUtil.hpp"

#include <iostream>

namespace utp::utils {

	std::filesystem::path FileUtil::lastPath = std::filesystem::current_path();

	FileResult FileUtil::openFileDialog(const std::vector<nfdu8filteritem_t> &filterList, const std::filesystem::path& defaultPath) {
		const nfdopendialogu8args_t args = {
				.filterList = filterList.data(),
				.filterCount = static_cast<nfdfiltersize_t>(filterList.size()),
				#ifndef _WIN32 // windows just sucks i guess???
				.defaultPath = defaultPath.c_str(),
				#endif
		};


		nfdu8char_t *outPath = nullptr;

		const auto result = NFD_OpenDialogU8_With(&outPath, &args);
		FileResult ret{};

		if (result == NFD_OKAY) {
			lastPath = std::filesystem::path(outPath).parent_path();
			ret = FileResult{.outPath = outPath, .result = result};
			NFD_FreePathU8(outPath);
		}

		return ret;
	}
} // namespace utp::utils
