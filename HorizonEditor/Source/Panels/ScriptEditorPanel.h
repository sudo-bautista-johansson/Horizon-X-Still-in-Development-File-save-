#pragma once

#include "Horizon/Core/Base.h"
#include <string>
#include <vector>
#include <filesystem>

namespace Horizon {

class ScriptEditorPanel
{
public:
	ScriptEditorPanel();

	void OpenFile(const std::filesystem::path& path);
	void OnImGuiRender();
	void Save();

	void AutoSave();
	void MarkDirty();
	void Close();
	void Reload();
	void NewFile();
	void Rename(const std::filesystem::path& newPath);
	void Delete();
	void Duplicate();
	void Move(const std::filesystem::path& newDirectory);
	void CopyToClipboard();
	void PasteFromClipboard();

private:
	std::filesystem::path m_OpenPath;
	std::string m_Buffer;
	std::filesystem::path m_UnsavedPath;
	bool m_IsOpen = false;
	bool m_Dirty = false;
	double m_LastAutoSaveTime = 0.0;
};

} // namespace Horizon
