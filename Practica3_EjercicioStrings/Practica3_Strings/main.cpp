#include <iostream>
#include <fstream>
#include <bitset>
#include <string>
#include <sstream>
#include <cmath>

using namespace std;

int determinarTamañoArchivo(string FILE_NAME);

string leerArchivo(string inputFile);
void escribirArchivo(string outputFile, string fileContents);
string codificar_m1(string data, int tamChunk);
string decodificar_m1(string data, int tamChunk);
int calcularCaso(string chunk);
string codificar_m2(string data, int tamChunk);
string decodificar_m2(string data, int tamChunk);


int main()
{
    int seed;
    cout << "Ingrese la semilla de codificacion: ";
    cin >> seed;

    char saberMetodo;
    cout << "Ingrese la opcion deseada...\n'A' es codificar con metodo 1\n'B' es decodificar con metodo 1\n'C' es codificar con metodo 2\n'D' es decodificar con metodo 2\n-->";
    cin >> saberMetodo;
    /* Si va a usar las opciones A o B, note que los datos a codificar se ponen en INPUT_FILE.txt.
     Si va a usar las opciones B o D para decodificar, tenga en cuenta que los datos codificados cuando se eligen la opciones
     A y B se guardan en un archivo llamado "ENCRIPTADO.bin."*/
    if(saberMetodo == 'A'){
        string mensaje = leerArchivo("INPUT_FILE.txt");
        cout << "Contenido del archivo:\n";
        cout << mensaje << endl;

        cout << "Codificando el contenido: \n";
        string codificado = codificar_m1(mensaje, seed);
        cout << codificado << endl;

        cout << "Guardando contenido codificado...\n";
        escribirArchivo("ENCRIPTADO.bin", codificado);
    }
    else if(saberMetodo == 'B'){
        string archivoParaLeer;
        cout << "Ingrese archivo para decodificar: ";
        cin >> archivoParaLeer;
        string archivoParaEscribir;
        cout << "Ingrese archivo para escribir los datos decodificados: ";
        cin >> archivoParaEscribir;

        cout << "Leyendo contenido codificado:\n";
        string paraDecodificar = leerArchivo(archivoParaLeer);
        cout << paraDecodificar << endl;

        cout << "Decodificando:\n";
        string decodificado = decodificar_m1(paraDecodificar, seed);
        cout << decodificado << endl;

        cout << "Guardando archivo decodificado...\n";
        escribirArchivo(archivoParaEscribir, decodificado);
    }
    else if(saberMetodo == 'C'){
        string mensaje = leerArchivo("INPUT_FILE.txt");
        cout << "Contenido del archivo:\n";
        cout << mensaje << endl;

        cout << "Codificando el contenido: \n";
        string codificado = codificar_m2(mensaje, seed);
        cout << codificado << endl;

        cout << "Guardando contenido codificado...\n";
        escribirArchivo("ENCRIPTADO.bin", codificado);
    }
    else if(saberMetodo == 'D'){
        string archivoParaLeer;
        cout << "Ingrese archivo para decodificar: ";
        cin >> archivoParaLeer;
        string archivoParaEscribir;
        cout << "Ingrese archivo para escribir los datos decodificados: ";
        cin >> archivoParaEscribir;

        cout << "Leyendo contenido codificado:\n";
        string paraDecodificar = leerArchivo(archivoParaLeer);
        cout << paraDecodificar << endl;

        cout << "Decodificando:\n";
        string decodificado = decodificar_m2(paraDecodificar, seed);
        cout << decodificado << endl;

        cout << "Guardando archivo decodificado...\n";
        escribirArchivo(archivoParaEscribir, decodificado);
    }












    /*int seed = 5; // Longitud de los bloques

    cout << "Leyendo archivo... \n";
    string mensaje = leerArchivo("INPUT_FILE.txt");

    cout << "Contenido del archivo:\n";
    cout << mensaje << endl;

    cout << "Codificando el contenido: \n";
    string codificado = codificar_m1(mensaje, seed);
    cout << codificado << endl;

    cout << "Guardando contenido codificado...\n";
    escribirArchivo("OUTPUT_FILE.txt", codificado);



    cout << "Leyendo contenido codificado:\n";
    string paraDecodificar = leerArchivo("OUTPUT_FILE.txt");
    cout << paraDecodificar << endl;

    cout << "Decodificando:\n";
    string decodificado = decodificar_m1(paraDecodificar, seed);
    cout << decodificado << endl;

    cout << "Guardando archivo decodificado...\n";
    escribirArchivo("OUT_DECODIFICADO.txt", decodificado);

    cout << "Sisasyeah: " << endl;
    string sisasYeah = decodificar_m1(leerArchivo("sisas.txt"), 5);
    cout << sisasYeah << endl;
    */

    return 0;
}

