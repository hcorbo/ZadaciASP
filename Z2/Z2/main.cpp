#include <iostream>
#include <vector>

using namespace std; 
template <typename Tip>
class Stek{
    struct Cvor{
      Tip element;
      Cvor *sljedeci;
      Cvor (const Tip &element, Cvor *sljedeci) : element(element), sljedeci(sljedeci){}
    };
    Cvor *vrha;
    int velicina;
    void kopiraj(const Stek &s){
        Cvor *q, *p = s.vrha;
        vrha = 0;
        velicina = s.velicina;
        while(p!=0){
            Cvor *n (new Cvor(p->element, 0));
            if(vrha==0)
                vrha=n;
            else q->sljedeci=n;
                p=p->sljedeci;
                q=n;
        }
    }
    
    public:
    //Konstruktor koji kreira prazan stek
    Stek() : vrha(0), velicina(0){}
    
    //Kopirajuci konstruktor
    Stek(const Stek &s) { kopiraj(s); }
    
    ~Stek(){ brisi(); }
    
    //Preklopljeni operator dodjele
    Stek &operator =(const Stek &s){
        if(&s == this) return *this;
        brisi();
        kopiraj(s);
        return *this;
    }
    
    //Funkcija koja dodaje na vrh steka
    void stavi (const Tip& el){
        vrha=new Cvor(el, vrha);
        velicina++;
    }
    
    void brisi();
    
    Tip skini();
    
    Tip &vrh();
    
    bool JeLiPrazan() const { return vrha == 0; }
    int brojElemenata() { return velicina; }
};

//Funkcija brise sve elemente sa steka
template <typename Tip>
void Stek<Tip>::brisi(){
    while(!JeLiPrazan()){
        skini();
    }
}

template <typename Tip>
Tip Stek<Tip>::skini(){
    if(vrha == 0) throw "Stek je prazan";
    Tip el = vrha->element;
    Cvor *pom = vrha->sljedeci;
    delete vrha;
    vrha = pom;
    velicina--;
    return el;
}

template <typename Tip>
Tip &Stek<Tip>::vrh(){
    if(vrha == 0) throw "Stek je prazan";
    return vrha->element;
}

int BinarnaPretraga(std::vector<int> v, int x){
    if(v.size() == 0) return -1;
    int pocetak = 0;
    int kraj = v.size()-1;
    while(pocetak<=kraj){
        int sredina=(pocetak+kraj)/2;
        if(x==v[sredina])
            return sredina;
        else if(x<v[sredina])
            kraj=sredina-1;
        else pocetak=sredina+1;
    }
    return -1;
}

void pretraga(Stek <vector<int>> &s, int trazeni){
    if(s.brojElemenata() == 0){
        std::cout << "Nema elementa";
        return;
    }
    int brojac = s.brojElemenata()-1;
    std::vector<int> pomocna = s.vrh();
    if(pomocna.size()!=0 && pomocna.at(0)<=trazeni){
        int rezultatBinPretrage = BinarnaPretraga(pomocna,trazeni);
        if(rezultatBinPretrage != -1)
            std::cout << rezultatBinPretrage << " " << brojac;
        else std::cout << "Nema elementa";
        return;
    }
    else {
        std::vector<int> temp = s.skini();
        pretraga(s, trazeni);
        s.stavi(temp);
    }
    
}

void testFunkcija1(){
    Stek<std::vector<int>> s;
    s.stavi({1, 4, 6, 9});
    s.stavi({10, 12, 13, 26});
    
    std::cout<< "Ocekivan je izlaz: 2 1" << std::endl;
    std::cout<< "Vas je program ispisao: ";
    pretraga(s, 13);
}

void testFunkcija2(){
    std::vector<int> v = {10, 12, 13, 26};
    std::cout << "\n\nOcekivan je izlaz: 2" << std::endl;
    std::cout << "Vas program je ispisao: " << BinarnaPretraga(v, 2);
}

void testFunkcija3(){
    Stek<std::vector<int>> s;
    s.stavi({1, 4, 6, 9});
    s.stavi({10, 12, 13, 26});
    
    std::cout<< "\n\nOcekivani izlaz je: Nema elementa" << std::endl;
    std::cout<< "Vas program je ispisao: ";
    pretraga(s, 38);
}

int Menu(){
    int izbor;
    std::cout << "1. Stavi element na stek\n";
    std::cout << "2. Skini element sa steka\n";
    std::cout << "3. Element na vrhu steka\n";
    std::cout << "4. Broj elemenata steka\n";
    std::cout << "0. Izlaz\n";
    std::cin>> izbor;
    if(std::cin)
        return izbor;
    else {
        std::cin.clear();
        std::cin.ignore(100, '\n');
        return -1;
    }
}

int main() {
    testFunkcija1();
    testFunkcija2();
    testFunkcija3();
    return 0;
}
