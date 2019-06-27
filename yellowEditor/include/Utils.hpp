#ifndef __H_EUTILS__
#define __H_EUTILS__

#include <Windows.h>
#include <Commdlg.h>
#include <string>
#include <vector>

namespace yellowEngine
{
	// file dialog for Windows from nanogui
	std::string fileDialog(const std::vector<std::pair<std::string, std::string>> &filetypes, bool save)
	{
		static const int FILE_DIALOG_MAX_BUFFER = 16384;

		OPENFILENAMEW ofn;
		ZeroMemory(&ofn, sizeof(OPENFILENAMEW));
		ofn.lStructSize = sizeof(OPENFILENAMEW);
		wchar_t tmp[FILE_DIALOG_MAX_BUFFER];
		ofn.lpstrFile = tmp;
		ZeroMemory(tmp, sizeof(tmp));
		ofn.nMaxFile = FILE_DIALOG_MAX_BUFFER;
		ofn.nFilterIndex = 1;

		std::string filter;

		if (!save && filetypes.size() > 1)
		{
			filter.append("Supported file types (");
			for (size_t i = 0; i < filetypes.size(); ++i)
			{
				filter.append("*.");
				filter.append(filetypes[i].first);
				if (i + 1 < filetypes.size())
					filter.append(";");
			}
			filter.append(")");
			filter.push_back('\0');
			for (size_t i = 0; i < filetypes.size(); ++i)
			{
				filter.append("*.");
				filter.append(filetypes[i].first);
				if (i + 1 < filetypes.size())
					filter.append(";");
			}
			filter.push_back('\0');
		}
		for (auto pair : filetypes)
		{
			filter.append(pair.second);
			filter.append(" (*.");
			filter.append(pair.first);
			filter.append(")");
			filter.push_back('\0');
			filter.append("*.");
			filter.append(pair.first);
			filter.push_back('\0');
		}
		filter.push_back('\0');

		int size = MultiByteToWideChar(CP_UTF8, 0, &filter[0], (int)filter.size(), NULL, 0);
		std::wstring wfilter(size, 0);
		MultiByteToWideChar(CP_UTF8, 0, &filter[0], (int)filter.size(), &wfilter[0], size);

		ofn.lpstrFilter = wfilter.data();

		if (save)
		{
			ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
			if (GetSaveFileNameW(&ofn) == FALSE)
				return {};
		}
		else
		{
			ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			/*if (multiple)
				ofn.Flags |= OFN_ALLOWMULTISELECT;*/
			if (GetOpenFileNameW(&ofn) == FALSE)
				return {};
		}

		size_t i = 0;
		std::vector<std::string> result;
		while (tmp[i] != '\0')
		{
			std::string filename;
			int tmpSize = (int)wcslen(&tmp[i]);
			if (tmpSize > 0)
			{
				int filenameSize = WideCharToMultiByte(CP_UTF8, 0, &tmp[i], tmpSize, NULL, 0, NULL, NULL);
				filename.resize(filenameSize, 0);
				WideCharToMultiByte(CP_UTF8, 0, &tmp[i], tmpSize, &filename[0], filenameSize, NULL, NULL);
			}

			result.emplace_back(filename);
			i += tmpSize + 1;
		}

		if (result.size() > 1)
		{
			for (i = 1; i < result.size(); ++i)
			{
				result[i] = result[0] + "\\" + result[i];
			}
			result.erase(begin(result));
		}

		return result.empty() ? "" : result.front();
	}
}

#endif