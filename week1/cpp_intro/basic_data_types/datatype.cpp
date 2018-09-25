#include <iostream>
#include <iomanip>

void testcase(){
    int a; long long b; std::string c; double d;
    std::cin >> a >> b >> c >> d;
    std::cout << a << " " << b << " " << c << " ";
    std::cout << std::fixed << std::setprecision(2) << d << std::endl;
}

int main(){
    int T = 0;
    std::cin >> T;
    for(int i = 0; i < T; ++i){
        testcase();
    }
    return 0;
}
