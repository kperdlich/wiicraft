#pragma once

#include <string>

namespace io {

void Init();
bool CreateDirectory(const std::string& directoryPath);
bool DirectoryExist(const std::string& directoryPath);
bool FileExist(const std::string& filePath);
void Write(const std::string& file, const char* data, size_t size);
int RemoveDirectory(const std::string& directoryPath);
int RemoveDirectory(const char* directoryPath);
int RemoveFile(const std::string& filePath);
int RemoveFile(const char* filePath);

}
