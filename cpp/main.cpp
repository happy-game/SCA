#include "processbar.hpp"
#include "dataProcess.hpp"
#include <unistd.h>
#include <chrono>
#include <Eigen/Dense>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/reader.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include "tables.hpp"


using namespace std;


int main(int argc, char* argv[]) {
    int succ = txt2bin("plain.txt", "plain2.bin", INT8, 10);
    if (succ) {
        cout << "success" << endl;
    } else {
        cout << "fail" << endl;
    }
    return 0;
}