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
class AVLStabloMapa : public Mapa<TipKljuca, TipVrijednosti>{
    struct Cvor{
        TipKljuca kljuc;
        TipVrijednosti vrijednost;
        Cvor *lijevo, *desno, *roditelj;
        int balans = 0;
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
    AVLStabloMapa(){
        korijen = nullptr;
        brojEl = 0;
    }
    
    AVLStabloMapa(const AVLStabloMapa &mapa) : korijen(nullptr), brojEl(0){
        kopirajStablo(korijen, mapa.korijen, nullptr);
    }
    
    AVLStabloMapa& operator=(const AVLStabloMapa &mapa){
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
        Cvor *tmp = new Cvor(kljuc, TipVrijednosti{});
        this->Umetni(tmp);
        return tmp->vrijednost;
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
            p->desno->roditelj = rp;
        }
        if(q == nullptr){
            korijen = rp;
            if(korijen) korijen -> roditelj = nullptr;
        } else if (p == q->lijevo){
            q->lijevo = rp;
            if(rp) rp->roditelj = q;
        }
        else{
            q->desno = rp;
            if(rp) rp->roditelj = q;
        }
        if(p->roditelj && p->roditelj->lijevo == p) p->roditelj->balans--;
        else if(p->roditelj) p->roditelj->balans++;
        delete p;
        brojEl--;
        
    }
    
    ~AVLStabloMapa(){
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
    void Umetni(Cvor* &z){
        Cvor* y = 0;
        Cvor *x = korijen;
        while (x != 0) {
            y = x;
            if (z->kljuc < x->kljuc)
                x = x->lijevo;
            else
                x = x->desno;
        }
        bool imaBrudu = false;
        if (y == 0) {
            korijen = z;
            brojEl++;
            imaBrudu = true;
        } else {
            if (z->kljuc < y->kljuc) {
                y->lijevo = z;
                z->roditelj = y;
                brojEl++;
                if(y->desno) {
                    imaBrudu = true;
                    y->balans = 0;
                }
            } else {
                y->desno = z;
                z->roditelj = y;
                brojEl++;
                if(y->lijevo) {
                    imaBrudu = true ;
                    y->balans = 0;
                }
            }
        }
        if(!imaBrudu)
            azurirajBalans(z);
    }
    
    void rotacijaDesno(Cvor *&cv){
        if(cv->roditelj && cv->roditelj->lijevo == cv)
            cv->roditelj->lijevo = cv->lijevo;
        else if(cv->roditelj)
            cv->roditelj->desno = cv->lijevo;
        Cvor * pom = cv->lijevo;
        bool postoji(false);
        if(cv->lijevo->desno){
            cv->lijevo = pom->desno;
            pom->desno->roditelj = cv;
            pom->desno = nullptr;
            postoji = true;
        }
        pom->roditelj = cv->roditelj;
        if(cv->roditelj)
            cv->roditelj = pom;
        else{
            cv->roditelj = pom;
            korijen = pom;
        }
        pom->desno = cv;
        pom->balans = 0;
        cv->balans = 0;
        if(!postoji)
            cv->lijevo = nullptr;
    }
    
    void rotacijaLijevo(Cvor *&cv){
        if(cv->roditelj && cv->roditelj->desno == cv)
            cv->roditelj->desno = cv->desno;
        else if(cv->roditelj)
            cv->roditelj->lijevo = cv->desno;
        Cvor * pom = cv->desno;
        bool postoji(false);
        if(cv->desno->lijevo){
            cv->desno = pom->lijevo;
            pom->lijevo->roditelj = cv;
            pom->lijevo = nullptr;
            postoji = true;
        }
        pom->roditelj = cv->roditelj;
        if(cv->roditelj)
            cv->roditelj = pom;
        else{
            cv->roditelj = pom;
            korijen = pom;
        }
        pom->lijevo = cv;
        pom->balans = 0;
        cv->balans = 0;
        if(!postoji)
            cv->desno = nullptr;
    }
    
    void azurirajBalans(Cvor *&cv){
        if(cv->roditelj == nullptr) return;
        if(!(cv->lijevo && cv->desno && cv->balans == 0)){
            if(cv->roditelj->lijevo == cv)
                cv->roditelj->balans++;
            else 
                cv->roditelj->balans--;
        }
        bool pom = true;
        Cvor *cv1 = cv->roditelj;
        Cvor *cv2 = cv;
        if(cv->balans > 0 && cv->roditelj->balans < -1){
            rotacijaDesno(cv2);
            rotacijaLijevo(cv1);
            pom = false;
        } else if(cv->balans < 0 && cv->roditelj->balans > 1){
            rotacijaLijevo(cv2);
            rotacijaDesno(cv1);
            pom = false;
        } else if(cv->roditelj->balans < -1){
            rotacijaLijevo(cv1);
            pom = false;
        } else if(cv->roditelj->balans > 1){
            rotacijaDesno(cv1);
            pom = false;
        }
        if(pom)
            azurirajBalans(cv->roditelj);
    }    
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


int main() {
    //vidimo da je vrijeme dodavanja u AVLmapu brze od BSmape
    AVLStabloMapa <int, int> avlmap;
    BinStabloMapa<int, int> bsmap;
    int vel(100000);
    clock_t vrijeme1 = clock();
    for (int i = 0; i<vel; i++)
        bsmap[rand()%(2*vel)-vel] = i;
    for (int i = -vel; i<vel; i++)
        bsmap[i] = i;
    clock_t vrijeme2 = clock();
    int ukvrijemeBsMape = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    cout << "Vrijeme izvrsenja: " << ukvrijemeBsMape << " ms." << endl;
    
    vrijeme1 = clock();
    for (int i = 0; i<vel; i++)
        avlmap[rand()%(2*vel)-vel] = i;
    for (int i = -vel; i<vel; i++)
        avlmap[i] = i;
    vrijeme2 = clock();
    int ukvrijemeAVLMape = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    cout << "Vrijeme izvrsenja: " << ukvrijemeAVLMape << " ms." << endl;
    
    
    vrijeme1 = clock();
    for (int i = 0; i<100; i++)
        bsmap.obrisi(i);
    vrijeme2 = clock();
    vrijeme2 = clock();
    ukvrijemeBsMape = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    cout << "Vrijeme izvrsenja: " << ukvrijemeBsMape << " ms." << endl;
    
    vrijeme1 = clock();
    for (int i = 0; i<100; i++)
        avlmap.obrisi(i);
    vrijeme2 = clock();
    ukvrijemeAVLMape = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    cout << "Vrijeme izvrsenja: " << ukvrijemeAVLMape << " ms." << endl;

    return 0;
}
