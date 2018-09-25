#include <iostream>
#include <vector>

void testcase(){
    int n = 0, k = 0;
    std::cin >> n >> k;
    std::vector<int> cards;
    cards.reserve(n);
    for(int i = 0; i < n; ++i){
        int vi = 0;
        std::cin >> vi;
        cards.push_back(vi);
    }
    int sum = cards[0], min_sum = cards[0];
    int i = 0, j = 0;
    int min_i = 0, min_j = 0;

    while(j < n){
        if(abs(sum - k) < abs(min_sum - k)){
            min_i = i;
            min_j = j;
            min_sum = sum;
        }
        if(sum < k){
            j++;
            sum += cards[j];
        }
        else{
            sum -= cards[i];
            i++;
            if(i > j){
                j = i;
                sum = cards[j];
            }
        }
    }
    std::cout << min_i << " " << min_j << std::endl;
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
