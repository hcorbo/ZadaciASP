#include <iostream>
#include <cstring>
#include <fstream>
#include <ctime>
#include <vector>

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

int dajMaksimum(std::vector<int> &a){
    int velicina = a.size();
    int max = a[0];
    for(int i=0; i<velicina; i++)
    if(a[i] > max)
        max=a[i];
    return max;
}
/*Method to sort the array*/
void Counting_Sort(std::vector<int> A, std::vector<int> &B)    
{   int k = dajMaksimum(A);
	std::vector<int> C(k);
	for(int i=0; i<=k; i++){
		/*It will initialize the C with zero*/
		C[i]=0;
	}
	for(int j=0; j<A.size(); j++)
	{
		/*It will count the occurence of every element x in A 
		and increment it at position x in C*/
		C[A[j]]++;			    
	}
	for(int i=1;i<=k;i++)
	{
		/*It will store the last 
		occurence of the element i */
		C[i]+=C[i-1];            
	}
	for(int j=A.size()-1; j>=0; j--)
	{
	            
		/*It will help if an element occurs 
		more than one time*/
		C[A[j]]=C[A[j]]-1;
		/*It will place the elements at their 
		respective index*/
		B[C[A[j]]]=A[j];		  
	}
}

int main(){
    std::vector<int> v {13, 3, 5, 4, 9, 12, 13, 15, 2, 6, 7};
    std::vector<int> b = v;
    Counting_Sort(v, b);
    //std::cout << "Particija: " << Counting_Sort(v, b, 15) << std::endl;
    for(int i=0; i<b.size(); i++){
        std::cout << b.at(i) << " ";
    }
    return 0;
}