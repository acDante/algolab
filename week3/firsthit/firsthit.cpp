// trick: shorten the ray into segment at each intersecting point! 剪枝！
#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <algorithm>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 Point;
typedef K::Segment_2 Segment;
typedef K::Ray_2 Ray;

double floor_to_double(const K::FT& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

void testcase(int n){
    long x, y, a, b;
    std::cin >> x >> y >> a >> b;
    Point p1(x, y), p2(a, b);
    Ray ray(p1, p2);
    std::vector<Segment> segs;
    segs.reserve(n);
    for(int i = 0; i < n; ++i){
        long r, s, t, u;
        std::cin >> r >> s >> t >> u;
        Point px(r, s), py(t, u);
        segs.push_back(Segment(px, py));
    }
    Point firstHit;
    Segment raySegment;
    bool hitted = false;
    std::random_shuffle(segs.begin(), segs.end());
    for(int i = 0; i < n; ++i){
        if(!hitted){
            if(CGAL::do_intersect(ray, segs[i])){
                hitted = true;   // find an intersecting point
                auto o = CGAL::intersection(ray, segs[i]);
                if (const Point* op = boost::get<Point>(&*o)){ // intersection is a point
                    firstHit = *op;
                    raySegment = Segment(p1, firstHit);
                }  
                else if (const Segment* os = boost::get<Segment>(&*o)){  // intersection is a segment
                    K::FT dist_source = CGAL::squared_distance(p1, os->source());
                    K::FT dist_target = CGAL::squared_distance(p1, os->target());
                    if(dist_source >= dist_target){
                        firstHit = os->target();
                        raySegment = Segment(p1, os->target());
                    }
                    else{
                        firstHit = os->source();
                        raySegment = Segment(p1, os->source());
                    }
                }
            }
        }
        else{
            if(CGAL::do_intersect(raySegment, segs[i])){
                hitted = true;   // find an intersecting point
                auto o = CGAL::intersection(raySegment, segs[i]);
                if (const Point* op = boost::get<Point>(&*o)){ // intersection is a point
                    firstHit = *op;
                    raySegment = Segment(p1, firstHit);
                }  
                else if (const Segment* os = boost::get<Segment>(&*o)){  // intersection is a segment
                    K::FT dist_source = CGAL::squared_distance(p1, os->source());
                    K::FT dist_target = CGAL::squared_distance(p1, os->target());
                    if(dist_source >= dist_target){
                        firstHit = os->target();
                        raySegment = Segment(p1, os->target());
                    }
                    else{
                        firstHit = os->source();
                        raySegment = Segment(p1, os->source());
                    }
                }
            }
        }
    }
    if(hitted)
        std::cout << floor_to_double(firstHit.x()) 
                  << " " << floor_to_double(firstHit.y()) << std::endl; 
    else
        std::cout << "no" << std::endl;
}

int main(){
    int n = 0;
    std::cin >> n;
    std::cout << std::fixed << std::setprecision(0);
    while(n != 0){
        testcase(n);
        std::cin >> n;
    }
    return 0;
}