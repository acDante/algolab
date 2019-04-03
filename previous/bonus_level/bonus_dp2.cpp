/* Dynamic programming solution (iterative way)
* Can be considered as: two people start from top-left corner and go to bottom-right corner
* A naive formulation is dp(row1, col1, row2, col2), and it can be optimized to 3 states (since there exist dependency between row and col indices)
* dp(k, row1, row2) := max number of coins that can be collected until k-th step with two positions specified by row indices 
*/
#include <iostream>
#include <vector>
#include <algorithm>

typedef std::vector<std::vector<std::vector<int> > > VVVI;
typedef std::vector<std::vector<int> > VVI;
typedef std::vector<int> VI;

bool isValid(int n, int k, int x){
    int y = k - x;
    return (x >= 0 && x < n) && (y >= 0 && y < n);
}

void testcase(){
    int n;
    std::cin >> n;
    std::vector<std::vector<int> > a(n, std::vector<int>(n, 0));
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            std::cin >> a[i][j];
        }
    }
    VVVI maxCoin(2 * n - 1, VVI(n, VI(n, -1)));
    maxCoin[0][0][0] = a[0][0];
    for(int k = 1; k < 2 * n - 1; ++k){
        for(int x1 = 0; x1 < n; x1++){
            for(int x2 = 0; x2 < n; x2++){
                // deal with invalid indices
                if(!isValid(n, k, x1) || !isValid(n, k, x2))
                    continue;
                // Avoid adding the same reward repeatedly
                int reward = (x1 == x2) ? a[x1][k - x1] : a[x1][k - x1] + a[x2][k - x2];
                std::vector<int> dx(2, 0); dx[1] = -1;
                int res = 0;
                for(int i = 0; i < 2; ++i){
                    for(int j = 0; j < 2; ++j){
                        // Be really careful with boundary check!!!!
                        if(isValid(n, k - 1, x1 + dx[i]) && isValid(n, k - 1, x2 + dx[j])){
                            res = std::max(res, maxCoin[k - 1][x1 + dx[i]][x2 + dx[j]]);
                        }
                    }
                }
                maxCoin[k][x1][x2] = res + reward;
            }
        }
    }
    std::cout << maxCoin[2 * n - 2][n - 1][n - 1] << "\n";
}

int main(){
    int T = 0;
    std::cin >> T;
    for(int i = 0; i < T; ++i){
        testcase();
    }
    return 0;
}
