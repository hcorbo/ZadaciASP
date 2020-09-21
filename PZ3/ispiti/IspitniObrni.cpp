#include <iostream>
#include <stdexcept>

static const int  VEL_NIZ = 1000;
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
class NizLista : public Lista<Tip>{
    Tip *niz;
    int kapacitet = 0, brojEl = 0, t = 0;
public:
    //konstruktor
    NizLista() : Lista<Tip>(), niz (new Tip[VEL_NIZ]), kapacitet(VEL_NIZ), brojEl(0), t(0) {}
    //destruktor
    ~NizLista(){
        delete [] niz;
    }
    //konstruktor kopije
    NizLista (const NizLista<Tip> &n){
        brojEl = n.brojEl;
        kapacitet = n.kapacitet;
        t=n.t;
        niz = new Tip[n.kapacitet];
        std::copy(n.niz, n.niz+n.kapacitet, niz);
    }
    //operator dodjele
    NizLista <Tip>& operator =(const NizLista<Tip>& n){
        if(this == &n) return *this;        //provjera samododjele
        delete [] niz;
        brojEl = n.brojEl;
        kapacitet = n.kapacitet;
        t=n.t;
        niz = new Tip[n.kapacitet];
        std::copy(n.niz, n.niz+n.kapacitet, niz);
        return *this;

    }

    int brojElemenata () const{
        return brojEl;
    }
    
    Tip& trenutni (){
        if(brojEl == 0) throw std::domain_error("Izuzetak");
        return niz[t];
    }
    
    Tip trenutni () const{
        if(brojEl == 0) throw std::domain_error("Izuzetak");
        return niz[t];
    }
    
    bool prethodni(){
        if(brojEl == 0) throw std::domain_error("Izuzetak");
        if(t == 0) return false;        //ako je prvi element nema prethodni
        t--;
        return true;
    }
    
    bool sljedeci(){
        if(brojEl == 0) throw std::domain_error("Izuzetak");
        if(t == brojEl-1) return false;         //ako je podljednji element nema sljedeci
        t++;
        return true;
    }
    
    void pocetak(){
        if(brojEl == 0) throw std::domain_error("Izuzetak");
        t = 0;
    }
    
    void kraj (){
        if(brojEl == 0) throw std::domain_error("Izuzetak");
        t = brojEl-1;
    }
    
    Tip& operator[] (int i){
        if(i >= brojEl || i < 0) throw std::domain_error("Izuzetak");
        return niz[i];
    }
    
    Tip operator[] (int i) const{
        if(i >= brojEl || i < 0) throw std::domain_error("Izuzetak");
        return niz[i];
    }
    //Pisemo elemente preko prethodnog i brisemo trenutni i smanjimo brojEl
    void obrisi (){
        if(brojEl == 0) throw std::domain_error("Izuzetak");
        for(int i=t; i<brojEl-1; i++) {
            niz[i] = niz[i+1];
        }
        brojEl--;
        if(t == brojEl && brojEl > 0) t--;
    }
    
    void dodajIspred(const Tip& el){
        if(brojEl == kapacitet) {
            kapacitet*=2;
            Tip *niz2 = new Tip [kapacitet];
            for(int i=0 ; i<brojEl; i++) {
                niz2[i] = niz[i];
            }
            delete[] niz;
            niz = niz2;
        }
        for(int i=brojEl; i>t; i--)
            niz[i]=niz[i-1];
        niz[t]=el;
        brojEl++;
        if(brojEl != 1) t++;
    }
    
    void dodajIza(const Tip& el){
        if(brojEl == kapacitet) {
            kapacitet*=2;
            Tip *niz2 = new Tip [kapacitet];
            for(int i=0 ; i<brojEl; i++) {
                niz2[i] = niz[i];
            }
            delete[] niz;
            niz = niz2;
        }
        if(brojEl == 0) {
            niz[t] = el;
        } else {
            for(int i=brojEl; i>t+1; i--)
                niz[i]=niz[i-1];
            niz[t+1] = el;
        }
        brojEl++;
    }
};

