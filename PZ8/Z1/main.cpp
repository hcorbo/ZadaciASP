#include <iostream>
#include <stdexcept>
#include <utility>
#include <stdlib.h>     /* srand, rand */
#include <time.h>

const int VEL = 1000;

using namespace std;
template <typename TipKljuca, typename TipVrijednosti>
class Mapa{
    public:
    Mapa(){};
    virtual TipVrijednosti& operator[] (const TipKljuca &kljuc) = 0;
    virtual TipVrijednosti operator[] (const TipKljuca &kljuc) const = 0;
    virtual int brojElemenata() const=0;
    virtual void obrisi() = 0;
    virtual void obrisi(const TipKljuca &x) = 0;
    virtual ~Mapa(){};
};



template <typename TipKljuca, typename TipVrijednosti>
class BinStabloMapa : public Mapa<TipKljuca, TipVrijednosti>{
    struct Cvor{
        TipKljuca kljuc;
        TipVrijednosti vrijednost;
        Cvor *lijevo, *desno, *roditelj;
        Cvor () { lijevo = desno = roditelj = nullptr; }
        Cvor (const TipKljuca &kljuc, const TipVrijednosti &vrijednost, Cvor *l = nullptr, Cvor *d = nullptr, Cvor *r = nullptr){
            this->kljuc = kljuc;
            this->vrijednost = vrijednost;
            lijevo = l;
            desno = d;
            roditelj = r;
        }
    };
    
    Cvor *korijen;
    int brojEl;
    
    public:
    BinStabloMapa(){
        korijen = nullptr;
        brojEl = 0;
    }
    
    BinStabloMapa(const BinStabloMapa &mapa) : korijen(nullptr), brojEl(0){
        kopirajStablo(korijen, mapa.korijen, nullptr);
    }
    
    BinStabloMapa& operator=(const BinStabloMapa &mapa){
        if(this == &mapa)
            return *this;
        obrisiPomocna(korijen);
        korijen = nullptr;
        brojEl = 0;
        kopirajStablo(korijen, mapa.korijen, nullptr);
        return *this;
    }
    TipVrijednosti operator[] (const TipKljuca &kljuc) const{
        auto rezultat = trazi(this->korijen, kljuc);
        if(rezultat != nullptr)
            return rezultat->vrijednost;
        TipVrijednosti pom {};
        return pom;
    }
    
    TipVrijednosti& operator[] (const TipKljuca &kljuc) {
        Cvor* cv = trazi(korijen, kljuc);
        if(cv != nullptr)
            return cv->vrijednost;
        cv = dodaj(korijen, kljuc, nullptr);
        return cv->vrijednost;
    }
    
    int brojElemenata() const {
        return brojEl;
    }
    
    //Brise sve elemente
    void obrisiPomocna(Cvor *cv){
        if(cv == nullptr) return;
        obrisiPomocna(cv->lijevo);
        obrisiPomocna(cv->desno);
        delete cv;
    }
    
    void obrisi(){
        obrisiPomocna(korijen);
        korijen = nullptr;
        brojEl = 0;
    }
    
    void obrisi(const TipKljuca &x){
        Cvor *p = korijen, *q = nullptr, *tmp = nullptr, *rp = nullptr, *pp = nullptr;
        while(p != nullptr && p->kljuc != x){
            q = p;
            if(x < p->kljuc)
                p = p->lijevo;
            else 
                p = p->desno;
        }
        if(p == nullptr)
            throw std::logic_error("Nije pronadjen element x");
        if(p->lijevo == nullptr)
            rp = p->desno;
        else if(p->desno == nullptr)
            rp = p->lijevo;
        else{
            pp = p;
            rp = p->lijevo;
            tmp = rp->desno;
            while(tmp!=nullptr){
                pp = rp;
                rp = tmp;
                tmp = rp->desno;
            }
            if(pp != p){
                pp->desno = rp->lijevo;
                rp->lijevo = p->lijevo;
                pp->roditelj = rp;
                rp->roditelj = q;
                if(pp->desno)
                    pp->desno->roditelj = pp;
            }
            rp->desno = p->desno;
        }
        if(q == nullptr)
            korijen = rp;
        else if(p == q->lijevo)
            q->lijevo = rp;
        else
            q->desno = rp;
        delete p;
        brojEl--;
        
    }
    
    ~BinStabloMapa(){
        obrisiPomocna(korijen);
    }
    
    //Trazi rekurzivno
    Cvor* trazi(Cvor *cv, const TipKljuca &x) const{
        if (cv == nullptr || cv->kljuc == x)
            return cv;
        else if(x < cv->kljuc)
            return trazi(cv->lijevo, x);
        else 
            return trazi(cv->desno, x);
    }
    
    Cvor *dodaj (Cvor *&cv, const TipKljuca &kljuc, Cvor *r){
        if(cv == nullptr){
            brojEl++;
            cv = new Cvor(kljuc, TipVrijednosti(), nullptr, nullptr, r);
            return cv;
        }
        else if(cv->kljuc > kljuc)
            return dodaj(cv->lijevo, kljuc, cv);
        else return dodaj(cv->desno, kljuc, cv);
    }
    
    void kopirajStablo(Cvor *&cv, Cvor *cv2, Cvor *r){
        if(cv2 != nullptr){
            cv = new Cvor(cv2->kljuc, cv2->vrijednost, nullptr, nullptr, r);
            kopirajStablo(cv->lijevo, cv2->lijevo, cv);
            kopirajStablo(cv->desno, cv2->desno, cv);
            brojEl++;
        }
    }
};

