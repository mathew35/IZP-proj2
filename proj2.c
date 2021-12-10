///////////////////////////////////////////////////////////////////
//								 //
//			Iteracne vypocty			 //
//								 //
//			  Matus Vrablik				 //
//								 //
//			27.November 2019			 //
//								 //
///////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#define I0 pow(10,-12)
#define UT 0.0258563
bool load(double *u0,double *r,double *eps,char *argv[])
{
	char *end;			//funkcia nacita premenne a kotroluje ci nemaju chybne hodnoty
	*u0=strtod(argv[1],&end);	//ak prebehla uspesne vrati TRUE inak FALSE
	*r=strtod(argv[2],&end);
	*eps=strtod(argv[3],&end);
	bool check=true;
	if ((*u0<0)||(*r<=0)||(*eps<=0)) check=false;
	if (check==true) return true;
	else return false;
}

double f(double x,double r,double u0)
{					//funkcia vypocita honotu funkcie ktorej koren hladame so zadanou
	return (I0*(exp(x/UT)-1)-((u0-x)/r));
}					//hodnotou napetia-x

double diode(double u0,double r,double eps,char uORi)
{
	double up=0,a=0,b=u0,ip;	//funkcia najprv zvacsi hodnotu premennej eps na hodnotu s ktorou vie
	int i=0;			//program pracovat a s ktorou sa nezacykly nasledujuci while cyklus
	for (i=0;(eps<10e-16);)
		eps=eps*10;
	char *end;
	double inf=strtod("inf",&end);	//vytvori sa premenna inf s hodnotou inf pre porovnanie a tak pridanie 
	if (r==inf)			//vystupov pre danu moznost kde sa ci uz odpor r alebo napetie na zdroji
		ip=u0/r;		//rovna hodnote "inf"
	else 
		if (u0==inf)
		{
			ip=inf;
			up=inf;
		}
		else
			while (fabs(b-a)>=eps)	
			{		//cyklus while pomocou metody bisekcie a predoslej funkcie f hlada
				i++;	//pracovny bod diody kde sa 2 body lisia o menej ako absolutna odchylka
				double c=(a+b)/2;//eps
				if (f(c,r,u0)<eps||fabs(b-a)<eps)
					up=c;
				if (((f(c,r,u0)>0)&&(f(a,r,u0)>0))||((f(c,r,u0)<0)&&(f(a,r,u0)<0)))
					a=c;
				else b=c;
				ip=fabs(I0*(exp(c/UT)-1));
			}
	if (uORi=='u') return up;	//v pripade ze chceme z funkcie napetie tak stvrty parameter funkcie bude
	else return ip;			//"u" v druhom ze chceme prud tak stvrty parameter bude "i"
}

int main(int argc, char* argv[])
{
	double U0,R,EPS;
	if (argc==4)			//podmienka if kontroluje pocet argumentov a spravne nacitanie premennych
	{	if (load(&U0,&R,&EPS,argv))//ak sa podari tak vypise hodnotu napetia a prudu na diode
			fprintf(stdout,"Up=%g V\nIp=%g A\n",diode(U0,R,EPS,'u'),diode(U0,R,EPS,'i'));
		else fprintf(stderr,"error: invalid arguments\n");//v pripade ak pocet argumentov spravny ale
	}				//premenne sa nenacitali spravne tak vypise "error: invalid arguments"
	return 0;			//ak je nespravny pocet argumentov tak sa program ukonci
}
