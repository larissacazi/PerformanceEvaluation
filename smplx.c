#include <stdio.h> 
#include <math.h> 
#include <stdlib.h>     /* rotinas de memoria e geracao de numeros aleatorios */
#include <string.h>
#include "smplx.h"
#include "declaracoes.h" /* estruturas e var*/
#include "randpar.h"



/* -------------- smplx_exit
----------------------------------------------------------------- */

void smplx_exit()
{
 /* libera ponteiros */
 free(lista_eventos_futuros);
 free(cabecalho_descritor);
 exit(0);
}
/* ------------------------------------------------------------------------------------------------- */




/* -------------- erro -----------------------------------------------------------------------------
 
   Entrada: char *mensagem
   Funcao:  imprime mensagem de erro
   Saida:  
   
   ---------------------------------------------------------------------------------------------------------- */   
    
void erro(char *mensagem)
{
 printf("\n %s \n",mensagem);
 smplx_exit();
}

/* ---------------------------------------------------------------------------------------------------------- */




/* -------------------- rotinas para analises estatisticas ----------------------------------------- 
   ---------------------------------------------------------------------------------------------------------- */




/* -------------- utilizacao_recurso ---------------------------------------------------------------  
 
   Entrada: nome_recurso
   Funcao: Determina a utilizacao do recurso pelo cliente
   Saida:  double utilizacao do recurso
           (rotina U do MacDougall)
           
   ---------------------------------------------------------------------------------------------------------- */   


double utilizacao_recurso(char *nome_recurso)
{
 double somatorio_periodo_ocupado = 0.0;
 tipo_servidor_recursos *ponteiro_serv_rec;
 tipo_cabecalho *ponteiro_cab;
 
 ponteiro_cab = cabecalho_descritor;
 
 while ( (ponteiro_cab != NULL) && 
         (strcmp(ponteiro_cab->pont_descritor->nome,nome_recurso)) )
   ponteiro_cab = ponteiro_cab->prox_cabecalho;
   
   
  if (ponteiro_cab == NULL)
    erro("Rotina: request() - Erro: recurso nao identificado");

 
 /* ponteiro_cab aponta para o o descritor do recurso */
 ponteiro_serv_rec = ponteiro_cab->pont_descritor->fila_servidor_recursos;
 
 while (ponteiro_serv_rec != NULL)
  {
   somatorio_periodo_ocupado += ponteiro_serv_rec->soma_periodo_ocupado;
   ponteiro_serv_rec = ponteiro_serv_rec->prox_servidor_recursos;
  }
  
  return((somatorio_periodo_ocupado)/(clock_simulacao-start));
}
/* ---------------------------------------------------------------------------------------------------------- */




/* -------------- comprimento_fila -----------------------------------------------------------------  
 
   Entrada:
   Funcao: Determina o comprimento atual da fila do recurso 
   Saida:  int comprimento fila
           (rotina Inq do MacDougall)
           
   ---------------------------------------------------------------------------------------------------------- */ 


int comprimento_fila(char *nome_recurso)
{
 tipo_cabecalho *ponteiro_cab;

 ponteiro_cab = cabecalho_descritor;
 
 /* procurar pelo recurso identificado por nome_recurso */

 while ( (ponteiro_cab != NULL) && 
         (strcmp(ponteiro_cab->pont_descritor->nome,nome_recurso)) )
   ponteiro_cab = ponteiro_cab->prox_cabecalho;
   
   
  if (ponteiro_cab == NULL)
    erro("Rotina: request() - Erro: recurso nao identificado");

 
 return(ponteiro_cab->pont_descritor->comprimento_fila);  
}

/* ---------------------------------------------------------------------------------------------------------- */




/* -------------- periodo_medio_ocupado ------------------------------------------------------------  
  
   Entrada:
   Funcao: Determina o periodo medio de ocupacao do recurso 
   Saida:  double periodo medio ocupado
           (rotina B do MacDougall)
           
   ---------------------------------------------------------------------------------------------------------- */ 


