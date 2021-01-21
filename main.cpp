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
#define ARQUIVO_R "text.txt"            // Arquivo que será lido
#define ARQUIVO_W "afd.txt"             // Arquivo em que será escrito
#define EPS "&"                         // Epsilon

using namespace std;

class Producoes{
    string estado;                      // Estado atual
    string t;                           // Terminal
    string nt;                          // Não-Terminal
    int estado_final;                   // Estado Final?

    public:
        void adicionar(string new_e, string new_t, string new_nt, int new_ef = 0){
            estado = new_e;
            t = new_t;
            nt = new_nt;
            estado_final = new_ef;

            return;
        };

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
    vector <vector<string>> table;
    vector <int> estado_final;
    int size_lin;
    int size_col;

    public:
        AF(int lin, int col){
            int i, j;

            for(i=0; i<col; i++){
                vector <string> nt;
                for(j=0; j<lin; j++)
                    nt.push_back(0);
                table.push_back(nt);
            }
            for(i=0; i<lin; i++)
                estado_final.push_back(0);
            size_lin = lin;
            size_col = col;
        }

        void adicionar(int lin, int col, int ef, string novo = " "){
            table[col][lin].append(novo+" ");
            if(ef)
                estado_final[lin] = 1;

            return;
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
    string gramatica;
    string tabela;

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
    int ntchar[2];                          // Caracter do Não Terminal
    int ascii[127] = {0};                   // Lista de uso ASCII
    string simboloe;                        // Símbolo Estado
    string simbolot;                        // Símbolo Terminal
    string simbolont;                       // Símbolo Não-Terminal
    string simbolopar;                      // Símbolo Par da Produção
    string txtline;                         // Linha do .txt
    vector <string> terminais;              // Vetor de Terminais
    vector <string> nterminais;             // Vetor de Não-Terminais
    vector <Gra_Tab> gra_tab;               // Relação NT Gramática-Tabela
    vector <Producoes> prod;                // Vetor das produções de cada Estado
    Producoes par;                          // Item do Vetor <prod>
    Gra_Tab dupla;                          // Item do vetor <gra_tab>
    vector <Producoes>::iterator it_p;      // Iterador do Vetor <prod>
    vector <Gra_Tab>::iterator it_gt;       // Iterador do Vetor <gra_tab>
    ifstream txtfiles(ARQUIVO_R);           // Abrir arquivo .txt



/*
    ####
        Leitura do Arquivo de Texto
    ####
*/

    if(txtfiles.is_open()){

        nterminais.push_back("S");
        ntchar[0] = 'A';
        dupla.adicionar("S", "S");
        gra_tab.push_back(dupla);

        while(getline(txtfiles, txtline)){

            if(txtline[0] == '<'){              // GRAMATICAS
                int ef = 0;
                char txtchar;
                string estado;
                string auxchar;
                j = 1;

                while(txtline[j] != '>'){
                    estado.push_back(txtline[j]);
                    j++;
                }
                for(it_gt=gra_tab.begin(); it_gt<gra_tab.end(); it_gt++){
                    if((*it_gt).compara(estado)){
                        estado = (*it_gt).get_t();
                        it_gt = gra_tab.end();
                    } else if((it_gt+1) == gra_tab.end()){
                        auxchar = (*ntchar);
                        dupla.adicionar(estado, auxchar);
                        gra_tab.push_back(dupla);
                        estado = auxchar;
                        nterminais.push_back(estado);
                        ntchar[0]++;
                    }
                }
                simboloe = estado;
                estado = "NUL";

                while (txtline[j] != '='){
                    j++;
                }

                for(i=j+1; i<txtline.length(); i++){
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
                                par.adicionar(simboloe, simbolot, "NUL", 1);
                                prod.push_back(par);
                            }else{
                                simbolont = (*ntchar);
                                nterminais.push_back(simbolont);
                                ntchar[0]++;

                                par.adicionar(simboloe, simbolot, simbolont);
                                prod.push_back(par);
                                par.adicionar(simbolont, EPS, "NUL", 1);
                                prod.push_back(par);
                            }
                            ef = 0;
                        }else{
                            par.adicionar(simboloe, simbolot, simbolont);
                            prod.push_back(par);
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
                    par.adicionar(simboloe, simbolot, simbolont);
                    prod.push_back(par);
                    ntchar[0]++;
                    simboloe = simbolont;
                    nterminais.push_back(simbolont);

                    // NT 'S' é o inicial
                    if(ntchar[0] == 'S' && ntchar[1] != 39)
                        ntchar[0]++;
                }
                simbolot = EPS;
                par.adicionar(simboloe, simbolot, "NUL", 1);
                prod.push_back(par);
                
                // Adicona ' aos novos NTs case passe de Z
                if(ntchar[0]>90){

                    ntchar[0] = 65;
                    ntchar[1] = 39;
                }
            }
            cout << endl;
        }
        txtfiles.close();
    } else{
        cout << "Impossivel abrir arquivo\n";
        
        return 0;
    }

// Preenchendo vetor de terminais com os caracteres da ascii utilizados
    for(i=0; i<127; i++){
        if(ascii[i]!=0){
            char ctemp = i;
            string stemp;
            stemp.push_back(ctemp);
            terminais.push_back(stemp);
        }
    }

// Prints de Verificação
    for(i=0; i<terminais.size(); i++)
        cout << terminais[i] << " ";
    cout << endl;
    for(i=0; i<nterminais.size(); i++)
        cout << nterminais[i] << " ";
    cout << endl;

