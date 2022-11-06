#include "pretty.h"

#include <algorithm>
#include <sstream>
#include <iomanip>

std::string PrettyPrint(std::vector<uint64_t> hist) {
    if (hist.empty()) {
        return "";
    }

    std::reverse(hist.begin(), hist.end());
    while (!hist.empty() && hist.back() == 0) {
        hist.pop_back();
    }

    auto max = *std::max_element(hist.begin(), hist.end());

    std::vector<std::string> matrix(hist.size(), std::string(max, ' '));

    for (size_t i = 0; i < hist.size(); i++) {
        for (size_t j = 0; j < hist[i]; j++) {
            matrix[i][max - j - 1] = '_';
        }
    }

    std::stringstream result;
    result << "Max is: " << max << "\n";
    for (size_t i = 0; i < hist.size(); i++) {
        result << matrix[i] << "  " << hist[i] << "\n";
    }
    result << "\n";

    return result.str();
}
