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

bool testDodjele(NizMapa<int, double> nm){
    NizMapa<int, double> testna = nm;
    bool rezultat = true;
    for(int i=0; i<testna.brojElemenata(); i++){
        if(testna.brojElemenata()!=nm.brojElemenata()){
            rezultat = false;
            break;
        }
    }
    return rezultat;
}

template <typename TipKljuca, typename TipVrijednosti>
void testBrisanja(NizMapa<TipKljuca, TipVrijednosti> nm){
    std::cout<< "Niz mapa je imala: " << nm.brojElemenata() << " elemenata\n";
    nm.obrisi();
    std::cout << "Ocekivani rezultat: 0" << std::endl;
    std::cout << "Vas program je ispisao: " << nm.brojElemenata();
}

template <typename TipKljuca, typename TipVrijednosti>
void testBrisanja2(NizMapa<TipKljuca, TipVrijednosti> nm){
    std::cout<< "Niz mapa je imala: " << nm.brojElemenata() << " elemenata\n";
    std::string s = "Azija";
    std::string s1 = "Afrika";
    nm.obrisi(s);
    nm.obrisi(s1);
    std::cout << "Ocekivani rezultat: 2" << std::endl;
    std::cout << "Vas program je ispisao: " << nm.brojElemenata();
}

void testBrojaElemenata(NizMapa<std::string, double> nm){
    int brEl=nm.brojElemenata();
    std::cout<< "Niz mapa je imala: " << brEl << " elemenata\n";
    nm["Zenica"] = 50000;
    nm["Tuzla"] = 60000;
    std::cout << "Ocekivani rezultat: " << brEl+2 << std::endl;
    std::cout << "Vas program je ispisao: " << nm.brojElemenata();
    
}

void test5(){
    NizMapa<int, int> nm;
    nm[1] = 13456;
    nm[2] = 13543;
    nm[3] = 13789;
    int broj = 13790;
    for(int i=4; i<9; i++){
        nm[i] = broj++;
    }
    
}

PrebaciStabloAB(vector<int> A, Cvor*B){
    
} 
int main() {
    NizMapa<std::string,double> m;
    m["Sarajevo"] = 100000;
    m["Mostar"] = 40000;
    testBrojaElemenata(m);
    NizMapa<int, double> m1;
    m["18227"] = 9.5;
    m["18225"] = 14;
    bool rezultat = testDodjele(m1);
    if(rezultat) std::cout<< "\nDodjela radi";
        else std::cout << "\nDodjela ne radi";
    testBrisanja(m);
    NizMapa<std::string,std::string> m2;
    m2["Azija"] = "Kina";
    m2["Afrika"] = "Tunis";
    m2["Europa"] = "BiH";
    m2["Amerika"] = "Canada";
    testBrisanja2(m2);
    test5();
    return 0;
}
