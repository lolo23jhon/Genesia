#ifndef UNORDERED_PAIR_HASH_HPP
#define UNORDERED_PAIR_HASH_HPP

#include <functional>
#include <utility>
#include <unordered_set>

template<typename T>
static void hash_combine(std::size_t& seed, T const& key) {
    std::hash<T> hasher;
    seed ^= hasher(key) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

namespace std {
    template<typename T1, typename T2>
    struct hash<std::pair<T1, T2>> {
        std::size_t operator()(std::pair<T1, T2> const& p) const {
            std::size_t seed1(0);
            ::hash_combine(seed1, p.first);
            ::hash_combine(seed1, p.second);

            std::size_t seed2(0);
            ::hash_combine(seed2, p.second);
            ::hash_combine(seed2, p.first);

            return std::min(seed1, seed2);
        }
    };
}

namespace utilities {

    struct UnorderedEqual {

        template <class T1, class T2>
        bool operator() (const std::pair<T1, T2>& lhs, const std::pair<T1, T2>& rhs) const
        {
            return lhs == rhs || (
                lhs.second == rhs.first && rhs.second == lhs.first
                );
        }
    };
}

#endif // !UNORDERED_PAIR_HASH_HPP