#include <iostream>
#include <vector>

using namespace std;

class Cvor
{
public:
    int vrijednost;
    Cvor *lijevo = nullptr;
    Cvor *desno = nullptr;
};

int visina(Cvor *cvor)
{
    if(cvor == nullptr) return 0;
    if(cvor->desno == nullptr && cvor->lijevo == nullptr) return 1;
    int lijevo = visina(cvor->lijevo);
    int desno = visina(cvor->desno);
    if(lijevo > desno) return lijevo + 1;
    else return desno + 1;
}

int visina(vector<int> A, int indeks)
{
    if(A[indeks] == -1) return 0;
    if(A[2*indeks+1] == -1 && A[2*indeks + 2] == -1) return 1;
    int lijevo = visina(A, 2*indeks + 1);
    int desno = visina(A, 2*indeks + 2);
    if(lijevo > desno) return lijevo + 1;
    else return desno + 1;
}

void obrisi(Cvor *cvor)
{
    if(cvor) {
        obrisi(cvor->lijevo);
        obrisi(cvor->desno);
        cvor = nullptr;
    }
}

void dodaj(Cvor *roditelj, bool desnoDijete, vector<int> clanovi, int indeks)
{
    if(clanovi[indeks] != -1) {
        Cvor *novi;
        novi->vrijednost = clanovi[indeks];
        if(desnoDijete) roditelj->desno = novi;
        else roditelj->lijevo = novi;
        dodaj(novi, false, clanovi, 2*indeks+1);
        dodaj(novi, true, clanovi, 2*indeks+2);
    }
}

void prebaciStabloAB(vector<int> A, Cvor *B)
{
    //izracunati balans stabla A i stabla B

    //racunamo balans stabla B
    int balansB = visina(B->lijevo) - visina(B->desno);

    //racunamo balans stabla A
    int balansA = visina(A, 1) - visina(A, 2);

    //ako je balans A manji od balansa B potrebno je prebaciti citavo stablo A kao podstablo jednog od cvorova stabla B
    //tako da struktura stabla A ostane ocuvana u stablu B i da i A i B ostanu stabla binarne pretrage
    if(balansA < balansB) {
        int korijenA = A[0];
        Cvor *pom = B;
        while(pom) {
            if(korijenA > pom->vrijednost)
                pom = pom->desno;
            else
                pom = pom->lijevo;
        }
        if(korijenA > pom->vrijednost) { //stavljamo stablo A u desno podstablo
            dodaj(pom, true, A, 0);
        } else { // stavljamo stablo A u lijevo podstablo
            dodaj(pom, false, A, 0);
        }
    }



    //ako je balans A veci od balansa B potrebno je obrisati sve cvorove stabla B
    else if(balansA > balans B)
        obrisi(B); // pomocna funkcija obrisi

    //ako im je balans isti potrebno je pronaci minimalni element  stabla B i minimalni element stabla A i ispisati ih na ekran

    else {
        //minimalni element je najljevlji element
        int minA = A[0];
        for(int i = 0; i < A.size(); i = 2*i+1)
            if(A[i] < minA) minA = A[i];

        int minB = B->vrijednost;
        Cvor *pom = cvor;
        while(pom)
            pom = pom->lijevo;
        minB = pom->vrijednost;

        cout<< "Minimalni element stabla A je "<< minA <<", a minimalni element stabla B je " << minB << endl;

    }

}

}


int drugiNajveci(vector <int> &stabloA){
    int i=0;
    while(stabloA.at(2*i+2) != -1) i = 2*i+2;
    if(i != 0 && stabloA.at(2*i+1) != -1) 
    {   i = 2*i+1;
        while (stabloA.at(2*i+2) != -1) i = 2*i+2;
        return i;
    }
    else if(i != 0) return (i/2)-1
    else {
        i = 1;
        while (stabloA.at(2*i+2) != -1) i = 2*i+2;
        return i;
    }
    
}

void ZamjenaPozicija(std::vector<int> &niz, const int i, const int j){
    int temp = niz[i];
    niz[i] = niz[j];
    niz[j] = temp;
}

int Particija(std::vector<int> &v){
    int pivot = v.at(v.size()-1);
    int p = 0;
    while(p<=v.size()-2){
        if(v[p]<=pivot)
            p++;
        else break;
    }
    for(int i = p+1; i<=v.size()-2; i++){
        if(v[i]<pivot){
            //moze i zamjena pozicija
            int temp = v[i];
            v[i] = v[p];
            v[p] = temp;
            p++;
        }
    }
    ZamjenaPozicija(v, v.size()-1, p-1);
    return p-1;
}

Cvor* trazi(Cvor *cv, const TipKljuca &x) const{
        if (cv == nullptr || cv->kljuc == x)
            return cv;
        else if(x < cv->kljuc)
            return trazi(cv->lijevo, x);
        else 
            return trazi(cv->desno, x);
}

int trazi(std::vector<int> &stabloA, const int &x) const{
        int i=0;
        while(stabloA.at(i) != -1){
            if(stabloA.at(i) == x) return i;
            if(stabloA.at(i) < x)  i=2*i+2;
            else i = 2*i+1;
        }
        return -1;
}

void obrisi(Cvor *cvor)
{
    if(cvor) {
        obrisi(cvor->lijevo);
        obrisi(cvor->desno);
        cvor = nullptr;
    }
}

StabloQuickStablo(vector<int> stabloA, Cvor* stabloB, vector<int> brojeviC){
    int a = drugiNajveci(stabloA);
    brojeviC.push_back(stabloA.at(a));
    if (stabloA.at(2*a+2) == -1 && stabloA.at(2*a+1) == -1)
        stabloA.at(a) = -1;
    else if(stabloA.at(2*a+2) != -1){
        stabloA.at(a) = stabloA.at(2*a+2);
        stabloA.at(2*a+2) = -1;
    } else {
        i = a;
        while(stabloA.at(2*i+1) != -1){
            stabloA.at(i) = stabloA.at(2*i+1);
            i = 2*i+1;
        }
    }
    int p = Particija(brojeviC);
    for(int i=0; i<p; i++){
        Cvor *temp = trazi(stabloB, brojeviC.at(p));
        obrisi(temp);
    }
}

void Preorder (Cvor *cv, int korijen, vector<int> &C){
    if(cv != nullptr || cv->element < (korijen/2)){
        if(cv->element > korijen) C.push_back(cv->element);
        Preorder(cv->lijevo);
        Preorder(cv->desno);
    }
}    
    
void GomilaCountStablo(vector<int> stabloA, Cvor* stabloB, vector<int> C, vector<int> D){
    for(int i=0; i<p; i++){
        int indeks = trazi(stabloA, brojeviC.at(p));
        if(stabloA.at(2*indeks+2) == -1 && stabloA.at(2*indeks+1) != -1)
            stabloA.at(indeks) = stabloA.at(2*indeks+1);
        else if(stabloA.at(2*indeks+2) != -1 && stabloA.at(2*indeks+1) == -1)
            stabloA.at(indeks) = stabloA.at(2*indeks+1);
}

int main()
{
    return 0;
}