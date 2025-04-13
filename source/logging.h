#ifndef LOGGING_H
#define LOGGING_H

#include <QObject>
#include <format>
#include <qtmetamacros.h>
#include <string>
#include <fstream>
#include <iostream>
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

    Q_SIGNAL
    void messageLogged(const std::string &message, LogLevel level);


private:
    std::string log_filepath;
    std::unique_ptr<std::ofstream> log_file;
    static std::weak_ptr<Logger> active_log;

    void logMessage(const std::string &msg, LogLevel level);
    static std::string getLevelDescription(LogLevel level);
    static std::string getTimestamp();
    
};

/// Shorthand function for logging formated debug message.
/// @param: msg Message string.
/// @param: Args Format arguments.
template <typename... Args>
void logDebug(const std::string &msg, Args&&... args)
{
    if (!Logger::active()) 
    {
        return;
    }

    try
    {
        std::string message = std::vformat(msg, std::make_format_args(args...));
        Logger::active()->debug(message);
    }
    catch (const std::format_error &err)
    {
        std::cerr << "Error logging message message: " << err.what() << std::endl;
    }
}

/// Shorthand function for logging formated info message.
/// @param: msg Message string.
/// @param: Args Format arguments.
template <typename... Args>
void logInfo(const std::string &msg, Args&&... args)
{
    if (!Logger::active()) 
    {
        return;
    }

    try
    {
        std::string message = std::vformat(msg, std::make_format_args(args...));
        Logger::active()->info(message);
    }
    catch (const std::format_error &err)
    {
        std::cerr << "Error logging info message: " << err.what() << std::endl;
    }
}

/// Shorthand function for logging formated warning message.
/// @param: msg Message string.
/// @param: Args Format arguments.
template <typename... Args>
void logWarning(const std::string &msg, Args&&... args)
{
    if (!Logger::active()) 
    {
        return;
    }

    try
    {
        std::string message = std::vformat(msg, std::make_format_args(args...));
        Logger::active()->warning(message);
    }
    catch (const std::format_error &err)
    {
        std::cerr << "Error logging warning message: " << err.what() << std::endl;
    }
}

/// Shorthand function for logging formated error message.
/// @param: msg Message string.
/// @param: Args Format arguments.
template <typename... Args>
void logError(const std::string &msg, Args&&... args)
{
    if (!Logger::active()) 
    {
        return;
    }

    try
    {
        std::string message = std::vformat(msg, std::make_format_args(args...));
        Logger::active()->error(message);
    }
    catch (const std::format_error &err)
    {
        std::cerr << "Error logging error message: " << err.what() << std::endl;
    }
}

#endif
