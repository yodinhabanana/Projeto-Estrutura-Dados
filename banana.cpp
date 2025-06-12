#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <utility>

using namespace std;

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

// comparacao minheap
bool operator>(const athletes& a1, const athletes& a2) {
    return a1.value > a2.value;  // a1.value > a2.value
}

bool operator<(const athletes& a1, const athletes& a2) {
    return a1.value < a2.value;  // a1.value < a2.value
}

// classe do MinHeap para arquivos - yoda
class MinHeap {
friend class Binario;

private:

    athletes* heap;
    int capacidade;
    int tamanho;

    int pai(int i) { return (i - 1) / 2; }
    int esquerdo(int i) { return 2 * i + 1; }
    int direito(int i) { return 2 * i + 2; }

    void corrigeDescendo(int i) {
        int Oesquerdo = esquerdo(i);
        int Odireito = direito(i);
        int menor = i;

        if (Oesquerdo < tamanho && heap[Oesquerdo] < heap[menor]) {
            menor = Oesquerdo;
        }

        if (Odireito < tamanho && heap[Odireito] < heap[menor]) {
            menor = Odireito;
        }

        if (menor != i) {
            swap(heap[i], heap[menor]);
            corrigeDescendo(menor);
        }
    }

    void corrigeSubindo(int i) {
        if (i > 0 && heap[i] < heap[pai(i)]) {
            swap(heap[i], heap[pai(i)]);
            corrigeSubindo(pai(i));
        }
    }

protected:
    MinHeap(int cap) : capacidade(cap), tamanho(0) {
        heap = new athletes[capacidade];
    }

    ~MinHeap() {
        delete[] heap;
    }

    void retiraRaiz() {
        if (tamanho == 0) {
            cout << "Erro ao retirar raiz";
            return;
        }

        cout << heap[0].value << endl;
        heap[0] = heap[tamanho - 1];
        tamanho--;
        corrigeDescendo(0);
    }

    void insere(athletes info) {
        if (tamanho >= capacidade) {
            cout << "Erro ao inserir" << endl;
            return;
        }

        heap[tamanho] = info;
        tamanho++;
        corrigeSubindo(tamanho - 1);
    }
};

// sobrecarga de operador de saida - yoda
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

// classe do arquivo binario e suas operacoes
class Binario{

private:

    int tamanhoArquivo = 0;
    char nomeArquivoBin[50];

public:
	
    void transBinarioEmCsv(string nomeArquivoBin, string nomeArquivoCSV);
    void transCsvEmBinario(string nomeArquivoCSV);
    void imprimirTrecho(int posInicial, int posFinal);
    int alterarEmPosicao(int posicao);
    void espiarPosicao(int posicao);
    Binario() = default;
    ~Binario() = default;
    string getNomeArquivo();
    int getTamanho();
    bool inserePosicao(int posicao);
    bool testarNomeArquivoBin(char* nomeArquivoBin);
    void dividirArquivo(ifstream& arquivoBinario, const string& nomeArquivo, int registrosPorBloco);
    void recursaoDividirArquivo();
    void trocaPosicao();
    void particao(athletes* vetor, int inicio, int meio, int fim);
    int buscaBinaria(float Value);
    void intercalaArquivos();
    void juntarArquivos(const int numeroArquivos);
    void ordena(string nomeArquivo0);
    void recursaoOrdena(string vetorNomes[]);
    bool compararTrocar(fstream& arquivo, int pos1, int pos2);

};

// retorna o nome do arquivo binario
string Binario::getNomeArquivo(){
    return nomeArquivoBin;
}

// funcao feita pq toda hora eu tinha q fazer a mesma coisa pra pegar o tamanho era mais facil so fazer a funcao - yoda
int Binario::getTamanho(){

    ifstream verTamanho(nomeArquivoBin, ios::binary);

    int tamanhoRegistro = sizeof(athletes);
    verTamanho.seekg(0, ios::end);
    int totalBytes = verTamanho.tellg();
	int totalRegistros = totalBytes / tamanhoRegistro;

	return totalRegistros+1;
}

