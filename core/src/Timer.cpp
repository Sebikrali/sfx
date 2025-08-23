#include "sfx/Timer.hpp"

Timer::Timer() {
    m_start = std::chrono::steady_clock::now();
}

Timer::Timer(bool Manual) {
    m_start = std::chrono::steady_clock::now();
    manual = Manual;
}

Timer::Timer(const std::string& Name) {
    m_start = std::chrono::steady_clock::now();
    name = Name;
}

Timer::~Timer() {
    if (manual) return;
    m_end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(m_end - m_start);
    float ms = duration.count();
    float s = floor(ms / 1000.0f);

    std::stringstream builder;
    builder << (name.empty() ? "Timer: " : name + ": ");
    if (s >= 1.0f)
        builder << s << "s ";
    builder << (ms - 1000 * s) << "ms\n";

    std::cout << builder.str();
}

void Timer::restart() {
    m_start = std::chrono::steady_clock::now();
}

float Timer::stop() {
    m_end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration<float>(m_end - m_start);
    return duration.count();
}

float Timer::stopAndRestart() {
    m_end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration<float>(m_end - m_start);
    m_start = std::chrono::steady_clock::now();
    return duration.count();
}

float Timer::stopToMs() {
    m_end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(m_end - m_start);
    return duration.count();
}

float Timer::stopToS() {
    m_end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(m_end - m_start);
    return duration.count();
}

std::string Timer::stopToString() {
    m_end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(m_end - m_start);
    float ms = duration.count();
    float s = floor(ms / 1000.0f);

    std::stringstream builder;
    if (s >= 1.0f)
        builder << s << "s ";
    builder << (ms - 60 * s) << "ms";
    return builder.str();
}
