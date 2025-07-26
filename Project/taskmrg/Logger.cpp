#include "Logger.h"
#include <iostream>
#include <chrono>
#include <ctime>

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

Logger::Logger() {
    logFile.open("log.txt", std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "无法打开日志文件。" << std::endl;
    }
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void Logger::log(const std::string& message) {
    std::lock_guard<std::mutex> lock(mtx);
    if (logFile.is_open()) {
        // 获取当前时间
        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
        char buffer[100];
        std::strftime(buffer,sizeof(buffer),"%y-%m-%d %H:%M:%S",localtime(&now_time));
        logFile << std::string(buffer) << ": " << message << std::endl;
    }
}