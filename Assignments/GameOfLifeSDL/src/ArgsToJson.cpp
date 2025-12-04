#include "ArgsToJson.hpp"
#include <string>

using nlohmann::json;

static bool isNumber(const std::string &s)
{
    if (s.empty()) return false;
    char *end = nullptr;
    double val = std::strtod(s.c_str(), &end);
    (void)val;
    return end != s.c_str() && *end == '\0';
}

json ArgsToJson(int argc, char *argv[])
{
    json result = json::object();

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        auto pos = arg.find('=');
        if (pos == std::string::npos) continue;

        std::string key = arg.substr(0, pos);
        std::string value = arg.substr(pos + 1);

        if (isNumber(value)) {
            if (value.find('.') != std::string::npos) {
                result[key] = std::stod(value);
            } else {
                result[key] = std::stoi(value);
            }
        } else {
            result[key] = value;
        }
    }

    return result;
}

