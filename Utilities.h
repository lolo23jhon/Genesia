#ifndef UTILITIES_H
#define UTILITIES_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
#include <SFML/Graphics/Text.hpp>


namespace utilities {
#if defined(WIN32) || defined(_WIN32) || defined (__WIN32) && !defined(__CYGWIN__)

#include <Windows.h>
#include <Shlwapi.h>

	////////////////////////////////////////////////////////////
	inline std::string getWorkingDirectory() {
		HMODULE hModule{ DrvGetModuleHandle(nullptr) };
		if (!hModule) { return ""; }
		char path[256];
		GetModuleFileName(hModule, path, sizeof(path));
		PathRemoveFileSpec(path);
		return std::move(std::string(path) + "\\");
	}

	////////////////////////////////////////////////////////////
	inline std::vector<std::pair<std::string, bool>> getFileList(const std::string& t_directory, const std::string& t_search = "*.*", bool t_directories = false) {
		std::vector<std::pair<std::string, bool>> files;
		if (t_search.empty()) { return std::move(files); }
		std::string path{ t_directory + t_search };
		WIN32_FIND_DATA data;
		HANDLE found{ FindFirstFile(path.c_str(), &data) };
		if (found == INVALID_HANDLE_VALUE) { return std::move(files); }
		do {
			if (!(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) || t_directories) {
				files.emplace_back(std::make_pair(std::string(data.cFileName),
					((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)));
			}
		} while (FindNextFile(found, &data));
		FindClose(found);
		return std::move(files);
	}

#endif // WINDOWS 

	const std::string EMPTY_STR{ "" };

	////////////////////////////////////////////////////////////
	inline void readQuotedString(std::stringstream& t_stream, std::string& t_string) {
		t_stream >> t_string;
		if (t_string.at(0) == '"') {
			while (t_string.at(t_string.length() - 1) != '"' || !t_stream.eof()) {
				std::string str;
				t_stream >> str;
				t_string.append(" " + str);
			}
		}
		t_string.erase(std::remove(t_string.begin(), t_string.end(), '"'), t_string.end());
	}


	////////////////////////////////////////////////////////////
	template <class T>
	inline T interpolate(float t_begin, float t_end, const T& t_begin_val, const T& t_end_val, float t_x) {
		return static_cast<T>((
			((t_end_val - t_begin_val) / (t_end - t_begin)) * (t_x - t_begin) + t_begin_val));
	}

	inline float getSFMLTextMaxHeight(const sf::Text& t_text) {
		auto charSize{ t_text.getCharacterSize() };
		auto font{ t_text.getFont() };
		auto string{ t_text.getString().toAnsiString() };
		bool bold{ (t_text.getStyle & sf::Text::Bold) };
		float max{ 0.f };
		for (size_t i{ 0 }; i < string.length; i++) {

			sf::Uint32 character{ string[i] };
			auto glyph{ font->getGlyph(character, charSize, bold) };
			auto height{ glyph.bounds.height };
			if (height <= max) { continue; }
		}
		return max;
	}

	inline void centerSFMLText(sf::Text& t_text) {
		sf::FloatRect rect{ t_text.getLocalBounds() };
		auto maxHeight{ getSFMLTextMaxHeight(t_text) };
		t_text.setOrigin(
			rect.left + (rect.width * 0.5f),
			rect.top + ((maxHeight >= rect.height ? maxHeight * 0.5f : rect.height * 0.5f)));

	}

	inline void sortFileList(std::vector<std::pair<std::string, bool >>& t_list) {
		std::sort(t_list.begin(), t_list.end(),
			[](std::pair<std::string, bool>& t_1, std::pair<std::string, bool>& t_2) {
				if (t_1.second && !t_2.second) { return true; }
				return false;
			}

		);
	}

	////////////////////////////////////////////////////////////
	inline bool readFile(const std::string& t_fileNameWithPath, std::stringstream& t_out_stream, bool t_printError = false) {
		std::ifstream file;
		file.open(t_fileNameWithPath);
		if (!file.is_open()) {
			if (t_printError) {
				std::cerr << "@ ERROR: Cannot open input file: \"" << t_fileNameWithPath << '\"' << std::endl;
			}
			return false;
		}
		t_out_stream.clear();
		t_out_stream << file.rdbuf();
		file.close();
		return true;		
	}


};
#endif // ! UTILITIES_H
