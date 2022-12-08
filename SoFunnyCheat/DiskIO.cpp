#include <DiskIO.h>
#include <filesystem>
#include <ShlObj.h>
#include <filesystem>
#include <ShlObj.h>
namespace DiskIO
{
	std::string SelectFile(const char* filter, const char* title)
	{
		auto currPath = std::filesystem::current_path();

		// common dialog box structure, setting all fields to 0 is important
		OPENFILENAME ofn = { 0 };
		TCHAR szFile[260] = { 0 };

		// Initialize remaining fields of OPENFILENAME structure
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.lpstrTitle = title;
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		std::string result = {};
		if (GetOpenFileName(&ofn) == TRUE)
			result = std::string(szFile);

		std::filesystem::current_path(currPath);
		return result;
	}
}