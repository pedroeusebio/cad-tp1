#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "definitions.h"


typedef struct {
	int nx;   	        // número de pontos em X
	int ny;   	        // número de pontos em Y
	int nz;   	        // número de pontos em Z
	int n_time_steps;   // número de passos de tempo
	float ***prev;      // Armazena solucao corrente
	float ***next;      // Armazena proxima solucao temporal
	float ***vel;       // Armazena a velocidade em cada ponto da malha
}Parameters;


void run_wave_propagation(float ***ptr_next, float ***ptr_prev, float ***ptr_vel, float *coeff, Parameters *p);
void iso_3dfd_it(float ***ptr_next, float ***ptr_prev, float ***ptr_vel, float *coeff, const int n1, const int n2, const int n3);
void write_plane_XY(float ***pnext, Parameters *p, int t_step, const char* rootname);


float*** Allocate3DMatrix(int nx, int ny, int nz)
{
		float *** array = (float ***)malloc(nx*sizeof(float**));
		for (int i = 0; i< nx; i++)
		{
			array[i] = (float **) malloc(ny*sizeof(float *));
			for (int j = 0; j < ny; j++) {
				array[i][j] = (float *)malloc(nz*sizeof(float));
			}
		}

		return array;
}


void Deallocate3DMatrix(float ***array, Parameters *p)
{
	for (int i = 0; i< p->nx; i++)
	{
		  for (int j = 0; j < p->ny; j++) {
			   free(array[i][j]);
		  }
		  free(array[i]);
	}

}



// Inicializa as matrizes com os valores iniciais
void initialize(float*** prev, float*** next, float*** vel, Parameters* p)
{
	for (int k = 0; k<p->nz; k++) {
		for (int j = 0; j<p->ny; j++) {
		   for (int i = 0; i<p->nx; i++) {
				prev[i][j][k] = 0.0f;
				next[i][j][k] = 0.0f;
				vel[i][j][k]  = 2250000.0f*DT*DT;
			}
		}
	}

	// Pulso inicial
	float val = 1.f;
	for (int s = 5; s >= 0; s--) {
		for (int k = p->nz / 2 - s; k<p->nz / 2 + s; k++) {
			for (int j = p->ny / 2 - s; j<p->ny / 2 + s; j++) {
				for (int i = p->nx / 2 - s; i<p->nx / 2 + s; i++) {
					prev[i][j][k] = val;
				}
			}
		}
		val *= 10;
	}
}


int main(int argc, char** argv)
{
	// Defaults
	// Malha com 256 x 256 x 256 pontos.
	Parameters p;
	p.nx = 256;
	p.ny = 256;
	p.nz = 256;
	p.n_time_steps = 200;

	// Make sure nreps is rouded up to next even number (to support swap)
	p.n_time_steps = ((p.n_time_steps + 1) / 2) * 2;

#if (HALF_LENGTH == 4)
	float coeff[HALF_LENGTH + 1] = {
		-2.847222222,
		+1.6,
		-0.2,
		+2.53968e-2,
		-1.785714e-3 };
#elif (HALF_LENGTH == 8)
	float coeff[HALF_LENGTH + 1] = {
		-3.0548446,
		+1.7777778,
		-3.1111111e-1,
		+7.572087e-2,
		-1.76767677e-2,
		+3.480962e-3,
		-5.180005e-4,
		+5.074287e-5,
		-2.42812e-6 };
#else
#  error "HALF_LENGTH not implemented"
#endif

	//Apply the DX DY and DZ to coefficients
	coeff[0] = (3.0f*coeff[0]) / (DXYZ*DXYZ);
	for (int i = 1; i <= HALF_LENGTH; i++) {
		coeff[i] = coeff[i] / (DXYZ*DXYZ);
	}

	// Arrays de Dados
	p.prev = NULL, p.next = NULL, p.vel = NULL;

	// Alocando dados
	size_t nsize = p.nx*p.ny*p.nz;
	size_t nbytes = nsize*sizeof(float);

	printf("Alocando prev, next e vel: total %g Mbytes\n", (3.0*(nbytes)) / (1024 * 1024)); fflush(NULL);

	p.prev = Allocate3DMatrix(p.nx, p.ny, p.nz);
	p.next = Allocate3DMatrix(p.nx, p.ny, p.nz);
	p.vel  = Allocate3DMatrix(p.nx, p.ny, p.nz);

	if (p.prev == NULL || p.next == NULL || p.vel == NULL) {
		printf("Não foi possivel alocar memoria: prev=%p next=%p vel=%p\n", p.prev, p.next, p.vel);
		printf("TESTE FALHOU!\n"); fflush(NULL);
		exit(-1);
	}

  printf("Inicializando dados\n");
	initialize(p.prev, p.next, p.vel, &p);

  // Escreve o pulso inicial
  write_plane_XY(p.prev, &p, 0, "wave");

  printf("Propagando a onda...\n");
  // Calcula a propagacão em um determinado intervalo de tempo
	run_wave_propagation(p.next, p.prev, p.vel, coeff, &p);

  printf("Finalizando...\n");
  // desaloca a memoria
	Deallocate3DMatrix(p.prev, &p);
	Deallocate3DMatrix(p.next, &p);
	Deallocate3DMatrix(p.vel , &p);
	return 0;

}



