//
// Created by happy on 12/11/23.
//

#include "tables.hpp"

using namespace std;

Tables::Tables() {
    // init tables
    char hw_table[] = "hw_table.bin";
    char hd_table[] = "hd_table.bin";
    char g_table[] = "g_table.bin";

    // load hw_table
    std::ifstream infile(hw_table, std::ios::binary);
    if (!infile.good()) {   // file not exist
        std::cout << hw_table << " File not exist!" << std::endl;
        infile.close();
        build_hw_table();
    } else {
        infile.seekg(0, std::ios::end);
        size_t file_size = (size_t)infile.tellg();  // file size
        infile.seekg(0, std::ios::beg);
        char *buffer = new char[file_size];
        infile.read(buffer, file_size);
        infile.close();
        memcpy(this->hw_table, buffer, 256);
        delete[] buffer;
    }

    // load hd_table
    infile.open(hd_table, std::ios::binary);
    if (!infile.good()) {
        std::cout << hd_table << " File not exist!" << std::endl;
        infile.close();
        build_hd_table();
    }else{
        infile.seekg(0, std::ios::end);
        size_t file_size = (size_t)infile.tellg();  // file size
        infile.seekg(0, std::ios::beg);
        char *buffer = new char[file_size];
        infile.read(buffer, file_size);
        infile.close();
        memcpy(this->hd_table, buffer, 256 * 256);
        delete[] buffer;
    }

    // load g_table
    infile.open(g_table, std::ios::binary);
    if (!infile.good()) {
        std::cout << g_table << " File not exist!" << std::endl;
        infile.close();
        build_g_table();
    }else{
        infile.seekg(0, std::ios::end);
        size_t file_size = (size_t)infile.tellg();  // file size
        infile.seekg(0, std::ios::beg);
        char *buffer = new char[file_size];
        infile.read(buffer, file_size);
        infile.close();
        memcpy(this->g_table, buffer, 256 * 9);
        delete[] buffer;
    }
}

Tables::~Tables() {
    // destructor
}

void Tables::build_hw_table() { // build hamming weight table
    char hw_table[] = "hw_table.bin";
    std::ofstream outfile(hw_table, std::ios::binary);
    for (int i = 0; i < 256; i++) {
        uint8_t hw = 0;
        for (int j = 0; j < 8; j++) {
            hw += (i >> j) & 1;
        }
        this->hw_table[i] = hw;
        outfile.write((char *)&hw, sizeof(uint8_t));
    }
    outfile.close();
}
void Tables::build_hd_table() { // build hamming distance table
    char hd_table[] = "hd_table.bin";
    std::ofstream outfile(hd_table, std::ios::binary);
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 256; j++) {
            uint8_t hd = 0;
            uint8_t  tmp = i ^ j;
            for (int k = 0; k < 8; k++) {
                hd += (tmp >> k) & 1;
            }
            this->hd_table[i][j] = hd;
            outfile.write((char *)&hd, sizeof(uint8_t));
        }
    }
    outfile.close();
}
void Tables::build_g_table() {  // build g table
    char g_table[] = "g_table.bin";
    std::ofstream outfile(g_table, std::ios::binary);
    for (int i = 0; i < 256; i++) {
        this->g_table[i][0] = 1;
        uint8_t tmp = i;
        for (int j = 0; j < 8; j++) {
            this->g_table[i][j + 1] = (tmp >> j) & 1;
        }
        outfile.write((char *)this->g_table[i], 9 * sizeof(uint8_t));
    }
    outfile.close();
}

void Tables::display_hw_table() {
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16;j++) {
            cout << (int)this->hw_table[i * 16 + j] << " ";
        }
        cout << endl;
    }
}

void Tables::display_hd_table() {
    for (int i = 0; i < 256; i++) {
        cout << i << ": ";
        for (int j = 0; j < 256;j++) {
            cout << (int)this->hd_table[i][j] << " ";
        }
        cout << endl;
    }
}

void Tables::display_g_table() {
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 9;j++) {
            cout << (int)this->g_table[i][j] << " ";
        }
        cout << endl;
    }
}

void Tables::clear() {  // delete tables file
    char hw_table[] = "hw_table.bin";
    char hd_table[] = "hd_table.bin";
    char g_table[] = "g_table.bin";
    remove(hw_table);
    remove(hd_table);
    remove(g_table);
}