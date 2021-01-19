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

//vector.at()

//cout << "\n-- Ɛ --\n";

using namespace std;

class Producoes{
    public:
        string estado;                      // Estado atual
        string p;                           // Produção/terminal
        string nt;                          // Não-Terminal
        int estado_final;                   // Estado Final?

        void adicionar(string new_e, string new_p, string new_nt, int new_ef = 0){
            estado = new_e;
            p = new_p;
            nt = new_nt;
            estado_final = new_ef;
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

int main(){
    int i;
    int ntchar[2];                          // Character do Não Terminal
    int ascii[127] = {0};                   // Lista de uso ASCII
    string simboloe;                        // Simbolo Estado
    string simbolot;                        // Simbolo Terminal
    string simbolont;                       // Simbolo Não-Terminal
    string simbolopar;                      // Simbolo Par da Produção
    string txtline;                         // Linha do .txt
    vector<char> terminais;                 // Vetor de Terminais
    vector<string> nterminais;              // Vetor de Não-Terminais
    vector<Producoes> prod;                 // Relação de T/NT
    Producoes par;                          // Item de Vetor <prod>

    ifstream txtfiles("text.txt");
    
    if(txtfiles.is_open()){

        nterminais.push_back("S");
        ntchar[0] = 'A';

        while(getline(txtfiles, txtline)){
            cout << txtline << "\n";

            if(txtline[0] == '<'){
// GRAMATICAS
            } else{
// TOKENS
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
                }
                simbolot = "eps";
                par.adicionar(simboloe, simbolot, " ", 1);
                prod.push_back(par);
            }

            if(ntchar[0]>90){
                ntchar[0] = 65;
                ntchar[1] = 39;
            }

            cout << endl;
        }
        txtfiles.close();
    } else
        cout << "Impossivel abrir arquivo\n";
    
    for(i=0; i<127; i++){
        if(ascii[i]!=0)
            terminais.push_back(i);
    }

    for(i=0; i<terminais.size(); i++)
        cout << terminais[i] << " ";
    cout << endl;
    for(i=0; i<nterminais.size(); i++)
        cout << nterminais[i] << " ";
    cout << endl;

    cout << "Producoes\n";
    vector<Producoes>::iterator it;
    for(it=prod.begin(); it<prod.end(); it++){
        if((*it).get_ef())
            cout << "*" << (*it).get_estado() << " ";
        else
            cout << (*it).get_estado() << " ";
    }
    cout << endl;

    for(it=prod.begin(); it<prod.end(); it++){
        cout << (*it).get_prod() << " ";
    }
    cout << endl;

    return 0;
}