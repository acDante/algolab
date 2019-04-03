#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <cassert>
#include <climits>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel   EK;
typedef EK::Point_2 Point;
typedef CGAL::Triangulation_vertex_base_2<EK> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<EK::FT, EK> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<EK,Tds> Triangulation;

typedef Triangulation::Face_handle Face_handle;
typedef Triangulation::Vertex_handle Vertex_handle;
typedef std::pair<EK::FT, Face_handle> PF;

// bool comp(PF& fa, PF& fb){
//     return fa.second < fb.second;
// }
EK::FT MAX_DIST = pow(2, 103);   // Attention here: estimate the upper bound with input
void testcase(){
    int n, m;
    long r;
    std::cin >> n >> m >> r;
    std::vector<Point> pts(n);
    for(int i = 0; i < n; ++i){
        long x, y;
        std::cin >> x >> y;
        pts[i] = Point(x, y);
    }
    Triangulation t;
    t.insert(pts.begin(), pts.end());

    // Precompute the maximal possible clearance starting from each face
    // following a Dijkstra's-like apprach
    std::priority_queue<PF> pq;
    for(auto f = t.all_faces_begin(); f != t.all_faces_end(); f++){
        if(t.is_infinite(f)){
            f->info() = MAX_DIST;
        }
        else{
            // compute the circumradius for each face
            Vertex_handle v1 = f->vertex(0);
            Vertex_handle v2 = f->vertex(1);
            Vertex_handle v3 = f->vertex(2);
            f->info() = CGAL::squared_radius(v1->point(), v2->point(), v3->point());
            // Point center = t.dual(f);  // will stuck.....
            // info = CGAL::squared_distance(f->vertex(0)->point(), center);
        }
        pq.push(std::make_pair(f->info(), f));
    }
    
    while(!pq.empty()){
        Face_handle cur_f = pq.top().second;
        EK::FT clearance = pq.top().first;
        pq.pop();
        // current face has been optimised 
        if(clearance < cur_f->info())
            continue;
        // Edge relaxation for finite neighbor faces
        for(int i = 0; i < 3; ++i){
            Face_handle nb = cur_f->neighbor(i);
            if(!t.is_infinite(nb)){
                EK::FT l2 = t.segment(cur_f, i).squared_length();
                EK::FT new_info = std::min(cur_f->info(), l2);
                if(new_info > nb->info()){
                    nb->info() = new_info;
                    pq.push(std::make_pair(new_info, nb));
                }
            }
        }
    }

    for(int i = 0; i < m; ++i){
        long x, y, s;
        std::cin >> x >> y >> s;
        Point p(x, y);
        Vertex_handle vh = t.nearest_vertex(p);
        EK::FT dist = CGAL::squared_distance(vh->point(), p);
        EK::FT radius = r + s;    // Attention: EK::FT((r + s) * (r + s)) does not work!!!
        
        if(dist < radius * radius){
            std::cout << "n";
            continue;
        }
        Face_handle cur = t.locate(p);
        if(cur->info() >= 4 * radius * radius){
            std::cout << "y";
        }
        else
            std::cout << "n";
    }
    std::cout << "\n";
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
