#include <iostream>

template <typename Tip>
class Red{
    struct Cvor{
      Tip element;
      Cvor *sljedeci;
      Cvor (const Tip &element, Cvor *sljedeci) : element(element), sljedeci(sljedeci){}
    };
    Cvor *pocetak=0, *kraj=0;
    int velicina=0;
    
    void kopiraj(const Red &r){
        Cvor *p (r.pocetak);
        while(p != 0){
            stavi(p->element);
            p = p->sljedeci;
        }
    }
    
    public:
    //Konstruktor koji kreira prazan red
    Red() : pocetak(0), kraj(0), velicina(0){}
    
    //Kopirajuci konstruktor
    Red(const Red &r) { kopiraj(r); }
    
    ~Red(){ brisi(); }
    
    //Preklopljeni operator dodjele
    Red &operator =(const Red &r){
        if(&r == this) return *this;
        brisi();
        kopiraj(r);
        return *this;
    }
    
    void brisi();
    
    //Funkcija koja dodaje na kraj reda
    void stavi (const Tip& el){
        Cvor *n = new Cvor(el, 0);
        if(pocetak == 0)
            pocetak = kraj = n;
        else {
            kraj->sljedeci = n;
           kraj = kraj -> sljedeci;
        }
        velicina++;
    }
    
    Tip skini();
    
    Tip &celo();
    
    int brojElemenata() { return velicina; }
};

//Funkcija brise sve elemente iz reda
template <typename Tip>
void Red<Tip>::brisi(){
    while(pocetak != 0){
        skini();
    }
}

template <typename Tip>
Tip Red<Tip>::skini(){
    if(pocetak == 0) throw "Red je prazan";
    Tip el = pocetak->element;
    Cvor *pom = pocetak;
    if(pocetak==kraj){
        pocetak = kraj = 0;
    }else
    pocetak = pocetak->sljedeci;
    delete pom;
    velicina--;
    return el;
}

template <typename Tip>
Tip &Red<Tip>::celo(){
    if(pocetak == 0) throw "Red je prazan";
    return pocetak->element;
}
template <typename Tip>
void testCelo(Red<Tip> &r){
    r.stavi(81);
    if(r.celo()=81) std::cout<< "Test je uspjesan";
    else std::cout<< "Greska";
}

template <typename Tip>
void testBrisiPisi(Red<Tip> &r){
    for(int i=1; i<10; i+=2) r.stavi(i);
    std::cout << "Trenutno red ima: " << r.brojElemenata() << " elemenata\n";
    r.brisi();
    std::cout<< "Nakon brisanja red ima: " << r.brojElemenata() << " elemenata\n";
}

int Menu(){
    int izbor;
    std::cout<< "Izaberize opciju:\n";
    std::cout << "1. Stavi element u red\n";
    std::cout << "2. Skini element iz reda\n";
    std::cout << "3. Element na celu reda\n";
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
    Red<int> r;
    testBrisiPisi(r);
    int izbor, element;
    while((izbor = Menu()) != 0){
        switch(izbor){
            case 0:
                return 0;
            case 1:
                std::cout<< "Unesite element: ";
                std::cin>> element;
                r.stavi(element);
                break;
            case 2:
                element = r.skini();
                std::cout << "Element " << element << " je skinut iz reda" << std::endl;
                break;
            case 3:
                std::cout << "Element na vrhu reda je: " << r.celo() << std::endl;
                break;
                default:
                std::cout << "Pogresan unos\n";
        }
    }
    
    return 0;
}
