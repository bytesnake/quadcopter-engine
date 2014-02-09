#ifndef MATH_KAlMAN_H
#define MATH_KALMAN_H

#define DT 100 /*Herz*/

// Diagonale of 3x3 Matrix how well the model represent the changes
#define Q1 5.0f
#define Q2 100.0f
#define Q3 0.01f

// Diagonale of 3x3 Matrix how much we trust the measurement
#define R1 1000.0f
#define R2 1000.0f

typedef struct
{
	float x1, x2, x3;
	float p11, p12, p13, p21, p22, p23, p31, p32, p33;
} math_kalman_data_t;

void Math_KalmanInit ( math_kalman_data_t *data );
void Math_KalmanInvoke ( math_kalman_data_t *data, float pos, float speed );
#endif
