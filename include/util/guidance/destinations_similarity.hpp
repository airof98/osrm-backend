#ifndef OSRM_UTIL_GUIDANCE_DESTINATIONS_SIMILARITY_HPP_
#define OSRM_UTIL_GUIDANCE_DESTINATIONS_SIMILARITY_HPP_

#include <algorithm>
#include <set>
#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/function_output_iterator.hpp>

#include <iostream>

namespace osrm
{
namespace util
{
namespace guidance
{

template <typename T> auto getDestinationTokens(const T &destination)
{
    std::vector<std::string> splited;
    boost::split(splited, destination, boost::is_any_of(",:"));

    std::set<std::string> tokens;
    std::transform(
        splited.begin(), splited.end(), std::inserter(tokens, tokens.end()), [](auto token) {
            boost::trim(token);
            boost::to_lower(token);
            return token;
        });
    return tokens;
}

template <typename T> double getDestinationsSimilarity(const T &lhs, const T &rhs)
{
    const auto &lhs_tokens = getDestinationTokens(lhs);
    const auto &rhs_tokens = getDestinationTokens(rhs);
    if (lhs_tokens.size() + rhs_tokens.size() == 0)
        return 1.;

    std::size_t count = 0;
    auto counter = [&count](const std::string &) { ++count; };
    std::set_intersection(lhs_tokens.begin(),
                          lhs_tokens.end(),
                          rhs_tokens.begin(),
                          rhs_tokens.end(),
                          boost::make_function_output_iterator(std::ref(counter)));

    return static_cast<double>(count) / (lhs_tokens.size() + rhs_tokens.size() - count);
}

} // namespace guidance
} // namespace util
} // namespace osrm

#endif /* OSRM_UTIL_GUIDANCE_DESTINATIONS_SIMILARITY_HPP_ */
