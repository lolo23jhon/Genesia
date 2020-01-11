#ifndef RANDOM_GENERATOR_H
#define RANDOM_GENERATOR_H

#include <random>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Lock.hpp>

class RandomGenerator {

	std::random_device m_device;
	std::mt19937 m_engine;
	std::normal_distribution<float> m_normalDistribution;
	std::uniform_int_distribution<int> m_intDistribution;
	std::uniform_real_distribution<float> m_floatDistribution;
	sf::Mutex m_mutex;

public:
	////////////////////////////////////////////////////////////
	RandomGenerator() : m_engine(m_device()) {}


	////////////////////////////////////////////////////////////
	float normalDisttribution(const float& t_mean, const float& t_stdDev) {
		sf::Lock lock{ m_mutex };
		if (t_mean != m_normalDistribution.mean() || t_stdDev != m_normalDistribution.stddev()) {
			m_normalDistribution = std::normal_distribution<float>(t_mean,t_stdDev);
		}
		return m_normalDistribution(m_engine);
	}

	////////////////////////////////////////////////////////////
	int generate(int t_min, int t_max) {
		sf::Lock lock{ m_mutex };

		if (t_min > t_max) { std::swap(t_min, t_max); };

		if (t_min != m_intDistribution.min() || t_max != m_intDistribution.max()) {
			m_intDistribution = std::uniform_int_distribution<int>(t_min, t_max);
		}

		return m_intDistribution(m_engine);
	}

	////////////////////////////////////////////////////////////
	int operator()(int t_min, int t_max) { return generate(t_min, t_max); }



	////////////////////////////////////////////////////////////
	float generate(float t_min, float t_max) {
		sf::Lock lock{ m_mutex };

		if (t_min > t_max) { std::swap(t_min, t_max); };

		if (t_min != m_floatDistribution.min() || t_max != m_floatDistribution.max()) {
			m_floatDistribution = std::uniform_real_distribution<float>(t_min, t_max);
		}

		return m_floatDistribution(m_engine);
	}


	////////////////////////////////////////////////////////////
	float operator()(float t_min, float t_max) { return generate(t_min, t_max); }

};

#endif // !RANDOM_GENERATOR_H