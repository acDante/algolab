#include <iostream>
void testcase(){
    std::string a, b, c, d;
    std::cin >> a >> b;
    std::cout << a.length() << " " << b.length() << std::endl;
    c = a + b;
    std::cout << c << std::endl;
    
    std::string a_end = a.substr(a.length() - 1, 1);
    std::string b_end = b.substr(b.length() - 1, 1);
    a.replace(a.length() - 1, 1, b_end);
    b.replace(b.length() - 1, 1, a_end);

    std::copy(a.crbegin(), a.crend(), std::back_inserter(d));
    std::cout << d << " ";
    d.clear();
    std::copy(b.crbegin(), b.crend(), std::back_inserter(d));
    std::cout << d << std::endl;
}

int main(){
    int t = 0;
    std::cin >> t;
    for(int i = 0; i < t; ++i){
        testcase();
    }
    return 0;
}