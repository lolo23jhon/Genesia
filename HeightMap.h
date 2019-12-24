#ifndef HEIGHT_MAP_H
#define HEIGHT_MAP_H

#include <memory>
#include <algorithm>
#include  <vector>

class ColorMap;

class HeightMap {
protected:
	unsigned m_width;
	unsigned m_height;
	std::vector<double> m_heights;

public:

	HeightMap(unsigned t_w, unsigned t_h, const std::vector<double>& t_values);
	HeightMap(unsigned t_w, unsigned t_h);
	bool compareDimensions(const HeightMap& t_hm) const;
	double mean()const;
	double standtDev()const;
	double getValue(unsigned t_x, unsigned t_y) const;
	double getMax()const;
	double getMin()const;
	void setValue(unsigned t_x, unsigned t_y, double t_n);
	HeightMap& sediment(const HeightMap& t_hm);
	HeightMap& merge(const HeightMap t_hm);
	HeightMap& increment(double t_n);
	HeightMap& multiply(double t_n);
	HeightMap& clamp(double t_min, double t_max);
	HeightMap& mapValuesToRange(double t_min, double t_max);
	std::pair<double, double> getRange() const;
	std::vector<double>::const_iterator cbegin() const;
	std::vector<double>::const_iterator cend() const;
	std::vector<double>::iterator begin();
	std::vector<double>::iterator end();
};

#endif 