// recibe string de bits codificados, la decodifica y la pasa a chars (retorna string de información (human-readable))
string decodificar_m1(string data, int tamChunk){
    int longData = data.size();
    int canChunks = longData / tamChunk;
    int casoAnt;

    for (int iChunk = 0; iChunk < canChunks; ++iChunk) {
        int indBloque = iChunk * tamChunk;

        if(iChunk == 0){ // Cuando se considera el primer bloque
            for(int i = 0; i < tamChunk; ++i){
                if(data[i] == '1') data[i] = '0'; // Se invierten los clips del bloque
                else data[i] = '1';
            }
            casoAnt = -1; // Para no activar ningun caso del switch
        }

        switch(casoAnt){
            case 1: // Caso 1: Hay igual cantidad de 1´s y 0´s
                for (int i = 0; i < tamChunk; ++i) {
                    if(data[indBloque + i] == '1') data[indBloque + i] = '0'; // Se invierten los clips del bloque
                    else data[indBloque + i] = '1';
                }
                break;
            case 2: // Caso 2: Hay más 0´s que 1´s
                for (int i = 1; i < tamChunk; i+=2) {
                    if(data[indBloque + i] == '1') data[indBloque + i] = '0'; // Cada 2 bits se invierten los clips del bloque
                    else data[indBloque + i] = '1';
                }
                break;
            case 3: // Caso 3: Hay más 1´s que 0´s
                for (int i = 2; i < tamChunk; i+=3) {
                    if(data[indBloque + i] == '1') data[indBloque + i] = '0'; // Cada 3 bits se invierten los clips del bloque
                    else data[indBloque + i] = '1';
                }
                break;
            // Note que en los casos 2 y 3, se deja el primer o los dos primeros
            //bits de bloque, respectivamente, sin modificar
        }
        casoAnt = calcularCaso(data.substr(indBloque, tamChunk));
    }
    // En caso de que hayan bits que no quedan agrupados en ningún grupo
    // (i.e. la cantidad de bits en que se representa el archivo no es divisible por el tamaño de los grupos)
    if(longData - (canChunks * tamChunk)){
        // Invertimos los bits que quedaron sobrando y no pudieron ser agrupados en ningún grupo
        for (int i = canChunks * tamChunk; i < longData; ++i) {
            if(data[i] == '1') data[i] = '0';
            else data[i] = '1';
        }
    }
    // Para pasar de binario a chars los bits decodificados
    string strDeco;
    int cantBytesDeco = longData / 8;
    for (int byte = 0; byte < cantBytesDeco; ++byte) {
        int indByte = byte*8;
        int valorByte = 0;
        for (int bit = 0; bit < 8; ++bit) {
            valorByte += ((int)data[indByte+7-bit] - 48) * pow(2, bit);
        }
        strDeco += (char)valorByte;
    }
    return strDeco;
}

