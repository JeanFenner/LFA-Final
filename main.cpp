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

using namespace std;

class Producoes{
    string estado;                      // Estado atual
    string p;                           // Produção/terminal
    string nt;                          // Não-Terminal
    int estado_final;                   // Estado Final?

    public:
        void adicionar(string new_e, string new_p, string new_nt, int new_ef = 0){
            estado = new_e;
            p = new_p;
            nt = new_nt;
            estado_final = new_ef;

            return;
        };

        string get_estado(){
            return estado;
        }
        string get_prod(){
            return p;
        };
        string get_nterminal(){
            return nt;
        };
        bool get_ef(){
            return estado_final;
        }
};

class Table{
    vector <vector<string>> table;

    public:
        Table(int lin, int col){
            int i, j;

            for(i=0; i<col; i++){
                vector <string> nt;
                for(j=0; j<lin; j++)
                    nt.push_back(" ");
                table.push_back(nt);
            }

        }

        void adicionar(int lin, int col, string novo){
            table[col][lin].append(novo+" ");

            return;
        }

        void print_table(int lin, int col){
            int i, j;
            for(i=0; i<lin; i++){
                for(j=0; j<col; j++){
                    cout << table[j][i] << "|\t";
                }
                cout << endl;
            }

            return;
        }

        string get_cel(int lin, int col){
            return table[lin][col];
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
    int ntchar[2];                          // Character do Não Terminal
    int ascii[127] = {0};                   // Lista de uso ASCII
    string simboloe;                        // Simbolo Estado
    string simbolot;                        // Simbolo Terminal
    string simbolont;                       // Simbolo Não-Terminal
    string simbolopar;                      // Simbolo Par da Produção
    string txtline;                         // Linha do .txt
    vector <int> nterminais_ef;             // Vetor de Não-Terminais com Estado Final
    vector <string> terminais;              // Vetor de Terminais
    vector <string> nterminais;             // Vetor de Não-Terminais
    vector <Gra_Tab> gra_tab;               // Relação NT Gramática-Tabela
    vector <Producoes> prod;                // Relação de T/NT
    Producoes par;                          // Item do Vetor <prod>
    Gra_Tab dupla;                          // Item do vetor <gra_tab>
    vector <Producoes>::iterator it_p;      // Iterador do Vetor <prod>
    vector <Gra_Tab>::iterator it_gt;       // Iterador do Vetor <gra_tab>

    ifstream txtfiles("text.txt");
    
    if(txtfiles.is_open()){

        nterminais.push_back("S");
        ntchar[0] = 'A';
        dupla.adicionar("S", "S");
        gra_tab.push_back(dupla);

        while(getline(txtfiles, txtline)){
            cout << txtline << "\n";

            if(txtline[0] == '<'){              // GRAMATICAS
                int ef = 0;
                char txtchar;
                string estado;
                string auxchar;
                j = 1;

                cout << endl;
                for(i=0; i<gra_tab.size(); i++){
                    cout << gra_tab[i].get_g() << "_" << gra_tab[i].get_t() << " ";
                }
                cout << endl;

                while(txtline[j] != '>'){
                    estado.push_back(txtline[j]);
                    j++;
                }
                cout << "G\nestado: " << estado << "\t";

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
                cout << "simboloe: " << simboloe << endl;

                while (txtline[j] != '='){
                    j++;
                }

                for(i=j+1; i<txtline.length(); i++){
                    txtchar = txtline[i];
                    cout << txtchar << "[";
                    if(txtchar>32 && txtchar<127){
                        if(txtchar == '|'){
                            par.adicionar(simboloe, simbolot, simbolont);
                            prod.push_back(par);
                            cout << "+";
                            if(ef){
                                par.adicionar(simbolont, "eps", "NUL", ef);
                                prod.push_back(par);
                                nterminais.push_back(simbolont);
                                ntchar[0]++;
                                ef = 0;
                                cout << "*" << simbolont;
                            }
                        }else if(txtchar=='<'){
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
                            cout << auxchar << " " << simbolont << " ";
                        }else {
                            simbolot = txtchar;
                            ascii[txtchar] = 1;
                            cout << txtchar << " ";
                            if(txtline[i+1]!='<'){
                                simbolont = *ntchar;
                                ntchar[0]++;
                                ef = 1;
                            }
                        }
                    }
                    cout << "] ";
                }
                par.adicionar(simboloe, simbolot, simbolont);
                prod.push_back(par);
                cout << "+";
                if(ef){
                    par.adicionar(simbolont, "eps", "NUL", ef);
                    prod.push_back(par);
                    nterminais.push_back(simbolont);
                    ntchar[0]++;
                    ef = 0;
                    cout << "*" << simbolont;
                }
                cout << endl;
            } else{                             // TOKENS

                simboloe = *nterminais.begin();

                for(i=0; i<txtline.length(); i++){
                    simbolot = txtline[i];
                    simbolont = *ntchar;
                    ascii[txtline[i]] = 1;
                        
                    par.adicionar(simboloe, simbolot, simbolont);
                    prod.push_back(par);

                    cout << simboloe << " " << simbolot << " " << simbolont << endl;                    

                    ntchar[0]++;
                    simboloe = simbolont;
                    nterminais.push_back(simbolont);

                    // NT 'S' é o inicial
                    if(ntchar[0] == 'S' && ntchar[1] != 39)
                        ntchar[0]++;
                }
                simbolot = "eps";
                par.adicionar(simboloe, simbolot, "NUL", 1);
                prod.push_back(par);

                cout << "*" << simboloe << " " << simbolot << " " << simbolont << endl;
            }

            // Adicona ' aos novos NTs case passe de Z
            if(ntchar[0]>90){
                ntchar[0] = 65;
                ntchar[1] = 39;
            }

            cout << endl;
        }
        txtfiles.close();
    } else
        cout << "Impossivel abrir arquivo\n";

// Populando vetor de terminais com os caracteres da ascii utilizados
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
            cout << "*" << (*it_p).get_estado() << " ";
        else
            cout << (*it_p).get_estado() << " ";
    }
    cout << endl;
    for(it_p=prod.begin(); it_p<prod.end(); it_p++){
        cout << (*it_p).get_prod() << " ";
    }
    cout << endl;

    const int t_size = terminais.size();
    const int nt_size = nterminais.size();

    Table table(nt_size, t_size);

// Popular Tabela AFND
    for(it_p=prod.begin(); it_p < prod.end(); it_p++){
        for(i=0; i < nt_size; i++){
            if((*it_p).get_estado() == nterminais[i]){
                for(j=0; j<t_size; j++){
                    if((*it_p).get_prod() == terminais[j]){
                        table.adicionar(i, j, (*it_p).get_nterminal());
                    }
                }
            }
        }
    }

// Tabela AFND
    cout << endl;
    cout << "Tabela AFND\n";
    cout << "\t| ";
    for(i=0; i<t_size; i++)
        cout << terminais[i] << "\t| ";
    cout << endl;
    for(i=0; i<nt_size; i++){
        cout << nterminais[i] << "\t| ";
        for(j=0; j<t_size; j++)
            cout << table.get_cel(j, i) << "\t| ";
        cout << endl;
    }

    return 0;
}