// espia posicao (chamado no alterar em posicao p ver como ficou lols)
void Binario::espiarPosicao(int posicao){

    string nomeArquivoBin = getNomeArquivo();
    
    ifstream arquivo(nomeArquivoBin, ios::binary);

    if(!arquivo){
        throw runtime_error("Erro, arquivo inexistente.");
    }

    athletes atleta;
    arquivo.seekg(posicao * sizeof(athletes));
    arquivo.read(reinterpret_cast<char*>(&atleta), sizeof(athletes));

    cout << atleta << endl;

    arquivo.close();
    return;
}

// trans csv em binrio - yoda
void Binario::transCsvEmBinario(string nomeArquivoCSV){

    string arquivochamada = nomeArquivoCSV;
    athletes atleta;
    ifstream entrada(nomeArquivoCSV);
    bool nomeCorreto = false;

    if (!entrada){
        throw runtime_error("Erro, arquivo inexistente.");
    }
    

	while (!nomeCorreto) {
		
		cout << "Qual o nome do arquivo binário no qual deseja guardar os dados? ";
		cin >> nomeArquivoBin;
		
	    if (testarNomeArquivoBin(nomeArquivoBin)){
	    
            ofstream saida(nomeArquivoBin, ios::binary);        
            nomeCorreto = true;
            
	        if (!saida.is_open()){
		        throw runtime_error("Erro ao abrir arquivo de saída.");
	        }

	        cout << "Aguarde enquanto os dados são lidos do arquivo CSV..." << endl;

            // Lê o cabeçalho do arquivo CSV
            string cabecalho;
            getline(entrada, cabecalho);
            
            while(entrada){
                // leitura dos dados
                entrada.getline(atleta.measure, 10, ',');
                entrada.getline(atleta.quantile, 80, ',');
                entrada.getline(atleta.area, 50, ',');
                entrada.getline(atleta.sex, 15, ',');
                entrada.getline(atleta.age, 20, ',');
                entrada.getline(atleta.geography, 90, ',');
                entrada.getline(atleta.ethnic, 60, ',');
                entrada >> atleta.value;

                if (entrada){
                    saida.write(reinterpret_cast<char*>(&atleta), sizeof(athletes));
                }
            }

            entrada.close();
            cout << "Dados gravados em binario com sucesso." << endl;
            saida.close();
            int quantidade = getTamanho();
            cout << "Quantidade de dados gravados: " << quantidade << endl;

            int opcao;
            cout << "Você deseja verificar se a leitura foi correta com a criação de outro CSV?" << endl;
            cout << "1. Sim" << endl;
            cout << "2. Não" << endl;
            cin >> opcao;
    
            if(opcao == 1){
                transBinarioEmCsv(nomeArquivoBin, arquivochamada);
            }else if(opcao == 2){
                return;
            }
        }

		else{
			cout << "--------------------------------------------------------------------" << endl;
			cout << "Por favor, não se esqueça de adicionar o '.bin' no final do nome." << endl; 
		}
	}	
}

// trans binario em csv - yoda
void Binario::transBinarioEmCsv(string nomeArquivoBin, string nomeArquivoCSV){
    
    ifstream arquivoBin(nomeArquivoBin, ios::binary);
    if(!arquivoBin){
        cerr << "Erro ao abrir o arquivo." << endl;
        return;
    }

    cout << "Digite o nome do arquivo no qual deseja gravar: ";
    string nomeArquivoCSVnovo;
    cin >> nomeArquivoCSVnovo;
    nomeArquivoCSVnovo += ".csv";
    ofstream arquivoCsv(nomeArquivoCSVnovo);

    if(!arquivoCsv){
        cerr << "Erro ao criar o arquivo CSV." << endl;
        return;
    }

    athletes a;
    arquivoCsv << "measure,quantile,area,sex,age,geography,ethnic,value" << endl;

    while(arquivoBin.read(reinterpret_cast<char*>(&a), sizeof(athletes))){
        arquivoCsv << a.measure << ',' << a.quantile << ',' << a.area  << ','
                   << a.sex << ',' << a.age << ',' << a.geography << ','
                   << a.ethnic << ',' << a.value;
    }


    arquivoBin.close();
    arquivoCsv.close();

    cout << "Conversão de binário para csv concluída, verifique abrindo o arquivo." << endl;
}

