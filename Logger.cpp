#include "Logger.h"

Logger *Logger::Get() {
    // Fast path.
    auto *object = Object_.load();
    if (object) {
        return object;
    }

    // Slow path.
    std::unique_lock<std::mutex> lock(Mutex_);
    // Double check.
    object = Object_.load();
    if (object) {
        return object;
    }

    object = new Logger();
    Object_.store(object);

    return object;
}

void Logger::setColored() {
    std::lock_guard lock(Mutex_);
    TypesToPrint_ = Logger::types_.at(1);
}

void Logger::setUncolored() {
    std::lock_guard lock(Mutex_);
    TypesToPrint_ = Logger::types_.at(0);
}

void Logger::Debug(const std::string &msg_, bool stdFlush = true) const {
    std::lock_guard lock(Mutex_);
    (*DEBUG_OUT) << getTime() << TypesToPrint_.at(LogType::DEBUG) + msg_;
    if (stdFlush) (*DEBUG_OUT) << std::endl;
    else (*DEBUG_OUT) << '\n';
}

void Logger::Info(const std::string &msg_, bool stdFlush = true) const {
    std::lock_guard lock(Mutex_);
    (*INFO_OUT) << getTime() << TypesToPrint_.at(LogType::INFO) + msg_;
    if (stdFlush) (*INFO_OUT) << std::endl;
    else (*INFO_OUT) << '\n';
}

void Logger::Warn(const std::string &msg_, bool stdFlush = true) const {
    std::lock_guard lock(Mutex_);
    (*WARN_OUT) << getTime() << TypesToPrint_.at(LogType::WARN) + msg_;
    if (stdFlush) (*WARN_OUT) << std::endl;
    else (*WARN_OUT) << '\n';
}

void Logger::Error(const std::string &msg_, bool stdFlush = true) const {
    std::lock_guard lock(Mutex_);
    (*ERROR_OUT) << getTime() << TypesToPrint_.at(LogType::ERROR) + msg_;
    if (stdFlush) (*ERROR_OUT) << std::endl;
    else (*ERROR_OUT) << '\n';
}

void Logger::setInfoStream(std::ostream *stream_) {
    std::lock_guard lock(Mutex_);
    INFO_OUT = stream_;
}

void Logger::setDebugStream(std::ostream *stream_) {
    std::lock_guard lock(Mutex_);
    DEBUG_OUT = stream_;
}

void Logger::setWarnStream(std::ostream *stream_) {
    std::lock_guard lock(Mutex_);
    WARN_OUT = stream_;
}

void Logger::setErrorStream(std::ostream *stream_) {
    std::lock_guard lock(Mutex_);
    ERROR_OUT = stream_;
}

void Logger::setAllStream(std::ostream *stream_) {
    setInfoStream(stream_);
    setDebugStream(stream_);
    setWarnStream(stream_);
    setErrorStream(stream_);
}

Logger::Logger() {
    INFO_OUT = &std::clog;
    DEBUG_OUT = &std::cout;
    WARN_OUT = &std::cerr;
    ERROR_OUT = &std::cerr;
    TypesToPrint_ = Logger::types_.at(0);
}

inline std::string Logger::getTime() { // get time in [H:M:S] format
    auto currentTime = std::chrono::system_clock::now();
    std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);
    std::tm *localTime = std::localtime(&currentTime_t);
    std::string hours = localTime->tm_hour >= 10 ? std::to_string(localTime->tm_hour)
                                                 : "0" + std::to_string(localTime->tm_hour);
    std::string minutes = localTime->tm_min >= 10 ? std::to_string(localTime->tm_min)
                                                  : "0" + std::to_string(localTime->tm_min);
    std::string secs = localTime->tm_sec >= 10 ? std::to_string(localTime->tm_sec)
                                               : "0" + std::to_string(localTime->tm_sec);

    return "[" + hours + ":" + minutes
           + ":" + hours + "]";
}

std::atomic<Logger *> Logger::Object_ = nullptr;

std::mutex Logger::Mutex_;

const std::unordered_map<int, PrintTypesDict> Logger::types_ = {
        {0, {
                    {LogType::DEBUG, " [DEBUG] "},
                    {LogType::ERROR, " [ERROR] "},
                    {LogType::WARN, " [WARN] "},
                    {LogType::INFO, " [INFO] "}
            }}, // uncolored
        {1, {
                    {LogType::DEBUG, " \\x1b[34;1m[DEBUG]\\x1b[0m "},
                    {LogType::ERROR, " \\x1b[31;1m[ERROR]\\x1b[0m "},
                    {LogType::WARN, " \\x1b[33;1m[WARN]\\x1b[0m "},
                    {LogType::INFO, " \\x1b[32;1m[INFO]\\x1b[0m "}
            }} // colored
};