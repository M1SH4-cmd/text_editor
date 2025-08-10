#include "jsonparser.h"

using json = nlohmann::json;

std::mutex JSONParser::mtx;

JSONParser& JSONParser::instance() {
    static JSONParser parser; // Гарантированно создаётся один раз
    return parser;
}

void JSONParser::saveSettings(const std::string& filename, CFG &cfg) {
    json j;
    std::lock_guard<std::mutex> lock(mtx);
    // Создание полной структуры конфига
    j["config"] = {
        {"theme", cfg.theme},
        {"windowSize", {
            {"width", cfg.width},
            {"height", cfg.height},
            {"maximized", cfg.maximized},
            {"sizeType", cfg.sizeType}
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
    std::lock_guard<std::mutex> lock(mtx);
    std::ifstream file(filename);
    if (!file.is_open()) {
        cfg = CFG();
        return false;
    };

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
            if (windowSize.contains("width")
            && windowSize.contains("height")
            && windowSize.contains("maximized")
            && windowSize.contains("sizeType")) {
                cfg.width = windowSize.value("width", 800);
                cfg.height = windowSize.value("height", 600);
                cfg.maximized = windowSize.value("maximized", false);
                cfg.sizeType = windowSize.value("sizeType", "Medium");
            }
        } else {
            cfg.width = 800;
            cfg.height = 600;
        }
    } else {
        // Старая версия конфига для обратной совместимости
        cfg.theme = j["config"].value("theme", "Default");
        cfg.width = j["config"]["windowSize"].value("width", 800);
        cfg.height = j["config"]["windowSize"].value("height", 600);
        cfg.maximized = j["config"]["windowSize"].value("maximized", false);
        cfg.sizeType = j["config"]["windowSize"].value("sizeType", "Medium");
    }

    return true;
}
