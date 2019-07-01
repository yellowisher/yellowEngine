#ifndef __H_EDITORUTILS__
#define __H_EDITORUTILS__

#include <string>
#include <vector>

namespace yellowEditor
{
	// file dialog for Windows from nanogui
	std::string fileDialog(const std::vector<std::pair<std::string, std::string>> &filetypes, bool save);
	void createDirectory(std::string name);
}

#endif