// Delaunay for precomputation + LP

// STL includes
#include <iostream>
#include <vector>
#include <cassert>
#include <climits>
// Delaunay includes
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
// LP includes
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;
typedef CGAL::Delaunay_triangulation_2<IK>  Triangulation;
typedef IK::Point_2 Point;

typedef CGAL::Gmpq ET;
// solution type the solver provides
typedef CGAL::Quotient<ET> SolT;
// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

// round down to next integer
double floor_to_double(const SolT& x){
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a + 1 <= x) a += 1;
    return a;
}

class Warehouse{
    public:
        Point p;
        int s, a;
    Warehouse(): p(Point(0, 0)), s(0), a(0) {}
    Warehouse(double x, double y, int s, int a): p(Point(x,y)), s(s), a(a) {}
};

class Stadium{
    public:
        Point p;
        int d, u;
    Stadium(): p(Point(0, 0)), d(0), u(0) {}
    Stadium(double x, double y, int d, int u): p(Point(x, y)), d(d), u(u) {}
}; 

class Cycle{
    public:
        Point p;
        double r;
    Cycle(): p(Point(0, 0)), r(0) {}
    Cycle(double x, double y, double r): p(Point(x, y)), r(r) {}
};

void testcase(){
    int n, m, c;
    std::cin >> n >> m >> c;
    std::vector<Point> pts;   // warehouse + stadium points, used for delaunay
    pts.reserve(n + m);
    std::vector<Warehouse> houses(n);
    std::vector<Stadium> stadiums(m);
    std::vector<std::vector<int> > revenues(n, std::vector<int>(m, 0));
    std::vector<Cycle> cycles;

    for(int i = 0; i < n; ++i){
        double x, y; int s, a;
        std::cin >> x >> y >> s >> a;
        houses[i] = Warehouse(x, y, s, a);
        pts.push_back(houses[i].p);
    }

    for(int i = 0; i < m; ++i){
        double x, y; int d, u;
        std::cin >> x >> y >> d >> u;
        stadiums[i] = Stadium(x, y, d, u);
        pts.push_back(stadiums[i].p);
    }

    std::vector<std::vector<int> > index(n, std::vector<int>(m, 0));   // LP variable index 
    std::vector<std::vector<int> > t(n, std::vector<int>(m, 0));      // the number of contour lines from w to s
    
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < m; ++j){
            std::cin >> revenues[i][j];
            index[i][j] = i * m + j;
        }
    }
    // Use Delaunay to find and store those cycles that contain points inside
    Triangulation DT;
    DT.insert(pts.begin(), pts.end());
    for(int i = 0; i < c; ++i){
        double x, y, r;
        std::cin >> x >> y >> r;
        Point pc = Point(x, y);
        Triangulation::Vertex_handle vh = DT.nearest_vertex(pc);
        if(CGAL::squared_distance(vh->point(), pc) < r * r)
            cycles.push_back(Cycle(x, y, r));
    }

    // count the number of contour lines t[w][s] by complete search
    if(cycles.size() > 0){
        for(int w = 0; w < n; ++w){
            for(int s = 0; s < m; ++s){
                for(int c = 0; c < cycles.size(); ++c){
                    double cw = CGAL::squared_distance(cycles[c].p, houses[w].p);
                    double cs = CGAL::squared_distance(cycles[c].p, stadiums[s].p);
                    double r = cycles[c].r;
                    if(cw < r * r && cs < r * r)
                       continue;
                    else if(cs < r * r || cw < r * r)
                        t[w][s]++;
                }
            }
        }
    }

    Program lp(CGAL::SMALLER, true, 0, false, 0);
    for(int w = 0; w < n; ++w){
        for(int s = 0; s < m; ++s){
            lp.set_a(index[w][s], w, 1);
        }
        lp.set_b(w, houses[w].s);
    }

    for(int s = 0; s < m; ++ s){
        for(int w = 0; w < n; ++w){
            lp.set_a(index[w][s], n + s, -1);
            lp.set_a(index[w][s], n + m + s, houses[w].a);
            lp.set_a(index[w][s], n + 2 * m + s, 1);  // Attention here: equal constraint!!
            lp.set_c(index[w][s], - 100 * revenues[w][s] + t[w][s]);
        }
        lp.set_b(n + s, -stadiums[s].d);
        lp.set_b(n + m + s, 100 * stadiums[s].u);
        lp.set_b(n + 2 * m + s, stadiums[s].d);      // Attention here! 
    }
    assert(lp.is_nonnegative());
    Solution s = CGAL::solve_nonnegative_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));

    if(s.status() == CGAL::QP_OPTIMAL)
        std::cout << floor_to_double(-s.objective_value() / 100.0) << "\n";
    else
        std::cout << "RIOT!\n";
}

int main(){
    std::ios_base::sync_with_stdio(false);
    std::cout << std::fixed << std::setprecision(0);
    int T = 0;
    std::cin >> T;
    for(int i = 0; i < T; ++i){
        testcase();
    }
    return 0;
}
