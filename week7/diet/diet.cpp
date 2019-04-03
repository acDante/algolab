#include <iostream>
#include <vector>
#include <cassert>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

typedef CGAL::Gmpq ET;
typedef CGAL::Quotient<ET> SolT;
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

void testcase(int n, int m){
    std::vector<int> lower(n, 0);
    std::vector<int> upper(n, 0);
    std::vector<std::vector<int> > C(m, std::vector<int>(n));
    std::vector<int> price(m, 0);

    for(int i = 0; i < n; ++i){
         std::cin >> lower[i] >> upper[i];
    }

    for(int i = 0; i < m; ++i){
        std::cin >> price[i];
        for(int j = 0; j < n; ++j){
            std::cin >> C[i][j];
        }
    }
    Program lp (CGAL::SMALLER, true, 0, false, 0);
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < m; ++j){
            lp.set_a(j, i, C[j][i]);
            lp.set_a(j, n + i, -C[j][i]);
        }
        lp.set_b(i, upper[i]);
        lp.set_b(n + i, -lower[i]);
    }
    for(int i = 0; i < m; ++i){
        lp.set_c(i, price[i]);
    }
    assert(lp.is_nonnegative());
    Solution s = CGAL::solve_nonnegative_linear_program(lp, ET());
    assert(s.solves_nonnegative_linear_program(lp));

    if(s.status() == CGAL::QP_OPTIMAL){
        std::cout << floor_to_double(s.objective_value()) << "\n";
    }
    else{
        std::cout << "No such diet." << "\n";
    }
}

int main(){
    int n, m;
    std::cin >> n >> m;
    while(n && m){
        testcase(n, m);
        std::cin >> n >> m;
    }
    return 0;
}
