// Some notes: 注意二维vector的初始化方法！！！！！！
#include <iostream>
#include <vector>

typedef std::vector<std::vector<int> > MAT;

// compute # of even pairs given a vector s
int find_even_pairs(std::vector<int> s){
    int partial_sum = 0;
    int even_num = 0;
    int odd_num = 0;
    int num_even_pairs = 0;
    for(int si : s){
        partial_sum += si;
        if (partial_sum % 2 == 0){
            even_num ++;
        }
        else{
            odd_num ++;
        }
    }
    num_even_pairs = even_num * (even_num - 1) / 2
                     + odd_num * (odd_num - 1) / 2
                     + even_num;
    return num_even_pairs;
}

void testcase(){
    int n = 0;
    std::cin >> n;
    MAT mat(n, std::vector<int>(n));
    MAT col_sum(n, std::vector<int>(n));

    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            std::cin >> mat[i][j];
            if(i == 0){
                col_sum[i][j] = mat[i][j];
            }
            else{
                col_sum[i][j] = col_sum[i - 1][j] + mat[i][j];
            }
        }
    }

    int num_even_pairs = 0;

    for(int i = 0; i < n; ++i){
        for(int j = i; j < n; ++j){
            if(i == 0){
                num_even_pairs += find_even_pairs(col_sum[j]);
            }
            else{
                std::vector<int> diff_sum;
                for(int k = 0; k < n; ++k){
                    diff_sum.push_back(col_sum[j][k] - col_sum[i - 1][k]);
                }
                num_even_pairs += find_even_pairs(diff_sum);
            }
        }
    }
    std::cout << num_even_pairs << std::endl;
}

int main(){
    int t = 0;
    std::cin >> t;
    for(int i = 0; i < t; ++i){
        testcase();
    }
    return 0;
}
