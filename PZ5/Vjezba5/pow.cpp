#include <iostream>

double powe (double a, unsigned int n){
    if(n == 0) return 1;
    else if(n == 2){
        double m = powe(a, n/2);
        return m*m;
    }
    else return a*powe(a, n-1);
}

int main(){
    double stepen = powe(2, 4);
    std::cout << stepen;
}

// za skidanje sa steka
int fun (Stek<int> &s){
    if(s.brEl()==0) return 0;
    int a = s.skini();
    fun(s);
    s.stavi(a);
}