template <typename Tip>
class JednostrukaLista : public Lista<Tip>{
    struct Cvor {
        Tip element;
        Cvor *p_sljedeci;
    };
    Cvor *p_pocetak, *p_kraj, *p_trenutni;
    int brojEl;
public:
    JednostrukaLista() : Lista<Tip>(), p_pocetak(0), p_kraj(0), p_trenutni(0), brojEl(0) {}

    ~JednostrukaLista(){
        Cvor *pomocna = p_pocetak, *pomocna2;
        while(pomocna != 0){
            pomocna2 = pomocna;
            pomocna = pomocna->p_sljedeci;
            delete pomocna2;
        }
    }
    
    JednostrukaLista(const JednostrukaLista<Tip> &l) {
        Cvor *pomocna = l.p_pocetak;
        Cvor *pomocna2 = 0;
        while(pomocna != 0){
            Cvor *novi = new Cvor;
            novi->element = pomocna->element;
            if(pomocna2 == 0){
                p_pocetak = novi;
            } else {
                pomocna2->p_sljedeci = novi;
            }
            pomocna2 = novi;
            if(pomocna == l.p_trenutni){
                p_trenutni = novi;
            }
            pomocna = pomocna->p_sljedeci;
        }
        p_kraj=pomocna2;
    }

    JednostrukaLista<Tip> & operator =(const JednostrukaLista<Tip> &l) {
        if(this == &l) return *this;
        Cvor *pomocna3 = p_pocetak, *pomocna4;
        while(pomocna3 != 0){
            pomocna4 = pomocna3;
            pomocna3 = pomocna3->p_sljedeci;
            delete pomocna4;
        }
         Cvor *pomocna = l.p_pocetak, *pomocna2 = 0;
        while(pomocna != 0){
            Cvor *novi = new Cvor;
            novi->element = pomocna->element;
            if(pomocna2 == 0){
                p_pocetak = novi;
            } else {
                pomocna2->p_sljedeci = novi;
            }
            pomocna2 = novi;
            if(pomocna == l.p_trenutni){
                p_trenutni = novi;
            }
            pomocna = pomocna->p_sljedeci;
        }
        p_kraj=pomocna2;
        return *this;
    }


    int brojElemenata ()const {
        return brojEl;
    }

    Tip& trenutni (){
        if(p_pocetak == 0) throw std::domain_error("Izuzetak");
        return p_trenutni->element;
    }

    Tip trenutni () const{
        if(p_pocetak == 0) throw std::domain_error("Izuzetak");
        return p_trenutni->element;
    }

    bool prethodni() {
        if(p_pocetak == 0) throw std::domain_error("Izuzetak");
        if(p_trenutni == p_pocetak) return false;
        Cvor *pomocna = p_pocetak;
        while(pomocna->p_sljedeci != p_trenutni)
            pomocna = pomocna->p_sljedeci;
        p_trenutni= pomocna;
        return true;

    }

    void pocetak(){
        if(p_pocetak == 0) throw std::domain_error("Izuzetak");
        p_trenutni = p_pocetak;
    }

    bool sljedeci(){
        if(p_pocetak == 0) throw std::domain_error("Izuzetak");
        if(p_trenutni == p_kraj) return false;
        p_trenutni = p_trenutni->p_sljedeci;
        return true;
    }

    void kraj (){
        if(p_pocetak == 0) throw std::domain_error("Izuzetak");
        p_trenutni=p_kraj;
    }

    Tip& operator[] (int i){
        if(i >= brojEl || i < 0) throw std::domain_error("Izuzetak");
        Cvor *pomocna = p_pocetak;
        for(int j=0; j<i; j++) {
            pomocna = pomocna -> p_sljedeci;
        }
        return pomocna->element;
    }

