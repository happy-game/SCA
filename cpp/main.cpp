#include "processbar.hpp"
#include "dataProcess.hpp"
#include <unistd.h>

using namespace std;
int main(int argc, char* argv[]) {
    // print pwd
    char pwd[100];
    getcwd(pwd, sizeof(pwd));
    cout << "pwd: " << pwd << endl;
    char *in_name = "wave.txt";
    char *out_name = "wave";
    uint8_t dtype = INT32;
    uint8_t base = 16;
    txt2bin(in_name, out_name, dtype, base);
    return 0;
}