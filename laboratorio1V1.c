#include <stdlib.h>
#include <stdio.h>

int filas = 0;
int filasFiltradas = 0;
int columnas = 0;
int ** totalSoluciones;
int ** solucionesFiltradas;
int * beneficios;
int * pesos;
int totalPesoFinal = 0;
int totalBeneficioFinal = 0;
int pesoMaximo = 0;

void generarSoluciones(int *solucion, int columnas, int posicion)
{
    if (columnas == posicion)
    {
        filas = filas + 1; 
        int** totalSolucionesAuxiliar = (int**)malloc(filas * sizeof(int*));
        for (int i = 0; i < filas; i++)
            totalSolucionesAuxiliar[i] = (int*)malloc(columnas * sizeof(int));

        for (int i = 0; i < filas-1; i++)       
            for(int j = 0; j<columnas; j++)
                totalSolucionesAuxiliar[i][j] = totalSoluciones[i][j] ;
                   
       
        totalSoluciones = malloc( sizeof * totalSoluciones * filas );
        if ( totalSoluciones )
            for ( int i = 0; i < filas; i++ )
                totalSoluciones[i] = malloc( sizeof *totalSoluciones[i] * columnas );

        
        int solucionAux[columnas];

        for (int k = 0; k < columnas; k++)
            solucionAux[k] = solucion[k];
        

        for(int j = 0; j < columnas; j++)
            totalSolucionesAuxiliar[filas-1][j] = solucionAux[j];
        

        for (int i = 0; i < filas; i++)
            for(int j = 0; j<columnas; j++)
                totalSoluciones[i][j] = totalSolucionesAuxiliar[i][j] ;

        free(solucionAux);  
        free(totalSolucionesAuxiliar);       

    }
    else
    {
        solucion[posicion] = 0;
        generarSoluciones(solucion, columnas, posicion + 1);
        solucion[posicion] = 1;
        generarSoluciones(solucion, columnas, posicion + 1);
    }
} 

int obtenerPeso(int indice){
    return pesos[indice];
}

int obtenerBeneficio(int indice){
    return beneficios[indice];
}

void agregarSolucion(int * solucion, int filas){

    int** soluciones = (int**)malloc(filas * sizeof(int*));
    for (int i = 0; i < filas; i++)
        soluciones[i] = (int*)malloc(columnas * sizeof(int));

    for (int i = 0; i < filas-1; i++)       
        for(int j = 0; j<columnas; j++)
            soluciones[i][j] = solucionesFiltradas[i][j];

    solucionesFiltradas = malloc( sizeof * solucionesFiltradas * filas );
    if ( solucionesFiltradas )
        for ( int i = 0; i < filas; i++ )
            solucionesFiltradas[i] = malloc( sizeof *solucionesFiltradas[i] * columnas );
   
    for(int j = 0; j < columnas; j++)
        soluciones[filas-1][j] = solucion[j];
    
    for (int i = 0; i < filas; i++)
        for(int j = 0; j<columnas; j++)
            solucionesFiltradas[i][j] = soluciones[i][j] ;

    free(soluciones);
    free(solucion);
}

void descartarSoluciones(){
    int solucion[columnas];
    for (int i = 0; i < filas; i++){
        int total = 0;
        for(int j = 0; j < columnas; j++){
            solucion[j] = totalSoluciones[i][j];
            if(solucion[j] == 1){
                total = total + obtenerPeso(j);
            }
        }
        if( total < pesoMaximo && total>0){
            filasFiltradas++;
            agregarSolucion(solucion,filasFiltradas);
        }
    }
    free(solucion);
}    

void buscarMayorBeneficio(){
    for (int i = 0; i < filasFiltradas; i++){
        int totalPeso      = 0;
        int totalBeneficio = 0;
        for(int j = 0; j < columnas; j++){
            int solucion = solucionesFiltradas[i][j];
            if(solucion == 1){
                totalPeso      = totalPeso + obtenerPeso(j);     
                totalBeneficio = totalBeneficio + obtenerBeneficio(j); 
            }           
            if(totalBeneficio>totalBeneficioFinal){
                totalPesoFinal      = totalPeso;
                totalBeneficioFinal = totalBeneficio;
            }
        }
    }
}

void pedirArchivo(FILE ** mochila){
    char nombreArchivo[200];
	printf("Ingrese el nombre del archivo a leer\n");
	scanf("%s", nombreArchivo);
    *mochila = fopen (nombreArchivo, "r");
     if(*mochila==NULL){
        printf("Error al abrir el archivo");
    }
}

void pedirPesoMaximo(){
    printf("Ingrese el peso maximo\n");
	scanf("%d", &pesoMaximo);
}

void leerArchivo(FILE ** mochila, char * nombreArchivo){   
    *mochila = fopen (nombreArchivo, "r");
    if(*mochila==NULL){
        printf("Error al abrir el archivo");
    }
}

void contarLineas(FILE * mochila){
    int contarLineas = 0;
    char c;
    do
    {
        c = fgetc(mochila);
        if( c == '\n'){
            contarLineas++;
        };

    } while(c != EOF);
    columnas = contarLineas;
    rewind(mochila);
}

void mostrarSoluciones(int filas, int columnas, int**soluciones){
    for (int i = 0; i < filas; i++){
        printf("---");  
        for (int j = 0; j < columnas; j++){
            printf("%d/",soluciones[i][j]);
        }
        printf("---\n");
    }
}

void llenarBeneficiosPesos(FILE * mochila){
    int i = 0;
    beneficios = (int*)malloc(sizeof(int)*columnas);
    pesos      = (int*)malloc(sizeof(int)*columnas);

    while(i<columnas){
        fscanf(mochila, "%d %d",&beneficios[i],&pesos[i]);
        printf("%d - %d",beneficios[i],pesos[i]);
        i++;
        printf("\n");
    }
      
}

int main()
{
    FILE * mochila;
    pedirArchivo(&mochila);   
    pedirPesoMaximo();
    contarLineas(mochila);   
    llenarBeneficiosPesos(mochila);

    int soluciones[columnas];   
    generarSoluciones(soluciones, columnas, 0);

    printf("Total Soluciones\n");
    mostrarSoluciones(filas,columnas,totalSoluciones);


    printf("\nSoluciones filtradas\n");
    descartarSoluciones();
    mostrarSoluciones(filasFiltradas,columnas,solucionesFiltradas);
    buscarMayorBeneficio();

    printf("peso:%d \n",totalPesoFinal);
    printf("beneficio:%d \n",totalBeneficioFinal);

    free(solucionesFiltradas);
    free(totalSoluciones);
    free(beneficios);
    free(pesos);

}
 



