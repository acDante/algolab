// 输出结果不太对 还需要debug一下。。
#include <iostream>
#include <vector>
#include <cassert>
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

// round up to next integer double
double ceil_to_double(const SolT& x)
{
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return a;
}

void testcase(int n, int m){ // n: # of assets, m: # of people
    std::vector<int> c(n, 0);
    std::vector<int> r(n, 0);
    std::vector<std::vector<int> > v(n, std::vector<int>(n));
    for(int i = 0; i < n; ++i){
        std::cin >> c[i] >> r[i];
    }

    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            std::cin >> v[i][j];
        }
    }
    Program qp(CGAL::SMALLER, true, 0, false, 0);
    for(int i = 0; i < n; ++i){
        qp.set_a(i, 0, c[i]);
        qp.set_a(i, 1, -r[i]);
        for(int j = i; j < n; ++j){
            qp.set_d(j, i, 2 * v[i][j]);
        }
    }

    for(int cur = 0; cur < m; ++cur){
        double C, R, V;
        std::cin >> C >> R >> V;
        qp.set_b(0, C);
        qp.set_b(1, -R);
        
        // Solve the quadratic program
        assert(qp.is_nonnegative());
        Solution s = CGAL::solve_nonnegative_quadratic_program(qp, ET());
        assert(s.solves_quadratic_program(qp));
        if(s.status() == CGAL::QP_OPTIMAL){
            ET val = s.objective_value().numerator() / s.objective_value().denominator();
            double min_var = CGAL::to_double(val);
            if(min_var > V){
                std::cout << "No." << "\n";
            }
            else{
                std::cout << "Yes." << "\n";
            }
        }
        else{
            std::cout << "No." << "\n";
        }
    }

}

int main(){
    std::ios_base::sync_with_stdio(false);
    int n, m;
    std::cin >> n >> m;
    while(n && m){
        testcase(n, m);
        std::cin >> n >> m;
    }
    return 0;
}
