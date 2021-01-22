/*
Universidade Federal da Fronteira Sul - UFFS
Linguagem Formal e Autômatos
Professor Braulio Mello
Alunos Elvis de Souza Machado e Jean Carlo Fenner


Construção de uma aplicação para construção, determinização e minimização (eliminação de mortos e inalcançáveis) de autômatos finitos.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define ARQUIVO_R "text.txt"                // Arquivo que será lido
#define ARQUIVO_W "afd.txt"                 // Arquivo em que será escrito
#define EPS "&"                             // Epsilon

using namespace std;

//Classe para controlar as produções

class Producoes {
    string estado;                      // Estado atual
    string t;                           // Terminal
    string nt;                          // Não-Terminal
    int estado_final;                   // Estado Final?

    public:
        //metodo adicionar que não possui um retorno de informação servindo apenas para setar o estado, terminar, não terminal e estado final
        void adicionar(string new_e, string new_t, string new_nt, int new_ef = 0){
            estado = new_e;
            t = new_t;
            nt = new_nt;
            estado_final = new_ef;

            return;
        };

        // get para recuperar as informações setadas no metodo adicionar
        string get_estado(){
            return estado;
        }
        string get_terminal(){
            return t;
        };
        string get_nterminal(){
            return nt;
        };
        int get_ef(){
            return estado_final;
        }
};

class AF{
    vector <vector<string>> table;          // Tabela de transições
    vector <int> estado_final;              // Vetor com identificador de estados finais
    int size_lin;                           // Número de linhas da tabela
    int size_col;                           // Número de colunas da tabela

    public:
        AF(int lin, int col){
            int i, j;

            for(i=0; i<col; i++){
                vector <string> nt;
                for(j=0; j<lin; j++)
                    nt.push_back(" ");
                table.push_back(nt);
            }
            for(i=0; i<lin; i++)
                estado_final.push_back(0);
            size_lin = lin;
            size_col = col;
        }

        void adicionar(int lin, int col, int ef, string novo){
            if(novo != " ")
                table[col][lin].append("<"+novo+">");
            if(ef)
                estado_final[lin] = 1;

            return;
        }

        bool determ(){
            int i, j, k, det_aux;
            string str_aux;

            for(i=0; i<size_lin; i++){
                for(j=0; j<size_col; j++){
                    str_aux = table[j][i];
                    int det_aux = 0;
                    for(k=0; k<str_aux.length(); k++){
                        if(str_aux[k]=='<'){
                            det_aux++;
                        }
                    }
                    if(det_aux > 1)
                        return true;
                }
            }

            return false;
        }

        void print_af(){
            for(int i=0; i<size_lin; i++){
                for(int j=0; j<size_col; j++){
                    cout << table[j][i] << " ";
                }
                cout << endl;
            }
        }

        string get_cel(int lin, int col){
            return table[col][lin];
        }

        bool get_ef(int lin){
            return estado_final[lin];
        }
};

class Gra_Tab{
    string gramatica;                   // Símbolo do estado na Gramática
    string tabela;                      // Símbolo do estado no AF

    public:
        void adicionar(string g, string t){
            gramatica = g;
            tabela = t;

            return;
        }

        bool compara(string g){
            if(gramatica == g)
                return true;

            return false;
        }

        string get_g(){
            return gramatica;
        }

        string get_t(){
            return tabela;
        }

};

int main(){
    int i, j;
    int ntchar[2];                              // Caracter do Não Terminal
    int ascii[127] = {0};                       // Lista de uso ASCII
    string simboloe;                            // Símbolo Estado
    string simbolot;                            // Símbolo Terminal
    string simbolont;                           // Símbolo Não-Terminal
    string txtline;                             // Linha do .txt
    vector <string> terminais;                  // Vetor de Terminais
    vector <string> nterminais;                 // Vetor de Não-Terminais
    vector <Gra_Tab> gra_tab;                   // Relação NT Gramática-Tabela
    vector <Producoes> prod;                    // Vetor das produções de cada Estado
    Producoes novo_prod;                        // Item do Vetor <prod>
    Gra_Tab dupla;                              // Item do vetor <gra_tab>
    vector <Gra_Tab>::iterator it_gt;           // Iterador do Vetor <gra_tab>
    ifstream txtfiles(ARQUIVO_R);               // Abrir arquivo .txt

/*
    ####
        Leitura do Arquivo de Texto
    ####
*/
    //verifica se existe o arquivo com a relação de tokens e/ou gramaticas regulares
    if(txtfiles.is_open()){
        // inicializa as variaveis 
        nterminais.push_back("S");
        ntchar[0] = 'A';
        dupla.adicionar("S", "S");
        gra_tab.push_back(dupla);

        while(getline(txtfiles, txtline)){ // loop para ler cada linha do arquivo
            // caso a mesma iniciar com o char '<' é identiciado como uma gramatica
            if(txtline[0] == '<'){              // GRAMATICAS
                int ef = 0;
                char txtchar;
                string estado;
                string auxchar;
                j = 1;

                while(txtline[j] != '>'){ // loop para guardar na variavel 'estado' todos os caracteres do estado ate encontar o final '>'
                    estado.push_back(txtline[j]);
                    j++;
                }
                
                for(it_gt=gra_tab.begin(); it_gt<gra_tab.end(); it_gt++){ // loop pela tabela de gramaticas
                    if((*it_gt).compara(estado)){ // compara se o estado lido é igual ao simbolo do estado da gramatica
                        estado = (*it_gt).get_t(); // recupera o simbolo do estado do AF 
                        it_gt = gra_tab.end(); // iterador de controle do loop recebe o simbolo final da tabela de gramaticas para sair do loop
                    } else if((it_gt+1) == gra_tab.end()){ // verifica se o proximo iterador é o ultimo a ser percorrido pela tabela de gramaticas 
                        auxchar = (*ntchar);
                        dupla.adicionar(estado, auxchar); // adicionado novo simbolo a tabela de gramaticas 
                        gra_tab.push_back(dupla);
                        estado = auxchar;
                        nterminais.push_back(estado); // incluido o estado no vetor de não terminais
                        ntchar[0]++;
                    }
                }
                simboloe = estado;
                estado = "NUL";

                while (txtline[j] != '='){ // loop ate encontrar o '=' que determina o inicio dos estados da gramatica
                    j++;
                }

                for(i=j+1; i<txtline.length(); i++){ // loop para passar por todos as produções da gramatica

                    if(txtline[i]>32 && txtline[i]<129){
                        while(txtline[i] !='|' && i < txtline.length()){
                            if(txtline[i]=='<'){
                                i++;
                                while(txtline[i] != '>'){
                                    auxchar = txtline[i];
                                    i++;
                                }

                                for(it_gt=gra_tab.begin(); it_gt<gra_tab.end(); it_gt++){
                                    if((*it_gt).compara(auxchar)){
                                        simbolont = (*it_gt).get_t();
                                        it_gt = gra_tab.end();
                                    } else if((it_gt+1) == gra_tab.end()){
                                        simbolont = (*ntchar);
                                        dupla.adicionar(auxchar, simbolont);
                                        gra_tab.push_back(dupla);
                                        nterminais.push_back(simbolont);
                                        ntchar[0]++;
                                    }
                                }
                            } else if(txtline[i]>32 && txtline[i]<127){
                                simbolot = txtline[i];
                                if(simbolot != EPS)
                                    ascii[txtline[i]] = 1;
                                if(txtline[i+1] != '<'){
                                    ef = 1;
                                }
                            }
                            i++;
                        }
                        if(ef){
                            if(simbolot == EPS){
                                novo_prod.adicionar(simboloe, simbolot, "NUL", 1);
                                prod.push_back(novo_prod);
                            }else{
                                simbolont = (*ntchar);
                                nterminais.push_back(simbolont);
                                ntchar[0]++;

                                novo_prod.adicionar(simboloe, simbolot, simbolont);
                                prod.push_back(novo_prod);
                                novo_prod.adicionar(simbolont, EPS, "NUL", 1);
                                prod.push_back(novo_prod);
                            }
                            ef = 0;
                        }else{
                            novo_prod.adicionar(simboloe, simbolot, simbolont);
                            prod.push_back(novo_prod);
                        }
                        
                        // Adicona ' aos novos NTs case passe de Z
                        if(ntchar[0]>90){
                            ntchar[0] = 65;
                            ntchar[1] = 39;
                        }

                        i++;
                    }
                }
            }
            else{                             // TOKENS

                simboloe = *nterminais.begin();

                for(i=0; i<txtline.length(); i++){
                    simbolot = txtline[i];
                    simbolont = *ntchar;
                    ascii[txtline[i]] = 1;
                    novo_prod.adicionar(simboloe, simbolot, simbolont);
                    prod.push_back(novo_prod);
                    ntchar[0]++;
                    simboloe = simbolont;
                    nterminais.push_back(simbolont);

                    // NT 'S' é o inicial
                    if(ntchar[0] == 'S' && ntchar[1] != 39)
                        ntchar[0]++;
                }
                simbolot = EPS;
                novo_prod.adicionar(simboloe, simbolot, "NUL", 1);
                prod.push_back(novo_prod);
                
                // Adicona ' aos novos NTs case passe de Z
                if(ntchar[0]>90){

                    ntchar[0] = 65;
                    ntchar[1] = 39;
                }
            }
            cout << endl;
        }
        txtfiles.close();
        
        // Preenchendo vetor de terminais com os caracteres da ascii utilizados
        for(i=0; i<127; i++){
            if(ascii[i]!=0){
                char ctemp = i;
                string stemp;
                stemp.push_back(ctemp);
                terminais.push_back(stemp);
            }
        }
    } else{
        cout << "Impossivel abrir arquivo " << ARQUIVO_R << endl;
        
        return 0;
    }

