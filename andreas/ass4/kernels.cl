__kernel void eqkernel(
    __global double *temps_1, 
    __global double *temps_2,    
    double dconst,
    int PADDING_HEIGHT,
    int PADDING_WIDTH, //only pointers can have __constant
    int WIDTH,
    int HEIGHT)         //default is private memory!!

{ 
    
    int gy = get_global_id(0); //row
    int gx = get_global_id(1); //column
    int gyy = gy-PADDING_HEIGHT;
    int gxx = gx-PADDING_WIDTH;

    
    if ( gxx >= 1 && gxx < WIDTH+1 && gyy >= 1 && gyy < HEIGHT+1){

        
        double me = temps_1[(WIDTH+2)*gyy+gxx];
        double up = temps_1[(WIDTH+2)*(gyy-1)+gxx];
        double down = temps_1[(WIDTH+2)*(gyy+1)+gxx];
        double left = temps_1[(WIDTH+2)*gyy+(gxx-1)];
        double right = temps_1[(WIDTH+2)*gyy+(gxx+1)];

        temps_2[(WIDTH+2)*gyy+gxx] = me*(1-dconst)+ dconst*(up+down+left+right)/4.0;  
                
    } 
   
}

__kernel void avgkernel( //private memory is very small
        __global double *temps,        
        __global double *res,
        __local double *ltemps,
        int PADDING) 
{
    
    int gx = get_global_id(0);
    int lx = get_local_id(0);
    int gsize = get_local_size(0);
    int gid = get_group_id(0);
    int globalsize = get_global_size(0);

    if(gx >= globalsize-PADDING)
        ltemps[lx] = 0;
    else
        ltemps[lx] = temps[gx];

    for(unsigned int s = gsize/2; s > 0; s/=2){
        barrier(CLK_LOCAL_MEM_FENCE); 
        if (lx < s)
            ltemps[lx] += ltemps[lx+s];
    }

    if(lx == 0)
        res[gid] = ltemps[0]/gsize;        
}


__kernel void varkernel( 
        __global double *temps,        
        __global double *res,
        __local double *ltemps,
        int PADDING,
        double num)
{
    int gx = get_global_id(0);
    int lx = get_local_id(0);
    int gsize = get_local_size(0);
    int gid = get_group_id(0);
    int globalsize = get_global_size(0);

    if(gx >= globalsize-PADDING)
        ltemps[lx] = 0;
    else{
        double diff = temps[gx]-num;
        ltemps[lx] = diff > 0 ? diff : -diff;   
    }

    for(unsigned int s = gsize/2; s > 0; s/=2){   
                                             
        barrier(CLK_LOCAL_MEM_FENCE); 
        if (lx < s)
            ltemps[lx] += ltemps[lx+s];
    }

    if(lx == 0)
        res[gid] = ltemps[0]/gsize;       
}




