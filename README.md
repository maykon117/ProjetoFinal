# ProjetoFinal
Descrição do projeto

O projeto consiste em implementar dois algoritmos independentes que façam uma operação conjunta.

algoritmo 1 

	-Receber uma imagem de domensão nxm do usuario no formato PGM sendo binaria ou texto
	-Gerar uma quantidade de Sub-imagens apartir de pontos pseudo-aleatorios.
	-Cada Sub-imagem deve ser processada pelo filtro da média com janela 3x3 e tratamento de borda
	-As Sub-imagens devem ser salvas em um diretorio especificado pelo usuario, decem ser salvas no formato PGM (Binarias ou texto)

Algoritmo 2
	
	-Permitir ao usuário informar o diretorio das Sub-imagens a serem buscadas.
	-Usar um criterio de Similaridade ou dissimilaridade (MSE)
	-Salvar em um diretorio um arquivo txt com as coordenadas

Como o usuário pode compilar?

Algoritmo 01

		<imagemEntrada.pgm> <diretorioSubImagens> <p> <q>

		subimage1.pgm "C:\\Users\\Maykon Jordson\\Desktop\\LabProgramacao-main\\ProjetoLabProg\\" 100 100

Algoritmo 02 

		<imagemEntrada.pgm> <diretorioSubImagens> <arquivoSaida.txt>

		bird.pgm "C:\\Users\\Maykon Jordson\Desktop\\LabProgramacao-main\\ProjetoLabProg\\" coordenadas.txt


Onde o usuário pode encontrar ajuda sobre o projeto?

	O codigo já está bem didatico dentro das aulas que foram ministradas, então o usuário não apresentará muita dificuldade para executar o programa.


Autores do projeto:

		@Maykon Jordson Barbosa Ribeiro

		@Fernado Mápole Fontenele

		@Pedro Victor Teixeira Queiroz
