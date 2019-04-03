// Delaunay Triangulation + LP + Binary Search the result
#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

typedef CGAL::Gmpq ET;
// solution type the solver provides
typedef CGAL::Quotient<ET> SolT;
// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Vertex_handle  Vertex_handle;
typedef K::Point_2 Point;

class Sensor{
    public:
        Point p; int E;
        Sensor(): p(Point(0, 0)), E(0) {}
        Sensor(long x, long y, int e): p(Point(x, y)), E(e) {}
};

class MPE{
    public:
        Point p; double squared_r;
        MPE(): p(Point(0, 0)), squared_r(INT64_MAX) {}
        MPE(long x, long y): p(Point(x, y)), squared_r(INT64_MAX) {}
};

// check if the chain [p0,..,pk-1] is enough to deactivate all sensors
bool checkChain(int k, int Imax, std::vector<Sensor>& sensors, std::vector<MPE>& mpe){
    Program lp (CGAL::LARGER, true, 0, false, 0);
    for(int i = 0; i < sensors.size(); ++i){
        for(int j = 0; j < k; ++j){
            double dist = CGAL::squared_distance(sensors[i].p, mpe[j].p);
            if(dist <= mpe[j].squared_r)
                lp.set_a(j, i, ET(1 / dist));
        }
        lp.set_b(i, sensors[i].E);
    }
    for(int i = 0; i < k; ++i)
        lp.set_c(i, 1);
    
    assert(lp.is_nonnegative());
    Solution s = CGAL::solve_nonnegative_linear_program(lp, ET());
    assert(s.solves_nonnegative_linear_program(lp));
    if(s.is_optimal()){
        return s.objective_value() <= Imax;
    }
    return false;
}

void testcase(){
    int n, m, h, Imax;    // n: # of sensors, m: # of MPEs, h: # of henchmen
    std::cin >> n >> m >> h >> Imax;
    std::vector<Sensor> sensors(n);
    std::vector<MPE> mpe(m);
    for(int i = 0; i < n; ++i){
        long x, y; int E;
        std::cin >> x >> y >> E;
        sensors[i] = Sensor(x, y, E);
    }
    for(int i = 0; i < m; ++i){
        long x, y;
        std::cin >> x >> y;
        mpe[i] = MPE(x, y);
    }

    std::vector<Point> hench(h);
    for(int i = 0; i < h; ++i){
        long x, y;
        std::cin >> x >> y;
        hench[i] = Point(x, y);
    }

    if(hench.size() > 0){   // Attention: remember to do sanity check !!
        Triangulation t;
        t.insert(hench.begin(), hench.end());
        for(int i = 0; i < m; ++i){
            Point pi = mpe[i].p;
            Vertex_handle vh = t.nearest_vertex(pi);
            mpe[i].squared_r = CGAL::squared_distance(vh->point(), pi);
        }
    }
    // Binary Search for optimal k
    int low = 0, high = m;
    int result = -1;
    while(low <= high){
        int mid = (low + high) / 2;
        if(checkChain(mid, Imax, sensors, mpe)){
            result = mid;
            high = mid - 1;
        }
        else
            low = mid + 1;
    }
    if(result != -1)
        std::cout << result << "\n";
    else
        std::cout << "impossible\n";
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int T = 0;
    std::cin >> T;
    for(int i = 0; i < T; ++i){
        testcase();
    }
    return 0;
}
