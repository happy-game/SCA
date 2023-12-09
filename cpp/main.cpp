#include "processbar.hpp"
#include <unistd.h>

using namespace std;
int main(int argc, char* argv[]) {
    ProcessBar* pb1 = new ProcessBar(100, 0, 100, "Processing1", '#', '.');
    for (int i = 0; i < 100; i++) {
        ProcessBar* pb2 = new ProcessBar(100, 1, 100, "Processing2", '#', '.');
        for (int j = 0; j < 100; j++) {
            pb2->update(1);
            sleep(1);
        }
        pb2->finish();
        delete pb2;
        pb1->update(1);
    }
    ProcessBar* pb = new ProcessBar(100, 0, 100, "Processing", '#', '.');
    for (int i = 0; i < 100; i++) {
        pb->update(1);
        sleep(0.1);
    }
    pb->finish();
}