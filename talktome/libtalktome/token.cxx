#include "token.hxx"

#include <algorithm>
#include <cctype>

// By Parham http://stackoverflow.com/a/28788127/2948122
void tokenize(std::string str, std::vector<std::pair<std::string, int>> &token_v, std::string delimiter){
    size_t start = str.find_first_not_of(delimiter), end=start;

    while (start != std::string::npos){
        // Find next occurence of delimiter
        end = str.find_first_of(delimiter, start);
        // Push back the token found into vector
        token_v.push_back(std::pair<std::string, int>(str.substr(start, end-start), start));
        // Skip all occurences of the delimiter to find new start
        start = str.find_first_not_of(delimiter, end);
    }
}