    Tip operator[] (int i) const{
        if(i >= brojEl || i < 0) throw std::domain_error("Izuzetak");
        Cvor *pomocna = p_pocetak;
        for(int j=0; j<i; j++) {
            pomocna = pomocna->p_sljedeci;
        }
        return pomocna->element;
    }

    void obrisi() {
        if(p_pocetak == 0) throw std::domain_error("Izuzetak");
        brojEl--;
        if(p_trenutni == p_pocetak) {
            p_pocetak = p_pocetak->p_sljedeci;
            delete p_trenutni;
            p_trenutni = p_pocetak;
            if(brojEl == 0) p_kraj = 0;
        } else {
            Cvor *pomocna = p_pocetak;
            while(pomocna->p_sljedeci != p_trenutni)
                pomocna = pomocna->p_sljedeci;
            pomocna->p_sljedeci = p_trenutni->p_sljedeci;
            delete p_trenutni;
            p_trenutni = pomocna->p_sljedeci;
            if(p_trenutni == 0) {
                p_kraj = p_trenutni = pomocna;
            }
        }
    }

    void dodajIspred(const Tip& el){
        Cvor *novi = new Cvor;
        novi->element = el;
        brojEl++;
        if(p_pocetak == 0) {
            p_pocetak = p_kraj = p_trenutni = novi;
            novi->p_sljedeci = 0;
        } else if(p_trenutni == p_pocetak) {
            novi -> p_sljedeci = p_pocetak;
            p_pocetak = novi;
        } else {
            Cvor *pomocna = p_pocetak;
            while (pomocna->p_sljedeci != p_trenutni)
                pomocna = pomocna->p_sljedeci;
            novi->p_sljedeci = pomocna->p_sljedeci;
            pomocna->p_sljedeci = novi;
        }
    }

    void dodajIza(const Tip& el){
        Cvor *novi = new Cvor;
        novi->element=el;
        brojEl++;
        if(p_pocetak == 0) {
            p_pocetak = p_kraj = p_trenutni = novi;
            novi->p_sljedeci=0;
        } else {
            novi->p_sljedeci = p_trenutni->p_sljedeci;
            p_trenutni->p_sljedeci = novi;
            if(p_kraj == p_trenutni)
                p_kraj = novi;
        }
    }
    
    void brisiOdAdoB(int a, int b){
        Cvor *p = p_pocetak;
        while(p->element!=a){
            p=p->p_sljedeci;
        }
        Cvor *q = p_pocetak;
        while(q->element!=a){
            q=q->p_sljedeci;
        }
        if(p<q){
            while(p->p_sljedeci!=q){
                Cvor *temp = p->p_sljedeci;
                p->p_sljedeci=temp->p_sljedeci;
                delete temp;
            }
        } else {
            while(q->p_sljedeci!=p){
                Cvor *temp = q->p_sljedeci;
                q->p_sljedeci=temp->p_sljedeci;
                delete temp;
            }
        }
        
        /*Cvor *temp = p_pocetak;
        while(temp->element != a && temp -> element != b){
            temp=temp->p_sljedeci;
        }

        if(temp->element==a){
            std::cout<< "dosao sam";
            while(temp->p_sljedeci->element!=b){
                std::cout<<"brisem ";
                Cvor *pom=temp->p_sljedeci;
                temp->p_sljedeci=pom->p_sljedeci;
                delete pom;
                brojEl--;
            }
            
        }
        else{
            std::cout<< "dosao samo tu";
            while(temp->p_sljedeci->element!=a){
                Cvor *pom=temp->p_sljedeci;
                temp->p_sljedeci=pom->p_sljedeci;
                delete pom;
                brojEl--;
            }
        }*/    
    }
    