double periodo_medio_ocupado(char *nome_recurso)
{
 double somatorio = 0.0;
 int n = 0;
 tipo_cabecalho *ponteiro_cab;
 tipo_servidor_recursos *ponteiro_serv_rec;

 ponteiro_cab = cabecalho_descritor;
 
 /* procurar pelo recurso identificado por nome_recurso */

 while ( (ponteiro_cab != NULL) && 
         (strcmp(ponteiro_cab->pont_descritor->nome,nome_recurso)) )
   ponteiro_cab = ponteiro_cab->prox_cabecalho;
   
   
  if (ponteiro_cab == NULL)
    erro("Rotina: request() - Erro: recurso nao identificado");

 
 ponteiro_serv_rec = ponteiro_cab->pont_descritor->fila_servidor_recursos;
 
 while (ponteiro_serv_rec != NULL)
 {
  somatorio += ponteiro_serv_rec->soma_periodo_ocupado;
  n += ponteiro_serv_rec->contador_release;
  ponteiro_serv_rec = ponteiro_serv_rec->prox_servidor_recursos;
 }    
   
 return( (n > 0) ? somatorio/n : somatorio);  
}

/* ---------------------------------------------------------------------------------------------------------- */




/* -------------- comprimento_medio_fila -----------------------------------------------------------  
  
   Entrada: nome_recuro
   Funcao: Determina o comprimento medio da fila do recurso 
   Saida:  double comprimento medio fila
           (rotina Lq do MacDougall )
           
   ---------------------------------------------------------------------------------------------------------- */ 


double comprimento_medio_fila(char *nome_recurso)
{
 double t = clock_simulacao - start; /* start eh variavel global */
 tipo_cabecalho *ponteiro_cab;
 tipo_servidor_recursos *ponteiro_serv_rec;

 ponteiro_cab = cabecalho_descritor;
 
 /* procurar pelo recurso identificado por nome_recurso */


 while ( (ponteiro_cab != NULL) && 
         (strcmp(ponteiro_cab->pont_descritor->nome,nome_recurso)) )
   ponteiro_cab = ponteiro_cab->prox_cabecalho;
   
   
  if (ponteiro_cab == NULL)
    erro("Rotina: request() - Erro: recurso nao identificado");

 
 return( (t>0.0) ? (ponteiro_cab->pont_descritor->lenght_time_product_sum/t) : 0.0);   

}

/* ---------------------------------------------------------------------------------------------------------- */






/* -------------- rotinas para alocacao de memoria ------------------------------------------------- 
   ---------------------------------------------------------------------------------------------------------- */
                                         



/* -------------- aloca_mem_cabecalho --------------------------------------------------------------  
 
   Entrada:
   Funcao: Aloca memoria para a estrutura do cabecalho.
   Saida:  Ponteiro para a memoria alocada ou null em caso de erro
      
   ---------------------------------------------------------------------------------------------------------- */   
                                                                  
                                                                    
tipo_cabecalho *aloca_mem_cabecalho()
{
 tipo_cabecalho *ponteiro;
 
 ponteiro = (tipo_cabecalho *)malloc(sizeof(tipo_cabecalho));
 return(ponteiro);
}
/* ---------------------------------------------------------------------------------------------------------- */




/* -------------- aloca_mem_descritor_recursos -----------------------------------------------------  
   
   Entrada:
   Funcao: Aloca memoria para a estrutura do recurso
   Saida:  Ponteiro para a memoria alocada ou null em caso de erro
   
   ---------------------------------------------------------------------------------------------------------- */                                                                      


tipo_descritor_recursos *aloca_mem_descritor_recursos()
{
 tipo_descritor_recursos *ponteiro;
 
 ponteiro = (tipo_descritor_recursos *)malloc(sizeof(tipo_descritor_recursos));
 return(ponteiro);
}
/*  --------------------------------------------------------------------------------------------------------- */




/* -------------- aloca_mem_fila_eventos -----------------------------------------------------------  
   Entrada:
   Funcao: Aloca memoria para a fila de eventos associada ao recurso
   Saida:  Ponteiro para a memoria alocada ou null em caso de erro
   
   ---------------------------------------------------------------------------------------------------------- */                                                                    


tipo_fila_eventos *aloca_mem_fila_eventos()
{
 tipo_fila_eventos *ponteiro;
 
 ponteiro = (tipo_fila_eventos *)malloc(sizeof(tipo_fila_eventos));
 return(ponteiro);
}
/* --------------------------------------------------------------------------------------------------------- */




