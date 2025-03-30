#include "logging.h"
#include <iostream>
#include <sstream>
#include <format>
#include <chrono>

std::weak_ptr<Logger> Logger::active_log;

/// Default constructor.
Logger::Logger(const std::string &filepath)
{
    this->log_filepath = filepath;
}

/// Default destructor.
Logger::~Logger()
{
    if (this->log_file && this->log_file->is_open())
    {
        this->log_file->close();
        this->log_file.reset();
    }
}

/// Get currently active global logger object.
Logger* Logger::active()
{
    if (Logger::active_log.expired())
    {
        throw std::runtime_error("Attempting to fetch active log when no log is bound");
        return nullptr;
    }

    return Logger::active_log.lock().get();
}

/// Bind new logger object which can be accessed globally via Logger class itself.
void Logger::setActive(std::weak_ptr<Logger> logger)
{
    Logger::active_log = logger;
}

/// Opens log file on disk associated with this logger object.
void Logger::initLogFile()
{
    if (this->log_file)
    {
        return;
    }

    this->log_file = std::make_unique<std::ofstream>(this->log_filepath, std::ios::out);
    if (!this->log_file || !this->log_file->is_open())
    {
        std::string err_msg = std::format("Failed to initialise log file -> {}", this->log_filepath);
        throw std::runtime_error(err_msg);
    }

    this->logMessage("Log file opened", LogLevel::Info);
}

/// Add new message to this logger.
/// @param msg New message to log.
/// @param level Message severity level.
void Logger::logMessage(const std::string &msg, LogLevel level)
{
    if (!this->log_file)
    {
        throw std::runtime_error("Attempting to log a message while the logger is uninitialized!");
    }

    std::ostringstream message_stream;
    std::string level_desc = this->getLevelDescription(level);
    std::string timestamp = this->getTimestamp();

    message_stream << timestamp << " - " << level_desc << " - " << msg;
    std::string message = message_stream.str();
    std::cout << message << std::endl;
    *this->log_file << message << std::endl; 

	emit this->messageLogged(message, level);
}

/// Get text value represennting given message severity level.
std::string Logger::getLevelDescription(LogLevel level)
{
    switch (level) 
    {
        case LogLevel::Debug:
            return "[DEBUG]";
        case LogLevel::Info:
            return "[INFO]";
        case LogLevel::Warning:
            return "[WARNING]";
        case LogLevel::Error:
            return "[ERROR]";
        default:
            return "[UNKNOWN]"	;
    }
}

/// Get text representation of current date and time.
std::string Logger::getTimestamp()
{
    auto now = std::chrono::system_clock::now();
    return std::format("{:%Y-%m-%d %H:%M:%S}", std::chrono::floor<std::chrono::seconds>(now));
}
/// Shorthand method for  loggin debug level messages.
void Logger::debug(const std::string &msg)
{
    this->logMessage(msg, LogLevel::Debug);
}

/// Shorthand method for  loggin info level messages.
void Logger::info(const std::string &msg)
{

    this->logMessage(msg, LogLevel::Info);
}

/// Shorthand method for  loggin warning level messages.
void Logger::warning(const std::string &msg)
{

    this->logMessage(msg, LogLevel::Warning);
}

/// Shorthand method for  loggin error level messages.
void Logger::error(const std::string &msg)
{

    this->logMessage(msg, LogLevel::Error);
}
