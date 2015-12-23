#include <iostream>
#include <Eigen/Dense>

// EigenML includes
#include <eigenml/tree/decision_tree.hpp>
#include <eigenml/logging/init_logging.hpp>
#include <eigenml/logging/logger.hpp>

using namespace std;
using namespace eigenml;

int main() {

    logging::init_cerr_log(logging::severity_level::trace);

    logging::Logger lg("MAIN");

    size_t N = 10;
    Matrix X = Matrix::Random(N, 3);
    Matrix c = Matrix::Random(3, 1);
    Vector Y(N);

    c << -1, 2, 1;
    Y = ((X * c).array() < 0).cast<double>();

    // // Declare a tree
    // tree::DecisionTreeParams params;
    // tree::DecisionTree<dMatrix, iVector> tree(params);

    // tree.fit(X, Y);
}
