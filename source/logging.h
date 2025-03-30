#ifndef LOGGING_H
#define LOGGING_H

#include <QObject>
#include <qtmetamacros.h>
#include <string>
#include <fstream>
#include <memory>

enum LogLevel
{
    Debug,
    Info,
    Warning,
    Error
};

class Logger : public QObject
{
	Q_OBJECT

public:
    Logger(const std::string &filepath);
    ~Logger();

    void initLogFile();
    void debug(const std::string &msg);
    void info(const std::string &msg);
    void warning(const std::string &msg);
    void error(const std::string &msg);

    static Logger* active();
    static void setActive(std::weak_ptr<Logger> logger);

signals:
	void messageLogged(const std::string &message, LogLevel level);


private:
    std::string log_filepath;
    std::unique_ptr<std::ofstream> log_file;
    static std::weak_ptr<Logger> active_log;

    void logMessage(const std::string &msg, LogLevel level);
    static std::string getLevelDescription(LogLevel level);
    static std::string getTimestamp();
    
};

#endif