// recibe string de información leída del archivo, la pasa a binario y la codifica (retorna string de bits codificados)
string codificar_m1(string data, int tamChunk){
    string datosBin; // String que guardará los datos en binario
    for (auto ch : data) {
        bitset<8> chBits(ch);
        datosBin.append(chBits.to_string());
    }

    int longDatosBin = datosBin.size();
    int canChunks = longDatosBin / tamChunk;

    for (int iChunk = 0; iChunk < canChunks; ++iChunk) {
        int indBloque = iChunk * tamChunk; // Indice del bloque actual en el string datos
        string chunk = datosBin.substr(indBloque, tamChunk); // Bloque de bits para llamar calcularCaso()
        int casoAnt; // Variable auxiliar que indica el estado del bloque anteriormente evaluado

        if(iChunk == 0){ // Si está en el primer bloque
            for (int i = 0; i < tamChunk; ++i) {
                if(datosBin[i] == '1') datosBin[i] = '0'; // Se invierten los clips del bloque
                else datosBin[i] = '1';
            }
            casoAnt = calcularCaso(chunk);
            continue;
        }

        switch(casoAnt){
            case 1: // Caso 1: Hay igual cantidad de 1´s y 0´s
                for (int i = 0; i < tamChunk; ++i) {
                    if(datosBin[indBloque + i] == '1') datosBin[indBloque + i] = '0'; // Se invierten los clips del bloque
                    else datosBin[indBloque + i] = '1';
                }
                break;
            case 2: // Caso 2: Hay más 0´s que 1´s
                for (int i = 1; i < tamChunk; i+=2) {
                    if(datosBin[indBloque + i] == '1') datosBin[indBloque + i] = '0'; // Cada 2 bits se invierten los clips del bloque
                    else datosBin[indBloque + i] = '1';
                }
                break;
            case 3: // Caso 3: Hay más 1´s que 0´s
                for (int i = 2; i < tamChunk; i+=3) {
                    if(datosBin[indBloque + i] == '1') datosBin[indBloque + i] = '0'; // Cada 3 bits se invierten los clips del bloque
                    else datosBin[indBloque + i] = '1';
                }
                break;
            // Note que en los casos 2 y 3, se deja el primer o los dos primeros
            //bits de bloque, respectivamente, sin modificar
        }

        casoAnt = calcularCaso(chunk); // el caso a considerar en el siguiente bloque
    }

    // En caso de que hayan bits que no quedan agrupados en ningún grupo
    // (i.e. la cantidad de bits en que se representa el archivo no es divisible por el tamaño de los grupos)
    if(longDatosBin - (canChunks * tamChunk)){
        // Invertimos los bits que quedaron sobrando y no pudieron ser agrupados en ningún grupo
        for (int i = canChunks * tamChunk; i < longDatosBin; ++i) {
            if(datosBin[i] == '1') datosBin[i] = '0';
            else datosBin[i] = '1';
        }
    }
    return datosBin;
}

int calcularCaso(string chunk){
    int caso = -1;
    int cont1 = 0;
    int cont0 = 0;
    for (char bit : chunk) {
        if(bit == '1') cont1++;
        else cont0++;
    }
    if(cont1 == cont0) caso = 1;
    if(cont0 > cont1) caso = 2;
    if(cont1 > cont0) caso = 3;
    return caso;
}

string leerArchivo(string inputFile){
    string file_contents;
    try{
        ifstream inFile;
        inFile.open(inputFile);
        if(inFile.fail()) cout << "No se ha podido leer el siguiente archivo: " << inputFile << endl;
        stringstream buffer;
        buffer << inFile.rdbuf();
        file_contents = buffer.str();
        inFile.close();
    }
    catch(...){
        cout << "Ha sucedido un error desconocido." << endl;
    }

    return file_contents;
}

void escribirArchivo(string outputFile, string fileContents){
    try{
        ofstream outFile;
        outFile.open(outputFile);
        if(outFile.fail()) cout << "No se ha podido abrir el siguiente archivo para escribir: " << outputFile << endl;
        outFile << fileContents;
        outFile.close();
    }
    catch(...){
        cout << "Ha sucedido un error desconocido." << endl;
    }
}

// recibe string de información leída del archivo, la pasa a binario y la codifica (retorna string de bits codificados)
string codificar_m2(string data, int tamChunk){
    string datosBin; // String que guardará los datos en binario
    for (auto ch : data) {
        bitset<8> chBits(ch);
        datosBin.append(chBits.to_string());
    }
    data = datosBin;
    string datosBinR = data;

    int longData = data.size();
    int canChunks = longData / tamChunk;
    for (int iChunk = 0; iChunk < canChunks; ++iChunk) {
        int indBloque = iChunk * tamChunk;
        datosBinR[indBloque] = data[(indBloque)+tamChunk-1];
        for (int i=0; i<tamChunk; i++){
            datosBinR[indBloque+i+1] = data[indBloque+i];
        }
    }
    return datosBinR;
}

// recibe string de bits codificados, la decodifica y la pasa a chars (retorna string de información (human-readable))
string decodificar_m2(string data, int tamChunk){
    int longData = data.size();
    int canChunks = longData / tamChunk;
    string datosBinR;
    for (int iChunk = 0; iChunk < canChunks; ++iChunk) {
        int indBloque = iChunk * tamChunk;
        for (int i=0; i<tamChunk; i++){
            datosBinR[indBloque+i] = data[indBloque+i+1];
        }
        datosBinR[(indBloque)+tamChunk-1] = data[indBloque];
    }
    string strDeco;
    int cantBytesDeco = longData / 8;
    for (int byte = 0; byte < cantBytesDeco; ++byte) {
        int indByte = byte*8;
        int valorByte = 0;
        for (int bit = 0; bit < 8; ++bit) {
            valorByte += ((int)datosBinR[indByte+7-bit] - 48) * pow(2, bit);
        }
        strDeco += (char)valorByte;
    }

    return strDeco;
}
