#include <iostream>
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
  while (a - 1 >= x) a -= 1;
  return a;
}

// round down to next integer double
double floor_to_double(const SolT& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a + 1 <= x) a+= 1;
  return a;
}

void testcase(int p, int a, int b){
    Program qp;
    const int X = 0, Y = 1, Z = 2;
    if(p == 1){
        qp = Program(CGAL::SMALLER, true, 0,false, 0);
        qp.set_a(X, 0, 1); qp.set_a(Y, 0, 1);
        qp.set_b(0, 4);
        qp.set_a(X, 1, 4); qp.set_a(Y, 1, 2);
        qp.set_b(1, a * b);
        qp.set_a(X, 2, -1); qp.set_a(Y, 2, 1);
        qp.set_b(2, 1);

        qp.set_d(X, X, 2 * a);
        qp.set_c(Y, -b);
    }
    else if(p == 2){
        qp = Program(CGAL::LARGER, false, 0, false, 0);
        qp.set_a(X, 0, 1); qp.set_a(Y, 0, 1); 
        qp.set_b(0, -4);
        qp.set_a(X, 1, 4); qp.set_a(Y, 1, 2); qp.set_a(Z, 1, 1);
        qp.set_b(1, - a * b);
        qp.set_a(X, 2, -1); qp.set_a(Y, 2, 1);
        qp.set_b(2, -1);

        qp.set_c(Y, b);
        qp.set_d(X, X, 2 * a);
        qp.set_d(Z, Z, 2);

        qp.set_u(X, true, 0);
        qp.set_u(Y, true, 0);
        qp.set_l(Z, true, 0); // Z= z^2 >= 0
    }

    Solution s = CGAL::solve_quadratic_program(qp, ET());
    assert(s.solves_quadratic_program(qp));
    switch(s.status()){
        case CGAL::QP_INFEASIBLE:{
            std::cout << "no" << "\n";
            break;
        }
        case CGAL::QP_UNBOUNDED:{
            std::cout << "unbounded" << "\n";
            break;
        }
        case CGAL::QP_OPTIMAL:{
            SolT val = s.objective_value();
            if(p == 1)
                std::cout << (long)floor_to_double(-val) << "\n";  // use type cast to avoid "-0"
            if(p == 2)
                std::cout << (long)ceil_to_double(val) << "\n";
            break;
        }
    }
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int p, a, b;
    std::cin >> p;
    std::cout << std::fixed << std::setprecision(0);
    while(p != 0){
        std::cin >> a >> b;
        testcase(p, a, b);
        std::cin >> p;
    }
    return 0;
}
