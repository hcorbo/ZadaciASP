#include <iostream>

template <typename Tip>
class Iterator;

template <typename Tip>
class Lista{
public:
    Lista() {};
    virtual ~Lista() {};
    virtual int brojElemenata () const = 0;
    virtual Tip& trenutni () =0;
    virtual Tip trenutni () const=0;
    virtual bool prethodni() = 0;
    virtual void pocetak() = 0;
    virtual bool sljedeci() = 0;
    virtual void kraj () = 0;
    virtual Tip& operator[] (int i) =0;
    virtual Tip operator[] (int i) const=0;
    virtual void obrisi() = 0;
    virtual void dodajIspred(const Tip& el) = 0;
    virtual void dodajIza(const Tip& el) = 0;

};


template <typename Tip>
class DvostrukaLista : public Lista<Tip>{
    
    struct Cvor{
        Tip element;
        Cvor *p_sljedeci;
        Cvor *p_prethodni;
        Cvor (const Tip &element, Cvor *s, Cvor *p) : element(element), p_sljedeci(s), p_prethodni(p){}
    };
    Cvor *p_pocetak = 0, *p_kraj = 0, *p_trenutni = 0;
    int velicina=0;
    
    public:
    DvostrukaLista() : Lista<Tip>(), p_pocetak(0), p_kraj(0), p_trenutni(0), velicina(0){}
    
    ~DvostrukaLista(){
        Cvor *pomocna = p_pocetak, *pomocna2;
        while(pomocna != 0){
            pomocna2 = pomocna;
            pomocna = pomocna->p_sljedeci;
            delete pomocna2;
        }
    }
    
    DvostrukaLista(const DvostrukaLista<Tip> &dl){
        Cvor *pomocna = dl.p_pocetak;
        while(pomocna != 0){
            this->dodajIza(pomocna->element);
            this->sljedeci();
            pomocna = pomocna -> p_sljedeci;
        }
        
    }
    
    DvostrukaLista<Tip>& operator =(const DvostrukaLista<Tip> &dl){
        if(this == &dl) return *this;
        int brojEl = velicina;
        for(int i=0; i<brojEl; i++) this->obrisi();
        velicina=0;
        Cvor *pomocna = dl.p_pocetak;
        while(pomocna!=0){
            this->dodajIza(pomocna->element);
            this->sljedeci();
            pomocna = pomocna->p_sljedeci;
        }
        return *this;
    }
    
    int brojElemenata() const{
        return velicina;
    }
    
    Tip trenutni() const{
        if(p_pocetak == 0) throw std::domain_error("Izuzetak");
        return p_trenutni->element;
    }
    
    Tip& trenutni(){
        if(p_pocetak == 0) throw std::domain_error("Izuzetak");
        return p_trenutni->element;
    }
    
    bool prethodni(){
        if(p_pocetak == 0) throw std::domain_error("Izuzetak");
        if(p_trenutni == p_pocetak) return false;
        p_trenutni = p_trenutni->p_prethodni;
        return true;
    }
    
    bool sljedeci(){
        if(p_pocetak == 0) throw std::domain_error("Izuzetak");
        if(p_trenutni == p_kraj) return false;
        p_trenutni = p_trenutni->p_sljedeci; 
        return true;
    }
    
    void pocetak(){
        if(p_pocetak == 0) throw std::domain_error("Izuzetak");
        p_trenutni = p_pocetak;
    }
    
    void kraj (){
        if(p_pocetak == 0) throw std::domain_error("Izuzetak");
        p_trenutni = p_kraj;
    }
    
    void obrisi(){
        if(p_pocetak == 0) throw std::domain_error("Izuzetak");
        velicina--;
        if(p_trenutni == p_pocetak){
            p_pocetak = p_pocetak->p_sljedeci;
            delete p_trenutni;
            p_trenutni = p_pocetak;
            if(velicina == 0) p_kraj=0;
        }
        else if(p_trenutni == p_kraj){
            p_kraj = p_kraj->p_prethodni;
            delete p_trenutni;
            p_kraj->p_sljedeci = 0;
            p_trenutni = p_kraj;
        }else{
            Cvor *p = p_trenutni;
            p_trenutni->p_prethodni->p_sljedeci = p_trenutni->p_sljedeci;
            p_trenutni->p_sljedeci->p_prethodni = p_trenutni->p_prethodni;
            p_trenutni = p_trenutni->p_sljedeci;
            delete p;
        }
    }
    
    Tip& operator[] (int i){
        if(i >= velicina || i < 0) throw std::domain_error("Izuzetak");
        Cvor *pomocna = p_pocetak;
        for(int j=0; j<i; j++) {
            pomocna = pomocna -> p_sljedeci;
        }
        return pomocna->element;
    }
    
    Tip operator[] (int i) const{
        if(i >= velicina || i < 0) throw std::domain_error("Izuzetak");
        Cvor *pomocna = p_pocetak;
        for(int j=0; j<i; j++) {
            pomocna = pomocna -> p_sljedeci;
        }
        return pomocna->element;
    }
    
    void dodajIspred(const Tip& el){
        Cvor *n = new Cvor(el, 0, 0);
        n->element = el;
        velicina++;
        if(p_pocetak == 0){
            n->p_sljedeci = 0;
            n->p_prethodni = 0;
            p_pocetak = p_kraj = p_trenutni = n;
        } 
        else {
            n->p_sljedeci=p_trenutni;
            n->p_prethodni=p_trenutni->p_prethodni;
            if(p_pocetak!=p_trenutni)
            p_trenutni->p_prethodni->p_sljedeci=n;
            else p_pocetak=n;
            p_trenutni->p_prethodni= n;
            
        }
    }
    
