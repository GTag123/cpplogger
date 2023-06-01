#ifndef LOGGER_LOGGER_H
#define LOGGER_LOGGER_H

#include <atomic>
#include <iostream>
#include <mutex>
#include "chrono"
#include "unordered_map"

enum class LogType {
    INFO = 0,
    DEBUG,
    WARN,
    ERROR,
};

using PrintTypesDict = std::unordered_map<LogType, std::string>;
class Logger {
public:
    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger& operator=(Logger&&) = delete;

    static Logger *Get();

    void setColored();

    void setUncolored();

    void Debug(const std::string &msg_, bool stdFlush) const;

    void Info(const std::string &msg_, bool stdFlush) const;

    void Warn(const std::string &msg_, bool stdFlush) const;

    void Error(const std::string &msg_, bool stdFlush) const;

    void setInfoStream(std::ostream* stream_);

    void setDebugStream(std::ostream* stream_);

    void setWarnStream(std::ostream* stream_);

    void setErrorStream(std::ostream* stream_);

    void setAllStream(std::ostream* stream_);

private:
    Logger();
    PrintTypesDict TypesToPrint_;
    std::ostream* DEBUG_OUT;
    std::ostream* WARN_OUT;
    std::ostream* ERROR_OUT;
    std::ostream* INFO_OUT;

    static inline std::string getTime(); // get time in [H:M:S] format

    static std::atomic<Logger *> Object_;
    static std::mutex Mutex_;
    static const std::unordered_map<int, PrintTypesDict> types_;
};


#endif //LOGGER_LOGGER_H
