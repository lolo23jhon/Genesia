#ifndef FILE_IO_H
#define FILE_IO_H

#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <memory>
#include <string>
#if defined(WIN32) || defined(_WIN32) || defined (__WIN32) && !defined(__CYGWIN__)
#define NOMINMAX
#include <Windows.h>
#endif



namespace fio {

	bool fileExists(const std::string& t_fileName);

	std::unique_ptr<std::list<std::string>> readFile(const std::string& t_fileName);

	bool writeFile(const std::string& t_toPrint, const std::string& t_fileName);

	bool hasExtension(const std::string& t_fileName, const std::string& t_ext);

	std::string getExtension(const std::string& t_fileName);

#if defined(WIN32) || defined(_WIN32) || defined (__WIN32) && !defined(__CYGWIN__) 
	std::vector<std::string> getFileNamesInFolder(const std::string& t_folder);
#endif

	struct Pixel {
		unsigned m_r;
		unsigned m_g;
		unsigned m_b;
		Pixel(unsigned t_r, unsigned t_g, unsigned t_b);
		Pixel(double t_grayScale);
		double toGrayscale();

	};
	typedef std::vector<Pixel> ImgData;

	void writePPM(unsigned t_w, unsigned t_h, const ImgData& t_imgData, const std::string& t_fileName = "img.ppm");

	class PPMimage {


		char* m_threeChan;

	public:
		std::string m_magicNumber;
		unsigned m_width;
		unsigned m_height;
		unsigned m_maxColorVal;


		PPMimage();
		~PPMimage();
		void saveToFile(const char* t_fileName);
		void grayscale();
		friend std::istream& operator >>(std::ifstream& t_is, PPMimage& t_img);
		friend std::ostream& operator <<(std::ofstream& t_os, const PPMimage& t_img);

	};



}; // namespace fio




#endif