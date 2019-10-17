__kernel void reductionkernel(
    __global int *arr,
    __local int *arr_loc,
    __global int *res)
{
    int ig = get_global_id(0);
    int il = get_local_id(0);
    int group  = get_group_id(0);
    int gs = get_local_size(0);

    arr_loc[il] = arr[ig];

    for(int s = gs/2; s > 0;  s/=2){

        barrier(CLK_LOCAL_MEM_FENCE);  
        if(il < s)
            arr_loc[il] += arr_loc[il+s];
    }

    if(il == 0)
        res[group] = arr_loc[0];

}    

