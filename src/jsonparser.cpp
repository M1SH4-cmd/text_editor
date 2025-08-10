#include "jsonparser.h"

using json = nlohmann::json;

JSONParser::JSONParser(){}

void JSONParser::saveSettings(const std::string& filename, const std::string& theme, int width, int height) {
    json j;

    // Создание полной структуры конфига
    j["config"] = {
        {"theme", theme},
        {"windowSize", {
            {"width", width},
            {"height", height},
            {"maximized", false}
        }},
        {"language", "en"},
        {"additionalOptions", {
            {"autoSave", true},
            {"showLineNumbers", true},
            {"showWorkCount", true}
        }}
    };

    j["filesCfg"] = {
        {"recentFiles", json::array()}
    };

    std::ofstream file(filename);
    if (file.is_open()) {
        file << j.dump(4);
        file.close();
    } else {
        throw std::runtime_error("Cannot open config file for writing");
    }
}

bool JSONParser::loadSettings(const std::string& filename, std::string& theme, int& width, int& height) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    json j;
    try {
        file >> j;
    } catch (const std::exception& e) {
        qDebug() << "JSON parse error:" << e.what();
        return false;
    }
    file.close();

    if (j.contains("config")) {
        const auto& config = j["config"];

        theme = config.value("theme", "Default");

        if (config.contains("windowSize")) {
            const auto& windowSize = config["windowSize"];
            width = windowSize.value("width", 800);
            height = windowSize.value("height", 600);
        } else {
            width = 800;
            height = 600;
        }
    } else {
        // Старая версия конфига для обратной совместимости
        theme = j.value("theme", "Default");
        width = j.value("width", 800);
        height = j.value("height", 600);
    }

    return true;
}
