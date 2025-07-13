#pragma once

#include "pch.h"

struct Timer {
    std::string name;
    bool manual = false;

    Timer();
    Timer(bool Manual);
    Timer(const std::string& Name);
    ~Timer();

    void restart();
    float stop();
    float stopAndRestart();
    float stopToMs();
    float stopToS();
    std::string stopToString();
private:
    std::chrono::time_point<std::chrono::steady_clock> m_start, m_end;
};
