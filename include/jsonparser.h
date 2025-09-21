#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <nlohmann\json.hpp>
#include <fstream>
#include <map>
#include <mutex>

#include <QMessageBox>
#include <QPushButton>

using json = nlohmann::json;

struct CFG {
    CFG(){};

    std::string theme;
    int width;
    int height;
    bool maximized;
    std::string sizeType;
    std::string language;
};

class JSONParser {

public:

    // Singleton pattern
    static JSONParser& instance();

    void saveSettings(const std::string& filename, CFG &cfg);
    bool loadSettings(const std::string& filename, CFG &cfg);

    // Запрет на копирование и присваивание
    JSONParser(const JSONParser&) = delete;
    JSONParser& operator=(const JSONParser&) = delete;


private:

    static std::mutex mtx;

    JSONParser() = default; // Приватный конструктор
    QString size;
    std::map<std::string, std::string> recentFiles; // 1 - дата последнего изменения, 2 - путь к файлу
};

#endif // JSONPARSER_H
