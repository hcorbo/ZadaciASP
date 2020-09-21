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


template <typename Tip>
class Iterator{
    const DvostrukaLista<Tip> *lista;
    typename DvostrukaLista<Tip>::Cvor *trenutniLista;
    
    public:
    
    Iterator (const Lista<Tip> &lista){
        DvostrukaLista<Tip> &pok = (DvostrukaLista<Tip> &) lista;
        this->lista = &pok;
        trenutniLista = pok.p_pocetak;
    }
    Iterator(const DvostrukaLista<Tip> &lista):lista(&lista), trenutniLista(lista.p_pocetak) { }
    Tip trenutni(){
        if(trenutniLista!=0)
            return trenutniLista->element;
    }
    
    bool prethodni(){
        if(trenutniLista==lista.p_pocetak) return false;
        trenutniLista = trenutniLista -> p_prethodni;
        return true;
    }
    
    bool sljedeci(){
        if(trenutniLista==lista->p_kraj) return false;
        trenutniLista = trenutniLista -> p_sljedeci;
        return true;
    }
    
    void pocetak(){
        trenutniLista = lista->p_pocetak;
    }
    
    void kraj(){
        trenutniLista = lista->p_kraj;
    }
};

template <typename Tip>
Tip dajMaksimum(const Lista<Tip> &n){
    int broj=n.brojElemenata();
    Iterator<Tip> it(n);
    it.pocetak();
    Tip max = it.trenutni();
    for(int i=1; i<broj; i++){
        if(it.trenutni()>max) max=it.trenutni();
        it.sljedeci();
    }
    return max;
}

int Menu(){
    int izbor;
    std::cout << "1. Dodaj element ispred" << std::endl;
    std::cout << "2. Dodaj element iza" << std::endl;
    std::cout << "3. Izbrisi element" << std::endl;
    std::cout << "4. Idi na sljedeci" << std::endl;
    std::cout << "5. Idi na prethodni" << std::endl;
    std::cout << "6. Broj elemenata" << std::endl;
    std::cout << "7. Trenutni element" << std::endl;
    std::cout << "8. Daj maksimum" << std::endl;
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

int main() {
    int izbor;
    DvostrukaLista<double> dvostruka_lista;
    while((izbor = Menu()) != 0){
        try{
            switch(izbor){
                case 0:
                    return 0;
                case 1:
                    std::cout << "Unesite element: ";
                    int x;
                    std::cin >> x;
                    dvostruka_lista.dodajIspred(x);
                    break;
                case 2:
                    std::cout << "Unesite element: ";
                    std::cin >> x;
                    dvostruka_lista.dodajIza(4);
                    break;
                case 3:
                    dvostruka_lista.obrisi();
                    break;
                case 4:
                    dvostruka_lista.sljedeci();
                    break;
                case 5:
                    dvostruka_lista.prethodni();
                    break;
                case 6:
                    std::cout << "Jednostruka lista ima: " << dvostruka_lista.brojElemenata() << " elemenata" << std::endl;
                    break;
                case 7:
                    std::cout << "Trenutni element jednostruke liste: "<< dvostruka_lista.trenutni() << std::endl;
                    break;
                case 8:
                    std::cout << "Maksimalni element: " << dajMaksimum(dvostruka_lista) << std::endl;
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
