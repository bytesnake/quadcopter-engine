#include "../../include/quadcopter.h"

void Q_init(QUATERN *Q) {
	Q_init_Qf32(Q, 0.0, 0.0, 0.0, 1.0);
}

void Q_init_Qf32(QUATERN *Q, f32 X, f32 Y, f32 Z, f32 W) {
	Q->X = X;
	Q->Y = Y;
	Q->Z = Z;
	Q->W = W;
}

void Q_init_Ef32(QUATERN *Q, f32 X, f32 Y, f32 Z) {
	X = X/2; Y = Y/2; Z = Z/2;

	f64 sx = sin(X); f64 cx = cos(X);
	f64 sy = sin(Y); f64 cy = cos(Y);
	f64 sz = sin(Z); f64 cz = cos(Z);

	f64 cycz = cy*cz;
	f64 sysz = sy*sz;
	f64 sycz = sy*cz;
	f64 cysz = cy*sz;
	
	Q_init_Qf32(Q,
		(f32) (cx * cycz + sx * sysz),
		(f32) (sx * cycz - cx * sysz),
		(f32) (cx * sysz + sx * cysz),
		(f32) (cx * cysz - sx * sycz)
	);

	Q_normalize(Q);
}

void Q_add_Q(QUATERN* Q1, QUATERN* Q2) {
	Q1->X = Q1->X + Q2->X;
	Q1->Y = Q1->Y + Q2->Y;
	Q1->Z = Q1->Z + Q2->Z;
	Q1->W = Q1->W + Q2->W;
}

void Q_sub_Q(QUATERN* Q1, QUATERN* Q2) {
	Q1->X = Q1->X - Q2->X;
	Q1->Y = Q1->Y - Q2->Y;
	Q1->Z = Q1->Z - Q2->Z;
	Q1->W = Q1->W - Q2->W;
}

void Q_mul_f32(QUATERN* Q, f32 scal) {
	Q->X = Q->X*scal;
	Q->Y = Q->Y*scal;
	Q->Z = Q->Z*scal;
	Q->W = Q->W*scal;
}

void Q_div_f32(QUATERN* Q, f32 scal) {
	return Q_mul_f32(Q, 1/scal);
}

void Q_copyTo_Q(QUATERN* Q1, QUATERN* Q2) {
	Q1->X = Q2->X;
	Q1->Y = Q2->Y;
	Q1->Z = Q2->Z;
	Q1->W = Q2->W;
}

void Q_normalize(QUATERN *Q) {
	// TODO: optimization
	f64 inv_length = 1/sqrt(Q->X*Q->X + 
				Q->Y*Q->Y + 
				Q->Z*Q->Z + 
				Q->W*Q->W);

	if(inv_length != 1)
		Q_mul_f32(Q, inv_length);
}

float Q_dotprod(QUATERN* q1, QUATERN* q2) {
	return (q1->X * q2->X) + (q1->Y * q2->Y) + (q1->Z * q2->Z) * (q1->W * q2->W);
}

void Q_lerp(QUATERN* q1, QUATERN* q2, f32 scale1) {
	QUATERN tq1, tq2;	
	Q_copyTo_Q ( &tq1, q1 );
	Q_copyTo_Q ( &tq2, q2 );

	const f32 scale2 = 1.0f - scale1;
	Q_mul_f32(&tq1, scale2);
	Q_mul_f32(&tq2, scale1);

	Q_add_Q(&tq1, &tq2);
	Q_copyTo_Q(q1, &tq1);
}

void Q_convert_E(QUATERN *Q, VECTOR *V) {
	const f64 sqw = Q->W*Q->W;
	const f64 sqx = Q->X*Q->X;
	const f64 sqy = Q->Y*Q->Y;
	const f64 sqz = Q->Z*Q->Z;
	
	/// heading = rotation about z-axis
	V->Z = (f32) atan2(2.0 * (Q->X*Q->Y +Q->Z*Q->W),(sqx - sqy - sqz + sqw)) * 180.0f / PI;
	//V->Z = (f32) atan2 ( 2.0f * (Q->X*Q->Y - Q->W*Q->Z), 2.0f * (sqw + sqx) - 1) * 180.0f / PI;
	// attitude = rotation about y-axis
	V->Y = (f32) -asin( 2.0f * ( Q->Y*Q->W + Q->X*Q->Z)) * 180.0f / PI;
	//V->Y = (f32) asin ( 2.0f * (Q->X*Q->Z + Q->W*Q->Y)) * 180.0f / PI;
	//V->Y = (f32) asin ( 2.0f * ( Q->X*Q->Y + Q->Z*Q->W ) ) * 180.0f / PI;
	// bank = rotation about x-axis
	V->X = (f32) atan2(2.0 * (Q->Z*Q->Y +Q->X*Q->W),(-sqx - sqy + sqz + sqw)) * 180.0f / PI;
	//V->X = (f32) atan2 ( 2.0f * (Q->Y*Q->Z - Q->W*Q->X), 2.0f * ( sqw + sqz ) -1 ) * 180.0f / PI;
}