/* -------------- aloca_mem_servidor_recursos ------------------------------------------------------  
   
   Entrada:
   Funcao: Aloca memoria para a estrutura do recurso
   Saida:  Ponteiro para a memoria alocada ou NULL em caso de erro.

   ---------------------------------------------------------------------------------------------------------- */   


tipo_servidor_recursos *aloca_mem_servidor_recursos()
{
 tipo_servidor_recursos *ponteiro;
 
 ponteiro = (tipo_servidor_recursos *)malloc(sizeof(tipo_servidor_recursos));
 return(ponteiro);
}
/* ---------------------------------------------------------------------------------------------------------- */




/* -------------- aloca_mem_lista_eventos_futuros --------------------------------------------------  
   
   Entrada:
   Funcao: Aloca memoria para a lista de eventos futuros do processo
   Saida:  Ponteiro para a memoria alocada ou NULL em caso de erro.
  
   ---------------------------------------------------------------------------------------------------------  */   

tipo_lista_eventos *aloca_mem_lista_eventos_futuros()
{
 tipo_lista_eventos *ponteiro;
 ponteiro = (tipo_lista_eventos *)malloc(sizeof(tipo_lista_eventos));
 return(ponteiro);
}
/* ---------------------------------------------------------------------------------------------------------- */



/* ------------- clock_sim -------------------------------------------------------------------------
   
   Entrada:
   Funcao: Retorna o clock da simulacao
   Saida: double clock da simulacao
 
   ------------------------------------------------------------------------------------------------- */   

double time()
{
 return(clock_simulacao);
}
   
/* ------------------------------------------------------------------------------------------------- */




/* ------------- facility --------------------------------------------------------------------------
   
   Entrada:
   Funcao: Alocar nos para o descritor do recurso na lista de recursos
   Saida:  
 
   ------------------------------------------------------------------------------------------------- */   


void facility(char *nome_recurso, int num_serv)
{
 int indice;                    /* para alocar total de nos = num_serv */
 tipo_descritor_recursos *ponteiro_rec;  /* ponteiro para o no recurso */
 tipo_cabecalho *auxiliar,               /* ponteiros para o cabecalho */
                *ponteiro_cab;
                 
 tipo_servidor_recursos *ponteiro_serv_rec,
                        *aux;
 
  
  ponteiro_cab = aloca_mem_cabecalho();
  if (ponteiro_cab == NULL)
   erro("Rotina: facility() - erro ao alocar memoria cabecalho");
  ponteiro_rec = aloca_mem_descritor_recursos();
  if (ponteiro_rec == NULL)
   erro("Rotina: facility erro ao alocar memoria descritor recurso");
  
   auxiliar = cabecalho_descritor;
   if (auxiliar == NULL)   /* nenhum recurso definido */
   {
    cabecalho_descritor = ponteiro_cab;
    auxiliar = ponteiro_cab;
    
   }
   else
    {
     while (auxiliar->prox_cabecalho != NULL) 
     {
       auxiliar = auxiliar->prox_cabecalho;
     }  
       
     auxiliar->prox_cabecalho = ponteiro_cab;  
    
     auxiliar = auxiliar->prox_cabecalho;
    
    }
     
     auxiliar->pont_descritor = ponteiro_rec;
     auxiliar->prox_cabecalho = NULL; /* a insercao e feita no final da lista */
     
     /* insere os dados no descritor do recurso */
     ponteiro_rec->nome = nome_recurso;
     ponteiro_rec->tempo_ultima_alteracao_fila = 0.0;
     ponteiro_rec->lenght_time_product_sum = 0.0;
     ponteiro_rec->contador_release_fila = 0;
     ponteiro_rec->contador_preempcao = 0;
     ponteiro_rec->comprimento_fila = 0;
     ponteiro_rec->num_servidores_ocupados = 0;
     ponteiro_rec->num_servidores = num_serv;
     ponteiro_rec->lista_eventos_recurso = NULL;
     
     /* para cada servidor e criado um no na lista de servidores do recurso */
     for (indice = 1; indice <= num_serv; indice++)
     {
      aux = aloca_mem_servidor_recursos();
      if (aux == NULL)
        erro("Rotina: facility - Erro ao alocar memoria para a lista de servidores");
      if (indice == 1)  /* primeiro no da lista */
      { 
       ponteiro_serv_rec = aux;
       ponteiro_rec->fila_servidor_recursos = aux;
      }
      else
      {
       ponteiro_serv_rec->prox_servidor_recursos = aux;
       ponteiro_serv_rec = aux;
      }
      ponteiro_serv_rec = aux;
      aux->inicio_periodo_ocupado = 0.0;
      aux->soma_periodo_ocupado = 0.0;
      aux->contador_release = 0;
      aux->prioridade_usuario = 0;
      aux->num_token = 0;
      aux->prox_servidor_recursos = NULL;  
     }
  /*printf("\n\n CPU - facility() - Estou na facility\n\n");
  printf("\n\n CPU - facility() - nome recurso %s\n\n",ponteiro_rec->nome);*/
} 

