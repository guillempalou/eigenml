#ifndef MODEL_HPP
#define MODEL_HPP

namespace random_forest {

    template<class Matrix> 
    class Model {
    public:
        virtual bool fit(const Matrix &X, const Matrix &Y) = 0;
        virtual Matrix transform(const Matrix &X) = 0;
        
        // TODO improve the error handling?
        virtual Matrix fit_transform(const Matrix& X, const Matrix& Y = nullptr) {
            bool fitted = fit(X, Y);
            if (fitted) {
                return transform(X);
            } else {
                return Matrix();
            }
        }
    };
}

#endif // MODEL_HPP