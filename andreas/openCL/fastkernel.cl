__kernel void matmul( //can only pass 1D arrays 
    __constant int *A,
    __constant int *B,
    __global int *C,
    __constant int rsA,
    int csA,
    int rsB,
    int csB)
{

    int rowGlob = get_global_id(0); //get the work item
    int colGlob = get_global_id(1);
    int rowLoc = get_local_id(0);
    int colLoc = get_local_id(1);

    __local ALoc[32][32];
    __local BLoc[32][32];

    int nTiles = 128;
    int sum = 0;
    for(int i = 0; i < nTiles; i++){

        ALoc[rowLoc][colLoc] = A[rowGlob*csA+32*i+colLoc];
        BLoc[rowLoc][colLoc] = B[csB*32*i+csB*rowLoc+colGlob];
        barrier(CLK_LOCAL_MEM_FENCE);
        for(int j = 0; j < 32; j++)
            sum += ALoc[rowLoc][j]*BLoc[j][colLoc];
        C[rowGlob*csB+colGlob] = sum; //put this at the end instead!!
        barrier(CLK_LOCAL_MEM_FENCE);
    }
       
}
