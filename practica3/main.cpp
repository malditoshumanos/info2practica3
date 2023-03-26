#include <iostream>
#include <fstream>
#include <bitset> // To easily represent ints(chars) in binary

using namespace std;
int calcularCaso(char chunk[], float tamChunk);


int main()
{
    ifstream inFile;
    int fileSize;
    char * arrayCh;   // Array de chars en el archivo

    int tamChunk = 7; // Tamaño de los bloques de bits en que se dividirá el archivo
    // TODO: comprobar que tamChink sea menor que binaryFileSize

    try{                                  // Leer archivo
        inFile.open("archivoTexto.txt");
        if(!inFile.is_open()) throw 1;

        inFile.seekg(0, inFile.end); // Ir al final del archivo
        fileSize = inFile.tellg(); // Determinar tamaño del archivo (cantidad de caracteres dentro de él)
        inFile.seekg(0, inFile.beg); // Volver al principio del archivo

        arrayCh = new char[fileSize];
        inFile.read(arrayCh, fileSize);

    }
    catch(int err){
        cout << "Error #" << err << ": " << endl;
        if(err == 1) cout << "El archivo no pudo abrirse.";
    }


    cout << "fileSize: " << fileSize << ", arrayCh:" << endl; // print file (char array)
    for (int i = 0; i < fileSize; ++i) {
        cout << arrayCh[i];
    }
    cout << endl << endl;


    cout << "arrayCh in bits with bitset: " << endl;        // print file in binary
    for (int i = 0; i < fileSize; ++i) {
        bitset<8> inBits(arrayCh[i]);
        cout << inBits << " ";
    }
    cout << endl << endl;


    // Cantidad de bits que representan el archivo
    int binaryFileSize = fileSize * 8;
    // Una array que va a guardar individualmente los bits que componen el archivo
    char fileBinary[binaryFileSize];
    for (int iChar = 0; iChar < fileSize; ++iChar) { // Iterate over every char in the file
        bitset<8> chBits(arrayCh[iChar]);  // Convert each char into a bitset object

        for (int iBit = 0; iBit < 8; ++iBit) { // Iterate over every bit

            // the test function returns true if the specified bit is 1 and false if it is 0
            bool boolBitTest = chBits.test(7-iBit);
            if(boolBitTest){
                fileBinary[iChar*8 + iBit] = '1';
            }
            else{
                fileBinary[iChar*8 + iBit] = '0';
            }
        }
    }
    // Imprimir la string de bits que representa el archivo
    cout << "fileBinary: " << endl;
    for (int i = 0; i < (binaryFileSize); ++i) {
        cout << fileBinary[i];
    }
    cout << "\n\n";


    /* Para testear la función calcularCaso
    cout << "sisasyeah" << endl;
    char test[] = {"01100"};
    cout << calcularCaso(test, 5) << endl;
    */


// ****** codificar el archivo con el método 1 *************
    int canChunks = binaryFileSize/tamChunk; // Calcular cuantos bloques de bits hay dentro del archivo
    for (int iChunk = 0; iChunk < canChunks; ++iChunk) { // Iterar sobre cada bloque de bits

        int indBloque = iChunk * tamChunk; // Indice de fileBinary donde comienza el bloque actual

        int casoAnt;  // Variable auxiliar que indica el estado del bloque anteriormente evaluado
        char arrChunk[tamChunk]; // array que cada grupo de bits para llamar la función calcularCaso
        for (int j = 0; j < tamChunk; ++j) {
            arrChunk[j] = fileBinary[indBloque + j]; // guardar el bloque de bits
        }

        if(iChunk == 0){  // Si se está en el primer bloque de bits...
            for (int i = 0; i < tamChunk; ++i) { // invertir los bits del primer bloque
                if(fileBinary[i] == '1'){
                    fileBinary[i] = '0';
                }else{
                    fileBinary[i] = '1';
                }
            }
            casoAnt = calcularCaso(arrChunk, tamChunk); // el caso a considerar en el siguiente bloque
            continue; // continuar al segundo bloque de bits
        }


        switch(casoAnt){
            case 1: // Caso 1: Hay igual cantidad de 1´s y 0´s, se invierte cada bit
                for (int i = 0; i < tamChunk; ++i) {
                    if(fileBinary[indBloque + i] == '1'){
                        fileBinary[indBloque + i] = '0';
                    }else{
                        fileBinary[indBloque + i] = '1';
                    }
                }
                break;
            case 2: // Caso 2: Hay más 0´s que 1´s, se invierten los bits cada 2 bits
                for (int i = 1; i < tamChunk; i+=2) {
                    if(fileBinary[indBloque + i] == '1'){
                        fileBinary[indBloque + i] = '0';
                    }else{
                        fileBinary[indBloque + i] = '1';
                    }
                }
                break;
            case 3:
                for (int i = 2; i < tamChunk; i+=3) { // Caso 3: Hay más 1´s que 0´s, se invierten los bits cada 3 bits
                    if(fileBinary[indBloque + i] == '1'){
                        fileBinary[indBloque + i] = '0';
                    }else{
                        fileBinary[indBloque + i] = '1';
                    }
                }
                break;
        }   // Note que en los casos 2 y 3, se deja el primer o los dos primeros
            //bits de bloque, respectivamente, sin modificar

        casoAnt = calcularCaso(arrChunk, tamChunk); // el caso a considerar en el siguiente bloque
    }
    // En caso de que hayan bits que no quedan agrupados en ningún grupo
    // (i.e. la cantidad de bits en que se representa el archivo no es divisible por el tamaño de los grupos)
    int bitsSobrantes = binaryFileSize - (canChunks*tamChunk);
    if(bitsSobrantes != 0){
        // Invertimos los bits que quedaron sobrando y no pudieron ser agrupados en ningún grupo
        for (int i = tamChunk*canChunks; i < binaryFileSize; ++i) {
            if(fileBinary[i] == '1'){
                fileBinary[i] = '0';
            }else{
                fileBinary[i] = '1';
            }
        }
    }


    cout << "fileBinaryModificada: " << endl;
    for (int i = 0; i < (fileSize*8); ++i) {
        cout << fileBinary[i];
    }
    cout << endl;

    return 0;
}


// Primer método de codificación; esta función determina si en un grupo de bits dado
// hay más 1´s, 0´s o igual cantidad
int calcularCaso(char chunk[], float tamChunk){
    int caso = -1;
    int sumChunk = 0;
    for (int i = 0; i < tamChunk; ++i){
        sumChunk += ((int)chunk[i]) - 48;
    }
    if(sumChunk == tamChunk/2){
        caso = 1; // Hay igual cantidad de 1´s y 0´s
    } else if(sumChunk < tamChunk/2){
        caso = 2; // Hay más 0´s que 1´s
    } else if(sumChunk > tamChunk/2){
        caso = 3; // Hay más 1´s que 0´s
    }
    return caso;
}

/*
Cómo puedo arreglar el código para que reciba archivos con nuevas lineas???
Cada nueva línea aumenta en 2 fileSize pero se imrpimen caracteres "raros" al final del print cuando imprimo arrayCh
*/
