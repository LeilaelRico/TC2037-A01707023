/*
 * Actividad 3.2
 * Olivia Morales Quezada
 * Cristian Leilael Rico Espinosa
 */

#include <ctype.h>
#include <fstream>
#include "estados.h"


void lexerAritmetico(string archivo) {
 
 string linea = "";
	fstream file = fstream(archivo);
	if( file.is_open() ){
		cout << "Token" << '\t' << "|Type" << '\n';
		while(! file.eof()){
			getline(file, linea);
            for(int i = 0 ; i < linea.length() ; i++){

                if (isdigit(linea[i])) { esNumero(linea, i, ""); }
				else if(linea[i] == '-'){
					if(isdigit(linea[i+1])) esNumero(linea, ++i, "-");
					else esOperador(linea, i);
				}
				else if(linea[i] == '/') esComentario(linea, i);
				else if(linea[i] == '(') cout << "(" << '\t' << "|LEFT PARENTHESIS" << '\n';
				else if(linea[i] == ')') cout << ")" << '\t' << "|RIGHT PARENTHESIS" << '\n';
				// else if(linea[i] == '.' || linea[i] == '_') {cout<< "|ERRORFUNCION"<<endl;}
				else if(isalpha(linea[i]) ) esVariable(linea, i);
                else esOperador(linea, i);
                
                //cout << "-------------------------------------------" << endl;

            }

        }
    }
    else cout << "Archivo no encontrado" << endl;
	
	file.close();
}

int main() {

    string documento = "";
	cout<<"Ingrese el nombre del archivo (Incluya el tipo de archivo, .txt, por ejemplo): ";
	cin>>documento;

    cout << endl;

	lexerAritmetico(documento);

    cout << endl;

	return 0;
}
