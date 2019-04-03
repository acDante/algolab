// LP (with Gmpz) + Exponential search + Binary Search
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <climits>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

// choose exact integral type
#include <CGAL/Gmpz.h>
typedef CGAL::Gmpz ET;
// solution type the solver provides
typedef CGAL::Quotient<ET> SolT;
// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;

class Point {
    public:
        int x, y, z;
        Point(): x(0), y(0), z(0) {}
        Point(int x, int y, int z): x(x), y(y), z(z) {}
};
typedef std::vector<Point> VP;

// check if there exists a polynomial of degree d that can separate healthy and tumor cells 
bool checkPoly(int d, VP& healthy, VP& tumor){
    Program lp(CGAL::SMALLER, false, 0, false, 0);
    int H = healthy.size(), T = tumor.size();
    int delta = 0;    // index of delta
    for(int h = 0; h < H; ++h){
        // Generate all poynomial terms and add constraints
        int var_idx = 0;
        int x = healthy[h].x, y = healthy[h].y, z = healthy[h].z;
        for(int i = 0; i <= d; ++i){
            for(int j = 0; j <= d; ++j){
                for(int k = 0; k <= d; ++k){
                    if(i + j + k <= d){
                        lp.set_a(var_idx, h, ET(pow(x, i) * pow(y, j) * pow(z, k)));
                        var_idx++;
                    }
                    else 
                        break;
                }
            }
        }
        // set delta
        lp.set_a(var_idx, h, 1);
        lp.set_b(h, 0);
        delta = var_idx;
    }
    for(int t = 0; t < T; ++t){
        int var_idx = 0;
        int x = tumor[t].x, y = tumor[t].y, z = tumor[t].z;
        for(int i = 0; i <= d; ++i){
            for(int j = 0; j <= d; ++j){
                for(int k = 0; k <= d; ++k){
                    if(i + j + k <= d){
                        lp.set_a(var_idx, H + t, -ET(pow(x, i) * pow(y, j) * pow(z, k)));
                        var_idx++;
                    }
                }
            }
        }
        // set delta
        lp.set_a(var_idx, H + t, 1);
        lp.set_b(H + t, 0);
    }
    // Maximize the margin
    lp.set_c(delta, -1);
    lp.set_l(delta, true, 0);

    // Use Bland's rule to improve the running time 
    CGAL::Quadratic_program_options options;
    options.set_pricing_strategy(CGAL::QP_BLAND);
    Solution s = CGAL::solve_linear_program(lp, ET(), options);
    assert(s.solves_linear_program(lp));
    
    // Check if the resulting delta is larger than 0
    if(s.is_optimal()){
        // auto opt = s.variable_values_begin();
        // return (*(opt + delta) > 0);
        return (s.objective_value() < 0);
    }
    // If LP is unbounded, there exists an infinitely large delta
    return s.is_unbounded();
}

void testcase(){
    int h, t;
    std::cin >> h >> t;
    std::vector<Point> healthy(h), tumor(t);
    for(int i = 0; i < h; ++i){
        int x, y, z;
        std::cin >> x >> y >> z;
        healthy[i] = Point(x, y, z);
    }
    for(int i = 0; i < t; ++i){
        int x, y, z;
        std::cin >> x >> y >> z;
        tumor[i] = Point(x, y, z);
    }

    // Exponential Search the possible range [dbeg, dend]
    int MAX_D = 30;
    bool not_found = true;
    int dbeg = 0, dend = 1;
    while((not_found = !checkPoly(dend, healthy, tumor)) && dend < MAX_D){
        dbeg = dend;
        dend *= 2;
    }

    if(not_found && !checkPoly(MAX_D, healthy, tumor)){
        std::cout << "Impossible!\n";
        return;
    }
    dend = std::min(dend, MAX_D);

    // Binary Search the smallest degree in [dbeg, dend]
    int d = - 1;  // -1 indicates no feasible degree is found within the range
    while(dbeg <= dend){
        int mid = (dbeg + dend) / 2;
        if(checkPoly(mid, healthy, tumor)){
            d = mid;
            dend = mid - 1;
        }
        else
            dbeg = mid + 1;
    }
    if(d >= 0)
        std::cout << d << "\n";
    else
        std::cout << "Impossible!\n";
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int N = 0;
    std::cin >> N;
    for(int i = 0; i < N; ++i){
        testcase();
    }
    return 0;
}