void run_wave_propagation(float ***ptr_next, float ***ptr_prev, float ***ptr_vel, float *coeff, Parameters *p)
{



	for (int it = 0; it<p->n_time_steps; it += 2) {

		iso_3dfd_it(ptr_next, ptr_prev, ptr_vel, coeff, p->nx, p->ny, p->nz);

		// Swap previous & next between iterations
		iso_3dfd_it(ptr_prev, ptr_next, ptr_vel, coeff,p->nx, p->ny, p->nz);


		if( it != 0 && it % 10 == 0)
			 write_plane_XY(ptr_prev, p, it, "wave");

	} // time loop

}


void iso_3dfd_it(float ***ptr_next, float ***ptr_prev, float ***ptr_vel, float *coeff, const int n1, const int n2, const int n3)
{
	for (int k = 0;k< n3; k++) {
	   for (int j = 0; j<n2; j++) {
			for (int i = 0; i<n1; i++)
			{

				if (i >= HALF_LENGTH && i<(n1 - HALF_LENGTH) && j >= HALF_LENGTH && j<(n2 - HALF_LENGTH) && k >= HALF_LENGTH && k<(n3 - HALF_LENGTH))
				{

					float value = 0.0;
					value += ptr_prev[i][j][k] * coeff[0];
					for (int ir = 1; ir <= HALF_LENGTH; ir++) {
						value += coeff[ir] * (ptr_prev[i+ir][j][k] + ptr_prev[i-ir][j][k]);        // horizontal
						value += coeff[ir] * (ptr_prev[i][j+ir][k] + ptr_prev[i][j-ir][k]);        // vertical
						value += coeff[ir] * (ptr_prev[i][j][k+ir] + ptr_prev[i][j][k-ir]); // in front / behind
					}
					ptr_next[i][j][k] = 2.0f* ptr_prev[i][j][k] - ptr_next[i][j][k] + value*ptr_vel[i][j][k];
				}
			}
		}
	}
}


void write_plane_XY(float ***r, Parameters *p, int t_step, const char* rootname)
{
	 char fname[80];
	 sprintf(fname,"%s_%03d.dat", rootname, t_step);

	 int iz = p->nz/2;

	 FILE* fout = fopen(fname,"w");

	 for (int i = 0; i< p->nx; i++) {
 	   for (int j = 0; j<p->ny; j++) {
			 fprintf(fout, "%8.8f %8.8f %8.8f\n", i*DXYZ, j*DXYZ, r[i][j][iz]);
		 }
	 }

	 fclose(fout);


	 sprintf(fname,"%s_%03d.plot",rootname, t_step);
	 fout = fopen(fname,"w");
	 fprintf(fout, "set terminal png\n");
	 fprintf(fout, "set output \'%s_%03d.png\'\n", rootname, t_step);
	 fprintf(fout, "set xlabel \'x\'\n");
	 fprintf(fout, "set xlabel \'y\'\n");
	 fprintf(fout, "set pm3d map\n");
	 fprintf(fout, "set palette gray\n");
	 fprintf(fout, "set dgrid3d 100,100\n");
	 fprintf(fout, "splot \'%s_%03d.dat\' u 1:2:3 t\"\"\n", rootname, t_step);
	 fclose(fout);



}
