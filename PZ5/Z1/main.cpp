#include <iostream>

int fib2_0 (int n, int a=1 , int b=1) { //a-prosli, b-pretprosli
if (n<=2) return a;
    int zbir = a + b;
return fib2_0(n-1, zbir, a);
}

int nzd(int x, int y){
    if(y == 0) return x;
    return nzd(y, x%y);
}

int main() {
    std::cout << nzd(8,12) << std::endl;
    std::cout << fib2_0(10);
    return 0;
}
