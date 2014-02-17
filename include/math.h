#ifndef QUAT_H
#define QUAT_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define f32 float
#define f64 double


typedef struct {
	f32 X;
	f32 Y;
	f32 Z;
	f32 W;
} QUATERN;

typedef struct {
	f32 X;
	f32 Y;
	f32 Z;
} VECTOR;

void Q_init_Qf32(QUATERN *Q, f32 X, f32 Y, f32 Z, f32 W);
void Q_normalize(QUATERN *Q);

#endif 
