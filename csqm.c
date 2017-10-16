/**********************************************************************/
/*                                                                    */
/*         File csqm.c:  Central Server Queueing Network Model        */
/*                                                                    */
/*  This is the queueing network simulation program of Figure 2.2     */
/*                                                                    */
/**********************************************************************/
/*  KJC (10/10/95) - Changed <> to "" on include statement            */
/**********************************************************************/
#define _GNU_SOURCE	// para tirar o warning da função asprintf

#include <stdio.h>
#include "smpl.h"
#include "randpar.h"

#define n0 6         /* no. class 0 tasks  */
#define n1 3         /* no. class 1 tasks  */
#define nt n0+n1     /* total no. of tasks */
#define nd 4         /* no. of disk units  */
#define qd 1         /* queued req. return */

struct token
  {
    int cls;         /* task's class (& priority) */
    int un;          /* disk for current IO req.  */
    float ts;         /* tour start time stamp     */
  } task[nt+1];

// float Te = 1000;  // para parar a simulação por tempo
int nts = 1000;	// number of tours to simulate
  
float
  tc[2]={10.0,5.0},  /* class 0,1 mean cpu times  */
  td=30.0, sd=7.5;   /* disk time mean, std. dev. */

FILE *arq, *saida;
  
void main(void)
  {

	int i,j,Event = 1,n[2]; 
	float t,s[2]; 
	struct token *p;
	
	char *disk_number;

    n[0]=n[1]=0; 
	s[0]=s[1]=0.0;

	saida = fopen("csqm.out","w"); 
  
	if ((arq = sendto(saida)) == NULL)
		printf("Erro na saida \n");

    for (i=1; i<=nt; i++) task[i].cls=(i>n0)? 1:0;

    smpl(0,"central server model");

    facility("CPU",1);

    for (i=1; i<=nd; i++) 
	{
	   asprintf(&disk_number,"disk%d",i);
	   facility(disk_number,1);
	}

    for (i=1; i<=nt; i++) 
	schedule(1,0.0,i);
	
    while (nts > 0)
    //while (time() < Te)		// Para simular parando por tempo
      {
        cause(&Event,&i); 
	//printf("Event: %d, i: %d\n", Event, i);
	p=&task[i];			/* Retorna a estrutura referente a tarefa (ou ao cliente???) i */
        switch(Event)
          {
            case 1:  /* begin tour */
              p->ts=time(); 
	      schedule(2,0.0,i);
              break;
            case 2:  /* request cpu */
              j=p->cls;
              if (request("CPU",2,i,0)==0) then
	      {
		float release_CPU = expntl(tc[j]);
                schedule(3,release_CPU,i);
		//printf("CPU schedule to release at %f, token: %d\n", release_CPU,i);
	      }
              break;
            case 3:  /* release cpu, select disk */
              release("CPU",i); 
	      p->un=randompar(1,nd);
	      //printf("CPU released. This request goes to disk number: %d\n",p->un);
              schedule(4,0.0,i);
              break;
            case 4:  /* request disk */
	      asprintf(&disk_number,"disk%d",p->un);
	      if (request(disk_number,4,i,0)==0) then
                  schedule(5,erlang(td,sd),i);
              break;
            case 5:  /* release disk, end tour */
	      asprintf(&disk_number,"disk%d",p->un);
              release(disk_number,i); 
	      j=p->cls;
              t=time(); 
	      s[j]+=t-p->ts; 
	      p->ts=t; 
	      n[j]++;
              schedule(1,0.0,i);
	      nts--;		// para decrementar o número de tours
              break;
          }
      }
    //reportf(); printf("\n\n");

    fprintf(saida,"class 0 tour time = %.2f\n",s[0]/n[0]);
    fprintf(saida,"class 1 tour time = %.2f\n",s[1]/n[1]);


  fprintf(saida,"TempoSimulado: %f\n", time() );

  fprintf(saida,"Utilizacao (\"CPU\") = %g\n", utilizacao_recurso("CPU"));
  fprintf(saida,"Comprimento medio fila (\"CPU\") = %g\n", comprimento_medio_fila("CPU"));
  fprintf(saida,"Periodo medio ocupado (\"CPU\") = %g\n", periodo_medio_ocupado("CPU"));

    for (i=1; i<=nd; i++) 
	{
	   asprintf(&disk_number,"disk%d",i);
	   fprintf(saida,"Utilizacao (\"Disk %d\") = %g\n", i, utilizacao_recurso(disk_number));
	   fprintf(saida,"Comprimento medio fila (\"Disk %d\") = %g\n", i, comprimento_medio_fila(disk_number));
	   fprintf(saida,"Periodo medio ocupado (\"Disk %d\") = %g\n", i, periodo_medio_ocupado(disk_number));
	}

   fclose(saida);

  }
