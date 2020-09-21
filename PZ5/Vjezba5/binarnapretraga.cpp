#include <iostream>

template <typename Tip>
int binarna_pretraga(Tip *niz, int n, Tip element){
    for (int i=0; i<n; i++){
        int srednja = (i+n)/2;
        
        if(element == niz[srednja]) return srednja;
        else if(element<niz[srednja]){
            n = srednja;
        } 
        else {
            i=srednja;
        }
    }
    return -1;
}

int main(){
    int niz [5] = {1, 2, 4, 7, 9};
    int x = binarna_pretraga(niz, 5, 6);
    std::cout<< "Element je: " << x;
    return 0;
}