// imprimir trecho - yoda
void Binario::imprimirTrecho(int posInicial, int posFinal){

    ifstream entrada(nomeArquivoBin, ios::binary);

    if (!entrada) {
        cerr << "Erro ao abrir o arquivo binário." << endl;
        return;
    }

    athletes atleta;

    int tamanhoRegistro = sizeof(athletes);
    entrada.seekg(0, ios::end);
    int totalBytes = entrada.tellg();
    int totalRegistros = totalBytes / tamanhoRegistro;

    if (posInicial < 0 || posFinal > totalRegistros || posInicial > posFinal) {
        cerr << "Intervalo inválido." << endl;
        return;
    }

    for (int i = posInicial; i <= posFinal; i++) {
        entrada.seekg(i * tamanhoRegistro);
        entrada.read(reinterpret_cast<char*>(&atleta), tamanhoRegistro);
        cout << atleta << endl;
    }

    entrada.close();
}

// troca posicao, tenho q verificar dps - yoda
void Binario::trocaPosicao(){

    int totalRegistros = getTamanho();
    cout << "Digite as posições que você quer trocar os dados de lugar entre si." << endl;
    cout << "Posição 1: ";
    int pos1;
    cin >> pos1;
    if (pos1 < 0 || pos1 > totalRegistros) {
        cerr << "Posição inválida." << endl;
        return;
    }
    cout << endl;
    cout << "Posição 2: ";
    int pos2;
    cin >> pos2;
     if (pos1 < 0 || pos1 > totalRegistros) {
        cerr << "Posição inválida." << endl;
        return;
    }
    cout << endl;

    athletes atletaAtual1;
    athletes atletaAtual2;
    string nomeArquivoBin = getNomeArquivo();

    fstream binario(nomeArquivoBin, ios::in | ios::out | ios::binary);

    //leitura
    binario.seekg(pos1 * sizeof(athletes));
    binario.read(reinterpret_cast<char*>(&atletaAtual1), sizeof(athletes));
    binario.seekg(pos2 * sizeof(athletes));
    binario.read(reinterpret_cast<char*>(&atletaAtual2), sizeof(athletes));
    // escrita
    binario.seekp(pos2 * sizeof(athletes));
    binario.write(reinterpret_cast<char*>(&atletaAtual1), sizeof(athletes));
    binario.seekp(pos1 * sizeof(athletes));
    binario.write(reinterpret_cast<char*>(&atletaAtual2), sizeof(athletes));

}

