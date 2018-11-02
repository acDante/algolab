 // some problems in memroy assignment ..?
#include <iostream>
#include <vector>
#include <algorithm>

typedef std::pair<int, int> POS;   // store <position, word_idx>
void testcase(){
    int n = 0;
    std::cin >> n;
    std::vector<int> m;
    m.reserve(n);
    for(int i = 0; i < n; ++i){
        int mi = 0;
        std::cin >> mi;
        m.push_back(mi);
    }

    std::vector<POS> words;
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < m[i]; ++j){
            int pij = 0;
            std::cin >> pij;
            words.push_back(std::make_pair(pij, i));
        }
    }
    std::sort(words.begin(), words.end());

    std::vector<double> occur(n, 0);    // store occurrence time of each word
    
    int i = 0, j = 0;
    int num_word = 1, min_length = INT32_MAX;
    occur[words[0].second] = 1;

    while(j < words.size()){
        if(num_word < n){
            j++;
            if(j == words.size()) break;
            occur[words[j].second]++;
            if(occur[words[j].second] == 1){
                num_word++;
            }
            
        }
        else{
            int length = words[j].first - words[i].first + 1;
            if(length < min_length){
                min_length = length;
            }
            
            occur[words[i].second]--;
            if(occur[words[i].second] == 0){
                num_word--;
            }
            i++; 
        }
    }
    std::cout << min_length << std::endl;
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
