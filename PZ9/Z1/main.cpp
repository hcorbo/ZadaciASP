#include <iostream>
#include <stdexcept>
#include <utility>

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
    virtual void obrisi(const TipKljuca &kljuc) = 0;
    virtual ~Mapa(){};
};

template<typename TipKljuca, typename TipVrijednosti>
class HashMapa: public Mapa<TipKljuca, TipVrijednosti>{
        struct Par{
        TipKljuca key;
        TipVrijednosti value;
        bool obrisan;
        Par(const TipKljuca& kljuc, const TipVrijednosti& vrijednost, bool obr):key(kljuc), value(vrijednost),obrisan(obr){}
        Par(const TipKljuca& kljuc, bool obr):key(kljuc), value(TipVrijednosti()),obrisan(obr){}
    };
    Par** parovi;
    int kapacitet;
    int broj_el;
    unsigned int (*h)(TipKljuca, unsigned int);
    void povecaj_kapacitet();
public:
    HashMapa();
    ~HashMapa();
    HashMapa(const HashMapa<TipKljuca,TipVrijednosti>& hmapa);
    HashMapa<TipKljuca,TipVrijednosti>& operator=(const HashMapa<TipKljuca,TipVrijednosti>& hmapa);
    TipVrijednosti operator [](const TipKljuca& kljuc) const;
    TipVrijednosti& operator [](const TipKljuca& kljuc);
    int brojElemenata() const {return broj_el; }
    void obrisi();
    void obrisi(const TipKljuca& kljuc);
    void definisiHashFunkciju(unsigned int (*f) (TipKljuca, unsigned int)) {h=f;}

};

template<typename TipKljuca, typename TipVrijednosti>
void HashMapa<TipKljuca,TipVrijednosti>:: povecaj_kapacitet(){ //povecamo ako je mal kapacitet pri alokaciji
    kapacitet*=2; //duplo
    Par** novi= new Par*[kapacitet];
    for(int i=0;i<kapacitet;i++)
        novi[i]=nullptr;
    for(int i=0;i<kapacitet/2;i++){
        if(parovi[i]!=nullptr && !parovi[i]->obrisan){
            int indeks;
            unsigned int hash=h(parovi[i]->key, kapacitet);
            for(int j=0;j<kapacitet;j++){
                indeks=(hash+j)%kapacitet;
                if(novi[indeks]== nullptr){
                    novi[indeks]=new Par(parovi[i]->key,parovi[i]->value, parovi[i]->obrisan);
                    break;
                }
            }
            delete parovi[i];
        }
    }
    delete[] parovi;
    parovi=novi;
}

template<typename TipKljuca, typename TipVrijednosti>
HashMapa<TipKljuca, TipVrijednosti>::HashMapa():Mapa<TipKljuca, TipVrijednosti>(){
    parovi=new Par*[VEL];
    kapacitet=VEL;
    broj_el=0;
    h=nullptr;
    for(int i=0;i<kapacitet;i++)
        parovi[i]=nullptr;
}

template<typename TipKljuca, typename TipVrijednosti>
HashMapa<TipKljuca,TipVrijednosti>::~HashMapa(){ //desturktor
    for(int i=0;i<kapacitet;i++){
        if(parovi[i]!=nullptr)
            delete parovi[i];}
        delete[] parovi;
}

template<typename TipKljuca, typename TipVrijednosti>
HashMapa<TipKljuca, TipVrijednosti>:: HashMapa(const HashMapa<TipKljuca, TipVrijednosti>& hmapa){
    broj_el=hmapa.broj_el;
    kapacitet=hmapa.kapacitet;
    h=hmapa.h;
    parovi=new Par*[kapacitet];
    for(int i=0;i<kapacitet;i++){
        if(hmapa.parovi[i]!=nullptr)
            parovi[i]=new Par(hmapa.parovi[i]->key, hmapa.parovi[i]->value, hmapa.parovi[i]->obrisan );
            else
            parovi[i]=nullptr;
    }
}

