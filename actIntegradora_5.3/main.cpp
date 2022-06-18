/*
 * Actividad Integradora 5.3
 * Olivia Morales Quezada
 * Cristian Leilael Rico Espinosa
 */

#include <regex>
#include <ctype.h>
#include <fstream>
#include <iostream>
#include <pthread.h>
#include <vector>
#include "utils.h"
#include <sstream>
#include <cstring>  

using namespace std;

pthread_mutex_t lw;

void esNumero (string caracter, int& posicion, string output, ofstream &out){
	output = " ";
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
                out << output;
            }
                

            else
                out << "<span class=\"numero\">"  << output << /*'\t' << */ "</span>";

        }

        else if(caracter[posicion] == '.') {
            out << output;

        }
            

        else
            out << "<span class=\"numero\">"  << output << /*'\t' <<*/  "</span>";

    }

	else if (isalpha(caracter[posicion])) {
        out << output;


    }
		
	else
		out << "<span class=\"numero\">" << output /*<< '\t' */<< "</span>";

	posicion--;

}

void esVariable(string caracter, int& posicion, ofstream &out){
	string output = "";
	string palabrasR = "include|define|#define|#include|auto|break|case|char|const|continue|default|do|double|else|enum|extern|float|for|goto|if|int|long|register|return|short|signed|sizeof|static|struct|switch|typedef|union|unsigned|void|volatile|while|scanf|printf|pthread_t";

	while (isalpha(caracter[posicion]) || isdigit(caracter[posicion]) || caracter[posicion] == '_') {
		output += caracter[posicion];
		posicion++;
	}
	if (caracter[posicion] == '.')
		out << output;
	else {
		 if(regex_match(output, regex(palabrasR)))
		 out << "";
		 else out << "<span class=\"variable\">" << output << '\t' <<  "</span>";
	}
	
	posicion--;
}


void esComentario(string caracter, int& posicion, ofstream &out){
	if (caracter[posicion+1] == '/'){

		out << "<span class=\"comentario\">" << caracter.substr( posicion, caracter.length()-posicion ) << /*'\t' <<*/  "</span>";
		posicion = caracter.length();

	}else out << "<span class=\"operador\">" << "/" << /*'\t' << */ "</span>";
}

void esString(string caracter, int& posicion, ofstream &out){


  string output = "";
	if (caracter[posicion+1] == '"') {

    caracter.replace(caracter.find('"'), 1, "<span class='string'>" );
	output = caracter.substr( posicion, caracter.length()-posicion);


	output.replace(output.find('"'),  caracter.length()-posicion, "'</span>'");

    posicion++;

		out  << output/* '\t' <<*/;
		posicion = caracter.length();

	}else 
	output = caracter.substr( posicion, output.length());
	out << "<span class=\"string\">" << '"' << output << /*<< '\t' */"</span>";
}

void esOperador(string caracter, int& posicion, ofstream &out) {

    if (caracter[posicion] == '+') out << "<span class=\"operador\">" << "+" <</* '\t' << */"</span>";
    else if(caracter[posicion] == '-') out << "<span class=\"operador\">"  << "-" << /*'\t' << */"</span>";
    else if(caracter[posicion] == '*') out << "<span class=\"operador\">" << "*" << /*'\t' <<*/ "</span>";
    else if(caracter[posicion] == '=') out << "<span class=\"operador\">" << "=" << /*'\t' <<*/ "</span>";
    else if(caracter[posicion] == '^') out << "<span class=\"operador\">" << "^" << /*'\t' << */"</span>";
	  else if(caracter[posicion] == '[') out << "<span class=\"operador\">" << "[" << /*'\t' <<*/ "</span>";
    else if(caracter[posicion] == ']') out << "<span class=\"operador\">" << "]" << /*'\t' <<*/ "</span>";
	  else if(caracter[posicion] == '{') out << "<span class=\"operador\">" << "{" << /*'\t' <<*/ "</span>";
    else if(caracter[posicion] == '}') out << "<span class=\"operador\">" << "}" << /*'\t' <<*/ "</span>";
	  else if(caracter[posicion] == '.') out << "<span class=\"operador\">" << "." << /*'\t' <<*/ "</span>";
    else if(caracter[posicion] == ';') out << "<span class=\"operador\">" << ";" << /*'\t' <<*/ "</span>";
	  else if(caracter[posicion] == ',') out << "<span class=\"operador\">" << ", " << /*'\t' << */"</span>";
	   else if(caracter[posicion] == '%') out << "<span class=\"operador\">" << "%" << /*'\t' <<*/ "</span>";
	  else if(caracter[posicion] == '&') out << "<span class=\"operador\">" << "&" << /*'\t' << */"</span>";
	  else if(caracter[posicion] == '\\') out << "<span class=\"operador\">" << "\\" << /*'\t' << */"</span>";
}

void esPalabra(string caracter, ofstream &out) {

	string palabrasR = "#include|#define|auto|break|case|char|const|continue|default|do|double|else|enum|extern|float|for|goto|if|int|long|register|return|short|signed|sizeof|static|struct|switch|typedef|union|unsigned|void|volatile|while|scanf|printf";

	regex e(palabrasR);

	regex_iterator<string::iterator> start(caracter.begin(), caracter.end(), e);
	regex_iterator<string::iterator> end;

	while (start != end) {
    if(regex_match(start->str(), regex(palabrasR))) {
      out << "<span class=\"palabraReservada\">" << start->str() << "</span>" << endl;
      break;
    } else out << "<span class=\"otro\">" << start->str() << "</span>" << endl;
	
    ++start;
  }
}


