/* ----------------------------------------------------------------------- 
C�digo gerado com o ASDA - Ambiente de Simula��o Distribu�da Autom�tico
--------------------------------------------------------------------------*/
#include <stdio.h>
#include "smplx.h"
#include "randpar.h"
#include <stdlib.h>

#define MAX_REQUESTS 8     /* total number of tasks */

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
} task[MAX_REQUESTS+10];

// float Te = 1000;  // para parar a simulação por tempo
int nts = 1000;	// number of tours to simulate

int typeCount = 0;
int timeCount = 0;
  
float
	tc[2]={10.0, 15.0},  /* class 0,1 mean cpu times: dynamic takes 2 times longer than static to process. */
	td=30.0, sd=7.5;   /* disk time mean, std. dev. */
float tDisk[2] = {20.0, 30.0};

FILE *arq, *saida;

void initializeClients() {
	int i = 0;

	for(i=0; i<MAX_REQUESTS+10; i++) {
		task[i].type = 0;
		task[i].diskAccess = FALSE;
		task[i].server = 0;
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
	int i = 0, j = 0, seed = 0, opt = 0;
	int nextTask = 0;
	int completedTasks = 0;

	struct client *pTask;

	FILE *p, *saida, *entr;

	if(argc != 3){
		//printf("Incorrect number of arguments. Exiting.\n");
		exit(EXIT_FAILURE);
	}

	//The seed to generate a random number
	seed = atoi(argv[1]);
	//Choose if it is taken from log or from some distribution
	opt = atoi(argv[2]);

	if(opt < 0 || opt > 1){
		//printf("Incorrect option for values source. Exiting.\n");
		exit(EXIT_FAILURE);
	}

	seed = 1;

	while(seed <= 1) {

		char outputFileName[60];
		sprintf(outputFileName, "Projeto%d.out", seed);
		saida = fopen(outputFileName,"w");

		if((p = sendto(saida)) == NULL) //printf("Erro na saida \n");

		if(opt == 1) entr = fopen("log.db", "rb+");

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

	   	completedTasks = 0;

		//while((time() < Te)) {
	   	while(completedTasks < MAX_REQUESTS - 1) {

	   		////printf("\n========= i %d completedTasks %d\n", i, completedTasks);

		    cause(&Event, &i);
		    ////printf("Event %d i %d\n", Event, i);

		    pTask = &task[i];

		    switch(Event) {
		        case 1 : 
		          schedule(2, 0.0, i);
		          pTask->tsStart = time(); 
		          nextTask++; 
		          ////printf("i %d nextTask %d\n", i, nextTask);
		          if(nextTask < MAX_REQUESTS) {
		          	schedule(1, expntl(Ta0), nextTask);
		          }
		        break;

		        /*  centro de serviço = Escalonador */
		        case 2 : 
		        	////printf("Case 2:: Escalonador::Entrada.\n");
		        	j = pTask->type;
		        	////printf("j %d i %d\n", j, i);
		        	if(request("Escalonador", Event, i, 0) == 0) {
		            	float releaseScheduler = expntl(tc[j]);
		            	////printf("i %d\n", i);

		             	if(opt == 1) schedule(3, timefromfile(entr), i);
						else schedule(3, releaseScheduler, i);

		             	////printf("Case 2:: Escalonador::Schedule. i %d\n", i);
		          	}
		          	////printf("SAindo case2\n");
		        break;

		        case 3 : 
		        	////printf("Case 3:: Escalonador::Release. i %d\n", i);
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
		        	////printf("Case 4:: CPU0::Entrada.\n");
		        	if(request("CPU0", Event, i, 0) == 0) {
		        		////printf("request CPU0\n");
		        		float releaseCPU = expntl(tc[pTask->type]);

		        		if(opt == 1) schedule(5, timefromfile(entr), i);
						else schedule(5, releaseCPU, i);

		            	pTask->server = 0;
		            	////printf("Case 4:: CPU0::Schedule.\n");
		        	}
		        	////printf("Case 4:: CPU0::Saida. Seed %d\n", seed);
		        break;
		        case 5 :
		        	////printf("Case 5: CPU0::Release.\n"); 
		        	if(pTask->diskAccess == FALSE) {
		        		////printf("Case 5::Call Disco0. Disco0 was not accessed yet.\n");
		        		schedule(6, 0.0, i);
		        	}
		        	else {
		        		////printf("Case 5:: CPU0::Release.\n");
						release("CPU0", i);
						pTask->tsEnd = time();
			        	completedTasks++;
			        	////printf("completedTasks!! i [%d] completedTasks [%d]\n", i, completedTasks);
		        	}
					
				break;


		        /*  centro de serviço = CPU1 */
		        case 8 :
		        	////printf("Case 8:: CPU1::Entrada.\n");
		        	if(request("CPU1", Event, i, 0) == 0) {
		        		float releaseCPU = expntl(tc[pTask->type]);

		        		if(opt == 1) schedule(9, timefromfile(entr), i);
						else schedule(9, releaseCPU, i);

		            	pTask->server = 1;
		            	////printf("Case 8:: CPU1::Schedule.\n");
		        	}
		        break;
		        case 9 : 
		        	////printf("Case 9: CPU1::Release.\n");
		        	if(pTask->diskAccess == FALSE) {
		        		schedule(10, 0.0, i);
		        	}
		        	else {
		        		////printf("Case 9:: CPU1::Release.\n");
		        		release("CPU1", i);
		        		pTask->tsEnd = time();
			        	completedTasks++;
			        	////printf("completedTasks!! i [%d] completedTasks [%d]\n", i, completedTasks);
		        	}		           		
		        break;


		        /*  centro de serviço = CPU2 */
		        case 12 : 
		        	////printf("Case 12:: CPU2::Entrada.\n");
		        	if(request("CPU2", Event, i, 0) == 0) {
		        		float releaseCPU = expntl(tc[pTask->type]);

		        		if(opt == 1) schedule(13, timefromfile(entr), i);
						else schedule(13, releaseCPU, i);

		            	pTask->server = 2;
		            	////printf("Case 12:: CPU2::Schedule.\n");
		        	}
		        break;
		        case 13 :
		        	////printf("Case 13: CPU2::Release.\n");
		        	if(pTask->diskAccess == FALSE) {
		        		schedule(14, 0.0, i);
		        	}
		        	else {
			        	////printf("Case 13:: CPU2::Release. i %d\n", i);
			        	release("CPU2", i);
			        	pTask->tsEnd = time();
			        	completedTasks++;
			        	////printf("completedTasks!! i [%d] completedTasks [%d]\n", i, completedTasks);
			        }		           	
		        break;


		        /*  centro de serviço = CPU3 */
		        case 16 : 
		        	////printf("Case 16: CPU3::Entrada.\n");
		        	if(request("CPU3", Event, i, 0) == 0) {
		        		float releaseCPU = expntl(tc[pTask->type]);

		        		if(opt == 1) schedule(17, timefromfile(entr), i);
						else schedule(17, releaseCPU, i);

		            	pTask->server = 3;
		            	////printf("Case 16:: CPU3::Release.\n");
		        	}
		        	////printf("Saida case 16\n");
		        break;
		        case 17 :
		        	////printf("Case 17: CPU3::Release.\n");
		        	if(pTask->diskAccess == FALSE) {
		        		schedule(18, 0.0, i);
		        	}
		        	else {
		        		////printf("Case 17:: CPU3::Release.\n");
		        		release("CPU3", i);
		        		pTask->tsEnd = time();
			        	completedTasks++;
			        	////printf("completedTasks!! i [%d] completedTasks [%d]\n", i, completedTasks);
		        	}
		        break;


		        /*  centro de serviço = Disco0 */
		        case 6 : 
					////printf("Case 6:: Disco0::Entrada.\n");
					if(request("Disco0", Event, i, 0) == 0) {
						float releaseDisk = expntl(tDisk[pTask->type]);

		        		if(opt == 1) schedule(7, timefromfile(entr), i);
						else schedule(7, releaseDisk, i);

						pTask->diskAccess = TRUE;
					}
				break;
		        case 7 : 
		        	////printf("Case 7:: Disco0::Release.\n");
					release("Disco0", i);
					schedule(4, 0.0, i);
				break;

		        /*  centro de serviço = Disco1 */
		        case 10 : 
		        	//printf("Case 10:: Disco1::Entrada.\n");
		        	if(request("Disco1", Event, i, 0) == 0) {
		        		float releaseDisk = expntl(tDisk[pTask->type]);

		        		if(opt == 1) schedule(11, timefromfile(entr), i);
						else schedule(11, releaseDisk, i);

		            	pTask->diskAccess = TRUE;
		          	}
		        break;
		        case 11 : 
		        	//printf("Case 11:: Disco1::Release.\n");
					release("Disco1", i);
					schedule(8, 0.0, i);
				break;

		        /*  centro de serviço = Disco2 */
		        case 14 :
		        	//printf("Case 14:: Disco2::Entrada.\n");
		        	if(request("Disco2", Event, i, 0) == 0) {
		        		float releaseDisk = expntl(tDisk[pTask->type]);

		        		if(opt == 1) schedule(15, timefromfile(entr), i);
						else schedule(15, releaseDisk, i);
	 
		            	pTask->diskAccess = TRUE;
		          	}
		        break;
		        case 15 : 
		        	//printf("Case 15:: Disco2::Release.\n");
					release("Disco2", i);
					schedule(12, 0.0, i);
				break;

		        /*  centro de serviço = Disco3 */
		        case 18 : 
		        	//printf("Case 18:: Disco3::Entrada.\n");
		        	if(request("Disco3", Event, i, 0) == 0) {
		        		float releaseDisk = expntl(tDisk[pTask->type]);

		        		if(opt == 1) schedule(19, timefromfile(entr), i);
						else schedule(19, releaseDisk, i);
	 
		            	pTask->diskAccess = TRUE;
		          	}
		        break;
		        case 19 : 
		        	//printf("Case 19:: Disco3::Release.\n");
					release("Disco3", i);
					schedule(16, 0.0, i);
				break;
			}
		}


			/* gera o relatorio da simulacao */
		fprintf(saida,"TempoSimulado: %f\n", time());

		fprintf(saida,"Utilizacao (\"Escalonador\") = %g\n", utilizacao_recurso("Escalonador"));
		fprintf(saida,"\nUtilizacao (\"CPU0\") = %g\n", utilizacao_recurso("CPU0"));
		fprintf(saida,"Utilizacao (\"Disco0\") = %g\n", utilizacao_recurso("Disco0"));
		fprintf(saida,"\nUtilizacao (\"CPU1\") = %g\n", utilizacao_recurso("CPU1"));
		fprintf(saida,"Utilizacao (\"Disco1\") = %g\n", utilizacao_recurso("Disco1"));
		fprintf(saida,"\nUtilizacao (\"CPU2\") = %g\n", utilizacao_recurso("CPU2"));
		fprintf(saida,"Utilizacao (\"Disco2\") = %g\n", utilizacao_recurso("Disco2"));
		fprintf(saida,"\nUtilizacao (\"CPU3\") = %g\n", utilizacao_recurso("CPU3"));
		fprintf(saida,"Utilizacao (\"Disco3\") = %g\n", utilizacao_recurso("Disco3"));

		fclose(saida);

		seed++;
	}
}
