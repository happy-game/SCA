#include <iostream>
#include <Eigen/Dense>
#include <boost/math/constants/constants.hpp>

using namespace std;

int main(int argc, char** argv) {
    Eigen::MatrixXd m(2,2);
    m(0,0) = 3;
    m(1,0) = 2.5;
    m(0,1) = -1;
    m(1,1) = m(1,0) + m(0,1);
    cout << m << endl;

    cout << "Hello, world!" << endl;
    cout << "pi: " << boost::math::constants::pi<double>() << endl;
    return 0;
}