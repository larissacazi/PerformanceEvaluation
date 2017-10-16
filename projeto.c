/* ----------------------------------------------------------------------- 
C�digo gerado com o ASDA - Ambiente de Simula��o Distribu�da Autom�tico
--------------------------------------------------------------------------*/
#include <stdio.h>
#include "smplx.h"
#include "randpar.h"
#include <stdlib.h>

#define n0 6         /* number of class 0 tasks  */
#define n1 3         /* number of class 1 tasks  */
#define nt n0+n1     /* total number of tasks */
#define nd 4         /* number of disk units  */
#define qd 1         /* queued req. return */

#define ESCALONADOR 	1
#define CPU0			2	
#define CPU1			3	
#define CPU2			4
#define CPU3			5
#define DISCO0			6	
#define DISCO1			7	
#define DISCO2			8
#define DISCO3			9		


typedef enum {
	FALSE, 
	TRUE
} BOOL;

struct client {
    int type;			/* Task type: static[0] or dynamic[1] */
    BOOL diskAccess;	/* If there is at least one disk access [1]	*/
    int server;			/* Current server [0, 1, 2, 3]  */
    float tsStart;  	/* Start time stamp     */
    float tsEnd;		/* End time stamp     */
} task[nt+1];

// float Te = 1000;  // para parar a simulação por tempo
int nts = 1000;	// number of tours to simulate

int typeCount = 0;
int timeCount = 0;
  
float
	tc[2]={5.0, 10.0},  /* class 0,1 mean cpu times: dynamic takes 2 times longer than static to process. */
	td=30.0, sd=7.5;   /* disk time mean, std. dev. */

FILE *arq, *saida;

void initializeClients() {
	int i = 0;

	for(i=0; i<nt+1; i++) {
		task[i].type = 0;
		task[i].diskAccess = FALSE;
		task[i].server = (nt + 1) % 4;
		task[i].tsStart = 0;
		task[i].tsEnd = 0;
	}
}

double timefromfile(FILE* arq){
	
	double time;

	fseek(arq, timeCount*(sizeof(double)+sizeof(int)), SEEK_SET);

	fread(&time, sizeof(double), 1, arq);

	timeCount++;

	return time;

}

int typefromfile(FILE* arq){
	
	int type;

	fseek(arq, (sizeof(double)+typeCount*(sizeof(double)+sizeof(int))), SEEK_SET);

	fread(&type, sizeof(int), 1, arq);

	typeCount++;

	return type;

}

