#include <iostream>

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
    
    void brisi();
    //Funkcija koja dodaje na vrh steka
    void stavi (const Tip& el){
        vrha=new Cvor(el, vrha);
        velicina++;
    }
    
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

bool provjeri_zagrade(std::string tekst){
    Stek<char> s;
    for(int i = 0; i<tekst.length(); i++){
        if(tekst[i]=='(' || tekst[i]=='[' || tekst[i]=='{') s.stavi(tekst[i]);
        if(tekst[i]==')' || tekst[i]==']' || tekst[i]=='}') s.skini();
    }
    if(s.brojElemenata()==0) return true;
    return false;
}

int Menu(){
    int izbor;
    std::cout << "1. Stavi element na stek\n";
    std::cout << "2. Skini element sa steka\n";
    std::cout << "3. Element na vrhu steka\n";
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
    Stek<int> s;
    std::cout<< " Rezultat: " << std::boolalpha <<(provjeri_zagrade("Ovo {[()]} je string")) << std::endl; 
    int izbor, element;
    while((izbor = Menu()) != 0){
        switch(izbor){
            case 0:
                return 0;
            case 1:
                std::cout<< "Unesite element: ";
                std::cin>> element;
                s.stavi(element);
                break;
            case 2:
                element = s.skini();
                std::cout << "Element " << element << " je skinut sa steka" << std::endl;
                break;
            case 3:
                std::cout << "Element na vrhu steka je: " << s.vrh() << std::endl;
                break;
                default:
                std::cout << "Pogresan unos\n";
        }
    }
    
    return 0;
}
