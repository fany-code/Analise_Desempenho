#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define TAM_LINK 10000

typedef struct little_ {
	unsigned long int numero_eventos;
	double tempo_anterior;
	double soma_areas;
} little;

void inicia_little(little *l) {
	l->numero_eventos = 0;
	l->tempo_anterior = 0.0;
	l->soma_areas = 0.0;
}

double aleatorio() {
	double u = rand() / ((double)RAND_MAX + 1);
	// limitando entre (0,1]
	u = 1.0 - u;
	return (u);
}

double tamPacote() {
	double u = aleatorio();
	if (u <= 0.5)
	{
		return 550.0;
	}
	else if (u <= 0.9)
	{
		return 40.0;
	}
	return 1500.0;
}

double minimo(double n1, double n2) {
	if (n1 < n2)
		return n1;
	return n2;
}

double maximo(double n1, double n2) {
	if (n1 < n2)
		return n2;
	return n1;
}

double tmp_col;

void coletaDados(little en, little ew_entrada, little ew_saida, double soma_ocupacao) {
	double en_final = (en.soma_areas) / tmp_col;
	double ew_final = ((ew_entrada.soma_areas - (ew_saida.soma_areas)) / (abs(ew_entrada.numero_eventos)));
	double lambda = ew_entrada.numero_eventos / tmp_col;


	printf("%f     | %lf", tmp_col, en_final);
	printf("        | %lf", ew_final);
	printf("        | %.15lf", en_final - lambda * ew_final);
	printf("        | %5lf\n", soma_ocupacao / tmp_col);

	tmp_col += 100;
}

int main() {

	/*****************************
	 *  Declaração das variaveis *
	 *****************************/

	/* Gerar numeros pseudoaleatorios
	 * com media igual a 100 tamPacote()s/segundos.
	 *
	 * Na exponencial, E[X] = 1/l
	 *
	 * E[X] = 100
	 * 1/l = 100
	 * l = 1/100
	 * l = 0.01
	 *
	 */

	double tempo_chegada, tempo_saida, soma_ocupacao = 0.0,
		   tempo_simulacao, tempo_decorrido = 0.0, media_chegadas,
		   link;

	unsigned long int fila = 0;

	// double pacote;

	// variaveis para o calculo de E[N] e E[W]
	little en;
	little ew_entrada;
	little ew_saida;

	inicia_little(&en);
	inicia_little(&ew_entrada);
	inicia_little(&ew_saida);

	media_chegadas = (1.0 / 0.01);
	// printf("Informe o intervalo medio entre chegadas (segundos): ");
	// scanf("%lF", &media_chegadas);

	link = (0.044545 * 100) * TAM_LINK; // 60% = 0.0735; 80% = 0.055125; 95% = 0.046421; 99% = 0.044545
	// scanf("%lF", &link);

	tempo_simulacao = 36000;
	// printf("Informe o tempo de simulacao (segundos): ");
	// scanf("%lF", &tempo_simulacao);

	/*	 ***************************
		 *  Iniciando semente      *
		 ***************************

		int sementeAleat;
		sementeAleat = time(NULL);
		srand(sementeAleat); //inicia o rand
	*/

	srand(4400);
	puts("	TEMPO 	| 	E[N]	| 	E[W]	| 	ERRO LITTLE	| 	OCUPAÇÃO");

	/************************
	 *Tempo chegada 1a req  *
	 ************************/
	tempo_chegada = (-1.0 / media_chegadas) * log(aleatorio());

	/************************
	 *	     Execucao       *
	 ************************/

	unsigned long int max_fila = 0;
	tmp_col = 100;
	while (tempo_decorrido <= tempo_simulacao) {

		tempo_decorrido = !fila ? minimo(tempo_chegada, tmp_col) : minimo(minimo(tempo_chegada, tempo_saida), tmp_col);

		if (tempo_decorrido == tempo_chegada) {
			// tratando a chegada de requisicao
			if (!fila) {
				tempo_saida = tempo_decorrido + tamPacote() / link;
				soma_ocupacao += tempo_saida - tempo_decorrido;
			}
			fila++;
			max_fila = maximo(max_fila, fila);
			tempo_chegada = tempo_decorrido + (-1.0 / media_chegadas) * log(aleatorio());

			en.soma_areas += (tempo_decorrido - en.tempo_anterior) * en.numero_eventos;
			en.tempo_anterior = tempo_decorrido;
			en.numero_eventos++;

			ew_entrada.soma_areas += (tempo_decorrido - ew_entrada.tempo_anterior) * ew_entrada.numero_eventos;
			ew_entrada.tempo_anterior = tempo_decorrido;
			ew_entrada.numero_eventos++;
		} else if (tempo_decorrido == tempo_saida) {
			// tratando a saida de requisicao
			// printf("Saida em %lF.\n", tempo_decorrido);
			fila--;

			if (fila){
				tempo_saida = tempo_decorrido + tamPacote() / link;
				soma_ocupacao += tempo_saida - tempo_decorrido;
			}

			// little
			en.soma_areas += (tempo_decorrido - en.tempo_anterior) * en.numero_eventos;
			en.tempo_anterior = tempo_decorrido;
			en.numero_eventos--;

			ew_saida.soma_areas += (tempo_decorrido - ew_saida.tempo_anterior) * ew_saida.numero_eventos;
			ew_saida.tempo_anterior = tempo_decorrido;
			ew_saida.numero_eventos++;
		} else if (tempo_decorrido == tmp_col) {

			en.soma_areas += (tempo_decorrido - en.tempo_anterior) * en.numero_eventos;
			ew_entrada.soma_areas += (tempo_decorrido - ew_entrada.tempo_anterior) * ew_entrada.numero_eventos;
			ew_saida.soma_areas += (tempo_decorrido - ew_saida.tempo_anterior) * ew_saida.numero_eventos;

			ew_saida.tempo_anterior = tempo_decorrido;
			en.tempo_anterior = tempo_decorrido;
			ew_entrada.tempo_anterior = tempo_decorrido;
			coletaDados(en, ew_entrada, ew_saida, soma_ocupacao);
		}
	}

	en.soma_areas += (tempo_decorrido - en.tempo_anterior) * en.numero_eventos;
	ew_entrada.soma_areas += (tempo_decorrido - ew_entrada.tempo_anterior) * ew_entrada.numero_eventos;
	ew_saida.soma_areas += (tempo_decorrido - ew_saida.tempo_anterior) * ew_saida.numero_eventos;

	printf("Ocupacao: %lf\n", soma_ocupacao / maximo(tempo_decorrido, tempo_saida));

	double en_final = (en.soma_areas / tempo_decorrido);
	double ew_final = (ew_entrada.soma_areas - ew_saida.soma_areas) / ew_entrada.numero_eventos;
	double lambda = ew_entrada.numero_eventos / tempo_decorrido;
	printf("E[N]: %f\n", en_final);
	printf("E[W]: %f\n", ew_final);

	// E[N] = lambda * E[W]
	// E[N] - lambda * E[W] ~ 0 (erro little)
	printf("Erro little: %.20f\n", en_final - lambda * ew_final);

	return 0;
}