#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

#define const 1

typedef struct little_{
    unsigned long int no_eventos;
    double tempo_anterior;
    double soma_areas;
} little;

double valorAbsoluto(double x) {
	if (x >= 0.0) return x;
	else return -x;
}

double aleatorio() {
	double u = rand() / ((double) RAND_MAX + 1);
	//limitando entre (0,1]
	u = 1.0 - u;

	return (u);
}


double minimo(double num1, double num2){
    if(num1 < num2){
        return num1;
    }
    return num2;
}

double maximo(double num1, double num2){
    if(num1 > num2){
        return num1;
    }
    return num2;
}

void inicia_little(little * l){
    l->no_eventos = 0;
    l->tempo_anterior = 0.0;
    l->soma_areas = 0.0;
}

int seg = 100;
double tmp_col;

void dados(little e_n, little e_w_chegada, little e_w_saida, double soma_tempo_servico){
/*    
    double dado_en = 0.0, dado_ew_saida = 0.0, dado_ew_chegada = 0.0, dado, aux = 0;
    dado_en = (tmp_col - e_n.tempo_anterior) * (e_n.no_eventos);
    dado_ew_chegada = (tmp_col - e_w_chegada.tempo_anterior) * (e_w_chegada.no_eventos);
    dado_ew_saida = (tmp_col - e_w_saida.tempo_anterior) * (e_w_saida.no_eventos);
*/
    double e_n_final = (e_n.soma_areas) / tmp_col;

    double e_w_final = 
    valorAbsoluto(((e_w_chegada.soma_areas - (e_w_saida.soma_areas)) / (e_w_chegada.no_eventos)));

    double lambda = e_w_chegada.no_eventos / tmp_col;

    printf("%f     | %lf", tmp_col, e_n_final);              
    printf("        | %lf", e_w_final);                         
    printf("        | %.20lf", e_n_final - lambda * e_w_final); 
    printf("        | %lf\n", soma_tempo_servico / tmp_col);      

    Ocupação = soma_tempo_servico / tmp_col;


    tmp_col += seg;
}

