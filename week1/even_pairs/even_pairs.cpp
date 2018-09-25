#include <iostream>
#include <vector>

void testcase(){
    int n = 0;
    std::cin >> n;
    std::vector<int> sum;
    sum.reserve(n);
    int s = 0;
    int odd_num = 0;
    int even_num = 0;
    for(int i = 0; i < n; ++i){
        int xi = 0;
        std::cin >> xi;
        s += xi;
        if(s % 2 == 0){
            even_num++;
        }
        else{
            odd_num++;
        }
        sum.push_back(s);
    }
    int num_even_pair = even_num 
                        + even_num * (even_num - 1) / 2.0 
                        + odd_num * (odd_num - 1) / 2.0;
    std::cout << num_even_pair << std::endl;

}

int main(){
    int t = 0;
    std::cin >> t;
    for(int i = 0; i < t; ++i){
        testcase();
    }
    return 0;
}
