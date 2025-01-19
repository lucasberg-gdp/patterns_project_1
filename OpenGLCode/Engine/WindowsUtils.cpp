#include "WindowsUtils.h"

#include <iostream>
#include <string>
#include <locale>
#include <codecvt>

std::string WindowsUtils::GetLastPartOfPath(const std::string& path) 
{
    size_t pos = path.find_last_of("/\\");

    if (pos != std::string::npos) 
    {
        return path.substr(pos + 1);
    }

    return path;
}

std::wstring WindowsUtils::StringToWString(const std::string& str)
{
    return std::wstring(str.begin(), str.end());
}

std::string WindowsUtils::WStringToString(const std::wstring& wstr)
{
    return std::string(wstr.begin(), wstr.end());
}

bool WindowsUtils::CreateDirectoryIfNotExist(const std::wstring& dir)
{
    return CreateDirectoryW(dir.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS;
}

bool WindowsUtils::CopyFileToDestination(const std::wstring& sourceFile, const std::wstring& destFile) 
{
    return CopyFileW(sourceFile.c_str(), destFile.c_str(), FALSE);
}

bool WindowsUtils::CreateFolderTree(const std::wstring& dir)
{

    return false;
}

std::wstring WindowsUtils::GetAbsolutePath(const std::wstring& relativePath)
{
    wchar_t fullPathBuffer[MAX_PATH];

    std::wstring fullPath;
    if (!GetFullPathName(relativePath.c_str(), MAX_PATH, fullPathBuffer, nullptr))
    {
        std::cerr << "Error getting full path: " << GetLastError() << std::endl;
    }

    return std::wstring(fullPath);
}

std::wstring WindowsUtils::GetAbsolutePath(const std::string& relativePath)
{
    wchar_t fullPathBuffer[MAX_PATH];

    if (!GetFullPathName(StringToWString(relativePath).c_str(), MAX_PATH, fullPathBuffer, nullptr))
    {
        std::cerr << "Error getting full path: " << GetLastError() << std::endl;
    }

    return std::wstring(fullPathBuffer);
}

bool WindowsUtils::GetAllFilesFromFolder(std::string path, std::vector<std::string>& fileNames)
{
    std::string searchPattern = path;
    searchPattern += "\\*";

    WIN32_FIND_DATAA fileData;
    HANDLE hFind = FindFirstFileA(searchPattern.c_str(), &fileData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) 
            {
                fileNames.push_back(fileData.cFileName);
            }
        } while (FindNextFileA(hFind, &fileData) != 0);
        FindClose(hFind);
    }
    else {
        //std::cerr << "Failed to open directory." << std::endl;
        return 1;
    }

    return true;
}

bool WindowsUtils::GetAllFoldersFromFolder(const std::string path, std::vector<std::string>& folderNames)
{
    std::string searchPattern = path;
    searchPattern += "\\*";

    WIN32_FIND_DATAA fileData;
    HANDLE hFind = FindFirstFileA(searchPattern.c_str(), &fileData);

    if (hFind == INVALID_HANDLE_VALUE) 
    {
        return false;
    }

    do {
        if (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            std::string folderName = fileData.cFileName;
            if (folderName != "." && folderName != "..") 
            {
                folderNames.push_back(folderName);
            }
        }
    } while (FindNextFileA(hFind, &fileData) != 0);

    FindClose(hFind);

    return true;
}

bool WindowsUtils::LoadPirateAssets(std::vector<std::string>& fileNames)
{
	const char* folderPath = "assets/models/PiratePack/Converted";

	if (!GetAllFilesFromFolder(folderPath, fileNames))
	{
		std::cout << "Pirates assets failed to load" << std::endl;
		return false;
	}

	return true;
}

bool WindowsUtils::CopyFolder(const std::wstring& sourceFolder, const std::wstring& destinationFolder)
{
    SHFILEOPSTRUCT fileOp = { 0 };
    wchar_t source[MAX_PATH];
    wchar_t destination[MAX_PATH];

    // Ensure paths are double-null terminated
    wcsncpy_s(source, sourceFolder.c_str(), MAX_PATH - 1);
    source[sourceFolder.length() + 1] = L'\0'; // Double-null termination

    wcsncpy_s(destination, destinationFolder.c_str(), MAX_PATH - 1);
    destination[destinationFolder.length() + 1] = L'\0'; // Double-null termination

    fileOp.wFunc = FO_COPY;            // Copy operation
    fileOp.pFrom = source;             // Source folder
    fileOp.pTo = destination;          // Destination folder
    fileOp.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT;

    int result = SHFileOperation(&fileOp);
    if (result != 0) {
        std::wcerr << L"Error copying folder. Error code: " << result << std::endl;
        return false;
    }

    return true;
}

bool WindowsUtils::CopyFolderContents(const std::wstring& sourceFolder, const std::wstring& destinationFolder)
{
    std::wstring sourcePath = sourceFolder;
    if (sourcePath.back() != L'\\') 
    {
        sourcePath += L'\\';
    }

    std::wstring destPath = destinationFolder;
    if (destPath.back() != L'\\') 
    {
        destPath += L'\\';
    }

    if (!CreateDirectoryIfNotExist(destPath))
    {
        std::wcerr << L"Error creating destination folder: " << GetLastError() << std::endl;
        return false;
    }

    std::wstring searchPath = sourcePath + L"*";
    WIN32_FIND_DATAW findFileData;
    HANDLE hFind = FindFirstFileW(searchPath.c_str(), &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) 
    {
        std::wcerr << L"Error: Unable to open source folder: " << GetLastError() << std::endl;
        return false;
    }

    bool allSuccessful = true;

    do {
        const std::wstring fileName = findFileData.cFileName;

        // Skip "." and ".." entries
        if (fileName != L"." && fileName != L"..") 
        {
            const std::wstring sourceFile = sourcePath + fileName;
            const std::wstring destFile = destPath + fileName;

            if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                if (!CopyFolderContents(sourceFile, destFile))
                {
                    std::wcerr << L"Error copying directory: " << sourceFile << std::endl;
                    allSuccessful = false;
                }
            }
            else
            {
                if (!CopyFileToDestination(sourceFile, destFile))
                {
                    std::wcerr << L"Error copying file " << sourceFile << L" to " << destFile
                        << L": " << GetLastError() << std::endl;
                    allSuccessful = false;
                }
                else 
                {
                    //std::wcout << L"Copied " << sourceFile << L" to " << destFile << std::endl;
                }
            }
        }
    } while (FindNextFileW(hFind, &findFileData) != 0);

    FindClose(hFind);

    return allSuccessful;
}

