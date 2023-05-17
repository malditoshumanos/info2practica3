#include <iostream>
#include <fstream>
#include <bitset>
#include <string>
#include <sstream>
#include <cmath>
/* Este programa modela la funcionalidad básica de un sistema de registros de usuario en un banco.
Datos importantes:
--> La contraseña del administrador es "SisasYeah"
--> Los datos de los usuarios de guardan en un archivo llamado USER_DATA.txt
--> La seed que usamos para decodificar la contraseña almacenada en sudo.txt es siempre 5 y se usa el "método 1" implementado usando strings
--> Las constraseñas deben ser de 4 caracteres y las cedulas de 10 numeros
--> Los datos de los usuarios se guardan con el formato "cedulaClaveSaldo," (sin espacio entre cada dato y una ";" al final)
*/
using namespace std;

string leerArchivo(string inputFile);
void escribirArchivo(string outputFile, string fileContents);
void reEscribirArchivo(string outputFile, string fileContents);
string codificar_m1(string data, int tamChunk);
string decodificar_m1(string data, int tamChunk);
int calcularCaso(string chunk);
int extractNumbers(string str);
string restarSaldoUsuario(string datosUsuarios, string cedula, int plataSacada);

int main()
{
    char option = 'F';


    while(option != 'E'){
        string info = "\n***Banco SisasYeah***\nIngrese la opcion deseada:\nA: Ingresar como administrador\nU: Ingresar como usuario\nE: Salir del programa\n";
        cout << info;
        cin >> option;
        while(option != 'A' && option!= 'U' && option != 'E'){
            cout << "Ingrese una opcion valida!" << endl;
            cin >> option;
        }

        switch(option){
            case 'A': // Ingresar como administrador
            {
                string password = leerArchivo("C:\\Users\\ASUS\\Documents\\docs\\A learning\\INF2\\Practica3\\Practica3_Aplicacion\\build-aplicacionBanco-Desktop_Qt_6_3_2_MinGW_64_bit-Debug\\sudo.txt"); // Leer contraseña de administrador
                password = decodificar_m1(password, 5); // Decodificar contraseña de administrador

                string possiblePassword;
                cout << "Ingrese contrasenia: ";
                cin >> possiblePassword;

                if(possiblePassword != password){
                    cout << "Contrasenia erronea!"<<endl; // Si la contraseña es incorrecta, no se ingresa como administrador
                    break;
                }

                cout << "Ingresado como administrador. Se va a registrar un nuevo usuario.\n";
                // Obtener datos del nuevo usuario
                string datosNuevoUsuario = "";
                string agregarDato = "";
                cout << "Cedula: "; cin >> agregarDato; datosNuevoUsuario.append(agregarDato);  agregarDato = "";
                cout << "Clave (4 digitos): "; cin >> agregarDato; datosNuevoUsuario.append(agregarDato);  agregarDato = "";
                cout << "Saldo: "; cin >> agregarDato; datosNuevoUsuario.append(agregarDato);  agregarDato = "";
                datosNuevoUsuario.append(",");

                escribirArchivo("USER_DATA.txt", datosNuevoUsuario); //Guardar datos del nuevo usuario
                break;
            }
            case 'U':
            {
                string datosUsuarios = leerArchivo("USER_DATA.txt"); // Se leen los datos de los usuarios

                string cedula;
                cout << "Ingrese su cedula: ";
                cin >> cedula;
                int indexCedula;
                indexCedula = datosUsuarios.find(cedula); // Indice de la cedula del usuario en los datos de los usuarios
                if(indexCedula == -1){ // Si no se encontró la cédula dada, find() retorna npos(i.e. -1)
                    cout << "No se encontro la cedula dada." << endl; // Si la cédula no está registrada, no se ingresa como usuario
                    break;
                }

                string claveIngresada;
                cout << "Ingrese su clave: ";
                cin >> claveIngresada;
                if(claveIngresada != datosUsuarios.substr((datosUsuarios.find(cedula)+10), 4)){
                    cout << "Clave erronea." << endl; // Si la clave es incorrecta, no se ingresa como usuario
                    break;
                }

                char optionUser ='F'; // Se inicializa esta variable en 'F' pues este caracter no corresponde a ninguna de las opciones posibles
                cout << "Ingresado como usuario.\nIngrese la opcion deseada:\nC: Consultar saldo\nS: Sacar plata\n";
                cin >> optionUser;

                if(optionUser == 'C'){ // Si se desea consultar saldo
                    int saldoUsuario;
                    int indexFinUsuario = datosUsuarios.find(",", indexCedula); // Hallar el index en el cual terminan los datos del usuario
                    // El numero de caracteres en el saldo va desde (indice de la cédula + 10(cedula) + 4(clave)) hasta el siguiente usuario (que empieza luego de ",")
                    int longSaldo= indexFinUsuario - (indexCedula+10+4);
                    string saldoEnString = datosUsuarios.substr(indexCedula+10+4, longSaldo); // Se obtiene el saldo
                    saldoUsuario = extractNumbers(saldoEnString);
                    cout << "Saldo (la lectura del saldo vale 1000): " << saldoUsuario << endl;
                    cout << "Saldo luego de realizar el cobro de 1000: " << saldoUsuario - 1000 << endl;

                    // Se realiza el cobro de 1000 al usuario
                    datosUsuarios = restarSaldoUsuario(datosUsuarios, cedula, 1000);
                    reEscribirArchivo("USER_DATA.txt", datosUsuarios);
                }

                if(optionUser == 'S'){ // Si se desea sacar plata
                    int plataParaSacar;
                    cout << "Ingrese la cantidad que desea retirar: " << endl;
                    cin >> plataParaSacar;
                    datosUsuarios = restarSaldoUsuario(datosUsuarios, cedula, plataParaSacar); // Sacar la plata
                    reEscribirArchivo("USER_DATA.txt", datosUsuarios); // guardar nuevos datos de ususarios
                    cout << "Plata retirada!" << endl;
                }
                break; // break caso U
            }
        }

    }
    return 0;
}

