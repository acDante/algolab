/**
 * Algorithm: Delaunay Triangulation + Binary Search, find the last lamp that has survivors.
 * Some tips to increase speed: early stopping in BinarySearch()
 */  
#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef K::Point_2 Point;
typedef Triangulation::Vertex_handle Vertex_handle;

class Participant{
    public:
        Point p;
        long r;
        Participant(Point p, long r = 0) :p(p), r(r) {}
        Participant(long x = 0, long y = 0, long r = 0): p(Point(x,y)), r(r) {}
};

void print(std::set<int> survivors);
void findSurvivors(std::vector<Participant>& ps, std::vector<Point>& lights, std::set<int>& survivors, int mid, int h);
int binarySearch(std::vector<Participant>& ps, std::vector<Point>& lights, int h);

void print(std::set<int> survivors){
    for(int si : survivors){
        std::cout << si << " ";
    }
    std::cout << "\n";
}

void findSurvivors(std::vector<Participant>& ps, std::vector<Point>& lights, 
                   std::set<int>& survivors, int mid, int h){
    Triangulation DT;
    DT.insert(lights.begin(), lights.begin() + mid);
    survivors.clear();
    for(int i = 0; i < ps.size(); ++i){
        Vertex_handle vh = DT.nearest_vertex(ps[i].p);
        long d2 = CGAL::squared_distance(vh->point(), ps[i].p);
        if(d2 >= (h + ps[i].r) * (h + ps[i].r)){
            survivors.insert(i);
        }
    }
}

int binarySearch(std::vector<Participant>& ps, std::vector<Point>& lights, int h){
    // Find the last light that has surviviors 
    int low = 0, high = lights.size();
    int index = 0;
    while(low <= high){
        int mid = (low + high) / 2;
        Triangulation DT;
        DT.insert(lights.begin(), lights.begin() + mid);
        bool hasSurvivors = false;
        for(int i = 0; i < ps.size(); ++i){
            Vertex_handle vh = DT.nearest_vertex(ps[i].p);
            long d2 = CGAL::squared_distance(vh->point(), ps[i].p);
            if(d2 >= (h + ps[i].r) * (h + ps[i].r)){
                hasSurvivors = true;
                break;
            }
        }
        if(hasSurvivors){
            index = mid;
            low = mid + 1;
        }
        else{
            high = mid - 1;
        }
    }
    return index;
}

void testcase(){
    int m, n;  // m := # of participants, n := # of lamps
    std::cin >> m >> n;
    std::vector<Participant> ps(m);
    for(int i = 0; i < m; ++i){
        long x, y, r;
        std::cin >> x >> y >> r;
        ps[i] = Participant(x, y, r);
    }

    long h; std::cin >> h;
    std::vector<Point> lights(n);
    for(int i = 0; i < n; ++i){
        long x, y;
        std::cin >> x >> y;
        lights[i] = Point(x, y);
    }

    Triangulation DT;
    DT.insert(lights.begin(), lights.end());

    std::set<int> survivors;
    for(int i = 0; i < m; ++i){
        Vertex_handle vh = DT.nearest_vertex(ps[i].p);
        long d2 = CGAL::squared_distance(vh->point(), ps[i].p);
        if(d2 >= (h + ps[i].r) * (h + ps[i].r)){
            survivors.insert(i);
        }
    }

    if(!survivors.empty())
        print(survivors);
    
    else{
        int index = binarySearch(ps, lights, h);
        findSurvivors(ps, lights, survivors, index, h);
        print(survivors);
    }
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int t = 0;
    std::cin >> t;
    for(int i = 0; i < t; ++i){
        testcase();
    }
    return 0;
}
