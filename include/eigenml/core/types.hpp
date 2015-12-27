#ifndef TYPES_HPP
#define TYPES_HPP

#include <Eigen/Dense>
#include <Eigen/Sparse>

namespace eigenml {

    // general type of matrices
    typedef Eigen::MatrixXd Matrix;
    typedef Eigen::VectorXd Vector;

    // integer type of matrices
    typedef Eigen::MatrixXi MatrixI;
    typedef Eigen::VectorXi VectorI;

    // typedef for histogram types
    typedef std::map<double, double> Histogram;

    // value that the split criterion should return
    typedef std::pair<double, double> ValueAndWeight;

    // sorted X columns by values
    typedef std::vector<size_t> IdxVector;
}


#endif // TYPES_HPP