// altera em posicao - yoda
int Binario::alterarEmPosicao(int posicao) {

    fstream binario(nomeArquivoBin, ios::in | ios::out | ios::binary);

    if(!binario){
        cout << "Erro ao abrir arquivo binário." << endl;
        return -1;
    }

    int tamanhoAtual = getTamanho();

    if (posicao < 0 || posicao > tamanhoAtual){
        cout << "A posição que você digitou é inválida." << endl;
        return -1;
    }

    cout << "O que você deseja alterar? " << endl;
    cout << "1. Measure." << endl;
    cout << "2. Quantile." << endl;
    cout << "3. Area." << endl;
    cout << "4. Sex." << endl;
    cout << "5. Age." << endl;
    cout << "6. Geography." << endl;
    cout << "7. Ethnic." << endl;
    cout << "8. Value." << endl;
    cout << "9. Esquece, decidi não mudar nada." << endl;


    int opcao;
    cin >> opcao;
    cin.ignore();


    if(opcao == 9){
        cout << "Nenhuma alteração feita." << endl;
        return -1;
    }

    athletes atleta;

    binario.seekg(posicao * sizeof(athletes));
    binario.read(reinterpret_cast<char*>(&atleta), sizeof(athletes));

    switch(opcao){
        case 1:
            cout << "Digite o novo 'Measure' do atleta: " << endl;
            cin.getline(atleta.measure, sizeof(atleta.measure));
        break;
        case 2:
            cout << "Digite o novo 'Quantile' do atleta: " << endl;
            cin.getline(atleta.quantile, sizeof(atleta.quantile));
        break;
        case 3:
            cout << "Digite a nova 'Area' do atleta: " << endl;
            cin.getline(atleta.area, sizeof(atleta.area));                
        break;
        case 4:
            cout << "Digite o novo 'Sex' do atleta: " << endl;
            cin.getline(atleta.sex, sizeof(atleta.sex));              
        break;
        case 5:
            cout << "Digite o novo 'Age' do atleta: " << endl;
            cin.getline(atleta.age, sizeof(atleta.age));                
        break;
        case 6:
            cout << "Digite o novo 'Geography' do atleta: " << endl;
            cin.getline(atleta.geography, sizeof(atleta.geography));
        break;
        case 7:
            cout << "Digite o novo 'Ethnic' do atleta: " << endl;
            cin.getline(atleta.ethnic, sizeof(atleta.ethnic));
        break;
        case 8:
            cout << "Digite o novo 'Value' do atleta: " << endl;
            cin >> atleta.value;
            cin.ignore();
        break;
        default:
            cout <<"Opção inválida. " << endl;
            return -1;
        }

    binario.seekp(posicao * sizeof(athletes));
    binario.write(reinterpret_cast<char*>(&atleta), sizeof(athletes));
    binario.close();
    cout << "Registro atualizado com sucesso." << endl;
    return posicao;

}

// insere posicao
bool Binario::inserePosicao(int posicao){

    fstream binario(nomeArquivoBin, ios::in | ios::out | ios::binary);
    int tamanhoAtual = getTamanho();

    if (posicao < 0 || posicao > tamanhoAtual){
        cout << "A posição que você digitou é inválida." << endl;
        return false;
    }

    athletes atleta_novo;

    cout << "Digite os dados que você quer inserir: " << endl;
    cin.ignore();
    cout << "Measure: " << endl;
    cin.getline(atleta_novo.measure, sizeof(atleta_novo.measure));
    cout << "Quantile: " << endl;
    cin.getline(atleta_novo.quantile, sizeof(atleta_novo.quantile));
    cout << "Area: " << endl;
    cin.getline(atleta_novo.area, sizeof(atleta_novo.area));
    cout << "Sex: " << endl;
    cin.getline(atleta_novo.sex, sizeof(atleta_novo.sex));
    cout << "Age: " << endl;
    cin.getline(atleta_novo.age, sizeof(atleta_novo.age));
    cout << "Geography: " << endl;
    cin.getline(atleta_novo.geography, sizeof(atleta_novo.geography));
    cout << "Ethnic: " << endl;
    cin.getline(atleta_novo.ethnic, sizeof(atleta_novo.ethnic));
    cout << "Value: " << endl;

   ofstream novoBinario("arquivoTemp.bin", ios::binary);

   if (!novoBinario) {
        cerr << "Erro ao criar o arquivo binário." << endl;
        return false;
    }

    athletes atletasLidos;
    int posicoesLidas = 0;

    while (posicoesLidas < getTamanho()) {
        binario.read(reinterpret_cast<char*>(&atletasLidos), sizeof(athletes));

        if (posicoesLidas == posicao) {
            binario.write(reinterpret_cast<char*>(&atleta_novo), sizeof(athletes));
        }

        binario.write(reinterpret_cast<char*>(&atletasLidos), sizeof(athletes));
        posicoesLidas++; 

	}

    if (posicao == tamanhoAtual) {
		binario.write(reinterpret_cast<char*>(&atletasLidos), sizeof(athletes));
    }

    tamanhoArquivo++;
    binario.close();
    
    return true;

}

