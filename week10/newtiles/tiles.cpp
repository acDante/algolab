// Newtiles
#include <iostream>
#include <vector>
#include <climits>
#include <cassert>

void testcase(){
    int h, w;
    std::cin >> h >> w;
    std::vector<int> tiles(h + 1, 0);    // store the integer-encoded bitset
    for(int i = 1; i <= h; ++i){
        for(int j = 0; j < w; ++j){
            int cur; std::cin >> cur;
            if(cur == 1)
                tiles[i] |= (1 << (w - 1 - j));
        }
    }

    int num_mask = (1 << w);
    std::vector<std::vector<int> > maxTiles(h + 1, std::vector<int>(num_mask, -1));
    
    // Initialization
    for(int mask = 0; mask < (1 << w); ++mask){
        maxTiles[1][mask] = 0;
    }

    // Store the maximum among all DP[i][bitmask] for i-th layer 
    std::vector<int> max_among_masks(h + 1, 0); 
    for(int i = 2; i <= h; ++i){
        for(int mask = 0; mask < (1 << w); ++mask){
            if(mask == 0){
                maxTiles[i][mask] = max_among_masks[i - 1];
            }
            if((mask & tiles[i]) == mask){   // if mask consistent with tiles
                int takeLess = 0, takeAll = 0;  // takeLess: optimal results if not taking all tiles, takeAll: result when taking all tiles
                int bitCount = 0;
                int allBits = 0;
                for(int j = 0; j < w; ++j){
                    allBits = (allBits ^ (1 << j));
                    if(mask & (1 << j)){
                        // int unset_bit = mask & !(1 << j);
                        int unset_bit = mask ^ (1 << j);
                        bitCount++;
                        if(maxTiles[i][unset_bit] != -1){
                            takeLess = std::max(takeLess, maxTiles[i][unset_bit]);
                            std::cout << "Reach \n";
                        }

                    }
                }
                // std::cout << allBits << "\n";
                int negated_mask = allBits - mask;
                if(maxTiles[i - 1][negated_mask] != -1){
                    std::cout << "Here \n";
                    takeAll = maxTiles[i - 1][negated_mask] + bitCount / 2;
                }
                maxTiles[i][mask] = std::max(takeLess, takeAll);
                // maxTiles[i][mask] = takeLess;
            }
            max_among_masks[i] = std::max(max_among_masks[i], maxTiles[i][mask]);
        }
    }

    int result = 0;
    for(int mask = 0; mask < (1 << w); ++mask){
        result = std::max(result, maxTiles[h][mask]);
    }
    std::cout << result << '\n';
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int T = 0;
    std::cin >> T;
    for(int i =  0; i < T; ++i){
        testcase();
    }
    return 0;
}
