#include "ktime.h"
#include <sys/time.h>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <regex>
#include <sstream>

using namespace std;

KTime::KTime() {
    clock_gettime(CLOCK_REALTIME, &time_start_);
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &cpu_time_start_);
    mono_time_ = getMonotonicTime();
}

double KTime::getTime() {
    timespec timeStart;
    clock_gettime(CLOCK_REALTIME, &timeStart);
    return timeStart.tv_sec + timeStart.tv_nsec / static_cast<double>(1e9);
}

double KTime::getMonotonicTime() {
    timespec timeStart;
    clock_gettime(CLOCK_MONOTONIC, &timeStart);
    return timeStart.tv_sec + timeStart.tv_nsec / static_cast<double>(1e9);
}

string KTime::getStringTimeSecond() {
    timespec time;
    clock_gettime(CLOCK_REALTIME, &time);
    stringstream ss;
    tm t;
    ss << put_time(localtime_r(&time.tv_sec, &t), "%Y-%m-%d_%H-%M-%S");
    return ss.str();
}

string KTime::getStringTimeMillisecond() {
    timespec time;
    clock_gettime(CLOCK_REALTIME, &time);
    stringstream ss;
    tm t;
    ss << put_time(localtime_r(&time.tv_sec, &t), "%Y-%m-%d_%H-%M-%S");
    return ss.str() + "-" + to_string(time.tv_nsec / 1e6);
}

double KTime::getElapseTime() { return getMonotonicTime() - mono_time_; }

double KTime::getElapseCpuTime() {
    timespec end;
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &end);
    double starts =
        cpu_time_start_.tv_sec + cpu_time_start_.tv_nsec / (double)1e9;
    double ends = end.tv_sec + end.tv_nsec / (double)1e9;
    return ends - starts;
}

KLocalTime KTime::utc2local(double utc) {
    time_t tt = static_cast<int>(utc);
    tm t;
    localtime_r(&tt, &t);

    KLocalTime local;
    local.day = t.tm_mday;
    local.year = t.tm_year + 1900;
    local.month = t.tm_mon + 1;
    local.hour = t.tm_hour;
    local.minute = t.tm_min;
    local.second = t.tm_sec;
    local.millisecond = (utc - static_cast<int>(utc)) * 1000;

    return local;
}

std::ostream &operator<<(std::ostream &os, const KLocalTime &local) {
    os << local.year << "-" << local.month << "-" << local.day << "_"
       << local.hour << "-" << local.minute << "-" << local.second << "-"
       << local.millisecond;
    return os;
}

double KTime::local2utc(const KLocalTime &local) {
    tm m;
    m.tm_year = local.year - 1900;
    m.tm_mon = local.month - 1;
    m.tm_mday = local.day;
    m.tm_hour = local.hour;
    m.tm_min = local.minute;
    m.tm_sec = local.second;
    time_t res = mktime(&m);

    return res + local.millisecond / 1000.0;
}

string KTime::local2str(const KLocalTime &local) {
    ostringstream os;
    os << local;
    return os.str();
}

std::string KTime::utc2str(double utc) {
    ostringstream os;
    os << utc;
    return os.str();
}

KLocalTime KTime::str2Local(const std::string &val) {
    istringstream is(regex_replace(val, regex("[^0-9]"), " "));
    int v;
    vector<int> data;
    while (is >> v) {
        data.push_back(v);
    }

    int y = 0, mm = 0, d = 0, h = 0, m = 0, s = 0;

    switch (data.size()) {
        case 1:
            s = data[0];
            break;
        case 2:
            s = data[1];
            m = data[0];
            break;
        case 3:
            s = data[2];
            m = data[1];
            h = data[0];
            break;
        case 4:
            s = data[3];
            m = data[2];
            h = data[1];
            d = data[0];
            break;
        case 5:
            s = data[4];
            m = data[3];
            h = data[2];
            d = data[1];
            mm = data[0];
            break;
        case 6:
            s = data[5];
            m = data[4];
            h = data[3];
            d = data[2];
            mm = data[1];
            y = data[0];
            break;
        default:
            throw std::runtime_error("str time format error");
            break;
    }
    return KLocalTime(y, mm, d, h, m, s);
}
