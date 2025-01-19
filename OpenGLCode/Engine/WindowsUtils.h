#pragma once

#ifdef APIENTRY
#undef APIENTRY
#endif
#include <windows.h>

#include <vector>
#include <string>

class WindowsUtils
{
	static std::string GetLastPartOfPath(const std::string& path);
	static std::wstring StringToWString(const std::string& str);
	static std::string WStringToString(const std::wstring& wstr);
	static bool CreateDirectoryIfNotExist(const std::wstring& dir);
	static bool CopyFileToDestination(const std::wstring& sourceFile, const std::wstring& destFile);
	static bool CreateFolderTree(const std::wstring& dir);

public:
	static std::wstring GetAbsolutePath(const std::wstring& relativePath);
	static std::wstring GetAbsolutePath(const std::string& relativePath);

	static bool GetAllFilesFromFolder(std::string path, std::vector< std::string>& fileNames);
	static bool GetAllFoldersFromFolder(std::string path, std::vector< std::string>& fileNames);
	static bool LoadPirateAssets(std::vector< std::string>& fileNames);

	static bool CopyFolder(const std::wstring& sourceFolder, const std::wstring& destinationFoldere);
	static bool CopyFolderContents(const std::wstring& sourceFolder, const std::wstring& destinationFolder);
};
