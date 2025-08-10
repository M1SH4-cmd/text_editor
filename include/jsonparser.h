#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <nlohmann\json.hpp>
#include <fstream>
#include <map>

#include <QMessageBox>
#include <QPushButton>

using json = nlohmann::json;

class JSONParser {

public:
    JSONParser();

    void saveSettings(const std::string& filename, const std::string& theme, int width, int height);
    bool loadSettings(const std::string& filename, std::string& theme, int& width, int& height);

private:
    QString size;
    std::map<std::string, std::string> recentFiles; // 1 - дата последнего изменения, 2 - путь к файлу
};


#endif // JSONPARSER_H
