// processbar.cpp --- Process bar
// Created by: happy

#include "processbar.hpp"


using namespace std;

ProcessBar::ProcessBar(int total, int position, int width, const char* title, char filled, char blank){
    this->total = total;
    this->processed = 0;
    this->position = position;
    this->width = width;
    this->title = title;
    this->filled = filled;
    this->blank = blank;
    this->percent = 0.0;
    this->start_time = time(nullptr);
    this->last_items = 0;
    this->last_time = this->start_time;
    this->last_percent = 0.0;
    this->update_interval = 1;

    // 获取当前终端行数和列数
    struct winsize size;
    ioctl(0, TIOCGWINSZ, &size);
    this->width = size.ws_col - 10;
    if (this->width > 200) {
        this->width = 200;
    }
    this->position = size.ws_row - position;
    this->display_str = new char[this->width + 1];
    this->update_interval = 1;  // 更新间隔 1s

   this->display(this->position);

}

ProcessBar::~ProcessBar() {
    delete[] this->display_str;
}

void ProcessBar::display(int position) {
    // display_str: title: [#####..........] 10.00% 00:00:00(used time) 00:00:00(remain time) 0.00 items/s or 0.00 s/item
    uint16_t pbar_width = this->width - strlen(this->title) - 78;
    this->percent = (float)this-> processed / this->total;
    int filled_num = (int)(this->percent * pbar_width);
    int blank_num = pbar_width  - filled_num;
    memset(this->display_str, 0, this->width + 1);  // 清空字符串

    sprintf(this->display_str, "%s: [", this->title);
    memset(this->display_str + strlen(this->display_str), this->filled, filled_num);
    memset(this->display_str + strlen(this->display_str), this->blank, blank_num);
    sprintf(this->display_str + strlen(this->display_str), "] %.2f%% ", this->percent * 100);   // 百分比
    this->time_stamp2str(this->used_time, this->time_str);
    sprintf(this->display_str + strlen(this->display_str), "%s(used time) ", this->time_str); // 已用时间
    this->time_stamp2str(this->remain_time, this->time_str);
    sprintf(this->display_str + strlen(this->display_str), "%s(remain time) ", this->time_str);   // 剩余时间
    if (this->speed > 1) {
        sprintf(this->display_str + strlen(this->display_str), "%.2f items/s", speed);    // 速度
    } else {
        sprintf(this->display_str + strlen(this->display_str), "%.2f s/item", 1 / speed);    // 速度
    }

    // 清空 position 行
    cout << "\033[" << position << ";0H";
    for (int i = 0; i < this->width + 10; i++) {
        cout << " ";
    }
    cout << "\033[" << position << ";0H" << this->display_str << flush;
}

void ProcessBar::finish() {
    this->processed = this->total;
    this->percent = 1.0;
    this->used_time = time(NULL) - this->start_time;
    this->remain_time = 0;
    this->speed = (float)this->total / this->used_time;
    this->display(this->position);
}

void ProcessBar::update(int items) {
    this->processed += items;
    this->percent = (float)this->processed / this->total;
    this->used_time = time(nullptr) - this->start_time;
    this->remain_time = (int)(this->used_time / this->percent - this->used_time);
    this->speed = (float)(this->processed) / (time(nullptr) - this->start_time);
    if (time(nullptr) - this->last_time >= this->update_interval || this->percent - this->last_percent >= 0.01) {
        this->display(this->position);
        this->last_time = time(nullptr);
    }
}

void ProcessBar::time_stamp2str(time_t time_stamp, char* str) {
    int hour = time_stamp / 3600;
    int minute = (time_stamp - hour * 3600) / 60;
    int second = time_stamp - hour * 3600 - minute * 60;
    sprintf(str, "%02d:%02d:%02d", hour, minute, second);
}

ostream& operator<<(ostream& os, const ProcessBar& pb) {
    cout << "total: " << pb.total << endl;
    cout << "position: " << pb.position << endl;
    cout << "title: " << pb.title << endl;
    cout << "filled: " << pb.filled << endl;
    cout << "blank: " << pb.blank << endl;
    cout << "percent: " << pb.percent << endl;
    cout << "width: " << pb.width << endl;
    cout << "start_time: " << pb.start_time << endl;
    cout << "time_str: " << pb.time_str << endl;
    cout << "display_str: " << pb.display_str << endl;
    cout << "used_time: " << pb.used_time << endl;
    cout << "remain_time: " << pb.remain_time << endl;
    cout << "total_time: " << pb.total_time << endl;
    return os;
}