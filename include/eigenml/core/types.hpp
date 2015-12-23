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
}


#endif // TYPES_HPP