// comecei a busca binaria - yoda
int Binario::buscaBinaria(float buscaValue){

    // verificacao se esta ordenado
    ifstream binario(nomeArquivoBin, ios::binary);

     if (!binario) {
        cout << "Erro ao abrir o arquivo.\n";
        return -2;
    }

    athletes atletaAtual, atletaAnterior;
    binario.read(reinterpret_cast<char*>(&atletaAnterior), sizeof(athletes));
    bool estaOrdenado = true;
    int i = 0;
    while(estaOrdenado && i < 1000){
        binario.read(reinterpret_cast<char*>(&atletaAtual), sizeof(athletes));
        // depois muda o campo para o que foi ordenado
        if(atletaAnterior.value > atletaAtual.value){
            estaOrdenado = false;
        }
        atletaAnterior = atletaAtual;
        i++;
    }

    if(!estaOrdenado){
        cerr << "Você precisa ordenar primeiro antes de tentar fazer a busca." << endl;
        return -2;
    }

    // aqui começa a real busca binaria no arquio .bin
    athletes atleta;
    binario.seekg(0, ios::end);
    int totalReg = getTamanho();

    int inicio = 0;
    int fim = totalReg-1;

    while(inicio <= fim){
        int meio = (inicio + fim)/2;

        binario.seekg(meio*sizeof(athletes), ios::beg);
        binario.read(reinterpret_cast<char*>(&atleta), sizeof(athletes));

        if(atleta.value == buscaValue){
            return meio;
        }
        else if(atleta.value < buscaValue){
            inicio = meio + 1;
        }else{
            fim = meio + 1;
        }
    }

    return -1;
}

void retornoMenu(bool& retorno){
    
	cout << "Deseja retornar para o menu? Por favor responda com 'S' ou 's' para sim e 'N' ou 'n' para não." << endl;
		
	string resposta;
	bool continuar = true;
		
	while (continuar){
		cin >> resposta;

		if (resposta == "S" or resposta == "s"){
			continuar = false;
			retorno = true;
		}
		else if (resposta == "N" or resposta == "n"){
			continuar = false;
			retorno = false;
		}
				
		else {
			continuar = false;
			cout << "Por favor, digite uma resposta válida." << endl;
		}
	}
}

bool testarNomeArquivoCSV(char* nomeDoCSV){
	
	int tamanhoNomeArquivo = 0;
	
	while (nomeDoCSV[tamanhoNomeArquivo] != '\0'){
		tamanhoNomeArquivo++;
	}
	
	if (tamanhoNomeArquivo < 5){
		return false;
	}
	
	else if (nomeDoCSV[tamanhoNomeArquivo - 4] == '.' and
        nomeDoCSV[tamanhoNomeArquivo - 3] == 'c' and
        nomeDoCSV[tamanhoNomeArquivo - 2] == 's' and
        nomeDoCSV[tamanhoNomeArquivo - 1] == 'v') {
            
        return true;
        
	}
	
	return false;
	
}

bool Binario::testarNomeArquivoBin(char* nomeArquivoBin){
	
	int tamanhoNomeArquivo = 0;
	
	while (nomeArquivoBin[tamanhoNomeArquivo] != '\0'){
		tamanhoNomeArquivo++;
	}
	
	if (tamanhoNomeArquivo < 5){
		return false;
	}
	
	else if (nomeArquivoBin[tamanhoNomeArquivo - 4] == '.' and
        nomeArquivoBin[tamanhoNomeArquivo - 3] == 'b' and
        nomeArquivoBin[tamanhoNomeArquivo - 2] == 'i' and
        nomeArquivoBin[tamanhoNomeArquivo - 1] == 'n') {
            
        return true;
	}
	return false;
}

