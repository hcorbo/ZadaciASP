#include <iostream>
#include <stdexcept>
#include <cstring>
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
    Cvor *p_pocetak=0, *p_kraj=0, *p_trenutni=0;
    int brojEl=0;
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
    int elBroj(){
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
    
    int suma(Cvor *poc, int sume=0){
        int sum=sume;
        if(poc==0) return sum;
        sum+=poc->element;
        suma(poc->p_sljedeci, sum);
    }
    
    void brisiUnazadManjeOdSumePrethodnih(){
        if(p_pocetak == 0) throw std::domain_error("Izuzetak");
        Cvor *pom = p_kraj;
        for(int i=this->brojElemenata(); i>0; i--){   //provjeriti broj el
            if(this->suma(pom->p_sljedeci)>(pom->element)){
                std::cout << "Izbacili smo: " << pom->element << std::endl;
                brojEl--;
                if(pom == p_pocetak){
                    if(p_trenutni==p_pocetak) p_trenutni = p_pocetak->p_sljedeci;
                    p_pocetak =p_pocetak->p_sljedeci;
                    delete pom;
                    if(brojEl==0) p_kraj=0;
                } else {
                    Cvor *pomocna = p_pocetak;
                    while(pomocna->p_sljedeci != pom)
                        pomocna = pomocna->p_sljedeci;
                    pomocna->p_sljedeci=pom->p_sljedeci;
                    if(p_trenutni==pom) p_trenutni = p_trenutni->p_sljedeci;
                    delete pom;
                    pom=pom->p_sljedeci;
                }
            }
            Cvor *pom2 = p_pocetak;
            while(pom2->p_sljedeci != pom)
            pom2=pom2->p_sljedeci;
            pom=pom2;
        }
    }
    
    void obrni(){
        Cvor *p = p_pocetak;
        Cvor *q = 0;
        while(p!=0){
            Cvor *r = q;
            q = p;
            p = p->p_sljedeci;
            q->p_sljedeci=r;
        }
        p_kraj=p_pocetak;
        p_pocetak=q;
    }
    
    
    int izbaciSvakiNTi(){
       /* if(n>this->brojElemenata()) throw std::domain_error("Greska u izbaciSvakiNTi\n");
         int brojac=1;
         Cvor *pom=p_trenutni;
         p_trenutni=p_pocetak;
         while(brojac < this->brojElemenata()+1){
             if(brojac%n==0){
                this->obrisi();
                
             }
             brojac++;
         }*/
         int sume = this->suma(p_pocetak);
         return sume;
    }
    
    void ispisiJed(){
        Cvor *temp = p_pocetak;
        while(temp!=0){
            std::cout<< temp->element << " ";
            temp=temp->p_sljedeci;
        }
    }
    
    void Umetanje(JednostrukaLista &rijec, JednostrukaLista &zamjena){
        if(rijec.brojElemenata() == 0) return;
        Cvor *p=p_pocetak;
        while(p != 0){  //krecem se kroz listu pocetnu
            rijec.pocetak();
            if(rijec.trenutni() == p->element){  //ako je isto prvo slovo
                Cvor *temp = p;
                std::cout<< "DA";
                bool pronasao = true;
                rijec.pocetak();
                while(rijec.sljedeci()){    //ako sam pronasao istu rijec
                    p = p->p_sljedeci;
                    if(rijec.trenutni() != p->element){ 
                        pronasao = false;
                        p = temp;
                        break;
                    }
                }
                if(pronasao){
                    if(rijec.brojElemenata()==zamjena.brojElemenata()){
                        zamjena.pocetak();
                        int brojac=zamjena.brojElemenata();
                        while(brojac>0){
                            temp->element = zamjena.trenutni();
                            zamjena.sljedeci();
                            temp=temp->p_sljedeci;
                            brojac--;
                        }
                    }
                    if(rijec.brojElemenata()<zamjena.brojElemenata()){
                        zamjena.pocetak();
                        int brojac=rijec.brojElemenata();
                        while(brojac>0){
                            temp->element = zamjena.trenutni();
                            zamjena.sljedeci();
                            if(brojac!=1)
                            temp=temp->p_sljedeci;
                            brojac--;
                        }
                        Cvor *novi = new Cvor;
                        brojEl++;
                        novi->element=zamjena.trenutni();
                        novi->p_sljedeci=temp->p_sljedeci;
                        temp->p_sljedeci = novi;
                        temp=temp->p_sljedeci;
                        
                        while(zamjena.sljedeci()){
                        Cvor *novi = new Cvor;
                        brojEl++;
                        novi->element=zamjena.trenutni();
                        novi->p_sljedeci=temp->p_sljedeci;
                        temp->p_sljedeci = novi;
                        temp=temp->p_sljedeci;
                        }
                        
                        novi = new Cvor;
                        brojEl++;
                        novi->element=zamjena.trenutni();
                        novi->p_sljedeci=temp->p_sljedeci;
                        temp->p_sljedeci = novi;
                    }
                    if(rijec.brojElemenata()>zamjena.brojElemenata()){
                        zamjena.pocetak();
                        rijec.pocetak();
                        int brojac=zamjena.brojElemenata();
                        int brojac1=rijec.brojElemenata();
                        while(brojac!=0){
                            temp->element = zamjena.trenutni();
                            zamjena.sljedeci();
                            if(brojac!=1)
                            temp=temp->p_sljedeci;
                            brojac--;
                            brojac1--;
                        }
                        while(brojac1>0){
                            Cvor *pom2 = temp;
                            temp->p_sljedeci=p->p_sljedeci->p_sljedeci;
                            pom2=pom2->p_sljedeci;
                           // delete pom2;
                            brojac1--;
                            brojEl--;
                        }
                    }
                }
            }
            p = p->p_sljedeci;
        }
    }
    /*void Obrni(int n,int m){
        //brisanje clana
        pocetak();
        int brojac=0;
        bool obrisan=false;
        int sredina=(m+n)/2;
        
        do{
            if(brojac==sredina){
                Cvor *brisat=tekuci->sljedeci;
                Cvor *pomocni=(tekuci->sljedeci)->sljedeci;
                tekuci->sljedeci=pomocni;
                delete brisat;
                duzina--;
                obrisan=true;
            }
            if(obrisan) break;
            brojac++;
        }while(sljedeci());
        
        
        //obrtanje
        m--;
        pocetak();
        brojac=0;
        int brojac1=0;
        Cvor *pomocni1;
        bool obrnuto=false;
        do{
            if(brojac==n){
                pomocni1=tekuci->sljedeci;
            }
            if(brojac1==m){
                sljedeci();
                while(n!=m){
                     int el=pomocni1->element;
                     pomocni1->element=tekuci->element;
                     tekuci->element=el;
                    n++;
                    m--;
                    pomocni1=pomocni1->sljedeci;
                    prethodni();
                }
                if(n==m) obrnuto=true;
            }
            brojac1++;
            brojac++;
            
        if(obrnuto) break;
        }while(sljedeci());
    }*/
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
    std::cout << "8. Izbaci svaki NTi" << std::endl;
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
{   JednostrukaLista<char> jednostrukaLista;
    JednostrukaLista<char> rijec;
    JednostrukaLista<char> z;
    jednostrukaLista.dodajIza('e');
    jednostrukaLista.sljedeci();
    jednostrukaLista.dodajIza('t');
    jednostrukaLista.sljedeci();
    jednostrukaLista.dodajIza('e');
    jednostrukaLista.sljedeci();
    jednostrukaLista.dodajIza('s');
    jednostrukaLista.sljedeci();
    jednostrukaLista.dodajIza('t');
    jednostrukaLista.sljedeci();
    jednostrukaLista.dodajIza('n');
    jednostrukaLista.sljedeci();
    jednostrukaLista.dodajIza('i');
    jednostrukaLista.sljedeci();
    jednostrukaLista.dodajIza('z');
    jednostrukaLista.sljedeci();
    jednostrukaLista.dodajIza('x');
    jednostrukaLista.sljedeci();
    jednostrukaLista.dodajIza('y');
    jednostrukaLista.sljedeci();
    jednostrukaLista.dodajIza('w');
    rijec.dodajIza('t');
    rijec.sljedeci();
    rijec.dodajIza('e');
    rijec.sljedeci();
    rijec.dodajIza('s');
    rijec.sljedeci();
    rijec.dodajIza('t');
    rijec.sljedeci();
    rijec.dodajIza('n');
    rijec.sljedeci();
    rijec.dodajIza('i');
    z.dodajIza('i');
    z.sljedeci();
    z.dodajIza('s');
    z.sljedeci();
    z.dodajIza('p');
    std::cout << "Rijec ima :" << z.brojElemenata();
    jednostrukaLista.ispisiJed();
    jednostrukaLista.Umetanje(rijec, z);
    
    std::cout<< std::endl;
    z.ispisiJed();
    std::cout<< std::endl;
    jednostrukaLista.ispisiJed();
    /*NizLista<int> niz_lista;
    int izbor;
    JednostrukaLista<double> jed_lista;
    while((izbor = Menu()) != 0){
        try{
            switch(izbor){
                case 0:
                    return 0;
                case 1:
                    std::cout << "Unesite element: ";
                    int x;
                    std::cin >> x;
                    niz_lista.dodajIspred(x);
                    jed_lista.dodajIspred(x);
                    break;
                case 2:
                    std::cout << "Unesite element: ";
                    std::cin >> x;
                    niz_lista.dodajIza(x);
                    jed_lista.dodajIza(x);
                    jed_lista.ispisiJed();
                    break;
                case 3:
                    niz_lista.obrisi();
                    jed_lista.obrisi();
                    break;
                case 4:
                    niz_lista.sljedeci();
                    jed_lista.sljedeci();
                    break;
                case 5:
                    niz_lista.prethodni();
                    jed_lista.prethodni();
                    break;
                case 6:
                    std::cout << "Niz lista ima " << niz_lista.brojElemenata() << " elemenata" << std::endl;
                    std::cout << "Jednostruka lista ima: " << jed_lista.brojElemenata() << " elemenata" << std::endl;
                    break;
                case 7:
                    std::cout << "Trenutni element niz liste: "<< niz_lista.trenutni() << std::endl; 
                    std::cout << "Trenutni element jednostruke liste: "<< jed_lista.trenutni() << std::endl;
                    break;
                case 8:
                    jed_lista.ispisiJed();
                    //std::cout << "\nSuma je " << jed_lista.izbaciSvakiNTi() << std::endl;
                    //jed_lista.brisiUnazadManjeOdSumePrethodnih();
                    jed_lista.obrni();
                    std::cout << std::endl;
                    jed_lista.ispisiJed();
                    
                    break;
                default:
                    std::cout << "Pogresan izbor" << std::endl;
            }
        } catch(...){
            std::cout<< "Greska";
        }
    }*/
    return 0;
}
