#include "file_io.h"


namespace fio {

	////////////////////////////////////////////////////////////
	bool fileExists(const std::string& t_fileName) {
		std::ifstream file{ t_fileName };
		return file.good();
	}


	////////////////////////////////////////////////////////////
	std::unique_ptr<std::list<std::string>> readFile(const std::string& t_fileName) {
		std::ifstream ifile{ t_fileName };
		if (!ifile) {
			std::cerr << "@ERROR: Cannot read file \"" << t_fileName << "\"." << std::endl;
			ifile.close();
			std::exit(1);
		}

		std::list<std::string> lines;
		std::string buffer;

		while (std::getline(ifile, buffer)) {
			lines.emplace_back(std::move(buffer));
		}
		ifile.close();
		return std::make_unique<std::list<std::string>>(std::move(lines));
	}

	////////////////////////////////////////////////////////////
	bool writeFile(const std::string& t_toPrint, const std::string& t_fileName) {
		std::string ext{ getExtension(t_fileName) };
		if (ext.empty()) {
			std::cerr << "@ERROR: Cannot write file \"" << t_fileName << "\"." << std::endl;
			return false;
		}

		std::string nameNoExt{ t_fileName.substr(0,t_fileName.size() - ext.length()-1) };
		if (fileExists(t_fileName)) {
			int fnum{ 1 };
			while (fileExists(nameNoExt + "_" + std::to_string(fnum) + "." + ext)) {
				fnum++;
			}
			nameNoExt.append("_" + std::to_string(fnum));
		}

		std::ofstream ofile{ nameNoExt + "." + ext };
		if (!ofile) {
			std::cerr << "@ERROR: Cannot write file \"" << t_fileName << "\"." << std::endl;
			ofile.close();
			return false;
		}

		ofile << t_toPrint;
		ofile.close();
		return true;
	}

	////////////////////////////////////////////////////////////
	std::string getExtension(const std::string& t_fileName) {
		size_t pos{ t_fileName.rfind('.') };
		if (pos == std::string::npos)
			return "\0";
		return t_fileName.substr(++pos, t_fileName.size());
	}

	////////////////////////////////////////////////////////////
	bool hasExtension(const std::string& t_fileName, const std::string& t_ext) {
		return getExtension(t_fileName) == t_ext;
	}


	////////////////////////////////////////////////////////////
	Pixel::Pixel(unsigned t_r, unsigned t_g, unsigned t_b) :
		m_r{ t_r > 255 ? 255 : t_r }, m_g{ t_g > 255 ? 255 : t_g }, m_b{ t_b > 255 ? 255 : t_b }{}



	////////////////////////////////////////////////////////////
	Pixel::Pixel(double t_grayScale) :
		Pixel((unsigned)t_grayScale, (unsigned)t_grayScale, (unsigned)t_grayScale) {}


	////////////////////////////////////////////////////////////
	double Pixel::toGrayscale() {
		return (m_r * 0.299) + (m_g * 0.587) + (m_b * 0.114);
	}


	////////////////////////////////////////////////////////////
	void writePPM(unsigned t_w, unsigned t_h, const ImgData& t_imgData, const std::string& t_fileName) {

		std::string ext{ getExtension(t_fileName) };
		if (ext != "ppm") {
			std::cerr << "@ERROR: Invalid extension \"" << ext << "\". Use \".ppm\"." << std::endl;
			return;
		}
		std::string nameNoExt{ t_fileName.substr(0,t_fileName.size() - 4) };

		if (fileExists(nameNoExt + "." + ext)) {
			int fnum{ 1 };
			while (fileExists(nameNoExt + "_" + std::to_string(fnum) + "." + ext)) {
				fnum++;
			}
			nameNoExt.append("_" + std::to_string(fnum));
		}

		std::ofstream img{ nameNoExt + "." + ext };

		if (!img) {
			std::cerr << "@ERROR: Cannot write image file \"" << nameNoExt + ext << "\"." << std::endl;
		}

		img << "P3" << std::endl
			<< t_w << " " << t_h << std::endl
			<< "255" << std::endl;

		for (unsigned y{ 0 }; y < t_h; y++) {
			for (unsigned x{ 0 }; x < t_w; x++) {
				const auto& p{ t_imgData[y * t_w + x] };
				img << p.m_r << ' ' << p.m_g << ' ' << p.m_b << std::endl;
			}
		}
		img.close();
	}


#if defined(WIN32) || defined(_WIN32) || defined (__WIN32) && !defined(__CYGWIN__) 
	////////////////////////////////////////////////////////////
	std::vector<std::string> getFileNamesInFolder(const std::string& t_folder)
	{
		std::vector<std::string> names;
		std::string search_path{ t_folder + "/*.*" };
		WIN32_FIND_DATA fd;
		HANDLE hFind{ FindFirstFile(search_path.c_str(), &fd) };
		if (hFind != INVALID_HANDLE_VALUE) {
			do {
				if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
					names.emplace_back(fd.cFileName);
				}
			} while (FindNextFile(hFind, &fd));
			FindClose(hFind);
		}
		return std::move(names);
	}
