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
        //     BSCvor<tip1, tip2>* tmp = new BSCvor<tip1, tip2>(x, tip2 {});
        // this->Umetni(tmp);
        // return tmp->vrijednost;
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
        }
        else if(p == q->lijevo){
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
        bool pom(false);
        if (y == 0) {
            korijen = z;
            brojEl++;
            pom = true;
        } else {
            if (z->kljuc < y->kljuc) {
                y->lijevo = z;
                z->roditelj = y;
                brojEl++;
                if(y->desno) {
                    pom = true;
                    y->balans = 0;
                }
            } else {
                y->desno = z;
                z->roditelj = y;
                brojEl++;
                if(y->lijevo) {
                    pom = true ;
                    y->balans = 0;
                }
            }
        }
        if(!pom)
            azurirajBalans(z);
    }
   //rotacije

    void azurirajBalans(Cvor* &cvor) {
        if(cvor->roditelj == nullptr) return;
        if(!(cvor->lijevo && cvor->desno && cvor->balans == 0)) {
            if(cvor->roditelj->lijevo == cvor) 
               cvor->roditelj->balans++;
            else 
                cvor->roditelj->balans--;
        }
        bool nastaviti(true);
        Cvor* r = cvor->roditelj;
        Cvor* r1 = cvor;
        if(cvor->balans > 0 && cvor->roditelj->balans < -1) {
            jednostrukaRotacijaUdesno(r1);
            jednostrukaRotacijaUlijevo(r);
            nastaviti = false;
        } 
        else if(cvor->balans < 0 && cvor->roditelj->balans > 1) {
            jednostrukaRotacijaUlijevo(r1);
            jednostrukaRotacijaUdesno(r);
            nastaviti = false;
        }
        else if(cvor->roditelj->balans < -1) {
            jednostrukaRotacijaUlijevo(r);
            nastaviti = false;
        }
        else if(cvor->roditelj->balans > 1) {
            jednostrukaRotacijaUdesno(r);
            nastaviti = false;
        }
        if(nastaviti) 
            azurirajBalans(cvor->roditelj);
        
    }
    
    void jednostrukaRotacijaUlijevo(Cvor* &cvor) {
       if(cvor->roditelj && cvor->roditelj->desno == cvor) 
            cvor->roditelj->desno = cvor->desno;
        else if(cvor->roditelj) 
            cvor->roditelj->lijevo = cvor->desno;
        Cvor *tmp = cvor->desno;
        bool imaDesno(false);
        if(cvor->desno->lijevo) { 
           cvor->desno = tmp->lijevo;
           tmp->lijevo->roditelj = cvor;
           tmp->lijevo = nullptr;
           imaDesno = true;
        }
        tmp->roditelj = cvor->roditelj;
        if(cvor->roditelj)
            cvor->roditelj = tmp;
        else {
            cvor->roditelj = tmp;
            korijen = tmp;
        }
        tmp->lijevo = cvor;
        cvor->balans = 0;
        tmp->balans = 0;
        if(!imaDesno)
            cvor->desno = nullptr;
    }

    void jednostrukaRotacijaUdesno(Cvor* &cvor) {
        if(cvor->roditelj && cvor->roditelj->lijevo == cvor) 
            cvor->roditelj->lijevo = cvor->lijevo;
        else if(cvor->roditelj) 
            cvor->roditelj->desno = cvor->lijevo;
        Cvor * tmp = cvor->lijevo;
        bool imaLijevo(false);
        if(cvor->lijevo->desno) { 
            cvor->lijevo = tmp->desno;
            tmp->desno->roditelj = cvor;
            tmp->desno = nullptr;
            imaLijevo = true;
        }
        tmp->roditelj = cvor->roditelj;
        if(cvor->roditelj)
            cvor->roditelj = tmp;
        else {
            cvor->roditelj = tmp;
            korijen = tmp;
        }
        tmp->desno = cvor;
        tmp->balans = 0;
        cvor->balans = 0;
        if(!imaLijevo)
            cvor->lijevo = nullptr;
    }

};

int main() {
// Malo jaci test performansi AVLStabloMape gdje se vidi 
// da li je zaista O(log n)
AVLStabloMapa <int, int> m;
int vel(2000000);
for (int i(0); i<vel; i++)
    m[rand()%(2*vel)-vel] = i;
for (int i(-vel); i<vel; i++)
    m[i] = i;
for (int i(-vel); i<vel; i++) {
    if (m[i] != i) { cout << i << " NOT "; break; }
    m.obrisi(i);
}
cout << "OK";
    return 0;
}
