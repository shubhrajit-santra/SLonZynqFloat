#include <hls_math.h>
#include <math.h>
#include <ap_int.h>

float UCB(float X, float T, float N){
	float Q_in ;
	Q_in = (float)(X/T) + (float) hls::sqrt((float)hls::log(N)/T);
	return Q_in;
}

// inform N_T_X_VACANT_ARMS
void machine(ap_uint<5> inform, float *Q){
#pragma HLS INTERFACE axis register both port=Q
#pragma HLS INTERFACE axis register both port=inform

	static float X ;
	static float T ;
	static float N ;

	float reward ;
	unsigned int vacant ;

	if(inform[0]==0){
		X = 1;
		T = 1;
		N = 11;
	}
	else{
		N += 1;

		if(inform[1]==1)
		{
			T += 1 ;
			vacant = (ap_uint<3>) inform.range(4,2);
			// reward = (float)((float)vacant/(float)4);

			if(vacant==0)
				reward = 0 ;
			else if(vacant==1)
				reward = 0.25 ;
			else if(vacant==2)
				reward = 0.5 ;
			else if(vacant==3)
				reward = 0.75 ;
			else if(vacant==4)
				reward = 1 ;

			X += reward;
		}
	}

	// printf("\nX = %f, T = %u, N = %u", (float)X, (unsigned int) T, (unsigned int) N) ;

	*Q = UCB(X, T, N);
}