void lexerAritmetico(string archivo) {
	ofstream out(archivo + ".mono.html");
	out<< "<!DOCTYPE html><html lang='en'><head> <title>mono-Actividad 3.4: Resaltador de Sintaxis</title><meta charset='UTF-8'><link rel='stylesheet' href='style.css'></head><body>" << '\n';
	string linea = "";
	fstream file = fstream(archivo);

	if( file.is_open() ){
		while(! file.eof()){
			
			

			getline(file, linea);

			esPalabra(linea, out);

            for(int i = 0 ; i < linea.length() ; i++){

                if(isalpha(linea[i]) ) esVariable(linea, i, out);
				else if (isdigit(linea[i])) { esNumero(linea, i, "", out); }
				else if(linea[i] == '-'){
					if(isdigit(linea[i+1])) esNumero(linea, ++i, "-", out);
					else esOperador(linea, i, out);
				}

				else if(linea[i] == '"') esString(linea, i, out);
				else if(linea[i] == '/') esComentario(linea, i, out);
				else if(linea[i] == '<') out << "<span class=\"operador\"><</span>";
				else if(linea[i] == '>') out << "<span class=\"operador\">></span>";
				else if(linea[i] == '(') out << "<span class=\"operador\">(</span>";
				else if(linea[i] == ')') out << "<span class=\"operador\">)</span>";
				else if(linea[i] == ')') out << "<span class=\"operador\">)</span>";
				
				else esOperador(linea, i, out);
								
            }

			

			out << "<br>";
        }
    }
    else cout << "Archivo no encontrado" << endl;
	out << "</body></html>" << endl;
	out.close();

}

typedef struct{
	string arr;
} Block;

void* lexerAritmeticoth (void* param) {

	Block *b;
	b = (Block*) param;

	ofstream out(b->arr + ".multi.html");
	out<< "<!DOCTYPE html><html lang='en'><head> <title>multi-Actividad 3.4: Resaltador de Sintaxis</title><meta charset='UTF-8'><link rel='stylesheet' href='style.css'></head><body>" << '\n';
	string linea = "";
	fstream file = fstream(b->arr);
	if( file.is_open() ){
		while(! file.eof()){
			
			getline(file, linea);

			esPalabra(linea, out);

            for(int i = 0 ; i < linea.length() ; i++){

                if(isalpha(linea[i]) ) esVariable(linea, i, out);
				else if (isdigit(linea[i])) { esNumero(linea, i, "", out); }
				else if(linea[i] == '-'){
					if(isdigit(linea[i+1])) esNumero(linea, ++i, "-", out);
					else esOperador(linea, i, out);
				}

				else if(linea[i] == '"') esString(linea, i, out);
				else if(linea[i] == '/') esComentario(linea, i, out);
				else if(linea[i] == '<') out << "<span class=\"operador\"><</span>";
				else if(linea[i] == '>') out << "<span class=\"operador\">></span>";
				else if(linea[i] == '(') out << "<span class=\"operador\">(</span>";
				else if(linea[i] == ')') out << "<span class=\"operador\">)</span>";
				else if(linea[i] == ')') out << "<span class=\"operador\">)</span>";
				
				else esOperador(linea, i, out);
								
            }

			out << "<br>";
        }
    }
    else cout << "Archivo no encontrado" << endl;
	out << "</body></html>" << endl;
	out.close();

	pthread_mutex_lock(&lw);
	pthread_mutex_unlock(&lw);


	pthread_exit(0);

}


int main(int argc, char* argv[]) {

	const int THREADS = argc;
	double ts, tc;

    string documento = "";

	ts = 0;
	start_timer();

	for (int i = 1; i < argc; i++){
		lexerAritmetico(string(argv[i]));
	}

	string documents[argc - 1];
	for (int i = 1; i < argc; i++) {
		documents[i - 1] = string(argv[i]);
	}

	ts = stop_timer();
	cout << "--------------------------------------------------------\n";
	cout << "Implementacion en Secuencial termino en: "<< ts << " ms"<<endl;

	pthread_t tids[THREADS];
    Block blocks[THREADS];

	
	tc = 0;
	int blockSize = (argc - 1) / THREADS;
	for (int i = 1; i < argc; i++) {
		blocks[i - 1].arr = argv[i];
	}
	start_timer();
    for (int i = 0; i < N; i++) {

        for (int j = 1; j < argc; j++) {
            pthread_create(&tids[j-1], NULL, lexerAritmeticoth, (void*) &blocks[j-1]);
        }

		
        for (int j = 1; j < argc; j++) {
			tc += stop_timer();
			cout << "--------------------------------------------------------\n";
			cout << "Implementacion en Concurrente termino en: "<< tc << " ms"<<endl;

			double sp = ts / tc;

			cout << "--------------------------------------------------------\n";
			cout << "El Speedup es de: "<< sp << " ms"<<endl;
			
            pthread_join(tids[j-1], NULL);
			
        }

 
    } 	
    return 0;
}
