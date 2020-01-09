#ifndef UNORDERED_BIMAP_H
#define UNORDERED_BIMAP_H

// WIP... DANGEROUS!

#include <unordered_map>

namespace diy {

	template <class T1, class T2>
	class unordered_bimap {

		std::unordered_map <T1, T2*> m_map_ab;
		std::unordered_map <T2, T1*> m_map_ba;

	public:
		const std::iterator find(const T1& t_key)const;

	};
}

#endif // !UNORDERED_BIMAP_H