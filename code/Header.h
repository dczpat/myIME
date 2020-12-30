#pragma once
#include <iostream>
#include <Windows.h>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <locale>
#include <algorithm>
#include <regex>
#include <io.h>
#include <direct.h>
#include <filesystem>
#include <sstream>
#include <cstdlib>
#include <set>

using namespace std;
namespace fs=std::filesystem;

#define ACCESS(fileName,accessMode) _access(fileName,accessMode)
#define MKDIR(path) _mkdir(path)
