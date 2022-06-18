/*
 * Actividad Integradora 5.3
 * Olivia Morales Quezada
 * Ricardo Núñez Alanís
 * Cristian Leilael Rico Espinosa
 */

#include <ctype.h>
#include <fstream>
#include <iostream>
#include <pthread.h>
#include <vector>
#include "utils.h"
#include <sstream>

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
                out << "|EXP ERROR";
            }
                

            else
                out << "<span class=\"numero\">"  << output << /*'\t' << */ "</span>";

        }

        else if(caracter[posicion] == '.') {
            out << "|REAL ERROR";

        }
            

        else
            out << "<span class=\"numero\">"  << output << /*'\t' <<*/  "</span>";

    }

	else if (isalpha(caracter[posicion])) {
        out << "|MAIN ERROR";


    }
		
	else
		out << "<span class=\"numero\">" << output /*<< '\t' */<< "</span>";

	posicion--;

}

void esVariable(string caracter, int& posicion, ofstream &out){
	string output = "";
	while (isalpha(caracter[posicion]) || isdigit(caracter[posicion]) || caracter[posicion] == '_') {
		output += caracter[posicion];
		posicion++;
	}
	if (caracter[posicion] == '.')
		out << output;
	else if(isalpha(caracter[posicion]))
		out <<  " " << output;
	else
		out << "<span class=\"variable\">" << output << /*'\t' << */"</span>";

	posicion--;
}


void esComentario(string caracter, int& posicion, ofstream &out){
	if (caracter[posicion+1] == '/'){

		out << "<span class=\"comentario\">" << caracter.substr( posicion, caracter.length()-posicion ) << /*'\t' <<*/  "</span>";
		posicion = caracter.length();

	}else out << "<span class=\"operador\">" << "/" << /*'\t' << */ "</span>";
}

void esString(string caracter, int& posicion, ofstream &out){
	if (caracter[posicion] == '"'){

		out << "<span class=\"string\">" << caracter.substr( posicion, caracter.length()-posicion ) <</* '\t' <<*/  "</span>";
		posicion = caracter.length();

	}else out << "<span class=\"string\">" << '"' /*<< '\t' */<<  "</span>";
}

void esLibreria(string caracter, int& posicion, ofstream &out){
	if (caracter[posicion] == '<'){

		out << "<span class=\"otro\">" << caracter.substr( posicion, caracter.length()-posicion ) << /*'\t' << */ "</span>";
		posicion = caracter.length();

	}else out << "<span class=\"otro\">" << '<' /*<< '\t'*/ <<  "</span>";
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
}






void lexerAritmetico(string archivo) {
	ofstream out(archivo + ".mono.html");
	out<< "<!DOCTYPE html><html lang='en'><head> <title>Actividad 3.4: Resaltador de Sintaxis</title><meta charset='UTF-8'><link rel='stylesheet' href='style.css'></head><body>" << '\n';
	string linea = "";
	fstream file = fstream(archivo);

	if( file.is_open() ){
		//cout << "<!DOCTYPE html><html lang='en'><head> <title>Actividad 3.4: Resaltador de Sintaxis</title><meta charset='UTF-8'><link rel='stylesheet' href='style.css'></head><body>" << '\n';
		while(! file.eof()){
			getline(file, linea);
            for(int i = 0 ; i < linea.length() ; i++){

                if (isdigit(linea[i])) { esNumero(linea, i, "", out); }
				else if(linea[i] == '-'){
					if(isdigit(linea[i+1])) esNumero(linea, ++i, "-", out);
					else esOperador(linea, i, out);
				}
				else if(linea[i] == '/') esComentario(linea, i, out);
				//else if(linea[i] == '<') esLibreria(linea, i, out);
				else if(linea[i] == '<') out << "<span class=\"operador\"><</span>";
				else if(linea[i] == '>') out << "<span class=\"operador\">></span>";
				else if(linea[i] == '(') out << "<span class=\"operador\"> (</span>";
				else if(linea[i] == ')') out << "<span class=\"operador\">)</span>";
				else if(linea[i] == '#') out << "<span class=\"variable\">#</span>";
				else if(linea[i] == '"') esString(linea, i, out);
				// else if(linea[i] == '.' || linea[i] == '_') {cout<< "|ERRORFUNCION"<<endl;}
				else if(isalpha(linea[i]) ) esVariable(linea, i, out);
                else esOperador(linea, i, out);
				
                //cout << "-------------------------------------------" << endl;
				
            }

			out << "<br>";
        }
    }
    else cout << "Archivo no encontrado" << endl;
	out << "</body></html>" << endl;
	out.close();

}

