// dataProcess.cpp  -- process data
// Created by: happy

#include "dataProcess.hpp"
#include "processbar.hpp"

using namespace std;

bool txt2bin(const char *in_name, const char *out_name, uint8_t dtype, uint8_t base) {
    // check file exist
    std::ifstream infile(in_name);
    if (!infile.good()) {
        std::cout << in_name << " File not exist!" << std::endl;
        return false;
    }
    infile.seekg(0, std::ios::end);
    size_t file_size = (size_t)infile.tellg();  // file size
    uint64_t one_line_size = 0; // one line size
    uint64_t one_line_num = 0;  // one line number
    infile.seekg(0, std::ios::beg);

    std::string line;
    std::getline(infile, line);
    for (char i : line) {
        if (i == ' ') {
            one_line_num++;
        }
        one_line_size++;
    }
    one_line_size++;    // '\n'
    one_line_num++;
    uint64_t lines = uint64_t (file_size / one_line_size) + 1; // lines
    infile.close();
    ProcessBar pb(file_size, 0, 100, "Converting", '#', '.');   // 构造进度条

    // read file
    infile.open(in_name);
    char *real_out_name = new char[strlen(out_name) + 100]; // real out name = {out_name}_{dtype}_{one_line_num}_{lines}.bin
    sprintf(real_out_name, "%s_%d_%08d_%08d.bin", out_name, dtype, one_line_num, lines);
    std::ofstream outfile(real_out_name, std::ios::binary);

    uint8_t dtype_size[] = {1, 2, 4, 8, 4, 8};

//    uint64_t buffer_size = one_line_num * dtype_size[dtype];
    uint64_t buffer_size = 1024 * 1024 * dtype_size[dtype];
    auto *read_buffer = new uint8_t[buffer_size];
    auto *write_buffer = new uint8_t[buffer_size];

    // create a thread to write file
    std::thread *t = nullptr;   // write file thread

    int j = 0;
    while (std::getline(infile, line)) {    // read one line
//        memset(read_buffer, 0, one_line_num * dtype_size[dtype]);   // clear buffer
        char *p = line.data();
        char *q = p;
        size_t len = line.length();
        for (int i = 0; i < one_line_num; i++) {
            if (i == one_line_num - 1) {
                q = strchr(p, '\n');
            } else {
                q = strchr(p, ' ');
            }
//            int16_t tmp = (int16_t)strtol(p, &q, base);
            switch (dtype) {
                case INT8:
                    ((int8_t *)read_buffer)[j] = (int8_t)strtol(p, &q, base);
                    break;
                case INT16:
                    ((int16_t *)read_buffer)[j] = (int16_t)strtol(p, &q, base);
                    break;
                case INT32:
                    ((int32_t *)read_buffer)[j] = (int32_t)strtol(p, &q, base);
                    break;
                case INT64:
                    ((int64_t *)read_buffer)[j] = (int64_t)strtol(p, &q, base);
                    break;
                case FLOAT32:
                    ((float *)read_buffer)[j] = strtof(p, &q);
                    break;
                case FLOAT64:
                    ((double *)read_buffer)[j] = strtod(p, &q);
                    break;
                default:
                    break;
            }
            p = q + 1;
            q = p;
            j++;
        }
        pb.update(len + 1);
        if (j == one_line_num || infile.eof()) {
//            j = 0;
        }
        else{
            continue;   // not enough data to write
        }
        void *tmp = read_buffer;
        read_buffer = write_buffer;
        write_buffer = (uint8_t *)tmp;
        write_file(&outfile, (char *)write_buffer, j * dtype_size[dtype]);
        memset(write_buffer, 0, one_line_num * dtype_size[dtype]);
        j = 0;
//        if (t != nullptr) { // wait for thread
//            t->join();
//            delete t;
//        }
//        t = new std::thread(write_file, &outfile, (char *)write_buffer, one_line_num * dtype_size[dtype]);
//        t->detach();    // detach thread
    }
    pb.finish();
    infile.close();
    outfile.close();

    free(read_buffer);
    free(write_buffer);
    delete[] real_out_name;
    return true;
}

size_t write_file(std::ofstream *outfile, const char *buffer, size_t size) {
    outfile->write(buffer, size);
    return size;
}