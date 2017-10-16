/*  -------------------------------------------------------------------------------------------------
        

       Arquivo declaracoes.h        
       
       
    ------------------------------------------------------------------------------------------------ */



/* --------------- definicao das estruturas -------------------------------------------------------- */

struct servidor_recursos /* para cada servidor do recurso */
{
 double inicio_periodo_ocupado,
       soma_periodo_ocupado;
 int   contador_release,
       prioridade_usuario,
       num_token;
 struct servidor_recursos *prox_servidor_recursos;
};


struct fila_eventos  /* eventos bloqueados ou preempteds associados ao recurso */
{
 double prioridade; /* precisa ser double por causa do release; esse campo vai receber
                      o tempo qdo o evento for tirado da fila e reescalonado   */
 double event_time; /* tempo de ocorrencia do evento */
 int num_evento,
     num_token;
 struct fila_eventos *prox_fila_eventos;
};


struct descritor_recursos   /* para cada recurso do modelo */
{
  double  tempo_ultima_alteracao_fila,   /* L5 */
         lenght_time_product_sum;
  int contador_release_fila,            /* L4 */
      contador_preempcao,
      comprimento_fila;                 /* L3 */
  char *nome;                           
  int num_servidores_ocupados,          /* L2 */
      num_servidores;                   /* L1 */
  struct servidor_recursos    *fila_servidor_recursos;
  struct fila_eventos         *lista_eventos_recurso;
};


struct cabecalho
{
 struct descritor_recursos *pont_descritor;
 struct cabecalho *prox_cabecalho;
};



struct lista_eventos /* lista de eventos futuros do processo */
{
 double tempo_ocorrencia;    /* L5 */
 int num_evento,            /* L3 */      /* L4 nao utilizado - L1 link */
     num_token;             /* L2 */
 struct lista_eventos *prox_lista_eventos;    
};

/* --------------- definicao de tipos -------------------------------------------------------------- */

typedef struct cabecalho tipo_cabecalho; 
typedef struct descritor_recursos tipo_descritor_recursos;
typedef struct servidor_recursos tipo_servidor_recursos;
typedef struct fila_eventos tipo_fila_eventos;   /* para processos bloqueados no recurso */
typedef struct lista_eventos tipo_lista_eventos; /* lista de eventos futuros do processo */
typedef struct buf_canais tipo_buf_canais; /* buffer dos tempos das mensagens dos canais */



/* --------------- variaveis globais do processo ---------------------------- */

char         *nome_modelo; /* identificacao do modelo - apenas para relatorio        */
int          evento,       /* numero do evento a ser escalonado                      */
             token;        /* identificacao do token - cliente */

double clock_simulacao, /* clock da simulacao - determina o progresso da simulacao    */ 
      start,       /* intervalo de tempo da simulacao - utiliza no caso do reset     */
      clock_atual; /* utilizada na rotina cause - verificar se precisa ser global    */

      
/*  -------------- ponteiros ------------------------------------------------------------------------*/

/* cabecalho dos recursos do processo */
tipo_cabecalho *cabecalho_descritor; 
              

/* lista de eventos futuros - local do processo */
tipo_lista_eventos *lista_eventos_futuros;
