#include <iostream>
#include <fstream>
#include <bitset> // To easily represent ints(chars) in binary
#include <cmath>

using namespace std;
int calcularCaso(char chunk[], float tamChunk);
void decodeM1(char * binaryData, int size, int seed);
void encodeM1(char * fileBinary, int binaryFileSize, int tamChunk);


int main()
{
    ifstream inFile;
    ofstream outFile;
    int fileSize;
    char * arrayCh;   // Array de chars en el archivo

    int tamChunk = 4; // Tamaño de los bloques de bits en que se dividirá el archivo
    // TODO: comprobar que tamChunk sea menor que binaryFileSize y sino tirar una excepción

    try{
        // Leer archivo de entrada
        inFile.open("INPUT_FILE.txt");
        if(!inFile.is_open()) throw 1;

        inFile.seekg(0, inFile.end); // Ir al final del archivo
        fileSize = inFile.tellg(); // Determinar tamaño del archivo (cantidad de caracteres dentro de él)
        inFile.seekg(0, inFile.beg); // Volver al principio del archivo

        arrayCh = new char[fileSize];
        inFile.read(arrayCh, fileSize);

        inFile.close();

        // Abrir archivo de salida
        outFile.open("ENCRIPTADO.bin", std::ios::binary); //Si no existe el archivo, lo crea
        // considere abrir el archivo con la flag trunc para que se borre lo que existe dentro de él cada vez que vaya a escribir
        if(!outFile.is_open()) throw 2;

    }
    catch(int err){
        cout << "Error #" << err << ": " << endl;
        if(err == 1) cout << "El archivo de entrada no pudo abrirse.\n";
        if(err == 2) cout << "El archivo de salida no pudo abrirse.\n";
    }catch (...){
        cout<<"Error no definido.\n";
    }

    /*Como Line Feed (i.e. LF, \n, ascii 10) es representado en dos caracteres, cada
    que hay un salto de linea en INPUT_FILE, fileSize aumenta en 2 (por la manera en que se inicializa esta variable);
    sin embargo, la función read (ver linea 28) guarda cada Line Feed como un 10 en la array arrayCh,
    por lo que si hay x LF en "INPUT_READ", fileSize es x unidades más grande de lo que debería.
    Para solucionar este problema usamos gcount, que dice cuántos caracteres fueron extraidos cuando se usó read*/
    fileSize = inFile.gcount();

    cout << "fileSize: " << fileSize << ", arrayCh:" << endl; // print file (char array)
    for (int i = 0; i < fileSize; ++i) {
        cout <<arrayCh[i];
    }
    cout << "##FIN##" << endl << endl;

    /* //For testing purposes
    cout << "arrayCh in bits with bitset: " << endl;        // print file in binary
    for (int i = 0; i < fileSize; ++i) {
        bitset<8> inBits(arrayCh[i]);
        cout << inBits << " ";
    }
    cout << endl << endl; */


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
    cout << "binaryFileSize: " << binaryFileSize << " -- fileBinary (archivo en binario): " << endl;
    for (int i = 0; i < (binaryFileSize); ++i) {
        cout << fileBinary[i];
    }
    cout << "\n\n";


// ############################Codificación método 1 #############################################################################
    cout << endl << "codificar con metodo 1: " << endl;
    encodeM1(fileBinary, binaryFileSize, tamChunk);
    for (int i = 0; i < binaryFileSize; ++i) {
        cout << fileBinary[i];
    }
    cout << endl << endl;

//##########################################Escribir y leer archivo codificado####################################################
    cout << "Guardando archivo codificado... \n"; // Ponerle la string que represente el nombre según el método usado (y el tiempo en que se hizoo)
    outFile.write(fileBinary, binaryFileSize);
    outFile.close(); // Importante cerrar el archivo binario; sino después el archivo no va a abrir correctamente

    // El parámetro (flag) ate establece la posición inicial del "cursor" al final del archivo
    inFile.open("ENCRIPTADO.bin", ios::in | ios::binary | ios::ate);
    streampos sizeBinFile;
    char * memBlockBinFile;

    if(!inFile.is_open()){
        cout << "Archivo encriptado no abierto." << endl;
    }

    sizeBinFile = inFile.tellg(); // Se puede calcular así el tamaño del archivo gracias a que se instanció inFileCod con el parámetro ate
    memBlockBinFile = new char [sizeBinFile];
    inFile.seekg(0, ios::beg);
    inFile.read(memBlockBinFile, sizeBinFile);
    inFile.close();

    cout << "sizeBinFile: " << sizeBinFile << " -- " << "memBlockBinFile (archivo codificado leido): " << endl;
    for (int i = 0; i < sizeBinFile; ++i) {
        cout << memBlockBinFile[i];
    }
    cout << endl<<endl;


//#####################################################Decodificación Método1##################################################
    decodeM1(memBlockBinFile, sizeBinFile, tamChunk);
    cout << "memBlockBinFile decoded (bits decodificados): " << endl;
    for (int i = 0; i < sizeBinFile; ++i) {
        cout << memBlockBinFile[i];
    }
    cout << endl<<endl;

//##############################################Guardar achivo decodificado######################################################
    int cantBytesDeco = sizeBinFile/8; // Numero de bytes (chars) en el archivo decodificado
    char decodedChars[cantBytesDeco];
    for (int byte = 0; byte < cantBytesDeco; ++byte) { // Iterar sobre cada byte (8 bits) que compone cada letra
        int indByte = 8*byte; // El indice done comienza el byte actual en el array de bits
        int valorByte = 0; // En esta variable se guarda el numero (en base 10) que representan los bits en cada byte
        for (int bit = 0; bit < 8; ++bit) {
            valorByte += ((int)memBlockBinFile[indByte+7-bit] - 48)*(pow(2, bit));
        }
        decodedChars[byte] = valorByte;
    }
    cout << "decodedChars: \n";
    for (int i = 0; i < cantBytesDeco; ++i) {
        cout << decodedChars[i];
    }
    cout<< "###FIN###" << endl << endl;
    // Falta escribir el resultado a un archivo pero eso es trivial.


    return 0;
}
//###############################################Fin del Main#######################################################