void main(int argc, char** argv) {
	/* definicoes */
	float Te = 10000;
	int Event = 1, Aleatorio;
	float Ta0 = 5, Ts0 = 4, Ts1 = 5, Ts2 = 6, Ts3 = 7, Ts4 = 5, Ts5 = 4, Ts6 = 5, Ts7 = 4, Ts8 = 5;
	int i = 0, j = 0, seed, opt;
	int nextTask = randompar(0, 3);

	struct client *pTask;

	FILE *p, *saida, *entr;

	if(argc != 3){
		printf("Incorrect number of arguments. Exiting.\n");
		exit(EXIT_FAILURE);
	}

	//The seed to generate a random number
	seed = atoi(argv[1]);
	//Choose if it is taken from log or from some distribution
	opt = atoi(argv[2]);

	if(opt < 0 || opt > 1){
		printf("Incorrect option for values source. Exiting.\n");
		exit(EXIT_FAILURE);
	}

	saida = fopen("Projeto.out","w");

	if((p = sendto(saida)) == NULL)
		printf("Erro na saida \n");

	if(opt == 1)
		entr = fopen("log.db", "rb+");

	 //Prepare the system for simulation and name it
	 smpl(0," Projeto");


	//Create and name the facilities
	facility("Escalonador", ESCALONADOR);
	facility("CPU0", CPU0);
	facility("Disco0", DISCO0);
	facility("CPU1", CPU1);
	facility("Disco1", DISCO1);
	facility("CPU2", CPU2);
	facility("Disco2", DISCO2);
	facility("CPU3", CPU3);
	facility("Disco3", DISCO3);

	initializeClients();

	//--------------------------------------------------------
 	/* Escalona a chegada do primeiro cliente */
   	stream(seed); //Colocar como parâmetro esse valor 10.

   	schedule(1, 0.0, i);

	while((time() < Te)) {

	    cause(&Event,&i);

	    pTask = &task[i];

	    switch(Event) {
	        case 1 : //
	          schedule(2, 0.0, i);
	          pTask->tsStart = time();
	          schedule(3, expntl(Ta0), nextTask);
	          break;

	        /*  centro de serviço = Escalonador */
	        case 2 : 
	        	printf("Case 2:: Escalonador::Entrada.\n");
	        	j = pTask->type;
	        	if(request("Escalonador", Event, i, 0) == 0) {
	            	float release_CPU = expntl(tc[j]);

	             	if(opt == 1) schedule(3, timefromfile(entr), i);
					else schedule(3, release_CPU, i);

	             	printf("Case 2:: Escalonador::Schedule.\n");
	          	}
	          	break;

	        case 3 : 
	        	printf("Case 3:: Escalonador::Release.\n");
				release("Escalonador", i);

				Aleatorio = randompar(1, 10000);
				if(( 1 <= Aleatorio) && ( Aleatorio <= 2500) )
				schedule(4, 0.0, i);
				if (( 2501 <= Aleatorio) && ( Aleatorio <= 5000) )
				schedule(8, 0.0, i);
				if (( 5001 <= Aleatorio) && ( Aleatorio <= 7500) )
				schedule(12, 0.0, i);
				if (( 7501 <= Aleatorio) && ( Aleatorio <= 10000) )
				schedule(16, 0.0, i);
				break;

	        /*  centro de serviço = CPU0 */
	        case 4 :
	        	printf("Case 4:: CPU0::Entrada.\n");
	        	if(pTask->diskAccess == FALSE) {
		        	if(request("CPU0", Event, i, 0) == 0) {
		        		float release_CPU = expntl(tc[pTask->type]);

		        		if(opt == 1) schedule(5, timefromfile(entr), i);
						else schedule(5, release_CPU, i);

		            	pTask->server = 0;
		            	printf("Case 4:: CPU0::Schedule.\n");
		        	}
		        }
		        else {
		        	pTask->tsEnd = time();
		        }
	          	break;
	        case 5 : 
	        	printf("Case 5:: CPU0::Release.\n");
				release("CPU0", i);
				Aleatorio = randompar(1,10000);
				if (( 1 <= Aleatorio) && ( Aleatorio <= 5000) )
				schedule(6, 0.0, i);
				break;


	        /*  centro de serviço = CPU1 */
	        case 8 :
	        	printf("Case 8:: CPU1::Entrada.\n");
	        	if(pTask->diskAccess == FALSE) {
		        	if(request("CPU1", Event, i, 0) == 0) {
		        		if(opt == 1) schedule(9, timefromfile(entr), i);
		            	else schedule(9, expntl(Ts3), i);
		            	pTask->server = 1;
		            	printf("Case 8:: CPU1::Schedule.\n");
		        	}
		        }
		        else {
		        	pTask->tsEnd = time();
		        }
	          	break;
	        case 9 : 
	        	printf("Case 9:: CPU1::Release.\n");
	        	release("CPU1", i);
	        	Aleatorio = randompar(1, 10000);
	          	if (( 1 <= Aleatorio) && ( Aleatorio <= 5000) )
	           		schedule(10, 0.0, i);
	          	break;


	        /*  centro de serviço = CPU2 */
	        case 12 : 
	        	printf("Case 12:: CPU2::Entrada.\n");
	        	if(pTask->diskAccess == FALSE) {
		        	if(request("CPU2", Event, i, 0) == 0) {

		            	if(opt == 1) schedule(13, timefromfile(entr), i);
		            	else schedule(13, expntl(Ts5), i);

		            	pTask->server = 2;
		            	printf("Case 12:: CPU2::Schedule.\n");
		        	}
		        }
		        else {
		        	pTask->tsEnd = time();
		        }
	          	break;
	        case 13 :
	        	printf("Case 13:: CPU2::Release.\n");
	        	release("CPU2", i);
	        	Aleatorio = randompar(1, 10000);
	          	if (( 1 <= Aleatorio) && ( Aleatorio <= 5000) )
	           		schedule(14, 0.0, i);
	          	break;


	        /*  centro de serviço = CPU3 */
	        case 16 : 
	        	printf("Case 16: CPU3::Entrada.\n");
	        	if(pTask->diskAccess == FALSE) {
		        	if(request("CPU3", Event, i, 0) == 0) {
		            	if(opt == 1) schedule(17, timefromfile(entr), i);
		            	else schedule(17, expntl(Ts7), i);
		            	pTask->server = 3;
		            	printf("Case 16:: CPU3::Release.\n");
		        	}
		        }
		        else {
		        	pTask->tsEnd = time();
		        }
	          	break;
	        case 17 :
	        	printf("Case 17:: CPU3::Release.\n");
	        	release("CPU3", i);
	        	Aleatorio = randompar(1, 10000);
	          	if (( 1 <= Aleatorio) && ( Aleatorio <= 5000) )
	           		schedule(18, 0.0, i);
	          	break;


	        /*  centro de serviço = Disco0 */
	        case 6 : 
	        	printf("Case 6:: Disco0::Entrada.\n");
				if(request("Disco0", Event, i, 0) == 0) {
				 if(opt == 1) schedule(7, timefromfile(entr), i);
				 else schedule(7, expntl(Ts2), i);
				 pTask->diskAccess = TRUE;
				}
				break;
	        case 7 : 
	        	printf("Case 7:: Disco0::Release.\n");
				release("Disco0", i);
				schedule(4, 0.0, i);
				break;

	        /*  centro de serviço = Disco1 */
	        case 10 : 
	        	printf("Case 10:: Disco1::Entrada.\n");
	        	if(request("Disco1", Event, i, 0) == 0) {
	            	if(opt == 1) schedule(11, timefromfile(entr), i);
	            	else schedule(11, expntl(Ts4), i);
	            	pTask->diskAccess = TRUE;
	          	}
	          	break;
	        case 11 : 
	        	printf("Case 11:: Disco1::Release.\n");
				release("Disco1", i);
				schedule(8, 0.0, i);
				break;

	        /*  centro de serviço = Disco2 */
	        case 14 :
	        	printf("Case 14:: Disco2::Entrada.\n");
	        	if(request("Disco2", Event, i, 0) == 0) {
	            	if(opt == 1) schedule(15, timefromfile(entr), i);
	            	else schedule(15, expntl(Ts6), i);
	            	pTask->diskAccess = TRUE;
	          	}
	          	break;
	        case 15 : 
	        	printf("Case 15:: Disco2::Release.\n");
				release("Disco2", i);
				schedule(12, 0.0, i);
				break;

	        /*  centro de serviço = Disco3 */
	        case 18 : 
	        	printf("Case 18:: Disco3::Entrada.\n");
	        	if(request("Disco3", Event, i, 0) == 0) {
	            	if(opt == 1) schedule(19, timefromfile(entr), i);
	            	else schedule(19, expntl(Ts8), i);
	            	pTask->diskAccess = TRUE;
	          	}
	          	break;
	        case 19 : 
	        	printf("Case 19:: Disco3::Release.\n");
				release("Disco3", i);
				schedule(16, 0.0, i);
				break;
		}
	}


	/* gera o relatorio da simulacao */
	fprintf(saida,"TempoSimulado: %f\n", time() );

	fprintf(saida,"Utilizacao (\"Escalonador\") = %g\n", utilizacao_recurso("Escalonador"));
	fprintf(saida,"Comprimento medio fila (\"Escalonador\") = %g\n", comprimento_medio_fila("Escalonador"));
	fprintf(saida,"Periodo medio ocupado (\"Escalonador\") = %g\n", periodo_medio_ocupado("Escalonador"));
	fprintf(saida,"Utilizacao (\"CPU0\") = %g\n", utilizacao_recurso("CPU0"));
	fprintf(saida,"Comprimento medio fila (\"CPU0\") = %g\n", comprimento_medio_fila("CPU0"));
	fprintf(saida,"Periodo medio ocupado (\"CPU0\") = %g\n", periodo_medio_ocupado("CPU0"));
	fprintf(saida,"Utilizacao (\"Disco0\") = %g\n", utilizacao_recurso("Disco0"));
	fprintf(saida,"Comprimento medio fila (\"Disco0\") = %g\n", comprimento_medio_fila("Disco0"));
	fprintf(saida,"Periodo medio ocupado (\"Disco0\") = %g\n", periodo_medio_ocupado("Disco0"));
	fprintf(saida,"Utilizacao (\"CPU1\") = %g\n", utilizacao_recurso("CPU1"));
	fprintf(saida,"Comprimento medio fila (\"CPU1\") = %g\n", comprimento_medio_fila("CPU1"));
	fprintf(saida,"Periodo medio ocupado (\"CPU1\") = %g\n", periodo_medio_ocupado("CPU1"));
	fprintf(saida,"Utilizacao (\"Disco1\") = %g\n", utilizacao_recurso("Disco1"));
	fprintf(saida,"Comprimento medio fila (\"Disco1\") = %g\n", comprimento_medio_fila("Disco1"));
	fprintf(saida,"Periodo medio ocupado (\"Disco1\") = %g\n", periodo_medio_ocupado("Disco1"));
	fprintf(saida,"Utilizacao (\"CPU2\") = %g\n", utilizacao_recurso("CPU2"));
	fprintf(saida,"Comprimento medio fila (\"CPU2\") = %g\n", comprimento_medio_fila("CPU2"));
	fprintf(saida,"Periodo medio ocupado (\"CPU2\") = %g\n", periodo_medio_ocupado("CPU2"));
	fprintf(saida,"Utilizacao (\"Disco2\") = %g\n", utilizacao_recurso("Disco2"));
	fprintf(saida,"Comprimento medio fila (\"Disco2\") = %g\n", comprimento_medio_fila("Disco2"));
	fprintf(saida,"Periodo medio ocupado (\"Disco2\") = %g\n", periodo_medio_ocupado("Disco2"));
	fprintf(saida,"Utilizacao (\"CPU3\") = %g\n", utilizacao_recurso("CPU3"));
	fprintf(saida,"Comprimento medio fila (\"CPU3\") = %g\n", comprimento_medio_fila("CPU3"));
	fprintf(saida,"Periodo medio ocupado (\"CPU3\") = %g\n", periodo_medio_ocupado("CPU3"));
	fprintf(saida,"Utilizacao (\"Disco3\") = %g\n", utilizacao_recurso("Disco3"));
	fprintf(saida,"Comprimento medio fila (\"Disco3\") = %g\n", comprimento_medio_fila("Disco3"));
	fprintf(saida,"Periodo medio ocupado (\"Disco3\") = %g\n", periodo_medio_ocupado("Disco3"));

	fclose(saida);
}
/* ----------------------------------------------------------------------- */
