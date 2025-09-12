#include <stdio.h>

#define TAM 5

void cargardatos(int n){  //link del repo https://github.com/aprinchas/repositorio-info1
	
	int codigos[n];
	float precios[n];
	int max = 0;
	int min = 0;
	
	for(int i=0; i<n; i++){
		
		do{
		printf("ingrese el codigo de barra del elemento numero %d (debe ser entero, positivo y tener menos de 9 cifras):", i);
		scanf("%d", &codigos[i]);
			
		} while(codigos[i]< 0 || codigos[i] > 999999999);
		
		
		do{
			printf("ingrese el precio del elemento numero %d (debe ser positivo):", i);
			scanf("%f", &precios[i]);
		} while(precios[i] < 0);
		printf("\n");
	}
	printf("CODIGO     PRECIO\n");
	for(int i=0; i<n; i++){
		printf("%d   ", codigos[i]);
		printf("%.2f   \n", precios[i]);
		
		if(precios[max] <  precios[i])
			max = i;
		if(precios[min] >  precios[i])
			min = i;
	}
	
	printf("mas caro: [%d] %.2f \n", codigos[max], precios[max]);
	printf("mas barato: [%d] %.2f", codigos[min], precios[min]);
}

int main() {
	
	cargardatos(TAM);
	
	return 0;
}

