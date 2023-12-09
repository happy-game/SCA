// dataProcess.hpp  -- process data
// Created by: happy

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdint>
#include <thread>
#include <Eigen/Dense>
#include <sys/ioctl.h>

#define INT8 0
#define INT16 1
#define INT32 2
#define INT64 3
#define FLOAT32 4
#define FLOAT64 5


bool txt2bin(const char *in_name, const char *out_name, uint8_t dtype, uint8_t base);  // load data from txt file
bool matrix2txt(const char *filename, Eigen::MatrixXf *matrix); // save data to txt file
bool matrix2bin(const char *filename, Eigen::MatrixXf *matrix); // save data to bin file
bool bin2matrix(const char *filename);  // load data from bin file

size_t write_file(std::ofstream *outfile, const char *buffer, size_t size); // write file
