// Delaunay trianguation to pre-compute radius + LP

#include <iostream>
#include <vector>
#include <cassert>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

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

typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;
typedef CGAL::Delaunay_triangulation_2<IK>  Triangulation;
typedef IK::Point_2 Point;

class Asteroid{
    public:
        Point pos;
        int density;
    Asteroid(): pos(Point(0, 0)), density(0) {}
    Asteroid(Point p, int d): pos(p), density(d) {}
    Asteroid(long x, long y, int d): pos(Point(x, y)), density(d) {}
};

class Shoot{
    public:
        Point pos;
        long radius;
    Shoot(): pos(Point(0, 0)), radius(INT_MAX) {} // <- Attention here !
    Shoot(long x, long y): pos(Point(x, y)), radius(INT_MAX) {} // <- Attention here !
};

// round up to next integer double
double ceil_to_double(const SolT& x)
{
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return a;
}

void testcase(){
    int A, S, B;    // A: # of asteriod particles, S: # of laser shots, B: # of bounty hunters 
    std::cin >> A >> S >> B;
    int e; std::cin >> e;   // e: energy upper bound
    std::vector<Asteroid> particles(A);
    for(int i = 0; i < A; ++i){
        long x, y; int d;
        std::cin >> x >> y >> d;
        particles[i] = Asteroid(x, y, d);
    }

    std::vector<Shoot> shoots(S);
    for(int i = 0; i < S; ++i){
        long x, y; 
        std::cin >> x >> y;
        shoots[i] = Shoot(x, y);
    }

    std::vector<Point> hunters(B);
    for(int i = 0; i < B; ++i){
        long x, y;
        std::cin >> x >> y;
        hunters[i] = Point(x, y);
    }

    Triangulation t;
    t.insert(hunters.begin(), hunters.end());
    
    // Pre-compute the radius of each shoot as the distance to nearest hunter
    Program lp(CGAL::LARGER, true, 0, false, 0);
    for(int i = 0; i < S; ++i){
        Point cur_shoot = shoots[i].pos;
        if(B > 0){   // <- Attention here !
            Triangulation::Vertex_handle v = t.nearest_vertex(cur_shoot);
            shoots[i].radius = CGAL::squared_distance(v->point(), cur_shoot);
        }
        lp.set_c(i, 1);
    }

    for(int i = 0; i < A; ++i){
        Point p1 = particles[i].pos;
        for(int j = 0; j < S; ++j){
            Point p2 = shoots[j].pos;
            long d2 = CGAL::squared_distance(p1, p2);
            if(d2 <= shoots[j].radius){
                lp.set_a(j, i, ET(1) / std::max<long>(1, d2));
            }
        }
        lp.set_b(i, particles[i].density);
    }
    assert(lp.is_nonnegative());
    Solution s = CGAL::solve_nonnegative_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));

    if(s.status() != CGAL::QP_OPTIMAL){
        std::cout << "n\n";
    }
    else{
        if(s.objective_value() > e)
            std::cout << "n\n";
        else
            std::cout << "y\n";
    }
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int t = 0; std::cin >> t;
    for(int i = 0; i < t; ++i){
        testcase();
    }
    return 0;
}