#include <iostream>
#include <Eigen/Dense>

// EigenML includes
#include <eigenml/decision_tree/decision_tree.hpp>
#include <eigenml/core/eigenml.hpp>

using namespace std;
using namespace eigenml;

int main() {

    logging::init_cerr_log(logging::severity_level::info);

    logging::Logger logger("MAIN");

    std::srand((unsigned int) time(0));

    size_t N = 100000;
    Matrix X = Matrix::Random(N, 10);
    Matrix c = Matrix::Random(10, 1);
    Vector Y(N);

    c << -1, 2, 1, 1, 2, -1, -2, 1, 1, 1;
    Y = ((X * c).array() < 0).cast<double>();

    Vector I(X.rows());
    Matrix XY(X.rows(), I.cols() + X.cols()+Y.cols());
    std::iota(I.data(), I.data() + I.size(), 0);
    XY << I, X, Y;

    LOG_DEBUG << XY;

    // // Declare a tree
    LOG_INFO << "Fitting a tree";
    decision_tree::DecisionTreeParams params;

    params.criterion = decision_tree::SplitCriterion::kGiniCriterion;

    decision_tree::DecisionTree<ModelType::kSupervisedClassifier> tree(params);
    tree.fit(X, Y);
}