//recursao que chama a dividir arquivo
void Binario::recursaoDividirArquivo(){

    string nomeAquivoBin = getNomeArquivo();

    ifstream arquivoBinario(nomeArquivoBin, ios::binary);

    if (!arquivoBinario) {
        cout << "Erro ao abrir arquivo binário." << endl;
        return;
    }
    const int registrosPorBloco = 25536;
    int i = 0;
    int numBlocos = getTamanho() / registrosPorBloco;
    // pego o resto do total por registros por bloco p/ verificacao do ultimo arquivo! (vai ter 1 a mais)
    int resto = getTamanho() % registrosPorBloco;
    
    string vetorNomes[10];

    while (arquivoBinario) {
        string nomeArquivo = "temp" + to_string(i) + ".bin";
        dividirArquivo(arquivoBinario, nomeArquivo, registrosPorBloco);
        i++;
        vetorNomes[i] = nomeArquivo;
    }

    // verificacao pro ultimo arquivo ficar com apenas 1 entrada a mais que os outros 9
    if (resto > 0) {
        string nomeArquivo = "temp" + to_string(i) + ".bin";
        dividirArquivo(arquivoBinario, nomeArquivo, resto);
        i++;
    }

    cout << "Quantidade de arquivos criados: " << i << endl;
    arquivoBinario.close();
    //chamada da recursao que ordena
    recursaoOrdena(vetorNomes);
}

// chamada q divide e cria varios arquivos
void Binario::dividirArquivo(ifstream& arquivoBinario, const string& nomeArquivo, int registrosPorBloco){

    athletes reg;
    int registrosLidos = 0;
    ofstream saida(nomeArquivo, ios::binary);

    // leitura de cada bloco
    for (int i = 0; i < registrosPorBloco; i++){
        if (arquivoBinario.read(reinterpret_cast<char*>(&reg), sizeof(athletes))) {
            registrosLidos++;
            saida.write(reinterpret_cast<char*>(&reg), registrosLidos * sizeof(athletes));
        }
    }
    saida.close();

    cout << "Arquivo " << nomeArquivo << " criado com " << registrosLidos << " registros ordenados." << endl;
}

bool Binario::compararTrocar(fstream& arquivo, int pos1, int pos2) {

    athletes a, b;

    // posição pos1
    arquivo.seekg(pos1 * sizeof(athletes), ios::beg);
    arquivo.read(reinterpret_cast<char*>(&a), sizeof(athletes));

    // le pos 2
    arquivo.seekg(pos2 * sizeof(athletes), ios::beg);
    arquivo.read(reinterpret_cast<char*>(&b), sizeof(athletes));

    if (a.value > b.value) { // ordenacao por value
        // escreve b na pos1
        arquivo.seekp(pos1 * sizeof(athletes), ios::beg);
        arquivo.write(reinterpret_cast<char*>(&b), sizeof(athletes));

        // escreve a na pos2
        arquivo.seekp(pos2 * sizeof(athletes), ios::beg);
        arquivo.write(reinterpret_cast<char*>(&a), sizeof(athletes));

        return true;
    }
    return false;
}


void Binario::ordena(string nomeArquivo0) {
    fstream arquivo(nomeArquivo0, ios::in | ios::out | ios::binary);
    if (!arquivo) {
        cout << "Erro ao abrir arquivo: " << nomeArquivo0 << endl;
        return;
    }

    // Descobre número de registros no arquivo
    arquivo.seekg(0, ios::end);
    long totalBytes = arquivo.tellg();
    int totalRegistros = totalBytes / sizeof(athletes);
    arquivo.seekg(0); // volta pro início

    for (int i = 0; i < totalRegistros - 1; i++) {
        for (int j = 0; j < totalRegistros - i - 1; j++) {
            compararTrocar(arquivo, j, j + 1);
        }
    }

    arquivo.close();
    cout << "Ordenado: " << nomeArquivo0 << endl;
}