template <typename TipKljuca, typename TipVrijednosti>
class NizMapa : public Mapa<TipKljuca, TipVrijednosti>{
    struct Pair{
      TipKljuca key;
      TipVrijednosti value;
      Pair(const TipKljuca &tk, const TipVrijednosti &tv){ 
          key = tk;
          value = tv;
      }
      
      Pair(const TipKljuca &tk){
          key = tk;
          value = TipVrijednosti();
      }
    };
    Pair **parovi;
    int kapacitet;
    int broj_elemenata;
    
    void povecajVel(){
        kapacitet*=2;
        Pair** pomocna = new Pair *[kapacitet];
        copy(parovi, parovi + broj_elemenata, pomocna);
        this->obrisi();
        delete [] parovi;
        parovi = pomocna;
    }
    
    public:
    NizMapa() : Mapa<TipKljuca, TipVrijednosti>(), parovi(new Pair*[VEL]), kapacitet(VEL), broj_elemenata(0){}
    
    NizMapa(const NizMapa<TipKljuca, TipVrijednosti> &n){
        broj_elemenata=n.broj_elemenata;
        kapacitet=n.kapacitet;
        parovi=new Pair*[kapacitet];
        for(int i=0; i<broj_elemenata; i++){
            TipKljuca pomk = n.parovi[i]->key;
            TipVrijednosti pomv = n.parovi[i]->value;
            parovi[i] = new Pair(pomk, pomv);
        }
    }
    
    ~NizMapa(){
        for(int i=0; i<broj_elemenata; i++){
            delete parovi[i];
        }
        delete [] parovi;
    }
    
    NizMapa<TipKljuca, TipVrijednosti> &operator =(const NizMapa<TipKljuca, TipVrijednosti> &nm){
        if(&nm == this)
            return *this;
            
        for(int i=0; i<broj_elemenata; i++){
            delete parovi[i];
        }
        delete [] parovi;
        
        broj_elemenata=nm.broj_elemenata;
        kapacitet=nm.kapacitet;
        parovi=new Pair*[kapacitet];
        for(int i=0; i<broj_elemenata; i++){
            TipKljuca pomk = nm.parovi[i]->key;
            TipVrijednosti pomv = nm.parovi[i]->value;
            parovi[i] = new Pair(pomk, pomv);
        }
        return *this;
    }
    
    void obrisi(){
        for(int i=0; i<broj_elemenata; i++){
            delete parovi[i];
        }
        broj_elemenata=0;
    }
    
    void obrisi(const TipKljuca &kljuc){
        for(int i=0; i<broj_elemenata; i++){
            if(parovi[i]->key == kljuc){
                delete parovi[i];
                parovi[i] = parovi[broj_elemenata-1];
                broj_elemenata--;
                parovi[broj_elemenata]=nullptr;
                return;
            }
        }
        throw std::domain_error("Kljuc ne postoji");
        
    }
    
    TipVrijednosti operator [](const TipKljuca &kljuc) const{   //mozda ne radi zbog istog imena
        for(int i=0; i<broj_elemenata; i++){
            if(parovi[i]->key == kljuc)
                return parovi[i]->value;
        }
        TipVrijednosti pomocna{};
        return pomocna;
    }
    
    TipVrijednosti& operator [](const TipKljuca &kljuc){
        for(int i=0; i<broj_elemenata; i++){
            if(parovi[i]->key == kljuc)
                return parovi[i]->value;
        }
        if(broj_elemenata<kapacitet){
            parovi[broj_elemenata] = new Pair(kljuc);
            parovi[broj_elemenata]->key = kljuc;
            return parovi[broj_elemenata++]->value;
        } else{
            povecajVel();
            parovi[broj_elemenata] = new Pair(kljuc);
            parovi[broj_elemenata]->key = kljuc;
            return parovi[broj_elemenata++]->value;
        }
    }
    int brojElemenata() const { return broj_elemenata; }
};

int main() {
    BinStabloMapa <int, int> bsMapa;
    NizMapa<int, int> nizMapa;
    int vel(1000);
    int n=10;
    //vrijeme dodavanja
    clock_t vrijeme1 = clock();
    for (int i=0; i<vel; i++){
        bsMapa[n++] = i;
    }
    clock_t vrijeme2 = clock();
    n=10;
    clock_t vrijeme3 = clock();
    for (int i=0; i<vel; i++)
        nizMapa[n++] = i;
    clock_t vrijeme4 = clock();
    //dodavanje novog elementa je brze u niz dok je mali broj elemenata u mapama, ali sa porastom broja elemenata 
    //ubacivanje u niz ce za neki broj postati sporije od ubcvanja u binarno stablo
    int ukvrijemeBsMape = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    cout << "Vrijeme izvrsenja: " << ukvrijemeBsMape << " ms." << endl;
    int ukvrijemeNizMape = (vrijeme4 - vrijeme3) / (CLOCKS_PER_SEC / 1000);
    cout << "Vrijeme izvrsenja: " << ukvrijemeNizMape << " ms." << endl;
    
    //vrijeme pristupa
    vrijeme1 = clock();
    bsMapa[4000] = 1234;
    vrijeme2 = clock();
    vrijeme3 = clock();
    nizMapa[4000] = 1234;
    vrijeme4 = clock();
    //vrijeme pristupa elementima niz mape je mnogo duze od pristupa bs mapi
    ukvrijemeBsMape = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 100000);
    cout << "Vrijeme izvrsenja: " << ukvrijemeBsMape << " ms./100" << endl;
    ukvrijemeNizMape = (vrijeme4 - vrijeme3) / (CLOCKS_PER_SEC / 100000);
    cout << "Vrijeme izvrsenja: " << ukvrijemeNizMape << " ms./100" << endl;
    return 0;
}
