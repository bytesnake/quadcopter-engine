#ifndef QUAT_H
#define QUAT_H

typedef struct {
	f32 X;
	f32 Y;
	f32 Z;
	f32 W;
} QUATER;

typedef struct {
	f32 X;
	f32 Y;
	f32 Z;
} VECTOR;

void Q_init ( QUATER *Q );
void Q_init_f32(QUATER *Q, f32 X, f32 Y, f32 Z, f32 W);
void Q_init_Ef32 ( QUATER *Q, f32 X, f32 Y, f32 Z );
void Q_add_Q ( QUATER *Q1, QUATER *Q2 );
void Q_sub_Q ( QUATER *Q1, QUATER *Q2 );
void Q_mul_f32 ( QUATER *Q, f32 scale );
void Q_div_f32 ( QUATER *Q, f32 scale );
void Q_copyTo_Q ( QUATER *Q1, QUATER *Q2 );
void Q_lerp_Q ( QUATER *Q1, QUATER *Q2, f32 scale );
void Q_normalize(QUATER *Q);
void Q_convert_E ( QUATER *Q, VECTOR *V );

#endif 
