// Triangulation + Linear Programming
#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

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

class Info{
    public:
        int u, v, w;
        Info() : u(0), v(0), w(0) {}
        Info(int u, int v, int w) : u(u), v(v), w(w) {}
};

typedef CGAL::Exact_predicates_inexact_constructions_kernel         K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K>        Vb;
typedef CGAL::Triangulation_data_structure_2<Vb>                    Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>                      Delaunay;
typedef Delaunay::Point                                             Point;
typedef Delaunay::Vertex_handle                                     Vertex_handle;

void testcase(){
    int z, u, v, w;
    std::cin >> z >> u >> v >> w;
    int a, g;    // a: # of agents, g: # of gang members
    std::cin >> a >> g;
    std::vector<std::pair<Point, int> > gangs(g);
    std::vector<Info> info(g);
    std::vector<int> spy(g, -1);    // store the index of agent that watches gang member i
    for(int i = 0; i < g; ++i){
        long x, y; int ui, vi, wi;
        std::cin >> x >> y >> ui >> vi >> wi;
        gangs[i] = std::make_pair(Point(x, y), i);
        info[i] = Info(ui, vi, wi);
    }
    Delaunay t;
    t.insert(gangs.begin(), gangs.end());

    std::vector<int> cost(a, 0);
    Program lp (CGAL::LARGER, true, 0, true, 24);
    for(int i = 0; i < a; ++i){
        long x, y; int zi;
        std::cin >> x >> y >> zi;
        Point p(x, y);
        Vertex_handle vh = t.nearest_vertex(p);

        // If a gang member is watched by multiple agents
        // assign the agent with least cost to this gang member
        if(spy[vh->info()] == -1)
            spy[vh->info()] = i;
        else if(zi < cost[spy[vh->info()]])
            spy[vh->info()] = i;
        cost[i] = zi;
    }

    for(int i = 0; i < g; ++i){
        int agent = spy[i];
        if(spy[i] != -1){    // the gang member is watched by an agent
            lp.set_a(agent, 0, info[i].u);
            lp.set_a(agent, 1, info[i].v);
            lp.set_a(agent, 2, info[i].w);

            lp.set_c(agent, cost[agent]);
        }
    }
    lp.set_b(0, u);
    lp.set_b(1, v);
    lp.set_b(2, w);
    
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));
    if (s.is_optimal()){
        if(s.objective_value() > z)
            std::cout << "H\n";
        else
            std::cout << "L\n";
    }
    else
        std::cout << "H\n";
}

int main(){
    int T = 0;
    std::cin >> T;
    for(int i = 0; i < T; ++i){
        testcase();
    }
    return 0;
}