typedef struct{
	//int start, end;
	string arr;
} Block;

void* lexerAritmeticoth (void* param) {

	Block *b;
	b = (Block*) param;

	ofstream out(b->arr + ".multi.html");
	out<< "<!DOCTYPE html><html lang='en'><head> <title>Actividad 3.4: Resaltador de Sintaxis</title><meta charset='UTF-8'><link rel='stylesheet' href='style.css'></head><body>" << '\n';
	string linea = "";
	fstream file = fstream(b->arr);
	if( file.is_open() ){
		//cout << "<!DOCTYPE html><html lang='en'><head> <title>Actividad 3.4: Resaltador de Sintaxis</title><meta charset='UTF-8'><link rel='stylesheet' href='style.css'></head><body>" << '\n';
		while(! file.eof()){
			getline(file, linea);
            for(int i = 0 ; i < linea.length() ; i++){

                if (isdigit(linea[i])) { esNumero(linea, i, "", out); }
				else if(linea[i] == '-'){
					if(isdigit(linea[i+1])) esNumero(linea, ++i, "-", out);
					else esOperador(linea, i, out);
				}
				else if(linea[i] == '/') esComentario(linea, i, out);
				//else if(linea[i] == '<') esLibreria(linea, i, out);
				else if(linea[i] == '<') out << "<span class=\"operador\"><</span>";
				else if(linea[i] == '>') out << "<span class=\"operador\">></span>";
				else if(linea[i] == '(') out << "<span class=\"operador\"> (</span>";
				else if(linea[i] == ')') out << "<span class=\"operador\">)</span>";
				else if(linea[i] == '#') out << "<span class=\"variable\">#</span>";
				else if(linea[i] == '"') esString(linea, i, out);
				// else if(linea[i] == '.' || linea[i] == '_') {cout<< "|ERRORFUNCION"<<endl;}
				else if(isalpha(linea[i]) ) esVariable(linea, i, out);
                else esOperador(linea, i, out);
				
                //cout << "-------------------------------------------" << endl;
				
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


    string documento = "";

	start_timer();

	for (int i = 1; i < argc; i++){
		lexerAritmetico(string(argv[i]));
	}

	string documents[argc - 1];
	for (int i = 1; i < argc; i++) {
		documents[i - 1] = string(argv[i]);
	}

	for (string s : documents) {
		cout << s << endl;
	}

	cout << "Termino en: "<< stop_timer()<< " ms"<<endl;

	pthread_t tids[THREADS];
    Block blocks[THREADS];

	
	int blockSize = (argc - 1) / THREADS;
	for (int i = 1; i < argc; i++) {
		/*blocks[i - 1].start = (i * argv[i]);
		blocks[i - 1].end = (i + 1) * argv[i];*/
		blocks[i - 1].arr = argv[i];
		cout << argv[i] << endl;
	}

    for (int i = 0; i < N; i++) {
		cout << "argv[i]" << endl;
        start_timer();

        for (int j = 1; j < argc; j++) {
            pthread_create(&tids[j-1], NULL, lexerAritmeticoth, (void*) &blocks[j-1]);
        }

        for (int j = 1; j < argc; j++) {
            pthread_join(tids[j-1], NULL);
        }

        cout << "Termino en: "<< stop_timer()<< " ms"<<endl;
    }  
    return 0;

	/*
	vector<string> allDocs(argv, argv + argc);

	start_timer();

	if (argc < 1) {
		cout << "usage: " << argv[0] << " pathname\n";
		return 0;
  	}

	for (int i = 1; i < allDocs.size(); i++){
		lexerAritmetico(allDocs[i]);
		cout << "Analisis de " << allDocs[i] << " terminado." << endl; //Borrar
	}

	cout << "Termino en: "<< stop_timer()<< " ms"<<endl;
	
	
	start_timer();
	pthread_t tid[allDocs.size()];
	Block blocks[allDocs.size()];

	for (int i = 1; i < allDocs.size(); i++) {
		blocks[i].documento = allDocs[i];
	}

	for (int i = 1; i < allDocs.size(); i++) {
		pthread_create(&tid[i], NULL, lexerAritmeticoth, (void*) &blocks[i]);
	}

	for (int i = 1; i < allDocs.size(); i++) {
	pthread_join(tid[i], NULL);
	}

	cout << "Multihilo termino en: " << stop_timer()  << "ms"<< std::endl;
	return 0;
	*/
}
