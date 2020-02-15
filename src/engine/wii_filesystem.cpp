#include <fat.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "core.h"
#include "Filesystem.h"

#ifndef ACCESSPERMS
#define ACCESSPERMS (S_IRWXU|S_IRWXG|S_IRWXO)
#endif

void io::Init()
{
    ASSERT(fatInitDefault());
}

bool io::CreateDirectory(const std::string& directoryPath)
{
	if (!directoryPath.empty())
	{
		return (mkdir(directoryPath.c_str(), ACCESSPERMS) == 0);
	}

	return false;
}

bool io::DirectoryExist(const std::string& directoryPath)
{
	struct stat sb;
	return stat(directoryPath.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode);
}

bool io::FileExist(const std::string& filePath)
{
	std::ifstream fstream;
	fstream.open(filePath);
	return fstream.is_open();
}

void io::Write(const std::string& file, const char* data, size_t size)
{
	std::ofstream stream(file);
	stream.write(data, size);
	stream.close();
}

int io::RemoveDirectory(const std::string& directoryPath)
{
	return RemoveDirectory(directoryPath.c_str());
}

int io::RemoveDirectory(const char* directoryPath)
{
	DIR* pdir = opendir(directoryPath);
	if (pdir != nullptr)
	{
		while (true)
		{
			struct dirent* pent = readdir(pdir);
			if (pent == nullptr)
				break;

			if (strcmp(".", pent->d_name) != 0 && strcmp("..", pent->d_name) != 0)
			{
				char dnbuf[260];
				sprintf(dnbuf, "%s/%s", directoryPath, pent->d_name);

				struct stat statbuf;
				stat(dnbuf, &statbuf);

				if (S_ISDIR(statbuf.st_mode))
				{					
					RemoveDirectory(dnbuf);
				}
				else
				{					
					RemoveFile(dnbuf);
				}
			}
		}
		closedir(pdir);
	}
	return rmdir(directoryPath);
}

int io::RemoveFile(const std::string& filePath)
{
	return RemoveFile(filePath.c_str());
}

int io::RemoveFile(const char* filePath)
{
	return unlink(filePath);
}
