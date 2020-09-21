#include <iostream>
#include <cstring>
#include <fstream>
#include <ctime>

template <typename Tip>
void selection_sort(Tip  *niz, int vel){
    for(int i = 0; i<vel-1; i++){
        Tip min = niz[i];
        int pmin = i;
        for(int j=i+1; j<vel; j++){
            if(niz[j] < min){
                min = niz[j];
                pmin = j;
            }
        }
        niz[pmin] = niz[i];
        niz[i] = min;
    }
}

template <typename Tip>
void bubble_sort(Tip *niz, int vel){
    for(int i = vel-1; i>0; i--){
        bool sortiran = false;
        for(int j = 1; j<=i; j++){
            if(niz[j-1]>niz[j]){
                Tip temp = niz[j];
                niz[j] = niz[j-1];
                niz[j-1] = temp;
                sortiran = true;
            }
        }
        if(sortiran==false) break;
    }
}

template <typename Tip>
void merge(Tip *niz, int l, int p, int q, int u){
    int i = 0;  //indeks pozicije iznad kojeg su nerasporedjeni elementi prvog podniza
    int j = q-l; //indeks pozicije iznad kojeg su nerasporedjeni elementi drugog podniza
    int  k = l; //indeks pozicije u rezultujucem nizu 
    Tip *B = new Tip[u-l+1];    //niz u koji privremeno smjestamo sortirane podnizove
    for(int m=0; m<=u-l; m++){
        B[m] = niz[l+m];
    }
    while(i <= p-l && j<= u-l){
        if(B[i] < B[j]) //manji element stavljamo u niz
            niz[k] = B[i++];
        else 
            niz[k] = B[j++];
        k++;
    } 
    while(i <= p-l)
        niz[k++] = B[i++];
    while(j <= u-l)
        niz[k++] = B[j++];
    delete [] B;
}

template <typename Tip>
void mergesort(Tip *niz, int l, int u){
    if(u>l){
        int p = (u + l - 1)/2;  //indeks zadnje pozicije prvog podniza
        int q = p + 1;  //indeks prve pozicije drugog podniza
        mergesort(niz, l, p);
        mergesort(niz, q, u);
        merge(niz, l, p, q, u);
    }
}

template <typename Tip>
void merge_sort(Tip *niz, int vel){
   mergesort(niz, 0, vel-1);
}

template <typename Tip>
void ZamjenaPozicija(Tip * niz, const int i, const int j){
    Tip temp = niz[i];
    niz[i] = niz[j];
    niz[j] = temp;
}

template <typename Tip>
int Particija(Tip *niz, int prvi, int zadnji){
    Tip pivot = niz[prvi];
    int p = prvi+1;
    while(p<=zadnji){
        if(niz[p]<=pivot)
            p++;
        else break;
    }
    for(int i = p+1; i<=zadnji; i++){
        if(niz[i]<pivot){
            ZamjenaPozicija(niz, p, i);
            p++;
        }
    }
    ZamjenaPozicija(niz, prvi, p-1);
    return p-1;
}

template <typename Tip>
void quicksort(Tip *niz, int prvi, int zadnji){
    if(prvi<zadnji){
        int j = Particija(niz, prvi, zadnji);
        quicksort(niz, prvi, j-1);
        quicksort(niz, j+1, zadnji);
    }
}

template <typename Tip>
void quick_sort(Tip *niz, int vel){
    quicksort(niz, 0, vel-1);
}

void ucitaj(std::string filename, int*& niz, int &vel){
    std::ifstream ulazni_tok(filename);
    if(!ulazni_tok) std::cout << "Datoteka ne postoji!\n";
    niz = new int [3000001];
    int i=0, broj;
    while(ulazni_tok >> broj){
        niz[i] = broj;
        i++;
    }
    vel = i;
}

void generisi(std::string filename, int vel){
    std::ofstream izlazni_tok(filename);
    if(!izlazni_tok) std::cout << "Datoteka ne postoji!\n";
    else{
        for(int i=0; i<vel; i++){
            izlazni_tok << rand() << " ";
        }
    }
}

int Menu(){
    int izbor;
    std::cout << "Izaberite metodu sortiranja: " << std::endl;
    std::cout << "1. Bubble sort" << std::endl;
    std::cout << "2. Selection sort" << std::endl;
    std::cout << "3. Quick sort" << std::endl;
    std::cout << "4. Merge sort" << std::endl;
    std::cout << "0. Izlaz iz programa" << std::endl;
    std::cin >> izbor;
    if(std::cin)
    return izbor;
    else{
        std::cin.clear();
        std::cin.ignore(100, '\n');
        return -1;
    }
}

template <typename Tip>
bool DaLiJeSortiran(Tip *niz, int vel){
    for(int i=0; i<vel-1; i++)
        if(niz[i+1]<niz[i]) return false;
    return true;
}

int main() {
    int izbor;
    int *niz;
    int vel, vrijeme;
    generisi("datoteka.txt", 1000);
    ucitaj("datoteka.txt", niz, vel);
    clock_t vrijeme_pocetka = clock();
    izbor = Menu();
        try{
            switch(izbor){
                case 0:
                    return 0;
                case 1:
                    bubble_sort(niz, vel);
                    break;
                case 2:
                    selection_sort(niz, vel);
                    break;
                case 3:
                    quick_sort(niz, vel);
                    break;
                case 4:
                    merge_sort(niz, vel);
                    break;
                default:
                    std::cout << "Pogresan izbor" << std::endl;
            }
        } catch(...){
            std::cout<< "Greska" << std::endl;
        }
    clock_t vrijeme_kraja = clock();
    vrijeme = (vrijeme_kraja - vrijeme_pocetka) / (CLOCKS_PER_SEC / 1000);
    std::cout << "Vrijeme sortiranja: " << vrijeme << " miliseknudi" << std::endl;
    if(DaLiJeSortiran(niz, vel)) std::cout << "Niz je sortiran" << std::endl;
    else std::cout << "Niz nije sortiran" << std::endl;
    std::ofstream tok("Novi");
    for(int i=0; i<vel; i++)
        tok << niz[i] << " ";
    return 0;
}
