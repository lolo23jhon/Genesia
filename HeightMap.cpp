#include "HeightMap.h"



////////////////////////////////////////////////////////////
HeightMap::HeightMap(unsigned t_w, unsigned t_h, const std::vector<double>& t_values) :
	m_height{ t_h }, m_width{ t_w }, m_heights{ t_values }{
	if (m_heights.size() > t_w* t_h) {
		while (m_heights.size() > t_w* t_h)
			m_heights.pop_back();
	}
	else if (m_heights.size() < t_w * t_h) {
		unsigned delta{ t_w * t_h - (unsigned)m_heights.size() };
		m_heights.reserve(t_w * t_h);
		for (unsigned i{ 0 }; i < delta; i++)
			m_heights.emplace_back(0.0);
	}
}


////////////////////////////////////////////////////////////
HeightMap::HeightMap(unsigned t_w, unsigned t_h) :
	m_height{ t_h }, m_width{ t_w }, m_heights((size_t)(t_w* t_h), 0.0){}


////////////////////////////////////////////////////////////
bool HeightMap::compareDimensions(const HeightMap& t_hm) const {
	return m_width == t_hm.m_width && m_height == t_hm.m_height;
}


////////////////////////////////////////////////////////////
double HeightMap::mean() const {
	double sum{ 0 };
	for (const auto& val : m_heights) {
		sum += val;
	}
	return sum / (m_width * m_height);
}

////////////////////////////////////////////////////////////
double HeightMap::standtDev() const {
	double ave{ mean() };
	double sum{ 0 };
	for (const auto& val : m_heights) {
		sum += (val - ave) * (val - ave);
	}
	return sqrt(sum / (m_width * m_height));
}


////////////////////////////////////////////////////////////
double HeightMap::getValue(unsigned t_x, unsigned t_y) const {
	return m_heights[t_y * m_width + t_x];
}


////////////////////////////////////////////////////////////
void HeightMap::setValue(unsigned t_x, unsigned t_y, double t_n) {
	m_heights[t_y * m_width + t_x] = t_n;
}


////////////////////////////////////////////////////////////
HeightMap& HeightMap::sediment(const HeightMap& t_hm) {
	if (compareDimensions(t_hm)) {
		unsigned size{ (unsigned)m_heights.size() };
		for (unsigned i{ 0 }; i < size; i++) {
			m_heights[i] += t_hm.m_heights[i];
		}
	}
	return *this;
}


////////////////////////////////////////////////////////////
HeightMap& HeightMap::merge(const HeightMap t_hm) {
	if (compareDimensions(t_hm)) {
		unsigned size{ (unsigned)m_heights.size() };
		for (unsigned i{ 0 }; i < size; i++) {
			m_heights[i] = std::max(m_heights[i], t_hm.m_heights[i]);
		}
	}
	return *this;
}


////////////////////////////////////////////////////////////
HeightMap& HeightMap::increment(double t_n) {
	for (auto& val : m_heights) {
		val += t_n;
	}
	return *this;
}


////////////////////////////////////////////////////////////
double HeightMap::getMax()const {
	double max{ -INFINITY };
	for (const auto& val : m_heights) {
		if (val > max) {
			max = val;
		}
	}
	return max;
}


////////////////////////////////////////////////////////////
double HeightMap::getMin()const {
	double min{ INFINITY };
	for (const auto& val : m_heights) {
		if (val < min) {
			min = val;
		}
	}
	return min;
}

////////////////////////////////////////////////////////////
std::pair<double, double> HeightMap::getRange() const {
	double min{ INFINITY };
	double max{ -INFINITY };

	for (const auto& val : m_heights) {
		if (val < min) {
			min = val;
		}
		else if (val > max) {
			max = val;
		}
	}
	return  std::move(std::make_pair(min, max));
}


////////////////////////////////////////////////////////////
HeightMap& HeightMap::multiply(double t_n) {
	for (auto& val : m_heights) {
		val *= t_n;
	}
	return *this;
}


////////////////////////////////////////////////////////////
HeightMap& HeightMap::clamp(double t_min, double t_max) {
	double min{ std::min(t_min, t_max) };
	double max{ std::max(t_min, t_max) };
	for (auto& val : m_heights) {
		if (val < min) {
			val = min;
		}
		else if (val > max) {
			val = max;
		}
	}
	return *this;
}


////////////////////////////////////////////////////////////
HeightMap& HeightMap::mapValuesToRange(double t_min, double t_max) {
	auto range{ getRange() };
	for (auto& val : m_heights) {
		val = t_min + (val - range.first) * (t_max - t_min) / (range.second - range.first);
	}
	return *this;
}


////////////////////////////////////////////////////////////
std::vector<double>::const_iterator HeightMap::cbegin() const {
	return m_heights.cbegin();
}


////////////////////////////////////////////////////////////
std::vector<double>::const_iterator HeightMap::cend() const {
	return m_heights.cend();
}


////////////////////////////////////////////////////////////
std::vector<double>::iterator HeightMap::begin() {
	return m_heights.begin();
}

////////////////////////////////////////////////////////////
std::vector<double>::iterator HeightMap::end() {
	return m_heights.end();
}