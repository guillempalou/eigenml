#ifndef CRITERIA_HPP
#define CRITERIA_HPP

#include <eigenml/core/eigenml.hpp>
#include <eigenml/decision_tree/decision_tree_traits.hpp>

namespace eigenml { namespace decision_tree {

    template<class Dist>
    ValueAndWeight entropy(const Dist& distribution) {
        const typename Dist::HistogramType& histogram = distribution.histogram();
        double n = distribution.weight();

        if (n == 0) 
            return ValueAndWeight(0, 0);
        
        double ent = 0;
        for (auto& e: histogram) {
            double p = e.second*1.0/n;
            ent += (p != 0) ? -p*std::log2(p) : 0;
        }

        return ValueAndWeight(ent, n);
    }

    template<class Dist>
    ValueAndWeight gini(const Dist& distribution) {
        const typename Dist::HistogramType& histogram = distribution.histogram();
        double n = distribution.weight();

        if (n == 0)
            return ValueAndWeight(0, 0);
        
        double gini = 0;
        for (auto& e: histogram) {
            double p = e.second*1.0/n;
            gini += p*(1-p);
        }

        return ValueAndWeight(gini, n);
    }

    template<class Dist>
    ValueAndWeight mse(const Dist& distribution) {
        return ValueAndWeight(distribution.var(), distribution.weight());
    }
}}

#endif // CRITERIA_HPP