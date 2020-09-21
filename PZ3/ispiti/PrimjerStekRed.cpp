#include <iostream>
#include <vector>
#include <cstring>

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

/*void funkcija(Stek<int> &s, Red<int> &r){
    int brojElReda = r.brojElemenata();
    int brojElSteka = s.brojElemenata();
    
    for(int i=0; i<brojElSteka; i++){
        r.stavi(s.skini());
    }
    
    int brEl = brojElReda+brojElSteka;
    for(int i=0; i<brEl; i++){
        if(i<brojElReda){
            int temp = r.skini();
            if(temp%2 == 0)
                s.stavi(temp);
            else r.stavi(temp);
        } else r.stavi(r.skini());
    }
    
    int brParnih = s.brojElemenata();
    for(int i=0; i<brEl-brParnih; i++){
        if(i<brojElReda-brParnih)
            r.stavi(r.skini());
        else
            s.stavi(r.skini());
    }
}
*/
void funkcija(Stek<int> &s, Red<int> &r){
    int brojElReda = r.brojElemenata();
    int brojElSteka = s.brojElemenata();
    
    //stavljam red na stek
    for(int i=0; i<brojElReda; i++){
        s.stavi(r.skini());
    }
    
    //vracam elemente reda u obrnutom redosljedu sa steka
    for(int i=0; i<brojElReda; i++){
        r.stavi(s.skini());
    }
    
    //stavljam elemente steka na red
    for(int i=0; i<brojElSteka; i++){
        r.stavi(s.skini());
    }
    
    int brEl = brojElReda+brojElSteka;
    int brojParnih = 0;
    for(int i=0; i<brEl; i++){
        if(i<brojElReda){
            int temp = r.skini();
            if(temp%2 == 0){
                s.stavi(temp);
                brojParnih++;
            }
            else r.stavi(temp);
        } else s.stavi(r.skini());
    }
   
    for(int i=0; i<brojElReda-brojParnih; i++){
        s.stavi(r.skini());
    }
    
    for(int i=0; i<brojElReda-brojParnih; i++){
        r.stavi(s.skini());
    }
    
    //okretanje elemenata steka i reda da se vrate u pocetne pozicije
    for(int i=0; i<brojElSteka; i++){
        r.stavi(s.skini());
    }
    for(int i=0; i<brojElReda-brojParnih+brojElSteka; i++){
        if(i<brojElReda-brojParnih) r.stavi(r.skini());
        else s.stavi(r.skini());
    }
}

//Sve elemente unutar reda koji su parni postaviti na dno steka
void funkcija1(Stek<int> &s, Red<int> &r){
    int brojElReda = r.brojElemenata();
    int brojElSteka = s.brojElemenata();
    
    //stavljam red na stek
    for(int i=0; i<brojElReda; i++){
        s.stavi(r.skini());
    }
    
    //vracam elemente reda u obrnutom redosljedu sa steka
    for(int i=0; i<brojElReda; i++){
        r.stavi(s.skini());
    }
    
    //stavljam elemente steka na red
    for(int i=0; i<brojElSteka; i++){
        r.stavi(s.skini());
    }
    
    int brEl = brojElReda+brojElSteka;
    int brojParnih = 0;
    for(int i=0; i<brojElReda; i++){
        int temp = r.skini();
            if(temp%2 == 0){
                s.stavi(temp);
                brojParnih++;
            }
            else r.stavi(temp);
    }
    for(int i=0; i<brEl-brojParnih; i++){
        s.stavi(r.skini());
    }
    for(int i=0; i<brEl-brojParnih; i++){
        r.stavi(s.skini());
    }
    for(int i=0; i<brEl-brojElSteka-brojParnih; i++){
        r.stavi(r.skini());
    }
    for(int i=0; i<brojElSteka; i++){
        s.stavi(r.skini());
    }
    // for(int i=0; i<brEl; i++){
    //     if(i<brojElReda){
    //         int temp = r.skini();
    //         if(temp%2 == 0){
    //             s.stavi(temp);
    //             brojParnih++;
    //         }
    //         else r.stavi(temp);
    //     } else s.stavi(r.skini());
    // }
   
    // for(int i=0; i<brojElReda-brojParnih; i++){
    //     s.stavi(r.skini());
    // }
    
    // for(int i=0; i<brojElReda-brojParnih; i++){
    //     r.stavi(s.skini());
    // }
    
    // //okretanje elemenata steka i reda da se vrate u pocetne pozicije
    // for(int i=0; i<brojElSteka; i++){
    //     s.stavi(r.skini());
    // }
    // // for(int i=0; i<brojElReda-brojParnih+brojElSteka; i++){
    // //     if(i<brojElReda-brojParnih) r.stavi(r.skini());
    // //     else s.stavi(r.skini());
    // // }
}

//funkcija koja će ostaviti prvi element(red) na vrhu steka, a ostale će obrnuti redosljed
//{1 2 3}   {1 2 3}
//{9 10}    {4 5}
//{7 8}     {7 8}
//{4 5}     {9 10}

void funkcija2(Stek<Red<int>> &s, Red<int> &r, Red<int> &r2){
    
    int brojElSteka = s.brojElemenata();
    int brojElReda1 = s.vrh().brojElemenata();
    r = s.skini();
    while(s.brojElemenata() != 0){
        int vel = s.vrh().brojElemenata();
        r2.stavi(vel);
        for(int i=0; i<vel; i++){
            r2.stavi(s.vrh().skini());
        }
        s.skini();
    }
    r2.stavi(brojElReda1);
    for(int i=0; i<brojElReda1; i++){
        r2.stavi(r.skini());
    }
    while(r2.brojElemenata() != 0){
        int temp = r2.skini();
        for(int i=0; i<temp; i++){
            std::cout << "Element na vrhu je : " << r2.celo();
            r.stavi(r2.skini());
        }
        std::cout << std::endl;
        s.stavi(r);
        while(r.brojElemenata() != 0)
            r.skini();
    }
}