template<typename TipKljuca, typename TipVrijednosti>
HashMapa<TipKljuca,TipVrijednosti>& HashMapa<TipKljuca,TipVrijednosti>::operator=(const HashMapa<TipKljuca,TipVrijednosti>& hmapa){
    if(&hmapa== this)    //samododjela
        return *this;
    for(int i=0;i<kapacitet;i++){ //desturktor
        if(parovi[i]!=nullptr)
            delete parovi[i];
    }

    delete[] parovi;
    broj_el=hmapa.broj_el; //kosnturkotr
    kapacitet=hmapa.kapacitet;
    h=hmapa.h;
    parovi=new Par*[kapacitet];
    for(int i=0;i<kapacitet;i++){
        if(hmapa.parovi[i]!=nullptr)
            parovi[i]=new Par(hmapa.parovi[i]->key, hmapa.parovi[i]->value, hmapa.parovi[i]->obrisan );
                else
                parovi[i]=nullptr;
    }
    return *this;
}

template<typename TipKljuca, typename TipVrijednosti>
TipVrijednosti HashMapa<TipKljuca,TipVrijednosti>:: operator [](const TipKljuca& kljuc) const {
    if(!h)
        throw"Greska";
        unsigned int hash=h(kljuc,kapacitet);
        int indeks;
        for(int i=0;i<kapacitet;i++){
            indeks=(hash+i)%kapacitet;
            if(parovi[indeks]!=nullptr){
                if(parovi[indeks]->key== kljuc && !parovi[indeks]->obrisan)
                    return parovi[indeks]->value;
            }
            else
                return TipVrijednosti();
        }
    return TipVrijednosti();
}

template<typename TipKljuca, typename TipVrijednosti>
TipVrijednosti& HashMapa<TipKljuca, TipVrijednosti>:: operator [](const TipKljuca& kljuc){
    if(!h) throw "Greska";
    if(broj_el+kapacitet/10>=kapacitet)
        povecaj_kapacitet();
    unsigned int hash=h(kljuc, kapacitet);
    int indeks, indeksUmetanja=-1;
    for(int i=0;i<kapacitet;i++){
        indeks=(hash+i)%kapacitet;
        if(parovi[indeks]!=nullptr){
            if(parovi[indeks]->key==kljuc && !parovi[indeks]->obrisan) return parovi[indeks]->value;
            if(parovi[indeks]->obrisan&& indeksUmetanja==-1)
                indeksUmetanja=indeks;
        }
        else {
            if(indeksUmetanja==-1){
                parovi[indeks]=new Par(kljuc,false);
                broj_el++;
                return parovi[indeks]->value;
            }
            else {
                delete parovi[indeksUmetanja];
                parovi[indeksUmetanja]=new Par(kljuc,false);
                broj_el++;
                return parovi[indeksUmetanja]->value;
            }
        }
    }
}

template <typename TipKljuca, typename TipVrijednosti>
void HashMapa<TipKljuca,TipVrijednosti>::obrisi(){ //ko dekst
    for(int i=0;i<kapacitet;i++){
        if(parovi[i]!=nullptr){
            delete parovi[i];
            parovi[i]=nullptr;
        }
    }
    broj_el=0;
}

template<typename TipKljuca, typename TipVrijednosti>
void HashMapa<TipKljuca,TipVrijednosti>::obrisi(const TipKljuca& kljuc){
    unsigned int hash=h(kljuc,kapacitet);
    unsigned int indeks;
    for(int i=0;i<kapacitet;i++){
        indeks=(hash+i)%kapacitet;
        if(parovi[indeks]!=nullptr){
            if(parovi[indeks]->key == kljuc && !parovi[indeks]->obrisan){
                parovi[indeks]->obrisan=true;
                broj_el--;
                return;
            }
        }
        else
            break;
    }
    throw "Izuzetak";
}

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

unsigned int hashfunckija(int x, unsigned int max)
{
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x % max;
}

int main() {
    BinStabloMapa <int, int> bsMapa;
    NizMapa<int, int> nizMapa;
    HashMapa<int, int> hashMapa;
    hashMapa.definisiHashFunkciju(hashfunckija);
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
    vrijeme1 = clock();
    //dodavanje u hashMapu
    n=10;
    for (int i=0; i<vel; i++){
        hashMapa[n++] = i;
    }
    vrijeme2 = clock();
    int ukvrijemeHashMape = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    cout << "Vrijeme izvrsenja: " << ukvrijemeHashMape << " ms." << endl << endl;
    
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
    
    //vrijeme pristupa Hash Mape
    vrijeme1 = clock();
    hashMapa[4000] = 1234;
    vrijeme2 = clock();
     ukvrijemeHashMape = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 100000);
    cout << "Vrijeme izvrsenja: " << ukvrijemeHashMape << " ms./100" << endl;
    return 0;
}