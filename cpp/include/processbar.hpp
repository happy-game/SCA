// processbar.hpp --- Process bar
// Created by: happy

#ifndef PROCESSBAR_HPP
#define PROCESSBAR_HPP

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <sys/ioctl.h>

class ProcessBar {
public:
    ProcessBar(int total, int position = 0, int width = 100, const char* title = "Processing", char filled='#', char blank='.');   // 构造函数
    ~ProcessBar();  // 析构函数
    void update(int items);   // 更新进度条
    void finish();  // 完成进度条
    void display(int position); // 显示进度条
    void time_stamp2str(time_t time_stamp, char* str); // 时间戳转字符串
    friend std::ostream& operator<<(std::ostream& os, const ProcessBar& pb); // 重载输出流
private:
    uint64_t total;  // 总数
    uint64_t processed; // 已处理数
    uint16_t position;   // 当前位置
    const char* title;  // 标题
    char filled;    // 填充字符
    char blank; // 空白字符
    float percent;  // 百分比
    uint16_t width;  // 进度条宽度
    time_t start_time;  // 开始时间
    char time_str[20];  // 时间字符串


    // 以下为进度条显示的信息
    char* display_str;  // 显示的字符串
    time_t used_time;   // 已用时间
    time_t remain_time; // 剩余时间
    time_t total_time;  // 总时间
    float speed;    // 速度

    uint64_t last_items;    // 上一次更新的items
    time_t last_time;   // 上一次更新的时间
    time_t update_interval; // 更新间隔
};

#endif //PROCESSBAR_HPP