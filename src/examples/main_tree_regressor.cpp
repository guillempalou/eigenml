#include <iostream>
#include <Eigen/Dense>

// EigenML includes
#include <eigenml/decision_tree/decision_tree.hpp>
#include <eigenml/core/eigenml.hpp>

using namespace std;
using namespace eigenml;

int main() {

    logging::init_cerr_log(logging::severity_level::trace);

    logging::Logger logger("MAIN");

    std::srand((unsigned int) time(0));

    size_t N = 10;
    size_t P = 1;
    Matrix X = Matrix::Random(N, P);
    Matrix c = Matrix::Random(P, 1);
    Vector Y(N);

    c << 1;
    Y = (X.array()*X.array()).matrix();

    Vector I(X.rows());
    Matrix XY(X.rows(), I.cols() + X.cols()+Y.cols());
    std::iota(I.data(), I.data() + I.size(), 0);
    XY << I, X, Y;

    LOG_DEBUG << XY;

    // // Declare a tree
    LOG_INFO << "Fitting a tree";
    decision_tree::DecisionTreeParams params;

    params.max_depth = 2;
    params.criterion = decision_tree::SplitCriterion::kMSECriterion;

    decision_tree::DecisionTree<ModelType::kSupervisedRegressor> tree(params);
    tree.fit(X, Y);

    std::cout << tree.transform(X) << std::endl << "*****" << std::endl << Y << std::endl;
}