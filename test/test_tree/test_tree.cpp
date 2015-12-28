#include <gtest/gtest.h>
#include <eigenml/decision_tree/decision_tree.hpp>

using namespace eigenml;
using namespace eigenml::decision_tree;

TEST(DecisionTree, NodeCreation) {
    size_t N = 2;
    Matrix X(N, 1);
    Vector Y(N);

    X << 1, 2; 
    Y << 0, 1;

    // // Declare a tree
    DecisionTreeParams params;
    DecisionTree<ModelType::kSupervisedClassifier, Matrix, Vector> tree(params);
    tree.fit(X, Y); 

    auto root = tree.root();
    auto left = root.left_child();
    auto right = root.right_child();

    ASSERT_EQ(1, left.n_samples());
    ASSERT_EQ(1, right.n_samples());
}

TEST(DecisionTree, CreationFailure) {

    // test creating it with wrong parameters
    try {
        DecisionTreeParams params;
        params.criterion = (SplitCriterion)(-1);
        DecisionTree<ModelType::kSupervisedClassifier, Matrix, Vector> tree(params);
    }
    catch(const core::WrongParametersException & err) {
        EXPECT_EQ(err.what(), "Wrong parameters : Wrong node split criterion");
    }
    catch(...) {
        FAIL() << "Expected core::EigenMLException";
    }
}

TEST(DecisionTree, FitFailure) {

    // test creating it with wrong parameters
    try {
        DecisionTreeParams params;
        DecisionTree<ModelType::kSupervisedClassifier, Matrix, Vector> tree(params);

        size_t N = 0;
        Matrix X(N, 1);
        Vector Y(N);

        tree.fit(X, Y);
    }
    catch(const core::InsufficientSamplesException & err) {
        EXPECT_EQ(err.what(), std::string("EigenML Exception: Insufficient Samples 0"));
    }
    catch(...) {
        FAIL() << "Expected core::EigenMLException";
    }
}