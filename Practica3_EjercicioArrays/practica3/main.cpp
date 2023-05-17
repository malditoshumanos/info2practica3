#include <iostream>
#include <fstream>
#include <bitset> // To easily represent ints(chars) in binary
#include <cmath>

using namespace std;
int calcularCaso(char chunk[], float tamChunk);
void decodeM1(char * binaryData, int size, int seed);
void encodeM1(char * fileBinary, int binaryFileSize, int tamChunk);
void encodeM2(char *cFileBinary, char *fileBinary, int binaryFileSize, int tamChunk);
void decodeM2(char *fileBinary,char *cFileBinary, int binaryFileSize, int tamChunk);

int main()
{
    ifstream inFile;
    ofstream outFile;
    int fileSize;
    char * arrayCh;   // Array de chars en el archivo

    int tamChunk; // Tamaño de los bloques de bits en que se dividirá el archivo

    cout << "Ingrese la semilla de codificacion: ";
    cin >> tamChunk;
    char saberMetodo;
    /* Si va a usar las opciones A o B, note que los datos a codificar se ponen en INPUT_FILE.txt.
     Si va a usar las opciones B o D para decodificar, tenga en cuenta que los datos codificados cuando se eligen la opciones
     A y B se guardan en un archivo llamado "ENCRIPTADO.bin."*/
    cout << "Ingrese la opcion deseada...\n'A' es codificar con metodo 1\n'B' es decodificar con metodo 1\n'C' es codificar con metodo 2\n'D' es decodificar con metodo 2\n-->";
    cin >> saberMetodo;

    if(saberMetodo == 'A'){
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

        cout << "Datos leidos en el archivo de entrada:" << endl; // Imprimir datos leídos en el archivo de entrada
        for (int i = 0; i < fileSize; ++i) {
            cout <<arrayCh[i];
        }
        cout << "##FIN##" << endl << endl;

    //#########################################GUARDAR ARCHIVO EN ARRAY DE CHARS (1´s y 0´s)###################################################
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
        cout << "Archivo en binario: " << endl;
        for (int i = 0; i < (binaryFileSize); ++i) {
            cout << fileBinary[i];
        }
        cout << "\n\n";

    // ############################Codificación método 1 CON CHARS#############################################################################
        cout << "Archivo en binario codificado: " << endl;
        encodeM1(fileBinary, binaryFileSize, tamChunk);
        for (int i = 0; i < binaryFileSize; ++i) {
            cout << fileBinary[i];
        }
        cout << endl << endl;

    //##########################################Escribir archivo codificado####################################################
        cout << "Guardando archivo codificado... \n";
        outFile.write(fileBinary, binaryFileSize);
        outFile.close(); // Importante cerrar el archivo binario; sino después el archivo no va a abrir correctamente
    }
    //###################################OPCIÓN B#############################################################################
    else if(saberMetodo == 'B'){
        char archivoParaLeer[20];
        cout << "Ingrese archivo para decodificar: ";
        cin >> archivoParaLeer;
        char archivoParaEscribir[20];
        cout << "Ingrese archivo para escribir los datos decodificados: ";
        cin >> archivoParaEscribir;

        //##########################LEER ARCHIVO CODIFICADO##################################################
        // El parámetro (flag) ate establece la posición inicial del "cursor" al final del archivo
        inFile.open(archivoParaLeer, ios::in | ios::binary | ios::ate);
        streampos sizeBinFile; // Para determinar el tamaño del array de 1´s y 0´s que representan el archivo codificado
        char * memBlockBinFile; // Para guardar los contenidos del archivo codificado

        if(!inFile.is_open()){
            cout << "Archivo encriptado no abierto." << endl;
        }

        sizeBinFile = inFile.tellg(); // Se puede calcular así el tamaño del archivo gracias a que se instanció inFileCod con el parámetro ate
        memBlockBinFile = new char [sizeBinFile];
        inFile.seekg(0, ios::beg); // Devolver el pointer de inFile al principio del archivo
        inFile.read(memBlockBinFile, sizeBinFile);
        inFile.close();

        cout << "Archivo codificado leido: " << endl;
        for (int i = 0; i < sizeBinFile; ++i) {
            cout << memBlockBinFile[i];
        }
        cout << endl<<endl;


    //#####################################################Decodificación Método1 CON CHARS##################################################
        decodeM1(memBlockBinFile, sizeBinFile, tamChunk);
        cout << "Bits decodificados: " << endl;
        for (int i = 0; i < sizeBinFile; ++i) {
            cout << memBlockBinFile[i];
        }
        cout << endl<<endl;

    //##############################################Guardar achivo decodificado######################################################
        int cantBytesDeco = sizeBinFile/8; // Numero de bytes (chars) en el archivo decodificado
        char decodedChars[cantBytesDeco];
        //Este for convierte los bits leídos en chars y los guarda en decodedChars
        for (int byte = 0; byte < cantBytesDeco; ++byte) { // Iterar sobre cada byte (8 bits) que compone cada letra
            int indByte = 8*byte; // El indice done comienza el byte actual en el array de bits
            int valorByte = 0; // En esta variable se guarda el numero (en base 10) que representan los bits en cada byte
            for (int bit = 0; bit < 8; ++bit) {
                valorByte += ((int)memBlockBinFile[indByte+7-bit] - 48)*(pow(2, bit));
            }
            decodedChars[byte] = valorByte;
        }
        cout << "Datos decodificados: \n";
        for (int i = 0; i < cantBytesDeco; ++i) {
            cout << decodedChars[i];
        }
        cout<< "###FIN###" << endl << endl;

        ofstream outFile;
        outFile.open(archivoParaEscribir, std::ios::binary); //Si no existe el archivo, lo crea
        outFile.write(decodedChars, cantBytesDeco);
    }
    //#################################################OPCIÓN C#########################################################
    else if(saberMetodo == 'C'){
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

        cout << "Datos leidos en el archivo de entrada:" << endl; // Imprimir datos leídos en el archivo de entrada
        for (int i = 0; i < fileSize; ++i) {
            cout <<arrayCh[i];
        }
        cout << "##FIN##" << endl << endl;

    //#########################################GUARDAR ARCHIVO EN ARRAY DE CHARS (1´s y 0´s)###################################################
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
        cout << "Archivo en binario: " << endl;
        for (int i = 0; i < (binaryFileSize); ++i) {
            cout << fileBinary[i];
        }
        cout << "\n\n";


    // ############################Codificación método 2 CON CHARS#############################################################################
        cout << endl << "Archivo en binario codificado: " << endl;
        char resultadoM2[binaryFileSize];
        encodeM2(resultadoM2, fileBinary, binaryFileSize, tamChunk);
        for (int i = 0; i < binaryFileSize; ++i) {
            cout << resultadoM2[i];
        }
        cout << endl << endl;

    //##########################################Escribir archivo codificado####################################################
        cout << "Guardando archivo codificado... \n";
        outFile.write(resultadoM2, binaryFileSize);
        outFile.close(); // Importante cerrar el archivo binario; sino después el archivo no va a abrir correctamente
    }
    //#############################################OPCION D######################################################
    else if(saberMetodo == 'D'){
        char archivoParaLeer[20];
        cout << "Ingrese archivo para decodificar: ";
        cin >> archivoParaLeer;
        char archivoParaEscribir[20];
        cout << "Ingrese archivo para escribir los datos decodificados: ";
        cin >> archivoParaEscribir;

        //##########################LEER ARCHIVO CODIFICADO##################################################
        // El parámetro (flag) ate establece la posición inicial del "cursor" al final del archivo
        inFile.open(archivoParaLeer, ios::in | ios::binary | ios::ate);
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

        cout << "Archivo codificado leido: " << endl;
        for (int i = 0; i < sizeBinFile; ++i) {
            cout << memBlockBinFile[i];
        }
        cout << endl<<endl;


    //#####################################################Decodificación Método2 CON CHARS##################################################
        char resultadoM2[(int)sizeBinFile];
        decodeM2(resultadoM2, memBlockBinFile, sizeBinFile, tamChunk);
        cout << "Bits decodificados: " << endl;
        for (int i = 0; i < sizeBinFile; ++i) {
            cout << resultadoM2[i];
        }
        cout << endl<<endl;

    //##############################################Guardar achivo decodificado######################################################
        int cantBytesDeco = sizeBinFile/8; // Numero de bytes (chars) en el archivo decodificado
        char decodedChars[cantBytesDeco];
        //Este for convierte los bits leídos en chars y los guarda en decodedChars
        for (int byte = 0; byte < cantBytesDeco; ++byte) { // Iterar sobre cada byte (8 bits) que compone cada letra
            int indByte = 8*byte; // El indice done comienza el byte actual en el array de bits
            int valorByte = 0; // En esta variable se guarda el numero (en base 10) que representan los bits en cada byte
            for (int bit = 0; bit < 8; ++bit) {
                valorByte += ((int)resultadoM2[indByte+7-bit] - 48)*(pow(2, bit));
            }
            decodedChars[byte] = valorByte;
        }
        cout << "Datos decodificados: \n";
        for (int i = 0; i < cantBytesDeco; ++i) {
            cout << decodedChars[i];
        }
        cout<< "###FIN###" << endl << endl;

        ofstream outFile;
        outFile.open(archivoParaEscribir, std::ios::binary); //Si no existe el archivo, lo crea
        outFile.write(decodedChars, cantBytesDeco);
    }




    return 0;
}
//###############################################Fin del Main#######################################################


