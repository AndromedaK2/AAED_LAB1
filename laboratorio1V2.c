#include <stdlib.h>
#include <stdio.h>
#include <time.h>


int columnas = 0;
int * beneficios;
int * pesos;
int totalPesoFinal = 0;
int totalBeneficioFinal = 0;
int pesoMaximo = 0;


int obtenerPeso(int indice){
    return pesos[indice];
}

int obtenerBeneficio(int indice){
    return beneficios[indice];
}

void generarSoluciones(int *solucion, int columnas, int posicion)
{
    if (columnas == posicion)
    {       
        int solucionAux[columnas];
        printf("---");  
        for (int k = 0; k < columnas; k++){
            solucionAux[k] = solucion[k];
            printf("%d/",solucionAux[k]);
        }
         
        printf("---\n");
        int totalPeso      = 0;
        int totalBeneficio = 0;  

        for(int j = 0; j < columnas; j++){

            int solucion = solucionAux[j];
            
            if(solucion == 1){
                totalPeso      = totalPeso + obtenerPeso(j);     
                totalBeneficio = totalBeneficio + obtenerBeneficio(j); 
            }           
            if(totalBeneficio > totalBeneficioFinal && totalPeso < pesoMaximo && totalPeso>0 ){
                totalPesoFinal      = totalPeso;
                totalBeneficioFinal = totalBeneficio;
            }
        }
   
        free(solucionAux);   

    }
    else
    {
        solucion[posicion] = 0;
        generarSoluciones(solucion, columnas, posicion + 1);
        solucion[posicion] = 1;
        generarSoluciones(solucion, columnas, posicion + 1);
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
    clock_t t;
    double time_taken;
    FILE * mochila; 
    pedirArchivo(&mochila);
    pedirPesoMaximo();
    contarLineas(mochila);   
    llenarBeneficiosPesos(mochila);

    int soluciones[columnas];
    t = clock();   
    generarSoluciones(soluciones, columnas, 0);
    t = clock()-t;
    time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("tiempo: %f segundos \n",time_taken);
    printf("peso:%d \n",totalPesoFinal);
    printf("beneficio:%d \n",totalBeneficioFinal);

    free(beneficios);
    free(pesos);
}
 