// recursao que chama cada par de arquivo
void Binario::recursaoOrdena(string vetorNomes[]) {
    for (int i = 0; i < 10; i++) { 
        ordena(vetorNomes[i]); 
    }
}

//implementacao do Menu para interação com o usuário
void menuPrincipal(){

    int opcao = 0;
    Binario binario;
    bool retorno = false;

    string nomeArquivoCSV;
    cout << "Qual o nome do arquivo que deseja ler? ";
    cin >> nomeArquivoCSV;
    binario.transCsvEmBinario(nomeArquivoCSV);
    string nomeArquivoBin = binario.getNomeArquivo();
    int tamanhoMostrar = binario.getTamanho();

    do{
        cout << endl;
        cout << "Bem-vindo ao sistema de gerenciamento de atletas!" << endl;
        cout << "Escolha uma opção:" << endl;
        cout << "1. Inserir atleta." << endl;
        cout << "2. Alterar atleta." << endl;
        cout << "3. Imprimir trecho." << endl;
        cout << "4. Buscar atleta." << endl;
        cout << "5. Excluir atleta" << endl;
        cout << "6. Transformar em csv." << endl;
        cout << "7. Ordenar." << endl;
        cout << "8. Busca." << endl;
        cout << "9. Sair." << endl;
        cout << "Digite sua opção: ";
        cin >> opcao;
    
        switch(opcao){
            case 1:
                cout << "Existem: " << tamanhoMostrar << " registros no arquivo." << endl;
                int posicaoInserir;
                cout << "Digite a posição na qual você deseja inserir um dado: ";
                cin >> posicaoInserir;
                cout << endl;
                
                if (binario.inserePosicao(posicaoInserir)) {
					cout << "O atleta foi inserido na posição " << posicaoInserir << "." << endl;
				}

                retornoMenu(retorno);
                break;
            case 2:
                int posicao;
                cout << "Digite a posição na qual deseja alterar um dado: ";
                cin >> posicao;
                cout << endl;
                if(binario.alterarEmPosicao(posicao) >= 0){

                    cout << "Você deseja ver como seu registro está atualmente?" << endl;
                    cout << "1. Sim." << endl;
                    cout << "2. Não." << endl;
                    int novaOpcao;
                    cin >> novaOpcao;
                    if(novaOpcao == 1){
                        binario.espiarPosicao(posicao);
                    }
                }
                retornoMenu(retorno);
                break;
            case 3:
                int posInicial, posFinal;
                cout << "Digite a posição inicial da impressão: ";
                cin >> posInicial;
                cout << endl;
                cout << "Digite a posição final da impressão: ";
                cin >> posFinal;
                cout << endl;
                binario.imprimirTrecho(posInicial, posFinal);
                retornoMenu(retorno);

                break;
            case 4:
                cout << "Sendo implementado, volte mais tarde."<< endl;
                retornoMenu(retorno);
                break;
            case 5:
                cout << "Sendo implementado, volte mais tarde."<< endl;
                retornoMenu(retorno);
                break;
            case 6:
                binario.transBinarioEmCsv(nomeArquivoBin, nomeArquivoCSV);
                retornoMenu(retorno);
                break;
            case 7:
                binario.recursaoDividirArquivo();
                retornoMenu(retorno);
                break;
            case 8:
                cout << "Digite o 'value' que você deseja achar: ";
                float valor;
                cin >> valor;
                cout << endl;
                if(int pos = binario.buscaBinaria(valor) != -1 && pos >= 0){
                    cout << "O registro buscado é: " << endl;
                    binario.espiarPosicao(pos);
                }else if(pos == -1){
                    cout << "Não encontrado! Esse 'value' não está presente no registro." << endl;
                }
                retornoMenu(retorno);
                break;
            case 9:
                cout << "Saindo..."<< endl;
                return;
                break;
        }
        
    }while(retorno && opcao != 9);
}

int main(){

    menuPrincipal();

    return 0;
}