//################################################CODIFICAR MÉTODO 2##########################################################
// El primer parámetro de la función es la array de bits ya codificados, el segundo es la array de bits a codificar
void encodeM2(char *cFileBinary, char *fileBinary, int binaryFileSize, int tamChunk){
    int canChunks = binaryFileSize/tamChunk; // Calcular cuantos bloques de bits hay dentro del archivo
    for (int iChunk = 0; iChunk < canChunks; ++iChunk) { // Iterar sobre cada bloque de bits
        int indBloque = iChunk * tamChunk; // Indice de fileBinary donde comienza el bloque actual
        cFileBinary[indBloque] = fileBinary[(indBloque)+tamChunk-1]; //cFileBinary es el archivo en binario codificado
        for (int i=0; i<tamChunk; i++){
            cFileBinary[indBloque+i+1] = fileBinary[indBloque+i];
        }
    }
}

//################################################DECODIFICAR MÉTODO 2##########################################################
// El primer parámetro de la función es la array de bits ya decodificados, el segundo es la array de bits a decodificar
void decodeM2(char *fileBinary,char *cFileBinary, int binaryFileSize, int tamChunk){
    int canChunks = binaryFileSize/tamChunk; // Calcular cuantos bloques de bits hay dentro del archivo
    for (int iChunk = 0; iChunk < canChunks; ++iChunk) { // Iterar sobre cada bloque de bits
        int indBloque = iChunk * tamChunk; // Indice de fileBinary donde comienza el bloque actual
        for (int i=0; i<tamChunk; i++){
            fileBinary[indBloque+i] = cFileBinary[indBloque+i+1];
        }
        fileBinary[(indBloque)+tamChunk-1] = cFileBinary[(indBloque)]; //cFileBinary es el archivo en binario codificado
    }
}



//################################# Para usar el primer método de codificación/decodificación ####################################
// esta función determina si en un grupo de bits dado hay más 1´s, 0´s o igual cantidad
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

//################################################DECODIFICAR MÉTODO 1 CON CHARS##########################################################
// El primer parámetro es la array de bits a decodificar, el segundo es el tamaño de dicha array, el tercero es la semilla del metodo de decodificación
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
//################################################CODIFICAR MÉTODO 1 CON CHARS##########################################################
// El primer parámetro es la array de bits a codificar, el segundo es el tamaño de dicha array, el tercero es la semilla del metodo de codificación
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
// Perdón por la redundancia. Este no es mi mejor código.
// Damn...