//Recibe datos de los usuarios sin codificar y le resta al usuario con cierta cedula cierta cantidad de plata
// Retorna nuevos datos de los usuarios
string restarSaldoUsuario(string datosUsuarios, string cedula, int plataSacada){
    int indexCedula = datosUsuarios.find(cedula); // Indice de la cedula del usuario en los datos de los usuarios
    int saldoUsuario;
    int indexFinUsuario = datosUsuarios.find(",", indexCedula);

    // El numero de caracteres en el saldo va desde el indice de la cédula + 10(cedula) + 4(clave) hasta el siguiente usuario
    int longSaldo= indexFinUsuario - (indexCedula+10+4);
    string saldoEnString = datosUsuarios.substr(indexCedula+10+4, longSaldo);
    datosUsuarios.erase(indexCedula+10+4, longSaldo); // Se elimina el saldo viejo

    saldoUsuario = extractNumbers(saldoEnString); // Se convierte la string del saldo a int
    saldoUsuario -= plataSacada;
    saldoEnString = to_string(saldoUsuario); // Convertir el saldo en int a string

    datosUsuarios.insert(indexCedula+10+4, saldoEnString); //Agregar el nuevo saldo a los datos de usuarios

    return datosUsuarios;
}

// Esta variable recibe una string con numeros y retorna una int con esos numeros
int extractNumbers(string str) {
    int result = 0;
    for (char c : str) {
        result = result * 10 + (c - '0');
    }
    return result;
}

// Función para agregar datos al final de un archivo
void escribirArchivo(string outputFile, string fileContents){
    try{
        ofstream outFile;
        outFile.open(outputFile, std::ofstream::app | std::ofstream::ate); // Usamos la flag append para que el archivo sea añadido al final de cualquier contenido preexistente en el archivo
        if(outFile.fail()) cout << "No se ha podido abrir el siguiente archivo para escribir: " << outputFile << endl;
        cout << fileContents << endl;
        outFile << fileContents;
        outFile.close();
    }
    catch(...){
        cout << "Ha sucedido un error desconocido." << endl;
    }
}

// Función para sobreescribir los datos en un archivo
void reEscribirArchivo(string outputFile, string fileContents){
    try{
        ofstream outFile;
        outFile.open(outputFile, ofstream::trunc); // Usando la flag truncate se borran todos los contenidos del archivo antes de escribir el él
        if(outFile.fail()) cout << "No se ha podido abrir el siguiente archivo para escribir: " << outputFile << endl;
        outFile << fileContents;
        outFile.close();
    }
    catch(...){
        cout << "Ha sucedido un error desconocido." << endl;
    }
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

// Función usada en decodificar_m1()
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
