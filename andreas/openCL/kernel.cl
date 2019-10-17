
__kernel void matmul( //can only pass 1D arrays 
    __constant int *A,
    __constant int *B,
    __global int *C,
    int rsA,
    int csA,
    int rsB,
    int csB)
{
    int row = get_global_id(0); //get the work item
    int col = get_global_id(1);

    int sum = 0;
    for(int j = 0; j < csA; j++)
        sum += A[row*csA+j]*B[col+csB*j];
    C[row*csB+col] = sum;
       
}