    void Premjesti(int n, int m, int k){
        if (k>=n && k<=m) throw std::domain_error("K u intervalu [n,m]");
        int razlika = m-n;
        
        Cvor *temp = p_pocetak, *n_cvor = p_pocetak, *m_cvor = p_pocetak, k_cvor=p_pocetak, prije_k = p_pocetak;
        
        int brojac = 0;
        while(brojac < n){
            n_cvor = n_cvor->p_sljedeci;    
            brojac++;
        }
        
        brojac = 0;
        while(brojac < m){
            m_cvor = m_cvor->p_sljedeci;    
            brojac++;
        }
        
        brojac = 0;
        //ako je k==0 pogresan je prije_k jer treba biti a on pokazuje na pocectak;
        while(brojac < k-1){
            prije_k = prije_k->p_sljedeci;    
            brojac++;
        }
        if(k==0){
            
        }
    }
};


int Menu(){
    int izbor;
    std::cout << "1. Dodaj element ispred" << std::endl;
    std::cout << "2. Dodaj element iza" << std::endl;
    std::cout << "3. Izbrisi element" << std::endl;
    std::cout << "4. Idi na sljedeci" << std::endl;
    std::cout << "5. Idi na prethodni" << std::endl;
    std::cout << "6. Broj elemenata" << std::endl;
    std::cout << "7. Trenutni element" << std::endl;
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
int main()
{
    JednostrukaLista<int> jed_lista;
    jed_lista.dodajIspred(450);
    jed_lista.dodajIza(500);
    jed_lista.sljedeci();
    jed_lista.dodajIza(466);
    jed_lista.sljedeci();
    jed_lista.dodajIza(20);
    jed_lista.sljedeci();
    jed_lista.dodajIza(15);
    jed_lista.sljedeci();
    jed_lista.dodajIza(5);
    jed_lista.sljedeci();
    jed_lista.dodajIza(2);
    jed_lista.sljedeci();
    jed_lista.dodajIza(1);
    jed_lista.pocetak();
    for(int i=0; i<jed_lista.brojElemenata(); i++){
        std::cout << jed_lista.trenutni() << " ";
        jed_lista.sljedeci();
    }
    //jed_lista.PremjestiUzObrtanje(3, 6, 2);
    jed_lista.pocetak();
    std::cout<< "Nova lista je: " << std::endl;
    for(int i=0; i<jed_lista.brojElemenata(); i++){
        std::cout << jed_lista.trenutni() << " ";
        jed_lista.sljedeci();
    }
    // NizLista<int> niz_lista;
    // int izbor;
    // JednostrukaLista<int> jed_lista;
    // while((izbor = Menu()) != 0){
    //     try{
    //         switch(izbor){
    //             case 0:
    //                 return 0;
    //             case 1:
    //                 std::cout << "Unesite element: ";
    //                 int x;
    //                 std::cin >> x;
    //                 niz_lista.dodajIspred(x);
    //                 jed_lista.dodajIspred(x);
    //                 break;
    //             case 2:
    //                 std::cout << "Unesite element: ";
    //                 std::cin >> x;
    //                 niz_lista.dodajIza(3);
    //                 jed_lista.dodajIza(4);
    //                 break;
    //             case 3:
    //                 niz_lista.obrisi();
    //                 jed_lista.obrisi();
    //                 break;
    //             case 4:
    //                 niz_lista.sljedeci();
    //                 jed_lista.sljedeci();
    //                 break;
    //             case 5:
    //                 jed_lista.brisiOdAdoB(4, 7);
    //                 break;
    //             case 6:
    //                 std::cout << "Niz lista ima " << niz_lista.brojElemenata() << " elemenata" << std::endl;
    //                 std::cout << "Jednostruka lista ima: " << jed_lista.brojElemenata() << " elemenata" << std::endl;
    //                 break;
    //             case 7:
    //                 std::cout << "Trenutni element niz liste: "<< niz_lista.trenutni() << std::endl; 
    //                 std::cout << "Trenutni element jednostruke liste: "<< jed_lista.trenutni() << std::endl;
    //                 break;
    //             default:
    //                 std::cout << "Pogresan izbor" << std::endl;
    //         }
    //     } catch(...){
    //         std::cout<< "Greska";
    //     }
    // }
    return 0;
}
