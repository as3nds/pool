#include <stdio.h>

#include <time.h>
#include <stdlib.h>

#define NUMBER_OF_MANDATORY_FUNC 3
#define NUMBER_OF_NAD_FUNC 2
#define CTRLFLOW_MIN_LEN 20
#define CTRLFLOW_MAX_LEN 50

void mf1() {puts("mandatory function 1");}
void mf2() {puts("mandatory function 2");}
void mf3() {puts("mandatory function 3");}

void nadf1() {puts("noise or anti-debug function 1");}
void nadf2() {puts("noise or anti-debug function 2");}

void 
shuffle (int *array, size_t array_size, size_t shuff_size)
{
	if (array_size > 1) {
		size_t i;
		for (i = 0; i < shuff_size - 1; i++) {
			size_t j = i + rand() / (RAND_MAX / (array_size - i) + 1);
			int t = array[j];
			array[j] = array[i];
			array[i] = t;
		}
	}
}

int compare( const void* a, const void* b)
{
	int int_a = * ( (int*) a );
	int int_b = * ( (int*) b );

	if ( int_a == int_b ) return 0;
	else if ( int_a < int_b ) return -1;
	else return 1;
}


int
main (void)
{
	int cf_size;
	int *cf_rpos;
	int i;
	int mf_indexes[NUMBER_OF_MANDATORY_FUNC];
	void (**control_flow)(void);
	void *mfs[3] = {mf1, mf2, mf3};
	void *nadfs[2] = {nadf1, nadf2};

	srand(time(NULL));
	/* generate random size of control flow */
	cf_size = rand() % (CTRLFLOW_MAX_LEN - CTRLFLOW_MIN_LEN) + CTRLFLOW_MIN_LEN;
	printf("[i] Control flow size: %d\n", cf_size);

	/* generate random position of mandatory function */
	// generate array of CFSIZE 
	printf("[i] Generate random array pos\n");
	cf_rpos = malloc(cf_size * sizeof(int));
	for (i = 0; i < cf_size; i++) {
		*(cf_rpos + i) = i;
	}
	// debug print array
	for (i = 0; i < cf_size; i++) {
		printf("%d ", *(cf_rpos + i));
	}
	printf("\n");
	// shuffle
	printf("[i] Shuffle random array pos\n");
	shuffle(cf_rpos, cf_size, cf_size);
	for (i = 0; i < cf_size; i++) {
		printf("%d ", *(cf_rpos + i));
	}
	printf("\n");
	// get the 1st 3 from the pool of cf_rpos for mandatory function index  
	printf("[i] Reduce to number of mandatory functions\n");
	for (i = 0; i < NUMBER_OF_MANDATORY_FUNC; i++) {
		mf_indexes[i] = *(cf_rpos + i);	
	}
	for (i = 0; i < NUMBER_OF_MANDATORY_FUNC; i++) {
		printf("%d ", mf_indexes[i]);
	}
	printf("\n");
	// sort
	printf("[i] Sort random array pos\n");
	qsort( mf_indexes, NUMBER_OF_MANDATORY_FUNC, sizeof(int), compare);
	for (i = 0; i < NUMBER_OF_MANDATORY_FUNC; i++) {
		printf("%d ", mf_indexes[i]);
	}
	printf("\n");

	/* allocate control_flow */
	control_flow = malloc (cf_size * sizeof(void (*)(void)));
	/* replace mandatory function in random control flow */
	for (i = 0; i < NUMBER_OF_MANDATORY_FUNC; i++) {
		control_flow[mf_indexes[i]] = mfs[i];
	}
	/* replace nadf in random control flow */
	for (i = 0; i < cf_size; i++) {
		if (control_flow[i] == NULL) {
			int nadf_rand = rand() % (NUMBER_OF_NAD_FUNC) ;
			control_flow[i] = nadfs[nadf_rand];
		}
	}
	/* execute */
	for (i = 0; i < cf_size; i++) {
		control_flow[i]();
	}	


	/* free */
	free (cf_rpos);
	free (control_flow);

	return 0;
}
