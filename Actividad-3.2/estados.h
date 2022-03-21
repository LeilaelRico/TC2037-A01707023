
#include <iostream>
using namespace std;

void esNumero (string caracter, int& posicion, string output){

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
            
            if (caracter[posicion] == '.' || caracter[posicion] == '_' || isalpha(caracter[posicion])) {}
                //cout << "|EXP ERROR";

            else
                cout << output << '\t' << "|REAL" << '\n';

        }

        else if(caracter[posicion] == '.') {}
            //cout << "|REAL ERROR";

        else
            cout << output << '\t' << "|REAL" << '\n';

    }

	else if (isalpha(caracter[posicion])) {}
		//cout << "|MAIN ERROR";

	else
		cout << output << '\t' << "|INTEGER" << '\n';

	posicion--;

}

void esVariable(string caracter, int& posicion){
	string output = "";
	while (isalpha(caracter[posicion]) || isdigit(caracter[posicion]) || caracter[posicion] == '_') {
		output += caracter[posicion];
		posicion++;
	}
	if (caracter[posicion] == '.')
		cout << "ERRORVAR";
	else
		cout << output << '\t' << "|VARIABLE" << '\n';

	posicion--;
}


void esComentario(string caracter, int& posicion){
	if (caracter[posicion+1] == '/'){

		cout << caracter.substr( posicion, caracter.length()-posicion ) << '\t' << "|COMMENTARY" << '\n';
		posicion = caracter.length();

	}else cout << "/" << '\t' << "|DIVISION" << '\n';
}

void esOperador(string caracter, int& posicion) {

    if (caracter[posicion] == '+') cout << "+" << '\t' << "!ADDITION" << '\n';
    else if(caracter[posicion] == '-') cout << "-" << '\t' << "!SUBSTRACTION" << '\n';
    else if(caracter[posicion] == '*') cout << "*" << '\t' << "!MULTIPLICATION" << '\n';
    else if(caracter[posicion] == '=') cout << "=" << '\t' << "!ASSIGMENT" << '\n';
    else if(caracter[posicion] == '^') cout << "^" << '\t' << "!POWER" << '\n';
}