/*  
    ####
        Construção do Autômato Finito
    ####
*/

    const int t_size = terminais.size();        // Tamanho do Vetor <terminais>
    const int nt_size = nterminais.size();      // Tamanho do Vetor <nterminais>
    vector <Producoes>::iterator it_p;          // Iterador do Vetor <prod>
    AF afnd(nt_size, t_size);                   // Inicialização do AF

    // Preencher transições do AF
    for(it_p=prod.begin(); it_p<prod.end(); it_p++){
        int ef_aux = (*it_p).get_ef();
        string e_aux = (*it_p).get_estado();
        string t_aux = (*it_p).get_terminal();
        string nt_aux = (*it_p).get_nterminal();

        for(i=0; i<nt_size; i++){
            if(e_aux == nterminais[i]){
                for(j=0; j<t_size; j++){
                    if(t_aux == terminais[j]){
                        afnd.adicionar(i, j, ef_aux, nt_aux);
                    }else if(t_aux == "&"){
                        afnd.adicionar(i, j, ef_aux, " ");
                    }
                }
            }
        }
    }

    // AFND
    cout << endl;
    cout << "AFND\n";
    cout << "NT / T\t| ";
    for(i=0; i<t_size; i++)
        cout << terminais[i] << "\t| ";
    cout << endl;
    for(i=0; i<nt_size; i++){
        cout << ((afnd.get_ef(i)) ? "<*" : "<") << nterminais[i] << ">\t| ";
        for(j=0; j<t_size; j++)
            cout << afnd.get_cel(i, j) << "\t| ";
        cout << endl;
    }

