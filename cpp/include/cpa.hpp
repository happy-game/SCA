// cpa.hpp -- correlation power analysis header file
// Created by: happy

#ifndef CPA_HPP
#define CPA_HPP

#include <iostream>
#include <cstdint>
#include <Eigen/Dense>

class CPA {
public:
    CPA();
    ~CPA();
    void load_traces(const char *filename, uint64_t trace_num, uint64_t trace_len); // load traces from file
    void load_traces(Eigen::MatrixXf *traces); // load traces from matrix
    void load_traces(const char *filename); // load traces from file, read trace_num and trace_len from file metadata
    void load_plaintexts(const char *filename, uint64_t trace_num, uint64_t trace_len); // load plaintexts from file
    void load_plaintexts(Eigen::MatrixXf *plaintexts); // load plaintexts from matrix
    void load_plaintexts(const char *filename); // load plaintexts from file, read trace_num and trace_len from file metadata
    void load_keys(const char *filename, uint64_t trace_num, uint64_t trace_len); // load keys from file
    void load_keys(Eigen::MatrixXf *keys); // load keys from matrix
    void load_keys(const char *filename); // load keys from file, read trace_num and trace_len from file metadata
    void load_hypothesis(const char *filename, uint64_t trace_num, uint64_t trace_len); // load hypothesis from file
private:
    Eigen::MatrixXf *traces; // traces matrix pointer
    Eigen::MatrixXf *plaintexts; // plaintexts matrix pointer
    Eigen::MatrixXf *keys; // keys matrix pointer
    Eigen::MatrixXf *hypothesis; // hypothesis matrix pointer

    uint64_t trace_num; // number of traces
    uint64_t trace_len; // length of each trace

};

#endif //CPA_HPP