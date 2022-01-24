#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define MATSIZE 5000

int mat1[MATSIZE][MATSIZE];
int mat2[MATSIZE][MATSIZE];

typedef struct argstruct {
    // range of x to add within (x_start <= x < x_end)
    int x_start;
    int x_end;
    // range of y to add within (y_start <= y < y_end)
    int y_start;
    int y_end;
    // pointer to the matrix representing the sum
    int **res_mat;
} args_t;

//TODO: Complete this function (to be accessed by a thread) which will
//      perform matrix addition on a portion of a matrix, dictated by
//      the input parameter 'args' (cast 'args' to an args_t pointer)
//	Should add the entries of mat1 and mat2 in a rectangle and
//	store the sums at the appropriate indices of res_mat
void * partial_matrix_add(void * args) {

    // Your code goes here 

    // pthread_join


    int i,j;
    args_t *cores = (struct argstruct*) args;
    //printf("%d, %d -> %d, %d\n",cores->x_start,cores->x_end,cores->y_start,cores->y_end);
    // printf("in: %p\n",cores->res_mat);

    for(i=cores->x_start; i<cores->x_end; i++){

        for(j=cores->y_start;j<cores->y_end;j++)

            cores->res_mat[i][j]=mat1[i][j]+mat2[i][j];
            

    }
        

    return NULL;
}

int main() {
//multiple threads
    pthread_t m_threads[4];
    args_t m_args[4];

//single thread
    pthread_t s_thread;
    args_t s_args;



    int i, j, k, c;
    clock_t t_multi_start, t_multi_end, t_single_start, t_single_end;



    int ** res_mat_multi = malloc(MATSIZE * sizeof(int*));
    int ** res_mat_single = malloc(MATSIZE * sizeof(int*));



    for(i = 0; i < MATSIZE; i++) {
        res_mat_multi[i] = malloc(MATSIZE * sizeof(int));
        res_mat_single[i] = malloc(MATSIZE * sizeof(int));
    }//allocate the memory the ????

    // Populate base matrices with random integers



    // Initialize result matrices with -1
    for(j = 0; j < MATSIZE; j++) {
        for(k = 0; k < MATSIZE; k++) {
            mat1[j][k] = rand() % 1000 + 1;
            mat2[j][k] = rand() % 1000 + 1;
            res_mat_multi[j][k] = -1;
            res_mat_single[j][k] = -1;
        }
    }






    // Measure time for multiple thread addition
    t_multi_start = clock();
    //TODO: Create threads to populate res_mat_multi with the result
    //      of performing matrix addition mat1 + mat2
    //      Each thread should operate on one quadrant of mat1 and mat2
    //      Remember to set the fields in each argument struct m_args[i]
    for(i = 0;i<4;i++){
        if(i==0)
        {
            m_args[0].x_start=0;
            m_args[0].x_end=2500;
            m_args[0].y_start=0;
            m_args[0].y_end=2500;
            m_args[0].res_mat=res_mat_multi;

        }
        else if(i==1){
            m_args[1].x_start=2500;
            m_args[1].x_end=5000;
            m_args[1].y_start=2500;
            m_args[1].y_end=5000;
            m_args[1].res_mat=res_mat_multi;

        }
        else if(i==2){
            m_args[2].x_start=0;
            m_args[2].x_end=2500;
            m_args[2].y_start=2500;
            m_args[2].y_end=5000;
            m_args[2].res_mat=res_mat_multi;

        }
        else if(i==3){
            m_args[3].x_start=2500;
            m_args[3].x_end=5000;
            m_args[3].y_start=0;
            m_args[3].y_end=2500;
            m_args[3].res_mat=res_mat_multi;

        }
        // printf("out: %p, %d\n",m_args[i].res_mat, i);
        pthread_create(&m_threads[i],NULL, partial_matrix_add,(void *)&m_args[i]);

    }

    //TODO: Remember to join with each thread created

	for (i = 0; i < 4 ; i++) {

		pthread_join(m_threads[i], NULL);
	}


    // This should go immediately after the end of the code for multiple threads
    t_multi_end = clock();

    // Measure time for single thread addition
    t_single_start = clock();

    //TODO: Create one thread to populate res_mat_multi with the result
    //      of performing matrix addition mat1 + mat2
    //      This thread should operate on the ENTIRETY of the matrices
    //      Remember to set the fields in the argument struct s_args

 
        s_args.x_start=0;
        s_args.x_end=5000;
        s_args.y_start=0;
        s_args.y_end=5000;
        s_args.res_mat=res_mat_single;
        pthread_create(&s_thread,NULL, partial_matrix_add,(void *)&s_args);
    
    //TODO: Remember to join with the one thread created

		pthread_join(s_thread, NULL);
    // This should go immediately after the end of the code for a single thread
    t_single_end = clock();

    // Do not modify code after this point!
    //
    // Test to ensure that both methods produce the same result
    c = 0;
    for(j = 0; j < MATSIZE; j++) {
        for(k = 0; k < MATSIZE; k++) {
            if(res_mat_multi[j][k] == res_mat_single[j][k] && res_mat_multi[j][k] != -1) {
                c++;
            }
        }
    }
    printf("Verification: %d out of %d entries matched.\n", c, MATSIZE * MATSIZE);

    // Display time for each version
    printf("Time for multiple threads: %f seconds\n", ((double) t_multi_end - t_multi_start)/CLOCKS_PER_SEC);
    printf("Time for single thread: %f seconds\n", ((double) t_single_end - t_single_start)/CLOCKS_PER_SEC);
    return 0;
}
