/*****************************************************/
/* Aluno 1: Maykon Jordson Barbora Ribeiro           */
/* Aluno 2: Fernando Máspole Fontenele               */
/* Aluno 3: Pedro Victor Teixeira de Queiroz         */
/* Matricula 1: 20212045050487                       */
/* Matricula 2: 20212045050630                       */
/* Matricula 3: 202120450506**                       */
/* Avaliação 04: Trabalho Final                      */
/* 04.505.23 - 2023.1 - Prof. Daniel Ferreira        */
/* Compilador: (Mingw GCC)                           */
/*****************************************************/



#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct pgm {
    int tipo;
    int c;
    int r;
    int mv;
    unsigned char *pData;
};



void readPGMImage(struct pgm *pio, const char *filename);
void buscarSubImagem(const struct pgm *inputImage, const char *subImageDir, const char *outputFile);
void writePGMImage(struct pgm *pio, const char *filename);

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Formato: %s <imagemEntrada.pgm> <diretorioSubImagens> <arquivoSaida.txt>\n", argv[0]);
        exit(1);
    }

    // Leitura da imagem de entrada
    struct pgm inputImage;
    readPGMImage(&inputImage, argv[1]);

    //Busca das sub-imagens
    buscarSubImagem(&inputImage, argv[2], argv[3]);
    
    // Libera a memória alocada para a imagem de entrada
    free(inputImage.pData);

    return 0;
}




void readPGMImage(struct pgm *pio, const char *filename) {
    FILE *fp;
    char ch;

    if (!(fp = fopen(filename, "rb"))) {
        perror("Erro ao abrir o arquivo.");
        exit(1);
    }

    if ((ch = getc(fp)) != 'P') {
        puts("A imagem fornecida nao esta no formato PGM.");
        exit(2);
    }

    pio->tipo = getc(fp) - 48;
    fseek(fp, 1, SEEK_CUR);

    while ((ch = getc(fp)) == '#') {
        while ((ch = getc(fp)) != '\n');
    }
    fseek(fp, -1, SEEK_CUR);

    fscanf(fp, "%d %d", &pio->c, &pio->r);
    if (ferror(fp)) {
        perror("Erro ao ler as dimensões da imagem.");
        exit(3);
    }

    fscanf(fp, "%d", &pio->mv);
    fseek(fp, 1, SEEK_CUR);

    pio->pData = (unsigned char *)malloc(pio->r * pio->c * sizeof(unsigned char));

    switch (pio->tipo) {
        case 2:
            puts("Lendo imagem PGM (dados em texto)");
            for (int k = 0; k < (pio->r * pio->c); k++) {
                fscanf(fp, "%hhu", pio->pData + k);
            }
            break;
        case 5:
            puts("Lendo imagem PGM (dados em binario)");
            fread(pio->pData, sizeof(unsigned char), pio->r * pio->c, fp);
            break;
        default:
            puts("Tipo de imagem PGM nao suportado.");
            exit(4);
    }

    fclose(fp);
}






void buscarSubImagem(const struct pgm *inputImage, const char *subImageDir, const char *outputFile) {
    FILE *fp;

    if (!(fp = fopen(outputFile, "w"))) {
        perror("Erro ao criar o arquivo de saida.");
        exit(1);
    }

    DIR *d;
    struct dirent *dir;
    d = opendir(subImageDir);

    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (dir->d_type == DT_REG) { // Verifica se é um arquivo regular
                if (strstr(dir->d_name, ".pgm") != NULL) { // Verifica se é um arquivo PGM
                    // Lê a sub-imagem do arquivo
                    char subImageFile[100];
                    sprintf(subImageFile, "%s/%s", subImageDir, dir->d_name);
                    printf("%s\n", dir->d_name);

                    struct pgm subImage;
                    readPGMImage(&subImage, subImageFile);

                    // Escreve as coordenadas encontradas no arquivo de saída
                    fprintf(fp, "%s, coord x: [SAIDA], coord y: [SAIDA]\n", dir->d_name);

                    // Libera a memória alocada para a sub-imagem
                    free(subImage.pData);
                }
            }
        }
        closedir(d);
    }

    fclose(fp);
}





void writePGMImage(struct pgm *pio, const char *filename) {
    FILE *fp;

    if (!(fp = fopen(filename, "wb"))) {
        perror("Erro ao criar o arquivo.");
        exit(1);
    }

    fprintf(fp, "P%d\n", pio->tipo);
    fprintf(fp, "%d %d\n", pio->c, pio->r);
    fprintf(fp, "%d\n", pio->mv);

    fwrite(pio->pData, sizeof(unsigned char), pio->r * pio->c, fp);

    fclose(fp);
}
