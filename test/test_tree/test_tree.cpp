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

TEST(DecisionTree, SimpleFitTransform) {
    size_t N = 10;
    Matrix X(N, 1);
    Vector Y(N);

    X << 0, 1, 2, 3, 4, 5, 6, 7, 8, 9; 
    Y << 0, 0, 0, 0, 0, 1, 1, 1, 1, 1;

    // // Declare a tree
    DecisionTreeParams params;
    params.max_depth = 1;

    DecisionTree<ModelType::kSupervisedClassifier, Matrix, Vector> tree(params);
    tree.fit(X, Y); 

    ASSERT_EQ(Y, tree.transform(X));
}

TEST(DecisionTree, FitTransform3Classes) {
    size_t N = 15;
    Matrix X(N, 1);
    Vector Y(N);

    X << 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14; 
    Y << 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2;

    // // Declare a tree
    DecisionTreeParams params;
    params.max_depth = 2;

    DecisionTree<ModelType::kSupervisedClassifier, Matrix, Vector> tree(params);
    tree.fit(X, Y); 
    
    ASSERT_EQ(Y, tree.transform(X));
}


TEST(DecisionTree, CreationFailure) {

    // test creating it with wrong parameters
    try {
        DecisionTreeParams params;
        params.criterion = (SplitCriterion)(-1);
        DecisionTree<ModelType::kSupervisedClassifier, Matrix, Vector> tree(params);
    }
    catch(const core::WrongParametersException & err) {
        // this is expected
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
        // this is expected
    }
    catch(...) {
        FAIL() << "Expected core::EigenMLException";
    }
}