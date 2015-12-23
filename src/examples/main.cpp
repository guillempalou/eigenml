#include <iostream>
#include <Eigen/Dense>

// EigenML includes
#include <eigenml/tree/decision_tree.hpp>
#include <eigenml/logging/init_logging.hpp>
#include <eigenml/logging/logger.hpp>

using namespace std;
using namespace eigenml;

typedef Eigen::MatrixXd dMatrix;
typedef Eigen::VectorXd dVector;
typedef Eigen::MatrixXi iMatrix;
typedef Eigen::VectorXi iVector;

int main() {

    logging::init_cerr_log(logging::severity_level::trace);

    logging::Logger lg("MAIN");
    // lg.trace << "Hello world!";
    // 
    LOG_INFO(lg) << "Hola";

    // BOOST_LOG_SEV(lg, logging::severity_level::error) << "Hello world!";
    

    // size_t N = 10;
    // dMatrix X = dMatrix::Random(N, 3);
    // dMatrix c = dMatrix::Random(3, 1);
    // iVector Y(N);

    // c << -1, 2, 1;
    // Y = ((X * c).array() < 0).cast<int>();

    // // Declare a tree
    // tree::DecisionTreeParams params;
    // tree::DecisionTree<dMatrix, iVector> tree(params);

    // tree.fit(X, Y);
}
