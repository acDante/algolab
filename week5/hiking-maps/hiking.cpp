/* Method
1. Use left_turn() to check whether a point/path lies inside a triangle, store the bool value in contained[i][j]
2. Find the optimal interval by sliding window
*/

#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;
typedef K::Segment_2 Segment;

int NUM = 6;
bool inTriangle(Point p, std::vector<Point>& trgPoints){  // 记得传引用变量！！！！
    for(int i = 0; i < NUM; i += 2){
        if(CGAL::left_turn(trgPoints[i], trgPoints[i + 1], trgPoints[(i + 2) % NUM])){
            std::swap(trgPoints[i], trgPoints[i + 1]);
        }
    }

    return !CGAL::left_turn(trgPoints[0], trgPoints[1], p) 
           && !CGAL::left_turn(trgPoints[2], trgPoints[3], p) 
           && !CGAL::left_turn(trgPoints[4], trgPoints[5], p);
}

void testcase(){
    int m, n;  // m: # of legs, n: # of maps
    std::cin >> m >> n;
    std::vector<Point> pts(m);
    for(int i = 0; i < m; ++i){
        int x, y;
        std::cin >> x >> y;
        pts[i] = Point(x, y);
    }

    std::vector<std::vector<Point> > maps(n, std::vector<Point>());
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < NUM; ++j){
            int x, y;
            std::cin >> x >> y;
            Point pi(x, y);
            maps[i].push_back(pi);
        }
    }

    std::vector<std::vector<bool> > contained(n, std::vector<bool>());  // contained[i][j] := whether map i contains j-th path
    std::vector<std::vector<bool> > inmap(n);  // inmap[i][j] := whether map i contains j-th point
    for(int i = 0; i < n; ++i){
        contained[i] = std::vector<bool>(m - 1, 0);
        inmap[i] = std::vector<bool>(m, 0);
    }
    
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < m; ++j){
            inmap[i][j] = inTriangle(pts[j], maps[i]);
            if(j >= 1){
                if(inmap[i][j] && inmap[i][j - 1])
                    contained[i][j - 1] = 1;
            }
        }
        // std::cout << std::endl;
    }

    std::vector<int> occur(m - 1, 0); // record the accumulated occurrence times of each path
    int num = 0, b = 0, e = 0;
    int minCost = INT32_MAX;
    while(e <= n){
        // std::cout << "num = " << num << ", m =" << m - 1 << std::endl;
        // std::cout << "b = " << b << "e = " << e << std::endl;
        if(num == m - 1){
            minCost = std::min(minCost, e - b);
            for(int k = 0; k < m - 1; ++k){
                if(contained[b][k]){
                    occur[k]--;
                    if(occur[k] == 0)
                        num--;
                }
            }
            b++;
            if(b > e)
                e = b;
        }
        else{
            if(e == n) break;
            for(int k = 0; k < m - 1; ++k){
                if(contained[e][k]){
                    occur[k]++;
                    if(occur[k] == 1)
                        num++;
                }
            }
            e++;
        }

    }
    std::cout << minCost << "\n";
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin >> t;
    for(int i = 0; i < t; ++i){
        testcase();
    }
    return 0;
}