int main(){

    double tempo_simulacao;
    double tempo_decorrido = 0.0;

    double intervalo_medio_chegada;
    double tempo_medio_servico;

    double chegada;
    double servico;

    double soma_tempo_servico = 0.0;
    double saida;

    unsigned long int fila = 0;
    unsigned long int max_fila = 0;

    /**
     Little
    */
    little e_n, e_n_dado;
    little e_w_chegada, e_w_dadoChegada;
    little e_w_saida, e_w_dadoSaida;

    inicia_little(&e_n);
    inicia_little(&e_w_chegada);
    inicia_little(&e_w_saida);

    /**
    Little -- fim
    */

    //srand(time(NULL));
    srand(10000);


    printf("Informe o tempo de simulacao (segundos): ");
    scanf("%lF",&tempo_simulacao);

    printf("Informe o intervalo medio entre chegadas (segundos): ");
    scanf("%lF",&intervalo_medio_chegada);

    printf("Informe o tempo medio de servico (segundos): ");
    scanf("%lF",&tempo_medio_servico);
    //printf("\n\n");

    printf("Tempo   |     E[N]       |     E[W]      | Erro_little   |  Ocupação\n\n");

    // gerando o seg de chegada da primeira requisicao
    chegada = (-1.0 / (1.0 / intervalo_medio_chegada))* log(aleatorio());
    
    tmp_col = seg;

    while(tempo_decorrido <= tempo_simulacao){
        tempo_decorrido = !fila ? minimo(chegada, tmp_col) : minimo(minimo(chegada, servico), tmp_col);
        if(tempo_decorrido == chegada){//chegada
            //printf("Chegada em %lF.\n", tempo_decorrido);
            //tmpEntrada = tmpEntrada + const;
            if(!fila){
                servico = tempo_decorrido + (-1.0/(1.0/tempo_medio_servico))* log(aleatorio());
                soma_tempo_servico += servico - tempo_decorrido;
            }

            fila++;
            max_fila = fila > max_fila? fila: max_fila;

            chegada = tempo_decorrido + (-1.0/(1.0/intervalo_medio_chegada))* log(aleatorio());
   
            // little
            e_n.soma_areas += 
            (tempo_decorrido - e_n.tempo_anterior)* e_n.no_eventos;
            e_n.tempo_anterior = tempo_decorrido;
            e_n.no_eventos++;

            e_w_chegada.soma_areas += 
            (tempo_decorrido - e_w_chegada.tempo_anterior)* e_w_chegada.no_eventos;
            e_w_chegada.tempo_anterior = tempo_decorrido;
            e_w_chegada.no_eventos++;


        } else if (tempo_decorrido == servico){
            //saida
            //printf("Saida em %lF.\n", tempo_decorrido);
            fila--;
            if(fila){
                servico = tempo_decorrido + (-1.0/(1.0/tempo_medio_servico))* log(aleatorio());

                soma_tempo_servico += servico - tempo_decorrido;
            }

            //e_n_dado = e_n;
            //little
            e_n.soma_areas += (tempo_decorrido - e_n.tempo_anterior) * e_n.no_eventos;
            e_n.tempo_anterior = tempo_decorrido;
            e_n.no_eventos--;

            //e_n_dado.soma_areas += (tmp_col - e_n_dado.tempo_anterior) * e_n_dado.no_eventos;
            //e_w_dadoSaida = e_w_saida;
            e_w_saida.soma_areas += (tempo_decorrido - e_w_saida.tempo_anterior)* e_w_saida.no_eventos;
            e_w_saida.tempo_anterior = tempo_decorrido;
            e_w_saida.no_eventos++;
            //e_w_dadoSaida.soma_areas += (tmp_col - e_w_dadoSaida.tempo_anterior) * e_w_dadoSaida.no_eventos;
            
        } else if (tempo_decorrido == tmp_col) {
            if (e_w_chegada.no_eventos != e_w_saida.no_eventos){
                e_n_dado = e_n;
                e_n_dado.soma_areas += (tmp_col - e_n.tempo_anterior)* (e_n.no_eventos);
                
                e_w_dadoChegada = e_w_chegada;
                e_w_dadoChegada.soma_areas += (tmp_col - e_w_chegada.tempo_anterior)* (e_w_chegada.no_eventos);
                
                e_w_dadoSaida = e_w_saida;
                e_w_dadoSaida.soma_areas += (tmp_col - e_w_saida.tempo_anterior)* (e_w_saida.no_eventos);
                
                dados(e_n_dado, e_w_dadoChegada, e_w_dadoSaida, soma_tempo_servico);

            } else {
                e_n_dado = e_n;
                e_n_dado.soma_areas = e_n.soma_areas - 
                                      (((e_n.tempo_anterior - e_w_chegada.tempo_anterior) * (e_n.no_eventos + const)) -
                                      ((e_n.tempo_anterior - e_w_chegada.tempo_anterior) * e_n.no_eventos));
                e_w_dadoChegada = e_w_chegada;
                e_w_dadoChegada.soma_areas += 
                    (e_w_saida.tempo_anterior - e_w_chegada.tempo_anterior) * (e_w_chegada.no_eventos - const);
                dados(e_n_dado, e_w_dadoChegada, e_w_saida, soma_tempo_servico);

            }
        }

    }

    e_w_chegada.soma_areas += 
    (tempo_decorrido - e_w_chegada.tempo_anterior) * e_w_chegada.no_eventos;

    e_n.soma_areas += 
    (tempo_decorrido - e_n.tempo_anterior) * e_n.no_eventos;


    e_w_saida.soma_areas += (tempo_decorrido - e_w_saida.tempo_anterior)* e_w_saida.no_eventos;

    double e_n_final = e_n.soma_areas / tempo_decorrido;

    double e_w_final = 
    (e_w_chegada.soma_areas - e_w_saida.soma_areas) / e_w_chegada.no_eventos;

    double lambda = e_w_chegada.no_eventos / tempo_decorrido;


    return 0;
}