    cout << "Producoes\n";
    for(it_p=prod.begin(); it_p<prod.end(); it_p++){
        if((*it_p).get_ef())
            cout << "*" << (*it_p).get_estado() << "\t";
        else
            cout << (*it_p).get_estado() << "\t";
    }
    cout << endl;
    for(it_p=prod.begin(); it_p<prod.end(); it_p++){
        cout << (*it_p).get_terminal() << "\t";
    }
    cout << endl;
    for(it_p=prod.begin(); it_p<prod.end(); it_p++){
        cout << (*it_p).get_nterminal() << "\t";
    }
    cout << endl;
    for(it_p=prod.begin(); it_p<prod.end(); it_p++){
        cout << (*it_p).get_ef() << "\t";
    }
    cout << endl;

    const int t_size = terminais.size();
    const int nt_size = nterminais.size();

/*  ####
        Criação do Autômato Finito
    ####
*/
    cout << "Criando AFND\n";
    AF afnd(nt_size, t_size);

    cout << "AFND criada\nPreenchendo tabela\n";

// Preencher Tabela AFND
    for(it_p=prod.begin(); it_p<prod.end(); it_p++){
        int ef_aux = (*it_p).get_ef();
        string e_aux = (*it_p).get_estado();
        string t_aux = (*it_p).get_terminal();
        string nt_aux = (*it_p).get_nterminal();

        for(i=0; i<nt_size; i++){
            if(e_aux == nterminais[i]){
                for(j=0; j<t_size; j++){
                    if(t_aux == terminais[j]){
                        cout << e_aux << " " << t_aux << " " << ef_aux << " " << nt_aux << endl;
                        afnd.adicionar(i, j, ef_aux, nt_aux);
                    }else if(t_aux == "&"){
                        afnd.adicionar(i, j, ef_aux);
                    }
                }
            }
        }
    }
    cout << "AFND preenchida" << endl;

// Tabela AFND
    cout << endl;
    cout << "Tabela AFND\n";
    cout << "NT / T\t| ";
    for(i=0; i<t_size; i++)
        cout << terminais[i] << "\t| ";
    cout << endl;
    for(i=0; i<nt_size; i++){
        cout << ((afnd.get_ef(i)) ? "*" : " ") << nterminais[i] << "\t| ";
        for(j=0; j<t_size; j++)
            cout << afnd.get_cel(i, j) << "\t| ";
        cout << endl;
    }

    return 0;
}