#endif


	////////////////////////////////////////////////////////////
	PPMimage::PPMimage() {}


	////////////////////////////////////////////////////////////
	PPMimage::~PPMimage() { delete[] m_threeChan; }


	////////////////////////////////////////////////////////////
	void PPMimage::saveToFile(const char* t_fileName) {
		std::vector <Pixel> imgData;
		imgData.reserve(m_width * m_height);
		for (unsigned i{ 0 }; i < m_width * m_height; i += 3) {
			imgData.emplace_back(m_threeChan[i], m_threeChan[i + 1], m_threeChan[i + 2]);

		}
		writePPM(m_width, m_height, imgData, t_fileName);
	}

	////////////////////////////////////////////////////////////
	void PPMimage::grayscale() {
		char* init;
		char* reset;

		unsigned char old_r{ 0 };
		unsigned char old_g{ 0 };
		unsigned char old_b{ 0 };
		unsigned char new_r{ 0 };
		unsigned char new_g{ 0 };
		unsigned char new_b{ 0 };

		reset = m_threeChan;

		for (unsigned i{ 0 }; i < m_width * m_height; i++) {
			init = m_threeChan;

			old_r = *m_threeChan;
			m_threeChan++;
			old_g = *m_threeChan;
			m_threeChan++;
			old_b = *m_threeChan;
			m_threeChan++;

			new_r = (unsigned)((old_r * 0.299) + (old_g * 0.587) + (old_b * 0.114));
			new_g = (unsigned)((old_r * 0.299) + (old_g * 0.587) + (old_b * 0.114));
			new_b = (unsigned)((old_r * 0.299) + (old_g * 0.587) + (old_b * 0.114));

			m_threeChan = init;
			*m_threeChan = new_r;
			m_threeChan++;
			*m_threeChan = new_g;
			m_threeChan++;
			*m_threeChan = new_b;
			m_threeChan++;
		}

		m_threeChan = reset;
	}



	////////////////////////////////////////////////////////////
	std::istream& operator>>(std::ifstream& t_is, PPMimage& t_img)
	{
		std::string fileName;
		char dump;

		t_is >> t_img.m_magicNumber;

		std::string buff;
		t_is >> buff;
		t_img.m_width = std::stoi(buff);

		t_is >> buff;
		t_img.m_height = std::stoi(buff);

		t_is >> buff;
		t_img.m_maxColorVal = std::stoi(buff);

		unsigned memBlockSize{ t_img.m_width * t_img.m_height * 3 };
		t_img.m_threeChan = new char[memBlockSize];

		t_is.read(&dump, 1);
		t_is.read(t_img.m_threeChan, memBlockSize);

		return t_is;
	}

	////////////////////////////////////////////////////////////
	std::ostream& operator<<(std::ofstream& t_os, const PPMimage& t_img) {
		unsigned memBlockSize{ t_img.m_width * t_img.m_height * 3 };
		t_os << t_img.m_magicNumber << '\n'
			<< t_img.m_width << ' ' << t_img.m_height << '\n'
			<< t_img.m_maxColorVal << '\n';
		t_os.write(t_img.m_threeChan, memBlockSize);

		return t_os;
	}

}; // namespace fio
