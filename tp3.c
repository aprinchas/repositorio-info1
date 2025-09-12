#include <stdio.h>


int main(void) {
	float peso = 0;
	float altura = 0;
	float imc = 0;
	
	do {
		printf("Ingrese el peso en kg: ");
		scanf("%f", &peso);
		
		if(peso < 0)
			printf("Error el peso debe ser positivo.\n");
		
	}
	while(peso < 0);
	
	do {
		printf("Ingrese la altura en metros: ");
		scanf("%f", &altura);
			
			if(altura < 0)
			printf("Error la altura debe ser positiva.\n");
		
	}
	while(altura < 0);
	
	
	imc = peso / (altura * altura);
	
	printf("\nSu indice de masa corporal es: %.2f\n", imc);
	printf("\nIndice         | Condicion\n");
	printf("----------------------------\n");
	printf("<18.5          | Bajo peso\n");
	printf("18.5 a 24.9    | Normal\n");
	printf("25.0 a 29.9    | Sobrepeso\n");
	printf(">=30           | Obesidad\n");
	
	if(imc < 18.5)
		printf("\nSu condicion: Bajo peso");
	
	else if(imc < 24.9)
		printf("\nSu condicion: Normal");
	
	else if(imc < 29.9)
		printf("\nSu condicion: Sobrepeso");
	
	else
		printf("\nSu condicion: Obesidad");
	
	return 0;
}
