#include <stdio.h>

int main(void) {
  float vet1[15], vet2[15];
  float a, b, c;
  for(int i=0; i<15; i++){
    printf("Digite um número\n");
    scanf("%f", &vet1[i]);
  }
  for(int i=0;i<15;i++){
    if(i==0){
      a=vet1[i];
      b=vet1[i];
    }
    if(vet1[i]<a){
      a=vet1[i];
    }
    if(vet1[i]>b){
      b=vet1[i];
    }
  }
  printf("A soma do maior elemento e do menor é: %.2f", a+b);
  return 0;
}