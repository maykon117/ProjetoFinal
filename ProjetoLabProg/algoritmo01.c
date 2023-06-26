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


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct pgm {
    int tipo;
    int c;
    int r;
    int mv;
    unsigned char *pData;
};

void readPGMImage(struct pgm *pio, const char *filename);
void writePGMImage(struct pgm *pio, const char *filename);
void aplicarFiltro(struct pgm *inputImage, int windowSize);
void gerarSubImages(const struct pgm *inputImage, int subWidth, int subHeight, const char *outputDir);

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Formato: %s <imagemEntrada.pgm> <diretorioSubImagens> <p> <q>\n", argv[0]);
        exit(1);
    }

    // Leitura da imagem de entrada
    struct pgm inputImage;
    readPGMImage(&inputImage, argv[1]);

    int p = atoi(argv[3]); // Converter o valor de p para inteiro
    int q = atoi(argv[4]); // Converter o valor de q para inteiro

    aplicarFiltro(&inputImage, 3);
    // Algoritmo 1: Geração de sub-imagens
    gerarSubImages(&inputImage, p, q, argv[2]);

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
        puts("A imagem fornecida não está no formato PGM.");
        exit(2);
    }

    pio->tipo = getc(fp) -48;

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

    pio->pData = (unsigned char*)malloc(pio->r * pio->c * sizeof(unsigned char));

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

void gerarSubImages(const struct pgm *inputImage, int subWidth, int subHeight, const char *outputDir) {
    int numSubImages = 10; // Número de sub-imagens a serem geradas (exemplo)
    srand(time(NULL)); // Inicializa a semente do gerador de números aleatórios
    
    for (int i = 1; i <= numSubImages; i++) {

        // Gera as coordenadas aleatórriamente da sub-imagem
        int startX = rand() % (inputImage->c - subWidth + 1);
        int startY = rand() % (inputImage->r - subHeight + 1);

        // Cria a sub-imagem
        struct pgm subImage;
        subImage.tipo = inputImage->tipo;
        subImage.c = subWidth;
        subImage.r = subHeight;
        subImage.mv = inputImage->mv;
        subImage.pData = (unsigned char*)malloc(subWidth * subHeight * sizeof(unsigned char));

        // Copia os valores dos pixels da sub-imagem a partir da imagem de entrada
        for (int y = 0; y < subHeight; y++) {
            for (int x = 0; x < subWidth; x++) {
                int inputX = startX + x;
                int inputY = startY + y;
                int inputIndex = inputY * inputImage->c + inputX;
                int subIndex = y * subWidth + x;
                subImage.pData[subIndex] = inputImage->pData[inputIndex];
            }
        }

        // Salva a sub-imagem em formato PGM no diretório de saída
        char outputFilename[100];
        sprintf(outputFilename, "%s/subimage%d.pgm", outputDir, i);
        writePGMImage(&subImage, outputFilename);

        // Libera a memória alocada para a sub-imagem
        free(subImage.pData);
    }
}
void aplicarFiltro(struct pgm *inputImage, int janela) {
    int imageWidth = inputImage->c;
    int imageHeight = inputImage->r;
    unsigned char *filteredData = (unsigned char*)malloc(imageWidth * imageHeight * sizeof(unsigned char));

    for (int y = 0; y < imageHeight; y++) {
        for (int x = 0; x < imageWidth; x++) {
            int pixelIndex = y * imageWidth + x;
            int sum = 0;
            int count = 0;

            for (int j = -janela / 2; j <= janela / 2; j++) {
                for (int i = -janela / 2; i <= janela / 2; i++) {
                    int vX = x + i;
                    int vY = y + j;
                        //valor do vizinho X e Y
                    if (vX >= 0 && vX < imageWidth && vY >= 0 && vY < imageHeight) {
                        int vIndex = vY * imageWidth + vX;
                        sum += inputImage->pData[vIndex];
                        count++;
                    } else {
                        sum += 0; //borda com valor zero
                        count++;
                    }
                }
            }

            filteredData[pixelIndex] = sum / count;
        }
    }

    free(inputImage->pData);
    inputImage->pData = filteredData;
}