/* ------------------------------------------------------------------------------------------------- */




/* -------------- insere_lista_eventos_futuros -----------------------------------------------------
 
   Entrada: int num_evento     - numero do evento a ser escalonado
            double te           - tempo de ocorrencia entre eventos
            int   token        - numero do token ao qual o recurso esta
                                 associado

                  ident_proc   - identificacao do processo origem do evento
                                 (necessario para cause() atualizar buffer de canais)
                                 essa identificacao nao e o tid, e o indice da tabela de tids !
   Funcao: Inserir um novo elemento na lista de eventos futuros do processo
   Saida:  
   
---------------------------------------------------------------------------------------------------- */  


void insere_lista_eventos_futuros(int num_evento, double te, int token)
{

 tipo_lista_eventos *ponteiro_lista_eventos,
                    *ponteiro, /* percorre a lista de eventos futuros */
                    *auxiliar;
 
   
 ponteiro_lista_eventos = aloca_mem_lista_eventos_futuros();
 if (ponteiro_lista_eventos == NULL)
 {
  printf("clock da simulacao: %f",time());
  erro("\n\n CPU Rotina: insere_lista_eventos_futuros() - Erro: nao ha memoria para a lista de eventos futuros\n\n");;
 }
   
   /* insere os dados no no alocado */
  ponteiro_lista_eventos->tempo_ocorrencia = te + clock_simulacao; 
  
  ponteiro_lista_eventos->num_evento = num_evento;
  ponteiro_lista_eventos->num_token = token;
  /* falta inserir o ponteiro na lista - ordem ascedente do tempo de ocorrencia*/
  ponteiro = lista_eventos_futuros; /* cabeca da lista */


  if (ponteiro == NULL) /* lista vazia */
  {
   lista_eventos_futuros = ponteiro_lista_eventos;
   ponteiro_lista_eventos->prox_lista_eventos = NULL;
  }
  else
  { /* procura posicao de insercao na lista */
    auxiliar = ponteiro;    
    while ((ponteiro != NULL) && (ponteiro_lista_eventos->tempo_ocorrencia > ponteiro->tempo_ocorrencia))
    {
     auxiliar = ponteiro;
     ponteiro = ponteiro->prox_lista_eventos;
    }
    if (auxiliar == ponteiro) /* insercao feita na cabeca da lista */
    {
      lista_eventos_futuros = ponteiro_lista_eventos;
      ponteiro_lista_eventos->prox_lista_eventos = ponteiro;
    }
    else
    {
     auxiliar->prox_lista_eventos = ponteiro_lista_eventos;
     ponteiro_lista_eventos->prox_lista_eventos = ponteiro;  
    } 
  }
 //printf("\n CPU - Passei pela insere lista eventos futuros \n");
}

/* ------------------------------------------------------------------------------------------------- */

 


/* -------------- release ---------------------------------------------------------------------------
  
   Entrada: char *nome_recurso - nome do recurso a ser liberado
            int   token        - numero do token ao qual o recurso esta
                                 associado
   Funcao: Liberar o recurso associado ao token
           Verificar se a fila de eventos do recurso nao esta vazia. Em caso afirmativo, escalonar 
           o evento para ocorrencia imediata, colocando-o na cabeca da lista de eventos futuros;
           senao o servidor liberado e alocado para o evento retirado da fila de eventos, e o evento 
           escalonado para o tempo te+remaing
   Saida:  

    ------------------------------------------------------------------------------------------------- */   


