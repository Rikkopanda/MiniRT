
#include "miniRT.h"

/**
 * https://www.freetext.org/Introduction_to_Linear_Algebra/Basic_Vector_Operations/Normalization/
*/
void normalize_vector(float v[3])
{
	float magnitude;
	double squared;

	squared = pow((double)v[0], 2) + pow((double)v[1], 2) + pow((double)v[2], 2);
	magnitude = (float)sqrt(squared);
	// printf("before\n");
	// print_matrix_1_3(v);
	vector_scaling(v, 1 / magnitude);
	// printf("after\n");
	// print_matrix_1_3(v);
	// return ()
}