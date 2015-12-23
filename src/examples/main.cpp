#include <iostream>
#include <Eigen/Dense>

// EigenML includes
#include <eigenml/tree/decision_tree.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

namespace boostlog = boost::log;

// example_tutorial_trivial_with_filtering
void init()
{
    boostlog::core::get()->set_filter
    (
        boostlog::trivial::severity >= boostlog::trivial::info
    );
}



using namespace std;
using namespace eigenml;

typedef Eigen::MatrixXd dMatrix;
typedef Eigen::VectorXd dVector;
typedef Eigen::MatrixXi iMatrix;
typedef Eigen::VectorXi iVector;

int main() {
    
    size_t N = 10;
    dMatrix X = dMatrix::Random(N, 3);
    dMatrix c = dMatrix::Random(3, 1);
    iVector Y(N);

    c << -1, 2, 1;
    Y = ((X * c).array() < 0).cast<int>();

    // Declare a tree
    tree::DecisionTreeParams params;
    tree::DecisionTree<dMatrix, iVector> tree(params);

    tree.fit(X, Y);
}
