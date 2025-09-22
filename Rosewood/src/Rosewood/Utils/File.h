#pragma once

#include "rwpch.h"
#include <shobjidl.h> 

namespace Rosewood
{
	namespace Utils
	{
		// TODO : platform independant interface to open file explorer 
		// Snippet from xCENTx at https://stackoverflow.com/questions/68601080/how-do-you-open-a-file-explorer-dialogue-in-c
		static bool OpenFileExplorer(std::string& out_FilePath, std::string& out_FileName, std::string& out_FileExtension)
		{
			//  CREATE FILE OBJECT INSTANCE
			HRESULT f_SysHr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
			if (FAILED(f_SysHr))
				return FALSE;

			// CREATE FileOpenDialog OBJECT
			IFileOpenDialog* f_FileSystem;
			f_SysHr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&f_FileSystem));
			if (FAILED(f_SysHr)) {
				CoUninitialize();
				return FALSE;
			}

			COMDLG_FILTERSPEC rgSpec[] =
			{
				{ L"Any", L"*.obj" },
				{ L"Obj", L"*.obj" },
			};
			f_SysHr = f_FileSystem->SetFileTypes(2, rgSpec);
			if (FAILED(f_SysHr)) {
				f_FileSystem->Release();
				CoUninitialize();
				return FALSE;
			}

			//  SHOW OPEN FILE DIALOG WINDOW
			f_SysHr = f_FileSystem->Show(NULL);
			if (FAILED(f_SysHr)) {
				f_FileSystem->Release();
				CoUninitialize();
				return FALSE;
			}

			//  RETRIEVE FILE NAME FROM THE SELECTED ITEM
			IShellItem* f_Files;
			f_SysHr = f_FileSystem->GetResult(&f_Files);
			if (FAILED(f_SysHr)) {
				f_FileSystem->Release();
				CoUninitialize();
				return FALSE;
			}

			//  STORE AND CONVERT THE FILE NAME
			PWSTR f_Path;
			f_SysHr = f_Files->GetDisplayName(SIGDN_FILESYSPATH, &f_Path);
			if (FAILED(f_SysHr)) {
				f_Files->Release();
				f_FileSystem->Release();
				CoUninitialize();
				return FALSE;
			}

			//  FORMAT AND STORE THE FILE PATH
			std::wstring path(f_Path);
			std::string c(path.length(), 0);
			std::transform(path.begin(), path.end(), c.begin(), [](wchar_t c) {
				return (char)c;
				});
			out_FilePath = c;

			//  FORMAT STRING FOR EXECUTABLE NAME AND FORMAT
			const size_t slash = out_FilePath.find_last_of("/\\");
			const size_t dot = out_FilePath.find_last_of(".");
			out_FileName = out_FilePath.substr(slash + 1, dot - (slash + 1));
			out_FileExtension = out_FilePath.substr(dot + 1);

			//  SUCCESS, CLEAN UP
			CoTaskMemFree(f_Path);
			f_Files->Release();
			f_FileSystem->Release();
			CoUninitialize();
			return TRUE;
		}

		static std::string ReadFile(const std::string& filepath)
		{
			std::string source;
			std::ifstream file;
			// ensure ifstream objects can throw exceptions:
			file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			try
			{
				// open files
				file.open(filepath);
				std::stringstream stream;
				// read file’s buffer contents into streams
				stream << file.rdbuf();
				// close file handlers
				file.close();
				// convert stream into string
				source = stream.str();
			}
			catch (std::ifstream::failure const&)
			{
				throw;
			}

			return source;
		}
	}
}