/*
    ####
        Determinização
    ####
*/

    vector <int> afd_estados_usados;            // Vetor de Estados da AFND usados na AFD
    vector <string> afd_nterminais;             // Vetor de Estados do AFD
    vector <Producoes> afd_prod;                // Vetor das produções de cada Estado do AFD
    ofstream txtafd(ARQUIVO_W);                 // Arquivo escrita AFD

    bool indet = (afnd.determ());
    cout << "Automato Finito " << (indet ? "Indeterministico" : "Deterministico") << endl;

    if(txtafd.is_open()){
        if(indet){
            int k, add=0;
            int afd_col = t_size;               // Colunas do AFD

            for(i=0; i<nt_size; i++)
                afd_estados_usados.push_back(0);

            for(i=0; i<nt_size; i++){           // Indeterminismos Primários
                if(!afd_estados_usados[i]){                     // Se o Estado ainda não apareceu continuar
                    simboloe = nterminais[i];                   // Estado atual
                    afd_nterminais.push_back(simboloe);
                    for(j=0; j<t_size; j++){
                        simbolot =  terminais[j];               // Terminal 

                        string str_aux, e_aux;
                        str_aux = afnd.get_cel(i, j);
                        for(k=0; k<str_aux.length(); k++){      // Ler os Não-Terminal 
                            if(str_aux[k]=='<'){
                                k++;
                                while(str_aux[k] != '>'){
                                    e_aux += str_aux[k];
                                    if(e_aux.length()>4){
                                        for(int z=0; z<nt_size; z++){
                                            if(nterminais[z][0] == str_aux[k])
                                                afd_estados_usados[z] = 1;
                                        }
                                    }
                                    k++;
                                }
                            }
                        }
                        if(!e_aux.empty()){                     // Não-Terminais
                            if(e_aux.length()>1)
                                simbolont = "["+e_aux+"]";
                            else
                                simbolont = e_aux;
                        }else{
                            simbolont = e_aux;
                        }
                                                                // Adiconar no Vetor <afd_prod>
                        novo_prod.adicionar(simboloe, simbolot, simbolont, afnd.get_ef(i));
                        afd_prod.push_back(novo_prod);

                        if(!simbolont.empty()){                 // Adicionar ao Vetor <afd_nterminais>
                            for(int z=0; z<nt_size; z++){
                                add = 1;
                                if(afd_nterminais[z]==simbolont || simboloe == simbolont){
                                    add = 0;
                                    z = nt_size;
                                }
                            }
                            if(add){
                                afd_nterminais.push_back(simbolont);
                            }
                        }
                        if(afnd.get_ef(i))
                            afd_estados_usados[i] = 1;
                    }
                }
            }

            cout << "\n__________\nResolucao de Indeterminismos\n__________\n";            
            for(i=0; i<afd_prod.size(); i++){                   // Acompanhamento das alterações
                if(!(i%t_size))
                    cout << endl;
                cout << (afd_prod[i].get_ef() ? "Estado: <*" : "Estado: <") <<afd_prod[i].get_estado() << "> Token: (" << afd_prod[i].get_terminal() << ") = <" << afd_prod[i].get_nterminal() << "> " << endl;
            }

            afd_estados_usados.clear();                         // Limpar Estados usados
            for(i=0; i<afd_nterminais.size(); i++)
                afd_estados_usados.push_back(0);
            
            for(i=0; i<afd_nterminais.size(); i++){
                string str_aux = afd_nterminais[i];

                if(str_aux[0] == '['){                          // Se for Estado novo
                     if(!afd_estados_usados[i]){                // Estado ainda não usado
                        simboloe = str_aux;                     // Estado
                        for(j=0; j<afd_col; j++){
                            simbolot =  terminais[j];           // Terminal
                            int ef_aux;
                            string str_nt_aux, str_e_aux;
                            ef_aux = 0;
                            k = 1;
                            while(str_aux[k]!=']'){             // Busca Não-Terminais de Estados encontrados
                                for(int z=0; z<nt_size; z++){
                                    if(nterminais[z][0] == str_aux[k]){
                                        str_nt_aux += afnd.get_cel(z, j);
                                        if(afnd.get_ef(z))
                                            ef_aux = 1;
                                    }
                                }
                                k++;
                            }                                   // Remover separadores dos estados
                            for(int z=0; z<str_nt_aux.length(); z++){
                                if(str_nt_aux[z]=='<' || str_nt_aux[z]=='['){
                                    z++;
                                    while(str_nt_aux[z]!='>' && str_nt_aux[z]!=']'){
                                        int y=1;
                                        for(int x=0; x<str_e_aux.length(); x++){
                                            if(str_e_aux[x] == str_nt_aux[z])
                                                y=0;
                                        }
                                        if(y)
                                            str_e_aux += str_nt_aux[z];
                                        z++;
                                    }
                                }
                            }
                            if(!str_e_aux.empty()){             // Não_terminal
                                if(str_e_aux.length()>1)
                                    simbolont = "["+str_e_aux+"]";
                                else
                                    simbolont = str_e_aux;
                            }else{
                                simbolont = str_e_aux;
                            }

                                                                // Adicionar ao Vetor <afd_nprod>
                            novo_prod.adicionar(simboloe, simbolot, simbolont, ef_aux);
                            afd_prod.push_back(novo_prod);

                            if(!simbolont.empty()){             // Adicionar ao Vetor <afd_nterminais>
                                for(int z=0; z<nt_size; z++){
                                    add = 1;
                                    if(afd_nterminais[z]==simbolont || simboloe == simbolont){
                                        add = 0;
                                        afd_estados_usados[z] = 0;
                                        z = nt_size;
                                    }
                                }
                                if(add){
                                    afd_nterminais.push_back(simbolont);
                                    afd_estados_usados.push_back(0);
                                }
                            }
                        }
                        afd_estados_usados[i] = 1;
                    }
                }
            }

            cout << "\n__________\nResolucao de Indeterminismos\n__________\n";
            for(i=0; i<afd_prod.size(); i++){                   // Acompanhamento das alterações
                if(!(i%t_size))
                    cout << endl;
                cout << (afd_prod[i].get_ef() ? "Estado: <*" : "Estado: <") <<afd_prod[i].get_estado() << "> Token: (" << afd_prod[i].get_terminal() << ") = <" << afd_prod[i].get_nterminal() << "> " << endl;
            }
            cout << endl;

            AF afd(afd_nterminais.size(), t_size);            // Inicialização do AFD

            // Preencher AFD
            for(it_p=afd_prod.begin(); it_p<afd_prod.end(); it_p++){
                int ef_aux = (*it_p).get_ef();
                string e_aux = (*it_p).get_estado();
                string t_aux = (*it_p).get_terminal();
                string nt_aux = (*it_p).get_nterminal();

                for(i=0; i<afd_nterminais.size(); i++){
                    if(e_aux == afd_nterminais[i]){
                        for(j=0; j<afd_col; j++){
                            if(t_aux == terminais[j]){
                                afd.adicionar(i, j, ef_aux, nt_aux);
                            }else{
                                afd.adicionar(i, j, ef_aux, " ");
                            }
                        }
                    }
                }
            }

            // AFD
            cout << "AFD\n";
            cout << "NT/T\t| ";
            for(i=0; i<t_size; i++)
                cout << terminais[i] << "\t| ";
            cout << endl;
            for(i=0; i<afd_nterminais.size(); i++){
                cout << ((afd.get_ef(i)) ? "<*" : "<") << afd_nterminais[i] << ">\t| ";
                for(j=0; j<t_size; j++)
                    cout << afd.get_cel(i, j) << "\t| ";
                cout << endl;
            }

            txtafd << "δ\t| ";                                  // Escrita do AFD em um arquivo de texto
            for(i=0; i<t_size; i++)
                txtafd << terminais[i] << "\t| ";
            txtafd << endl;
            for(i=0; i<afd_nterminais.size(); i++){
                txtafd << ((afd.get_ef(i)) ? "<*" : "<") << afd_nterminais[i] << ">\t| ";
                for(j=0; j<t_size; j++)
                    txtafd << afd.get_cel(i, j) << "\t| ";
                txtafd << endl;
            }

        }else{
            // AFD
            cout << endl;
            cout << "AFD\n";
            cout << "NT / T\t| ";
            for(i=0; i<t_size; i++)
                cout << terminais[i] << "\t| ";
            cout << endl;
            for(i=0; i<nt_size; i++){
                cout << ((afnd.get_ef(i)) ? "<*" : "<") << nterminais[i] << ">\t| ";
                for(j=0; j<t_size; j++)
                    cout << afnd.get_cel(i, j) << "\t| ";
                cout << endl;
            }

            txtafd << "δ\t| ";                                  // Escrita do AFD em um arquivo de texto
            for(i=0; i<t_size; i++)
                txtafd << terminais[i] << "\t| ";
            txtafd << endl;
            for(i=0; i<nt_size; i++){
                txtafd << ((afnd.get_ef(i)) ? "<*" : "<") << nterminais[i] << ">\t| ";
                for(j=0; j<t_size; j++)
                    txtafd << afnd.get_cel(i, j) << "\t| ";
                txtafd << endl;
            }
        }
        txtafd.close();
    } else
        cout << "Impossivel abrir arquivo " << ARQUIVO_W << endl;        

    return 0;
}