void release(char *nome_recurso,int token)
{
 tipo_cabecalho *ponteiro_cab, /* ponteiros para percorrer o cabecalho */
                *auxiliar;
 tipo_servidor_recursos *ponteiro_serv_rec;
 tipo_fila_eventos *ponteiro_fila_eventos;
 tipo_descritor_recursos *ponteiro_rec;
 tipo_lista_eventos *ponteiro_lista_eventos;
 
 double te; /* para determinar o tempo de ocorrencia de um evento preempted */
 
 ponteiro_cab = cabecalho_descritor;


 /* procura pelo recurso identificado por nome_recurso */
 
 while ( (ponteiro_cab != NULL) && 
         (strcmp(ponteiro_cab->pont_descritor->nome,nome_recurso)) )
   ponteiro_cab = ponteiro_cab->prox_cabecalho;
                
  
 if (ponteiro_cab == NULL)
   erro("Rotina: release() - Erro: recurso nao encontrado para ser liberado");
   
   
 /*recurso encontrado - busca pelo servidor associado ao token 
   se os tokens sao iguais, libera o primeiro servidor que encontrar */                
 
 ponteiro_serv_rec = ponteiro_cab->pont_descritor->fila_servidor_recursos;
 
 while ( (ponteiro_serv_rec != NULL) && (ponteiro_serv_rec->num_token != token) ) 
   ponteiro_serv_rec = ponteiro_serv_rec->prox_servidor_recursos;
 
 if (ponteiro_serv_rec == NULL) /* nao existe servidor do recurso associado ao token */
   erro("Rotina: release() - Erro: nao existe servidor associado ao token");
   
 /* servidor encontrado - sera liberado - o no permanece, os dados sao apagados */
 
 /* zero para num_token indica que o servidor foi liberado */
 ponteiro_serv_rec->num_token = 0;    
 ponteiro_serv_rec->contador_release++; /* incrementa o contador de release */

 /* -------- confirmar essa operacao no macdougall, alterado em 15/08/96 --------- */
 ponteiro_serv_rec->soma_periodo_ocupado += (clock_simulacao - ponteiro_serv_rec->inicio_periodo_ocupado); 
 
 ponteiro_serv_rec->inicio_periodo_ocupado = 0.0;
 
 ponteiro_serv_rec->prioridade_usuario = 0;
 
 /* decrementa o numero de servidores ocupados do recurso */
 ponteiro_cab->pont_descritor->num_servidores_ocupados--;
 
 /* depois de liberar servidor, verifica a fila associada ao recurso */
 if (ponteiro_cab->pont_descritor->lista_eventos_recurso != NULL)
 {
  /* tira evento da cabeca da fila de eventos e verifica se bloqueado ou preempted:
    - se bloqueado vai para a cabeca da lista de eventos futuros */
 
 
   ponteiro_rec = ponteiro_cab->pont_descritor;
   
   /* ponteiro_fila_eventos aponta para o no a ser removido */
   ponteiro_fila_eventos = ponteiro_rec->lista_eventos_recurso;

   ponteiro_rec->lista_eventos_recurso = ponteiro_rec->lista_eventos_recurso->prox_fila_eventos;
   

   ponteiro_rec->lenght_time_product_sum += ponteiro_rec->comprimento_fila * 
                                            (clock_simulacao - ponteiro_rec->tempo_ultima_alteracao_fila);
                                           
   ponteiro_rec->comprimento_fila--;
   ponteiro_rec->contador_release_fila++;
   ponteiro_rec->tempo_ultima_alteracao_fila = clock_simulacao;
   
   te = ponteiro_fila_eventos->event_time; /* remaining */
   
   if (te == 0.0)
   /* requisicao tinha sido bloqueada, vai para a cabeca da lista de eventos futuros
      para ocorrencia imediata
      
      ponteiro_fila_eventos aponta para o no a ser liberado da fila de eventos do recurso */
   
   {
    ponteiro_fila_eventos->prioridade = clock_simulacao;
    
    /* atualiza campos da fila de eventos para a lista de eventos futuros do processo */
    ponteiro_lista_eventos = aloca_mem_lista_eventos_futuros();
    if (ponteiro_lista_eventos == NULL)
      erro("Rotina: release() - Erro: alocar memoria para a lista de eventos futuros");
    
    /*ponteiro_lista_eventos->ident_processo = proc_local;*/
    
    ponteiro_lista_eventos->tempo_ocorrencia = ponteiro_fila_eventos->prioridade;
    ponteiro_lista_eventos->num_evento = ponteiro_fila_eventos->num_evento;
    ponteiro_lista_eventos->num_token = ponteiro_fila_eventos->num_token;
    
    /* insere no na cabeca da lista de eventos futuros */
    ponteiro_lista_eventos->prox_lista_eventos = lista_eventos_futuros;
    lista_eventos_futuros = ponteiro_lista_eventos;
    free(ponteiro_fila_eventos);
   }                                            
   else
   
   /* se te > 0 evento foi preempted; entao deve-se alocar o servidor liberado para esse token 
 
      nesse caso reescalona o evento prar terminar de ocorrer com o tempo restante
      
      ponteiro_rec ponteiro para o recurso
      ponteiro_serv_rec ponteiro para o server liberado                
      ponteiro_fila_eventos  ponteiro para o evento retirado da fila de 
        eventos do recurso                                                   */
        
   {
    ponteiro_serv_rec->num_token = ponteiro_fila_eventos->num_token;
    ponteiro_serv_rec->prioridade_usuario = (int) ponteiro_fila_eventos->prioridade;
    /* na fila de eventos prioridade e double ; ver declaracoes.h             */
    
    ponteiro_serv_rec->inicio_periodo_ocupado = clock_simulacao;
    
    /* incrementa o numero de servidores ocupados do recurso */
    ponteiro_rec->num_servidores_ocupados += 1;
    
    /* aqui o trace pode ser gerado !!!!!!!!!!!!!!!!!!!!!!1 */

    insere_lista_eventos_futuros(ponteiro_fila_eventos->num_evento,
                                    clock_simulacao+te,
                                    ponteiro_fila_eventos->num_token);
       
    free(ponteiro_fila_eventos); /* libera o no da fila de eventos do recurso     */  
   }
 }
}