    void dodajIza(const Tip& el){
        Cvor *n = new Cvor(el, 0, 0);
        n->element = el;
        velicina++;
        if(this->p_pocetak == 0){
            p_pocetak = p_kraj = p_trenutni = n;
            n->p_sljedeci = 0;
        } else if(p_trenutni == p_kraj){
            p_trenutni->p_sljedeci = n;
            n->p_prethodni = p_trenutni;
            n->p_sljedeci = 0;
            p_kraj=n;
        } else{
            n->p_prethodni = p_trenutni;
            n->p_sljedeci = p_trenutni->p_sljedeci;
            p_trenutni->p_sljedeci->p_prethodni=n;
            p_trenutni->p_sljedeci = n;
            
        }
        
    }
    friend class Iterator<Tip>;
};

int Menu(){
    int izbor;
    std::cout << "1. Stavi na vrh reda" << std::endl;
    std::cout << "2. Stavi na celo reda" << std::endl;
    std::cout << "3. Skini sa vrha reda" << std::endl;
    std::cout << "4. Skini sa cela reda" << std::endl;
    std::cout << "5. Element na vrhu" << std::endl;
    std::cout << "6. Element na celu" << std::endl;
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
class DvostraniRed{
    DvostrukaLista<Tip> dvostruki_red;
    public:
    void staviNaVrh(const Tip& el){
        if(dvostruki_red.brojElemenata()!=0)
        dvostruki_red.kraj();
        dvostruki_red.dodajIza(el);
    }
    
    Tip skiniSaVrha(){
        if(dvostruki_red.brojElemenata()==0) throw std::logic_error("Nema elemenata");
        Tip temp=0;
        dvostruki_red.kraj();
        temp = dvostruki_red.trenutni();
        dvostruki_red.obrisi();
        return temp;
    }
    
    void staviNaCelo(Tip el){
        if(dvostruki_red.brojElemenata()!=0)
            dvostruki_red.pocetak();
        dvostruki_red.dodajIspred(el);
    }
    
    Tip skiniSaCela(){
        if(dvostruki_red.brojElemenata()==0) throw std::logic_error("Nema elemenata");
        Tip temp=0;
        dvostruki_red.pocetak();
        temp = dvostruki_red.trenutni();
        dvostruki_red.obrisi();
        return temp;
    }
    int brojElemenata(){
       return  dvostruki_red.brojElemenata();
    }
    
    Tip &vrh(){
         dvostruki_red.kraj();
         return dvostruki_red.trenutni();
    }
    
    Tip &celo(){
        dvostruki_red.pocetak();
        return dvostruki_red.trenutni();
    }
    
    void brisi(){
        if(dvostruki_red.brojElemenata()!=0)
        dvostruki_red.pocetak();
        int vel=dvostruki_red.brojElemenata();
        for(int i=0; i<vel; i++){
            dvostruki_red.obrisi();
        }
    }
};

template <typename Tip>
void testStaviNaVrh(DvostraniRed<Tip> &r){
    for(int i=0; i<33; i++) 
        r.staviNaVrh(i);
    std::cout << "Uspjesno stavio na Vrh\n";
}

template <typename Tip>
void testStaviNaCelo(DvostraniRed<Tip> &r){
    for(int i=0; i<33; i++) 
        r.staviNaCelo(i);
    std::cout << "Uspjesno stavio na Celo\n";
}

template <typename Tip>
void testSkiniSaVrha(DvostraniRed<Tip> &r){
    if(r.brojElemenata()!=0)
        r.skiniSaVrha();
    std::cout << "Uspjesno skinuo sa vrha\n";
}

template <typename Tip>
void testBrojElemenata(DvostraniRed<Tip> &r){
    std::cout << "DvostraniRed ima: " << r.brojElemenata() << " elemenata\n";
}

template <typename Tip>
void testSkiniSaCela(DvostraniRed<Tip> &r){
    if(r.brojElemenata()!=0)
        r.skiniSaCela();
    std::cout << "Uspjesno skinuo sa cela";
}

int main() {
    DvostraniRed<int> r;
    testStaviNaCelo(r);
    testBrojElemenata(r);
    int izbor;
    DvostraniRed<double> dl;
    while((izbor = Menu()) != 0){
        try{
            switch(izbor){
                case 0:
                    return 0;
                case 1:
                    std::cout << "Unesite element: ";
                    double x;
                    std::cin >> x;
                    dl.staviNaVrh(x);
                    break;
                case 2:
                    std::cout << "Unesite element: ";
                    std::cin >> x;
                    dl.staviNaCelo(x);
                    break;
                case 3:
                    std:: cout << "Element na vrhu je: " << dl.skiniSaVrha() << std::endl;
                    break;
                case 4:
                    std:: cout << "Element na celu je: " << dl.skiniSaCela() << std::endl;
                    break;
                case 5:
                    std:: cout << "Element na vrhu je: " << dl.vrh() << std::endl;
                    break;
                case 6:
                    std:: cout << "Element na celu je: " << dl.celo() << std::endl;
                    break;
                default:
                    std::cout << "Pogresan izbor" << std::endl;
            }
        } catch(...){
            std::cout<< "Greska" << std::endl;
        }
    }
    return 0;
}


