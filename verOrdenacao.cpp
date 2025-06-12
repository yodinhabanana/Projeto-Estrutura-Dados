#include <iostream>
#include <fstream>

using namespace std;

// verificando a ordenacao lols

struct athletes{

    char measure[10];
    char quantile[80];
    char area[50];
    char sex[15];
    char age[20];
    char geography[90];
    char ethnic[60];
    float value;


};

ostream& operator<<(ostream& os, const athletes& a) {
    os << "Measure: " << a.measure
       << ", Quantile: " << a.quantile
       << ", Area: " << a.area
       << ", Sex: " << a.sex
       << ", Age: " << a.age
       << ", Geography: " << a.geography
       << ", Ethnic: " << a.ethnic
       << ", Value: " << a.value;
    return os;
}

int main(){

    cout << "DIGITE O NOME DO ARQUIVO: " << endl;
    string nomeArquivoler;
    cin >> nomeArquivoler;

    cout << " Lendo arquivo: " << nomeArquivoler << endl;

    athletes atleta;
    ifstream leitura(nomeArquivoler, ios::binary);
    int i = 0;
    while(leitura.read(reinterpret_cast<char*>(&atleta), sizeof(athletes))){
        if(i%2 == 0 && i < 1000){
            cout << atleta << endl;
        }
        i++;
    }

    return 0;
}