/* ------------------------------------------------------------------------------------------------- */



/* -------------- schedule -------------------------------------------------------------------------
   Entrada: int num_evento     - numero do evento a ser escalonado
            double te           - tempo de ocorrencia entre eventos
            int   token        - numero do token ao qual o recurso esta
                                 associado
            int ident_proc     - identificacao do processo onde o evento
                                 devera ser escalonado (proc_local para 
                                 processo local)                     
                                 identi_proc e o indice da tabela de tids
   Funcao: Escalonar o final do atendimento do cliente token no recurso
   Saida:  
   
---------------------------------------------------------------------------------------------------- */  
       
       
void schedule (int num_evento, double te, int token)
{
  int bufid; /* para retorno de erro da envia_mensagem */
  
  
  if (te < 0.0) /* nao existe tempo negativo */ 
    erro("Rotina: schedule() - Erro: te menor que zero");
  else
    insere_lista_eventos_futuros(num_evento, te, token);
 
}            
  
/* ------------------------------------------------------------------------------------------------- */       
       


   
/* ------------- insere_fila_eventos_recurso --------------------------------------------------------  

Entrada: char *nome_recurso     - numero do evento a ser escalonado
         int   token            - numero do token ao qual o recurso esta
                                  associado
         int   prioridade                         
         int   ident_proc       - identificacao do processo onde o evento
                                  devera ser escalonado (proc_local para 
                                  processo local)                     
         int   evento           - identificacao do evento a ser escalonado
         double remaining_event_time
                                - 0 para eventos bloqueados
                                                                                       
   Funcao: Insere eventos bloqueados (por falta de servidores) na lista de eventos do recurso.
           Eventos sao inseridos pela ordem de prioridade - prioridade maior, cabeca da fila;
           prioridade igual, FIFO
           
---------------------------------------------------------------------------------------------------- */      
  
   
void insere_fila_eventos_recurso(char *nome_recurso, int token, int prioridade, /*int proc,*/
                                 int evento, double remaining_event_time)
{
 tipo_cabecalho *ponteiro_cab;
 tipo_descritor_recursos *ponteiro_rec;
 tipo_fila_eventos *ponteiro_fila_eventos,
                   *ponteiro_aux, /* no para inserir o evento na lista                          */
                   *aux1,  /* ponteiros auxiliares para percorrer a lista de eventos do recurso */
                   *aux2;
 
 
 /* busca pelo recurso identificado por nome_recurso */
 
 ponteiro_cab = cabecalho_descritor;
 
 
 while ( (ponteiro_cab != NULL) && 
         (strcmp(ponteiro_cab->pont_descritor->nome,nome_recurso)) )
   ponteiro_cab = ponteiro_cab->prox_cabecalho;  
 
  if (ponteiro_cab == NULL)
    erro("Rotina: insere_fila_eventos_recurso() - Erro: recurso nao identificado");

  ponteiro_rec = ponteiro_cab->pont_descritor;

  ponteiro_rec->lenght_time_product_sum +=
                            ponteiro_rec->comprimento_fila * 
                           (clock_simulacao - ponteiro_rec->tempo_ultima_alteracao_fila);
  ponteiro_rec->comprimento_fila++;
  ponteiro_rec->tempo_ultima_alteracao_fila = clock_simulacao;
  
  /* ponteiro_fila_eventos aponta para inicio da fila de eventos */
  ponteiro_fila_eventos = ponteiro_rec->lista_eventos_recurso;
  
  /* alocar no e inserir dados */
  
  ponteiro_aux = aloca_mem_fila_eventos();
  if (ponteiro_aux == NULL) 
    erro("Rotina: insere_fila_eventos_recurso - Erro: falta de memoria");
  
    
  ponteiro_aux->prioridade = (double)prioridade;
  ponteiro_aux->event_time = remaining_event_time; /* zero para requisicoes bloqueadas */
  ponteiro_aux->num_evento = evento;
  ponteiro_aux->num_token = token;
  
  
  /* lista vazia, insercao na cabeca */
  if (ponteiro_fila_eventos == NULL)
  {
   ponteiro_aux->prox_fila_eventos = NULL;
   ponteiro_rec->lista_eventos_recurso = ponteiro_aux;
  }                           
  else
  {
   /* lista nao vazia - verifica se insercao deve ser feita na cabeca */
   if (ponteiro_aux->prioridade > ponteiro_fila_eventos->prioridade)
   {
     ponteiro_aux->prox_fila_eventos = ponteiro_rec->lista_eventos_recurso;
     ponteiro_rec->lista_eventos_recurso = ponteiro_aux;
   }
   else /* insercao sera feita no meio ou final da lista */
   {
     aux1 = ponteiro_fila_eventos;
     aux2 = ponteiro_fila_eventos;
     
     while ( (aux1 != NULL) && (ponteiro_aux->prioridade <= aux1->prioridade) )
     {
       aux2 = aux1;
       aux1 = aux1->prox_fila_eventos;
     }
     
     aux2->prox_fila_eventos = ponteiro_aux;
     ponteiro_aux->prox_fila_eventos = aux1;         
    }  
  }
    
}       
/* ------------------------------------------------------------------------------------------------- */
       
       

       
/* ------------- request ---------------------------------------------------------------------------  

Entrada: char *nome_recurso     - numero do evento a ser escalonado
         int   token            - numero do token ao qual o recurso esta
                                  associado
         int   prioridade                         
         int   ident_proc       - identificacao do processo onde o evento
                                  devera ser escalonado (proc_local para 
                                  processo local)                     
   Funcao: Verificar se existe servidor disponivel no recurso nome_recurso e em caso afirmativo, 
           reserva o servidor para o cliente token
   Saida:  
   
---------------------------------------------------------------------------------------------------- */  

