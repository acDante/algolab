/** Final solution: Linear programming + some smart preprocessing
 *  Idea: 1. Set constraint according to some conditions 
 *        2. Try to reduce the total number of constraints 
 *        3. Attention: use "double" instead of "int" to store the points and do other computation
 */
#include <iostream>
#include <vector>
#include <cassert>
#include <cmath>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

// choose exact rational type
#include <CGAL/Gmpq.h>
typedef CGAL::Gmpq ET;
// solution type the solver provides
typedef CGAL::Quotient<ET> SolT;
// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;

typedef IK::Point_2 Point;
// round up to next integer double
double ceil_to_double(const SolT& x)
{
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return a;
}

void print(std::vector<int> v){
    for(int i : v){
        std::cout << i << " ";
    }
    std::cout << "\n";
}

void testcase(){
    int n, m, h, w; // n: # of new posters, m: # of old posters, h: height, w: width
    std::cin >> n >> m >> h >> w;
    std::vector<Point> new_nails(n);
    std::vector<Point> old_nails(m);

    for(int i = 0; i < n; ++i){
        double x, y; 
        std::cin >> x >> y;
        new_nails[i] = Point(x, y);
    }

    for(int i = 0; i < m; ++i){
        double x, y; 
        std::cin >> x >> y;
        old_nails[i] = Point(x, y);
    }

    Program lp(CGAL::SMALLER, true, 1, false, 0);
    int num_constraints = 0;
    for(int i = 0; i < n; ++i){
        for(int j = i + 1; j < n; ++j){
            double delta_y = abs(new_nails[i].y() - new_nails[j].y());
            double delta_x = abs(new_nails[i].x() - new_nails[j].x());
            if(delta_y * w < delta_x * h){  // delta_y / delta_x < h / w
                lp.set_a(i, num_constraints, w);
                lp.set_a(j, num_constraints, w);
                lp.set_b(num_constraints, 2 * delta_x);
            }
            else{
                lp.set_a(i, num_constraints, h);
                lp.set_a(j, num_constraints, h);
                lp.set_b(num_constraints, 2 * delta_y);
            }
            num_constraints++;
        }
    }

    for(int i = 0; i < n; ++i){
        lp.set_c(i, - 1);

        // Find min{2 * delta_x - w} and min{2 * delta_y - h}
        double min_x = INT32_MAX, min_y = INT32_MAX;
        for(int j = 0; j < m; ++j){
            double delta_y = abs(new_nails[i].y() - old_nails[j].y());
            double delta_x = abs(new_nails[i].x() - old_nails[j].x());
            if(delta_y * w < delta_x * h)
                min_x = std::min(min_x, 2 * delta_x - w);
            else
                min_y = std::min(min_y, 2 * delta_y - h);
        }

        // Decide whether to set constraint on x or y
        bool set_x = true;  
        if(min_x != INT32_MAX && min_y != INT32_MAX){
            if(h * min_x >= w * min_y){ // if (delta_y / h) < (delta_x / w) : set constraint on y
                set_x = false;
            }
        }
        else if(min_y != INT32_MAX){  // if without constraint on x 
            set_x = false;
        }

        if(set_x){
            lp.set_a(i, num_constraints, w);
            lp.set_b(num_constraints, min_x);
        }
        else{
            lp.set_a(i, num_constraints, h);
            lp.set_b(num_constraints, min_y);
        }
        num_constraints++;
    }

    // assert(lp.is_nonnegative());
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));
    if(s.status() == CGAL::QP_OPTIMAL){
        double max_perimeter = ceil_to_double(-s.objective_value() * 2 * (h + w));
        std::cout << max_perimeter << "\n";
    }
}

int main(){
    std::ios_base::sync_with_stdio(false);
    std::cout << std::fixed << std::setprecision(0);
    int t = 0;
    std::cin >> t;
    for(int i = 0; i < t; ++i){
        testcase();
    }
}
