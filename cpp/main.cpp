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
    Tables::clear();
    Tables tables;
    cout << "hw_table \n";
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16;j++) {
            cout << (int)tables.hw_table[i * 16 + j] << " ";
        }
        cout << endl;
    }
    cout << "hd_table \n";
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 256;j++) {
            cout << (int)tables.hd_table[i][j] << " ";
        }
        cout << endl;
    }
    cout << "g_table \n";
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 9;j++) {
            cout << (int)tables.g_table[i][j] << " ";
        }
        cout << endl;
    }
}