int request(char *nome_recurso, int evento, int token, int prioridade/*, int proc*/)
{
 tipo_descritor_recursos *ponteiro_rec;
 tipo_servidor_recursos *ponteiro_serv_rec;
 tipo_cabecalho *ponteiro_cab;
 int r;
 
 /* verifica se existem servidores livre; eh so olhar o numero de servidores
    ocupados e o total de servidores do recurso                             */
    
 /* primeiro procura pelo recurso identificado por nome_recurso */
 ponteiro_cab = cabecalho_descritor;
 
 while ( (ponteiro_cab != NULL) && 
         (strcmp(ponteiro_cab->pont_descritor->nome,nome_recurso)) )
   ponteiro_cab = ponteiro_cab->prox_cabecalho;
   
  if (ponteiro_cab == NULL)
    erro("Rotina: request() - Erro: recurso nao identificado");
    
  /* recurso encontrado, busca por servidor livre */
  ponteiro_rec = ponteiro_cab->pont_descritor;
  
  if ( ponteiro_rec->num_servidores_ocupados < ponteiro_rec->num_servidores )
  {
    ponteiro_serv_rec = ponteiro_cab->pont_descritor->fila_servidor_recursos;
   
   /* procura servidor livre */
   ponteiro_serv_rec = ponteiro_cab->pont_descritor->fila_servidor_recursos;
  
   while ( (ponteiro_serv_rec != NULL) &&
           (ponteiro_serv_rec->num_token != 0) )
     ponteiro_serv_rec = ponteiro_serv_rec->prox_servidor_recursos;
     
    /* ponteiro_serv_rec aponta para o servidor livre */
    /* aloca servidor para o token                    */
    
    ponteiro_serv_rec->num_token = token;
    ponteiro_serv_rec->prioridade_usuario = prioridade;
    ponteiro_rec->num_servidores_ocupados++; /* incrementa total servidores ocupados */
    ponteiro_serv_rec->inicio_periodo_ocupado = clock_simulacao;
    r = 0; 
   }
   else
   {
    /* evento e variavel global - indica o evento corrente */
    /* remaining evet time e zero para eventos bloqueados; se preempcao, e diferente de 
       zero */
       
    insere_fila_eventos_recurso(nome_recurso, token, prioridade, /*proc,*/ evento, 0.0);
    r = 1; /* indica requisicao bloqueada por falta de servidor */
   }
   
   return(r);
 
}
/* ------------------------------------------------------------------------------------------------- */




 
 /* -------------- tempo_cabeca_lista_eventos_futuros -----------------------------------------------
 
   Entrada: 
   
   Funcao:   Retorna o tempo do evento da cabeca da lista de eventos futuros;
             se a lista estiver vazia retorna 1000000                   
             
   Saida:   double tempo_cabeca_lita_eventos
               
   ------------------------------------------------------------------------------------------------- */  
   
   
