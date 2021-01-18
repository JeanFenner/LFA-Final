/*
Universidade Federal da Fronteira Sul - UFFS
Linguagem Formal e Autômatos
Professor Braulio Mello
Aluno Jean Carlo Fenner


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
        string nt;
        string p;
        int estado_final;

        void adicionar(string new_nt, string new_p, int ef){
            nt = new_nt;
            p = new_p;
            estado_final = ef;
        };

        string get_nterminal(){
            return nt;
        };
        string get_prod(){
            return p;
        };
        bool get_ef(){
            return estado_final;
        }
};

/*
void txtleitor(string line, vector<char> *terminais, vector<char> *nterminais, vector<Producoes> *prod){
    int i=0;
    int ascii[127] = {0};
    char simbolo;

for(i=0; i<127; i++){
    cout << ascii[i] << " ";
    if(!(i%16))
        cout << endl;
}

    nterminais->push_back('S');

    while(line[i]){
        simbolo = line[i];
        if(line[0] == '<'){
            cout << "gramatica\n";
        } else{
            cout << "tokens\n";
            ascii[simbolo] = 1;

            if(i==0)
                Producoes p(nterminais->begin(), simbolo);
            
        }


        i++;
    }

    for(i=0; i<127; i++){
        if(ascii[i])
            terminais->push_back(ascii[i]);
    }
    
    return;
};
*/

int main(){
    int i, ntchar[2];
    int ascii[127] = {0};
    string simbolot;
    string simbolont;
    string simbolopar;
    string txtline;
    vector<char> terminais;
    vector<char> nterminais;
    vector<Producoes> prod;
    Producoes par;

    ifstream txtfiles("text.txt");
    
    if(txtfiles.is_open()){

        nterminais.push_back('S');
        ntchar[0] = 'A';

        while(getline(txtfiles, txtline)){
            cout << txtline << "\n";

//            txtleitor(txtline, &terminais, &nterminais, &prod);

            if(txtline[0] == '<'){
// GRAMATICAS
                for(i=1; i<txtline.length(); i++){
                    cout << txtline[i] << " ";
                    if(i=1){
                        if(txtline[i] == 'S')
                            simbolont = *simbolont.begin();
                        else
                            simbolont = ntchar[0]++;
                        i += 2;
                    }else if(txtline[i+1]==':' && txtline[i+2]=='='){
                        i += 3;
                    } else{
                        switch (txtline[i]){
                            case ' ':
                                break;
                            case '<':
                                break;
                            case '>':
                                break;
                            case '|':
                                break;
                            case '&':
                                simbolopar = txtline[i];
                                par.adicionar(simbolont, simbolopar, 1);

                                break;
                            default:
                                simbolopar = txtline[i];
                                ascii[txtline[i]] = 1;
                                par.adicionar(simbolont, simbolopar, 0);

                                break;
                        }
                    }
                }
                cout << endl;
            } else{
// TOKENS
                simbolont = *nterminais.begin();

                for(i=0; i<txtline.length(); i++){
        

                    simbolot = txtline[i];
                    ascii[txtline[i]] = 1;

                    if(i>0)
                        simbolont = ntchar[0]++;
                        
                    par.adicionar(simbolont, simbolot, 0);
                    prod.push_back(par);
                    
                    cout << simbolont << " " << simbolot << endl;
                }
                    simbolont = ntchar[0]++;
                    simbolot = '\n';
                    par.adicionar(simbolont, simbolot, 1);
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

    for(i=0; i<nterminais.size(); i++)
        cout << nterminais[i] << " ";
    cout << endl;
    for(i=0; i<terminais.size(); i++)
        cout << terminais[i] << " ";
    cout << endl;

    cout << "Producoes\n";
    vector<Producoes>::iterator it;
    for(it=prod.begin(); it<prod.end(); it++){
        if((*it).get_ef())
            cout << "*" << (*it).get_nterminal() << " ";
        else
            cout << (*it).get_nterminal() << " ";
    }

    return 0;
}