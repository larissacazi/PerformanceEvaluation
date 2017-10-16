/**********************************************************************/
/*                                                                    */
/*                           File "smpl.h"                            */
/*  Includes, Defines, & Extern Declarations for Simulation Programs  */
/*                                                                    */
/*                                         (c) 1987  M. H. MacDougall */
/*                                                                    */
/**********************************************************************/

#include <stdio.h>
#include <math.h>
//#include <process.h>
#include "randpar.h"


//typedef double real;
#define then

extern double Lq(), U(), B(), time();
extern char *fname(), *mname();
extern FILE *sendto();

extern double ranf(), uniform(), expntl(), erlang(), hyperx(), normal();
extern long seed();



/*---------------  INITIALIZE SIMULATION SUBSYSTEM  ------------------*/
void smpl ( int, char* );

/*-----------------------  RESET MEASUREMENTS  -----------------------*/
void reset( void );

/*---------------------------  SAVE NAME  ----------------------------*/
int save_name(char*, int );

/*-------------------------  GET MODEL NAME  -------------------------*/
char *mname( void );

/*------------------------  GET FACILITY NAME  -----------------------*/
char *fname( int );

/*---------------------------  GET BLOCK  ----------------------------*/
int get_blk( int );

/*--------------------------  GET ELEMENT  ---------------------------*/
int get_elm(void);

/*-------------------------  RETURN ELEMENT  -------------------------*/
void put_elm( int );

/*-------------------------  SCHEDULE EVENT  -------------------------*/
void schedule( int, double, int );

/*---------------------------  CAUSE EVENT  --------------------------*/
void cause(int*, int* );

/*--------------------------  RETURN TIME  ---------------------------*/
double time(void);

/*----------------------  RETURN CLOCK POINTER  ----------------------*/
double *p_time(void);

/*--------------------------  CANCEL EVENT  --------------------------*/
int cancel( int );

/*-------------------------  SUSPEND EVENT  --------------------------*/
int suspend( int );

/*--------------  ENTER ELEMENT IN QUEUE OR EVENT LIST  --------------*/
void enlist( int*, int );

/*-----------------------  DEFINE FACILITY  --------------------------*/
void facility( char*, int );

/*---------------  RESET FACILITY & QUEUE MEASUREMENTS  --------------*/
void resetf( void );

/*------------------------  REQUEST FACILITY  ------------------------*/
//int request( int, int, int );
int request(char *, int, int, int);

/*-------------------------  ENQUEUE TOKEN  --------------------------*/
void enqueue( int, int, int, int, double );

/*------------------------  PREEMPT FACILITY  ------------------------*/
int preempt( int, int, int );

/*------------------------  RELEASE FACILITY  ------------------------*/
void release( char *, int );

/*-----------------------  GET FACILITY STATUS  ----------------------*/
//int status( int );

/*--------------------  GET CURRENT QUEUE LENGTH  --------------------*/
//int inq( int );
int comprimento_fila(char *);

/*-----------------  GET LAST QUEUE ENTRY/EXIT TIME  -----------------*/
//double qext( int );

/*--------------------  GET FACILITY UTILIZATION  --------------------*/
//double U( int );
double utilizacao_recurso(char *);

/*----------------------  GET MEAN BUSY PERIOD  ----------------------*/
//double B(int );
double periodo_medio_ocupado(char *);

/*--------------------  GET AVERAGE QUEUE LENGTH  --------------------*/
//double Lq( int );
double comprimento_medio_fila(char *);

/*-----------------------  TURN TRACE ON/OFF  ------------------------*/
void trace( int );

/*--------------------  GENERATE TRACE MESSAGE  ----------------------*/
void msg( int, int, char*, int, int );

/*-------------------------  TRACE LINE END  -------------------------*/
void end_line( void );

/*-----------------------------  PAUSE  ------------------------------*/
void pause ( void );

/*------------------  DISPLAY ERROR MESSAGE & EXIT  ------------------*/
void error( int, char* );

/******************************* REPORT *******************************/
void report( void );

/*--------------------  GENERATE FACILITY REPORT  --------------------*/
void reportf( void );

/*----------------------  GENERATE REPORT PAGE  ----------------------*/
int rept_page( int );

/*---------------------------  COUNT LINES  --------------------------*/
int lns(int);

/*----------------------------  END PAGE  ----------------------------*/
void endpage();

/*----------------------------  NEW PAGE  ----------------------------*/
void newpage();

/*------------------------  REDIRECT OUTPUT  -------------------------*/
FILE *sendto( FILE * );

/*------------------------  get FACILITY  descriptor -----------------*/
int getfacility ( int );

/*---------------  DUMP MODULE  (OPTIONALLY COMPILED)  ---------------*/
int dump( void );

/*----------------------  OUTPUT DUMP HEADING  -----------------------*/
void dump_hdr(int n );

/*---------------------  OUTPUT FACILITY STATUS  ---------------------*/
void fdump(int f);

/*-----------------------  OUTPUT QUEUE STATUS  ----------------------*/
void qdump(int f);

/*----------------------  OUTPUT QUEUE HEADING  ----------------------*/
void q_hdr( void );

/*-----------------------  OUTPUT EVENT LIST  ------------------------*/
void edump( void );

int getfacility ( int );

/*--------------------- RETURN CURRENTE TOKEN ----------------------*/
int gettoken ( void );

/*--------------------- RETURN CURRENTE EVENT ----------------------*/
int getevent ( void );

/*--------------------- RETURN CURRENTE EVT ----------------------*/
int getevl ( void );

/*--------------------- RETURN FACILITY DESCRIPTOR ----------------------*/
int getfdes ( int f );

/*--------------------- RETURN POOL DESCRIPTOR ----------------------*/
int getpdes ( int f );

/*--------------------- RETURN START TIME ---------------------------*/
double getstart ( void );

double *p_time(void);

