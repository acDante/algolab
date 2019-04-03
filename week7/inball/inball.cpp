// a classical LP problem: find largest inscribed sphere in a polyhedron
#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

// choose exact rational type
#include <CGAL/Gmpq.h>
typedef CGAL::Gmpq ET;
// solution type the solver provides
typedef CGAL::Quotient<ET> SolT;
// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

// round down to next integer double
double floor_to_double(const SolT& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a + 1 <= x) a += 1;
  return a;
}

void testcase(int n, int d){
    const int r = d;
    // Construct the linear program
    Program lp(CGAL::SMALLER, false, 0, false, 0);
    lp.set_l(r, true, 0);  // r >= 0
    for(int i = 0; i < n; ++i){
        int squared_norm = 0;
        for(int j = 0; j < d; ++j){
            int aij = 0; std::cin >> aij;
            lp.set_a(j, i, aij);
            squared_norm += aij * aij;
        }
        int bi = 0;
        std::cin >> bi;
        lp.set_b(i, bi);

        int norm = std::sqrt(squared_norm);
        lp.set_a(r, i, norm);  // set the coefficient for r
    }
    lp.set_c(r, -1);  // minimize -r
    Solution s =  CGAL::solve_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));

    switch(s.status()){
        case CGAL::QP_UNBOUNDED:{
            std::cout << "inf" << "\n";
            break;
        }
        case CGAL::QP_INFEASIBLE:{
            std::cout << "none" << "\n";
            break;
        }
        case CGAL::QP_OPTIMAL:{
            std::cout << floor_to_double(-s.objective_value()) << "\n";
            break;
        }
        default:
            break;
    }
    return;
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int n, d;
    std::cin >> n;
    while(n){
        std::cin >> d;
        testcase(n, d);
        std::cin >> n;
    }
    return 0;
}
