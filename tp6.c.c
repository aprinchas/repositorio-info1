#include <stdio.h>
#include <math.h>

#define PI 3.14159

int figura=0;

float calcularAreaRectangulo(float altura, float largo){
	return (altura*largo);
}
	
float calcularPerimetroRectangulo(float altura, float largo){
	return altura*2+largo*2;
}

float calcularDiagonaRectangulo(float altura, float largo){
	return sqrt(altura*altura+largo*largo);
}
	
float calcularAreaCirculo(float radio){
	return PI*radio*radio;
}
	
float calcularPerimetroCirculo(float radio){
	return 2*PI*radio;
}
	
void imprimirResultado(float area, float perimetro){
	if(figura == 1){
		printf("El area del rectangulo es: %f \n", area);
		printf("El perimetro del rectangulo es: %f", perimetro);
	}
	
	else {
		printf("El area del circulo es: %f \n", area);
		printf("El perimetro del circulo es: %f", perimetro);
	}
	
	
}
	
int main(int argc, char *argv[]) {
	
	printf("Ingrese la figura que desea calcular (1: rectangulo, 2: circulo):");
	scanf("%d", &figura);
	
	while(figura != 1 && figura != 2){
		printf("Por favor seleccione una opcion valida:");
		scanf("%d", &figura);
	}
	
	
 	if(figura == 1){
		float largo = 0;
		float altura = 0;
		printf("Ingrese el largo:");
		scanf("%f", &largo);
		printf("Ingrese el alto:");
		scanf("%f", &altura);
		imprimirResultado(calcularAreaRectangulo(altura, largo),calcularPerimetroRectangulo(altura, largo));
		
	}
	else{
		float radio = 0;
		printf("Ingrese el radio:");
		scanf("%f", &radio);
		imprimirResultado(calcularAreaCirculo(radio), calcularPerimetroCirculo(radio));
		
	}
	
}