// Para usar el primer método de codificación; esta función determina si en un grupo de bits dado
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

//################################################DECODIFICAR MÉTODO 1##########################################################
void decodeM1(char * fileBinary, int binaryFileSize, int tamChunk){

    int canChunks = binaryFileSize / tamChunk;

    for (int iBloq = 0; iBloq < canChunks; ++iBloq) {
        int bloqIndex = iBloq * tamChunk; // Índice del bloque en el que se está trabajando
        int casoAnt; // Para saber qué operaciones ejecutar al transformar cada bloque

        if(iBloq == 0){  // Si se está en el primer bloque de bits...
            for (int i = 0; i < tamChunk; ++i) { // invertir los bits del primer bloque
                if(fileBinary[i] == '1'){
                    fileBinary[i] = '0';
                }else{
                    fileBinary[i] = '1';
                }
            }
            casoAnt = -1; // Para no activar ningun caso del switch
        }

        switch(casoAnt){
            case 1: // Caso 1: Hay igual cantidad de 1´s y 0´s, se invierte cada bit
                for (int i = 0; i < tamChunk; ++i) {
                    if(fileBinary[bloqIndex + i] == '1'){
                        fileBinary[bloqIndex + i] = '0';
                    }else{
                        fileBinary[bloqIndex + i] = '1';
                    }
                }
                break;
            case 2: // Caso 2: Hay más 0´s que 1´s, se invierten los bits cada 2 bits
                for (int i = 1; i < tamChunk; i+=2) {
                    if(fileBinary[bloqIndex + i] == '1'){
                        fileBinary[bloqIndex + i] = '0';
                    }else{
                        fileBinary[bloqIndex + i] = '1';
                    }
                }
                break;
            case 3:
                for (int i = 2; i < tamChunk; i+=3) { // Caso 3: Hay más 1´s que 0´s, se invierten los bits cada 3 bits
                    if(fileBinary[bloqIndex + i] == '1'){
                        fileBinary[bloqIndex + i] = '0';
                    }else{
                        fileBinary[bloqIndex + i] = '1';
                    }
                }
                break;
        }   // Note que en los casos 2 y 3, se deja el primer o los dos primeros
            //bits de bloque, respectivamente, sin modificar


        // Inicializar array que guarda el bloque en el que se está trabajando
        char bloqActual[tamChunk];
        for (int i = 0; i < tamChunk; ++i) {
            bloqActual[i] = fileBinary[bloqIndex+i];
        }
        casoAnt = calcularCaso(bloqActual, tamChunk); // Calcular caso para hacer operaciones
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
}
//################################################CODIFICAR MÉTODO 1##########################################################
void encodeM1(char * fileBinary, int binaryFileSize, int tamChunk){
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
}
