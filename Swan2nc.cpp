#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <cmath>
#include <fstream>
#include <netcdf.h>
#include <algorithm>


#include <math.h>


char infile[256];
char ncoutfile[256];
char outvarnameA[256];
char outvarnameB[256];
float * xx,*yy,*tt;
short *zz,*vv,*uu;
float xo,yo,to;
float dx,dy,dt;
int nx,ny,nt;
int nvar;

//void creatncfile(char outfile[], int nx,int ny,float *xval, float *yval,float totaltime,short *Hs);
void creatncfile(char outfile[],char varname[], int nx,int ny,float *xval, float *yval,float totaltime,short *Hs);
void creatncfile2(char outfile[],char varnameA[],char varnameB[], int nx,int ny,float *xval, float *yval,float totaltime,short *uu,short *vv);
//void writestep2nc(char outfile[], int nx,int ny,float totaltime,short *Hs);
void writestep2nc(char outfile[],char varname[], int nx,int ny,float totaltime,short *Hs);
void writestep2nc2(char outfile[],char varnameA[], char varnameB[],int nx,int ny,float totaltime,short *uu,short *vv);

int main(int argc, char **argv)
{
	//Read operational file
	char opfile[]="Swan2nc.dat";
	FILE * fop;
    fop=fopen(opfile,"r");
	
	fscanf(fop,"%*s %s\t%*s",&infile);
	fscanf(fop,"%d\t%*s",&nvar);
	fscanf(fop,"%f,%f\t%*s",&xo,&yo);
	fscanf(fop,"%f,%f\t%*s",&dx,&dy);
	fscanf(fop,"%d,%d\t%*s",&nx,&ny);
	fscanf(fop,"%f,%f,%d\t%*s",&to,&dt,&nt);
	fscanf(fop,"%s\t%*s",&ncoutfile);
	fscanf(fop,"%s\t%*s",&outvarnameA);
	fscanf(fop,"%s\t%*s",&outvarnameB);
	fclose(fop);

	printf("Infile:%s\n nvar=%d\n xo=%f;  dx=%f;  nx=%d\n yo=%f;  dy=%f;  ny=%d\n to=%f;  dt=%f;  nt=%d\n outfile:%s\n",infile,nvar,xo,dx,nx,yo,dy,ny,to,dt,nt,ncoutfile);

	//Allocate memory for read variable
	xx=(float *)malloc(nx*sizeof(float ));
	yy=(float *)malloc(ny*sizeof(float ));
	tt=(float *)malloc(nt*sizeof(float ));
	zz= (short *)malloc(nx*ny*sizeof(short));
	if (nvar>1)
	{
		uu=(short *)malloc(nx*ny*sizeof(short));
	}
	


	//create xx and yy array
	for (int i=0; i<nx; i++)
	{
		xx[i]=xo+i*dx;
	}

	for (int j=0; j<ny; j++)
	{
		yy[j]=yo+j*dy;
	}

	


	//open the output file and read the first step nt should at least be size 1
	FILE * fin;
    fin=fopen(infile,"r");
	float Dum;

	for (int i=0; i<nx; i++)
	{
		for (int j=0; j<ny; j++)
		{
			fscanf(fin,"%f",&Dum);
			Dum=Dum*100.0f;
			zz[j+ny*i]=(short)(Dum);
		}
	}





	//Prepare netcdf file
	if(nvar<2)
	{
	creatncfile(ncoutfile,outvarnameA,nx,ny,xx,yy,to,zz);
	}
	else
	{
		for (int i=0; i<nx; i++)
		{
			for (int j=0; j<ny; j++)
			{
				fscanf(fin,"%f",&Dum);
				Dum=Dum*100.0f;
				uu[j+ny*i]=(short)(Dum);
			}
		}
		creatncfile2(ncoutfile,outvarnameA,outvarnameB,nx,ny,xx,yy,to,zz,uu);
	}

	for (int nn=1; nn<nt; nn++)
	{
		for (int i=0; i<nx; i++)
		{
			for (int j=0; j<ny; j++)
			{
				fscanf(fin,"%f",&Dum);
				Dum=Dum*100.0f;
				zz[j+ny*i]=(short)(Dum);
			}
		}
	
//This is rubbishmnnn
	if(nvar<2)
	{
		writestep2nc(ncoutfile,outvarnameA,nx,ny,to+nn*dt,zz);
	}
	else
	{
		for (int i=0; i<nx; i++)
		{
			for (int j=0; j<ny; j++)
			{
				fscanf(fin,"%f",&Dum);
				Dum=Dum*100.0f;
				uu[j+ny*i]=(short)(Dum);
			}
		}
		writestep2nc2(ncoutfile,outvarnameA,outvarnameB,nx,ny,to+nn*dt,zz,uu);
	}

	}

	fclose(fin);


}