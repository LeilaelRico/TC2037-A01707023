#include <iostream>
using namespace std;
void esNumero (string caracter, int& posicion, string output, ofstream &out){

    while (isdigit(caracter[posicion])) {
		output += caracter[posicion];
		posicion++;
	}
	if( caracter[posicion] == '.') {
        // Para evaluar reales

        output += caracter[posicion];
        posicion++;
        while(isdigit(caracter[posicion])) {
            output += caracter[posicion];
            posicion++;
        }

        if (caracter[posicion] == 'E' || caracter[posicion] == 'e') {

            // Para hallar si son exponentes

            output += caracter[posicion];
            posicion++;
            if (caracter[posicion] == '-'){
                output += caracter[posicion];
                posicion++;
            }
            while (isdigit(caracter[posicion])) {
                output += caracter[posicion];
                posicion++;
            }
            
            if (caracter[posicion] == '.' || caracter[posicion] == '_' || isalpha(caracter[posicion])) {
                out << "|EXP ERROR";
            }
                

            else
                out << "<span class=\"numero\"> "  << output << '\t' <<  "</span>" << '\n';

        }

        else if(caracter[posicion] == '.') {
            out << "|REAL ERROR";

        }
            

        else
            out << "<span class=\"numero\"> "  << output << '\t' <<  "</span>" << '\n';

    }

	else if (isalpha(caracter[posicion])) {
        out << "|MAIN ERROR";


    }
		
	else
		out << "<span class=\"numero\"> " << output << '\t' <<  "</span>" << '\n';

	posicion--;

}

void esVariable(string caracter, int& posicion, ofstream &out){
	string output = "";
	while (isalpha(caracter[posicion]) || isdigit(caracter[posicion]) || caracter[posicion] == '_') {
		output += caracter[posicion];
		posicion++;
	}
	if (caracter[posicion] == '.')
		out << "ERRORVAR";
	else
		out << "<span class=\"variable\"> " << output << '\t' <<  "</span>" << '\n';

	posicion--;
}


void esComentario(string caracter, int& posicion, ofstream &out){
	if (caracter[posicion+1] == '/'){

		out << "<span class=\"comentario\"> " << caracter.substr( posicion, caracter.length()-posicion ) << '\t' <<  "</span>" << '\n';
		posicion = caracter.length();

	}else out << "<span class=\"operador\"> " << "/" << '\t' <<  "</span>" << '\n';
}

void esOperador(string caracter, int& posicion, ofstream &out) {

    if (caracter[posicion] == '+') out << "<span class=\"operador\"> " << "+" << '\t' << "</span>" << '\n';
    else if(caracter[posicion] == '-') out << "<span class=\"operador\">"  << "-" << '\t' << "</span>"  << '\n';
    else if(caracter[posicion] == '*') out << "<span class=\"operador\"> " << "*" << '\t' << "</span>" << '\n';
    else if(caracter[posicion] == '=') out << "<span class=\"operador\"> " << "=" << '\t' << "</span>" << '\n';
    else if(caracter[posicion] == '^') out << "<span class=\"operador\"> " << "^" << '\t' << "</span>" << '\n';
}