Stek<int> IzravnajStekove(Red<Stek<int>> &r){
    Stek<int> s;
    
    int brojElReda = r.brojElemenata();
    int ukupanBrojElemenata=0;
    
    //racunam ukupan broj elemenata u stekovima
    for(int i=0; i<brojElReda; i++){
        ukupanBrojElemenata += r.celo().brojElemenata();
        r.stavi(r.skini());
    }
    
    int prosjek = ukupanBrojElemenata/brojElReda;
    
    for(int i=0; i<brojElReda; i++){
        //ako je broj elemenata manji od prosjeka samo skidam element reda
        if(r.celo().brojElemenata()<prosjek) r.skini();
        //ako je broj jednak saljem na kraj reda
        else if(r.celo().brojElemenata()==prosjek) r.stavi(r.skini());
        else {
            //skidam visak elemenata u stek koji se vraca i saljem na kraj reda
            while(r.celo().brojElemenata()>prosjek){
                s.stavi(r.celo().skini());
            }
            r.stavi(r.skini());
        }
    }
    //promijenio se broj elemenata
    brojElReda = r.brojElemenata();
    
    //stavljam na celo reda stek koji se vraca
    r.stavi(s);
    for(int i=0; i<brojElReda; i++)
        r.stavi(r.skini());
        
    //sortiranje pomocu dva steka
    while(r.celo().brojElemenata()){
        int temp = r.celo().skini();
        while(s.brojElemenata() && temp < s.vrh()){
            r.celo().stavi(s.skini());
        }
        s.stavi(temp);
    }
    
    r.skini();
    return s;
}



std::string tackaSpajanja (std::vector<std::string> vs){
    for(int i=0; i<vs.size()-1; i++){
        if(vs.at(i)>vs.at(i+1)) return vs.at(i);
    }
}
int Menu1(){
    Red<Stek<int>> r;
    Stek<int> pom;
    pom.stavi(1); pom.stavi(2); pom.stavi(3);
    r.stavi(pom);
    pom.brisi();
    pom.stavi(1);
    r.stavi(pom);
    pom.brisi();
    pom.stavi(8); pom.stavi(5); pom.stavi(22);
    pom.stavi(3); pom.stavi(27); pom.stavi(11);
    r.stavi(pom);
    Stek<int> rez = IzravnajStekove(r);
    for(int i=0; i<rez.brojElemenata(); i++){
        std::cout<< rez.vrh() << " ";
        rez.skini();
    }
    
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
    Stek<int> s1;
    s1.stavi(1);
    s1.stavi(2);
    s1.stavi(3);
    Red<int> r1;
     r1.stavi(10);
     r1.stavi(11);
     r1.stavi(12);
    // r1.stavi(6);
    // r1.stavi(7);
    // r1.stavi(8);
    // r1.stavi(9);
    Red<int> r2;
    r2.stavi(7);
    r2.stavi(8);
    r2.stavi(9);
    // funkcija1(s1, r1);
    // int brel = s1.brojElemenata();
    // for(int i=0; i<brel; i++){
    //     std::cout << s1.skini() << " ";
    // }
    Stek<Red<int>> s2;
    s2.stavi(r1);
    s2.stavi(r2);
    r1.skini();
    r1.skini();
    r1.skini();
    r1.stavi(4);
     r1.stavi(5);
     r1.stavi(6);
     s2.stavi(r1);
     r1.skini();
    r1.skini();
    r1.skini();
    r1.stavi(1);
     r1.stavi(2);
     r1.stavi(3);
     s2.stavi(r1);
    
    Red<int> r3;
    Red<int> r4;
    funkcija2(s2, r3, r4);
    while(s2.brojElemenata()!=0){
        std::cout<< "ELA " << s2.vrh().skini() << std::endl;
        s2.skini();
    }
    std::vector<std::string> vs{"abc", "acc", "bt", "cab", "cba", "bhh", "ba"};
    std::cout << tackaSpajanja(vs);
    Stek<int> s;
    int izbor, element;
    while((izbor = Menu()) != 0){
        switch(izbor){
            case 0:
                return 0;
            case 1:
                std::cout << "Unesite element: ";
                std::cin >> element;
                s.stavi(element);
                break;
            case 2:
                element = s.skini();
                std::cout << "Element " << element << " je skinut sa steka" << std::endl;
                break;
            case 3:
                std::cout << "Element na vrhu steka je: " << s.vrh() << std::endl;
                break;
            case 4:
                std::cout << "Broj elemenata steka: " << s.brojElemenata() << std::endl;
                break;
            default:
                std::cout << "Pogresan unos\n";
        }
    }
    
    Red<int> r;
    while((izbor = Menu1()) != 0){
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
    
    funkcija(s, r);
    while((izbor = Menu()) != 0){
        switch(izbor){
            case 0:
                return 0;
            case 1:
                std::cout << "Unesite element: ";
                std::cin >> element;
                s.stavi(element);
                break;
            case 2:
                element = s.skini();
                std::cout << "Element " << element << " je skinut sa steka" << std::endl;
                break;
            case 3:
                std::cout << "Element na vrhu steka je: " << s.vrh() << std::endl;
                break;
            case 4:
                std::cout << "Broj elemenata steka: " << s.brojElemenata() << std::endl;
                break;
            default:
                std::cout << "Pogresan unos\n";
        }
    }
    return 0;
}
