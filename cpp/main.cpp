#include "processbar.hpp"
#include "dataProcess.hpp"
#include <unistd.h>
#include <chrono>

using namespace std;
int main(int argc, char* argv[]) {
//    char *in_name = "wave.txt";
//    char *out_name = "wave";
//
//
//    auto start = chrono::steady_clock::now();
//    for(int i = 0; i<10;i++){
//        cout << "i = " << i << endl;
//        txt2bin(in_name, out_name, INT16, 16);
//    }
//    auto end = chrono::steady_clock::now();
//
//    cout << "Time used: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() / 10 << " ms" << endl;

    char *in_name2 = "plain.txt";
    char *out_name2 = "plain";

    auto start2 = chrono::steady_clock::now();
    for(int i = 0; i<10;i++){
        cout << "i = " << i << endl;
        txt2bin(in_name2, out_name2, INT16, 16);
    }
    auto end2 = chrono::steady_clock::now();

    cout << "Time used: " << chrono::duration_cast<chrono::milliseconds>(end2 - start2).count() / 10 << " ms" << endl;
}