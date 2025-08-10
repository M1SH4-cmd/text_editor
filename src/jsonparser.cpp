#include "jsonparser.h"

using json = nlohmann::json;

JSONParser::JSONParser(){}

void JSONParser::saveSettings(const std::string& filename, CFG &cfg) {
    json j;

    // Создание полной структуры конфига
    j["config"] = {
        {"theme", cfg.theme},
        {"windowSize", {
            {"width", cfg.width},
            {"height", cfg.height},
            {"maximized", cfg.maximized}
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

bool JSONParser::loadSettings(const std::string& filename, CFG &cfg) {
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

        cfg.theme = config.value("theme", "Default");

        if (config.contains("windowSize")) {
            const auto& windowSize = config["windowSize"];
            cfg.width = windowSize.value("width", 800);
            cfg.height = windowSize.value("height", 600);
            cfg.maximized = windowSize.value("maximized", false);
        } else {
            cfg.width = 800;
            cfg.height = 600;
        }
    } else {
        // Старая версия конфига для обратной совместимости
        cfg.theme = j.value("theme", "Default");
        cfg.width = j.value("width", 800);
        cfg.height = j.value("height", 600);
    }

    return true;
}
