
/* ------------------------------------------------------------------------ */
/* rand.h */
/* ------------------------------------------------------------------------ */


//typedef double real;

#ifndef __RAND__
#define __RAND__

/*-------------  UNIFORM [0, 1] RANDOM NUMBER GENERATOR  -------------*/
/*                                                                    */
/* This implementation is for Intel 8086/8 and 80286/386 CPUs using   */
/* C compilers with 16-bit short integers and 32-bit long integers.   */
/*                                                                    */
/*--------------------------------------------------------------------*/
double ranf ( void );

/*--------------------  SELECT GENERATOR STREAM  ---------------------*/
int stream( int n );

/*--------------------------  SET/GET SEED  --------------------------*/
long seed( long Ik, int n );

/*------------  UNIFORM [a, b] RANDOM VARIATE GENERATOR  -------------*/
double uniform( double a, double b);

/*--------------------  RANDOM INTEGER GENERATOR  --------------------*/
int randompar( int i, int n );

/*--------------  EXPONENTIAL RANDOM VARIATE GENERATOR  --------------*/
double expntl( double x );

/*----------------  ERLANG RANDOM VARIATE GENERATOR  -----------------*/
double erlang( double x, double s );

/*-----------  HYPEREXPONENTIAL RANDOM VARIATE GENERATION  -----------*/
double hyperx( double x, double s );

/*-----------------  NORMAL RANDOM VARIATE GENERATOR  ----------------*/
double normal( double x, double s );

#endif /* RAND */
