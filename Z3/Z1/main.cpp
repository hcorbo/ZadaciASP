#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int dajMaksimum(std::vector<int> &a){
    int velicina = a.size();
    int max = a[0];
    for(int i=0; i<velicina; i++)
    if(a[i] > max)
        max=a[i];
    return max;
}

void radixSort(vector<int> &a){
    int max = dajMaksimum(a);
    std::vector<std::vector<int>> redovi(10);
    int k=1;
    for(int exp=1; max/exp>0; exp*=10){
        for(int broj : a){
            redovi[(broj/k)%10].push_back(broj);
        }
        k = k*10;
        int m=0;
        for(int i=0; i<redovi.size(); i++){
            for(int j=0; j<redovi[i].size(); j++){
                a[m++]=redovi[i][j];
            }
            redovi[i].clear();
        }
    }
    
}
void Zamijeni(int &x, int &y){
    int temp = x;
    x = y;
    y = temp;
}

bool jeLiList(int velicina, int i){
    return (i >= velicina/2 && i < velicina);
}

void popraviDolje(std::vector<int> &v, int velicina, int i){
    while(!jeLiList(velicina, i)){
        int veci = 2*i+1;
        int dd = 2*i+2;
        if(dd < velicina && v[dd] > v[veci])
            veci = dd;
        if(v[i] < v[veci]){
            Zamijeni(v[i], v[veci]);
            i = veci;
        }
        else return;
    }
}

void stvoriGomilu(vector<int> &a){
    for(int i=(a.size()-1)/2; i>=0; i--)
        popraviDolje(a, a.size(), i);
}

void popraviGore(std::vector<int> &v, int i){
    while(i != 0 && v[i]>v[(i-1)/2]){
        Zamijeni(v[i], v[(i-1)/2]);
        i = (i-1)/2;
    }
}
void umetniUGomilu(vector<int> &a, int umetnuti, int &velicina){
    a.push_back(umetnuti);
    velicina++;
    popraviGore(a, velicina-1);
}

int izbaciPrvi(vector<int> &a, int &velicina){
    if(velicina == 0) throw "Nema elemenata";
    Zamijeni(a[0], a[--velicina]);
    if(velicina!=0){
        popraviDolje(a, velicina, 0);
    }
    return a[velicina];
}

void gomilaSort(vector<int> &a){
    stvoriGomilu(a);
    int vel = a.size();
    for(int i=0; i<a.size()-1; i++){
        izbaciPrvi(a, vel); //jer je prvi uvijek najveci element
    }
}

int main() {
 vector<int> a;
    a.push_back(545);
    a.push_back(1545);
    a.push_back(4536);
    a.push_back(124121);
    a.push_back(1);
    a.push_back(95468);
    stvoriGomilu(a);
      int vel = a.size();
      umetniUGomilu(a, 333, vel);
      gomilaSort(a);
    for(int i(0);i<a.size();i++){
        cout<<a[i]<<" ";
    }
    return 0;
}