double tempo_cabeca_lista_eventos_futuros(void)

{
 if (lista_eventos_futuros == NULL)  /* lista vazia */
  return(200000000000000000000.0);
 else
  return(lista_eventos_futuros->tempo_ocorrencia);
}

/* ------------------------------------------------------------------------------------------------- */  

    


/* -------------- cause ----------------------------------------------------------------------------
 
   Entrada: int evento     - numero do evento a ser escalonado
            int token      - numero do token ao qual o recurso esta associado
   Funcao:   Retira eventos da lista de eventos futuros para execucao;
             se processo bloqueado (pelo relogio local), executa looping
             para recepcao de mensagens
   Saida:  
              obs: chamada cause(&evento,&token)   
   ------------------------------------------------------------------------------------------------- */  


void cause(int *evento, int *token)
{
 tipo_lista_eventos *ponteiro_lista_eventos, 
                    *aux;
 double tempo_cabeca_lista_eventos = 0.0;

  *token = lista_eventos_futuros->num_token;
  *evento = lista_eventos_futuros->num_evento;
  clock_simulacao = lista_eventos_futuros->tempo_ocorrencia;
  aux = lista_eventos_futuros;
  lista_eventos_futuros = lista_eventos_futuros->prox_lista_eventos; 
  free(aux);                  
}
/* ------------------------------------------------------------------------------------------------- */




/* -------------- smpl -----------------------------------------------------------------------------
 
   Entrada:                      
   Funcao:  
       
   ------------------------------------------------------------------------------------------------- */  


void smpl(int m,char *modelo)
{
 int indice;
 
 nome_modelo = modelo;
 cabecalho_descritor = NULL;
 clock_simulacao = 0.0;
 clock_atual = 0.0;
 start = 0.0;
 lista_eventos_futuros = NULL;
 cabecalho_descritor = NULL;
}
/* ------------------------------------------------------------------------------------------------- */

