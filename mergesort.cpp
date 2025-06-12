#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct athletes {
    char measure[10];
    char quantile[80];
    char area[50];
    char sex[15];
    char age[20];
    char geography[90];
    char ethnic[60];
    char value[15];
};

void dividirArquivo(ifstream& arquivoBinario, const string& nomeArquivo, int registrosPorBloco){

    athletes reg[10000];
    int registrosLidos = 0;

    for (int i = 0; i < registrosPorBloco; i++){
        if (arquivoBinario.read(reinterpret_cast<char*>(&reg[i]), sizeof(athletes))) {
            registrosLidos++;
        }
    }

    if (registrosLidos > 0){

        ofstream saida(nomeArquivo, ios::binary);

        if (!saida){
            cout << "Erro ao criar arquivo " << nomeArquivo << endl;
            return;
        }
        
        saida.write(reinterpret_cast<char*>(reg), registrosLidos * sizeof(athletes));
        saida.close();

        cout << "Arquivo " << nomeArquivo << " criado com " << registrosLidos << " registros." << endl;
    }
}

int main() {
    
    string nomeArquivoBinario;
    cout << "Digite o nome do arquivo binário para dividir: ";
    cin >> nomeArquivoBinario;
    
    ifstream arquivoBinario(nomeArquivoBinario, ios::binary);
    if (!arquivoBinario) {
        cout << "Erro ao abrir arquivo binário." << endl;
        return 1;
    }

    const int registrosPorBloco = 10000;
    int i = 0;

    while (arquivoBinario) {
        string nomeArquivo = "temp" + to_string(i) + ".bin";
        dividirArquivo(arquivoBinario, nomeArquivo, registrosPorBloco);
        i++;
    }

    arquivoBinario.close();
    return 0;
}
