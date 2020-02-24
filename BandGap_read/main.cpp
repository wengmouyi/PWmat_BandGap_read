//
//  main.cpp
//  BandGap_read
//
//  Created by 翁谋毅 on 2018/12/7.
//  Copyright © 2018 翁谋毅. All rights reserved.
//

#include <iostream>
#include "read_report.hpp"
#include <cstdlib>

int main(int argc, const char * argv[]) {
    report rep;
    rep.read_report("REPORT");
    int i,j;
    int flag=0;
    double *max, *min;
    double f;
    max=(double *)malloc(rep.num_band*rep.spin*sizeof(double));
    min=(double *)malloc(rep.num_band*rep.spin*sizeof(double));
    for (i=0;i<rep.num_band*rep.spin;i++)
    {
        max[i]=-9999;
        min[i]=9999;
    }
    
    f=(rep.eig[rep.num_kpt*rep.num_electron*rep.spin/2-1].energy+rep.eig[rep.num_kpt*rep.num_electron*rep.spin/2].energy)/2;
    for (i=0;i<rep.num_band*rep.num_kpt*rep.spin;i++)
    {
        if (max[rep.eig[i].band+rep.num_band*rep.eig[i].spin]<rep.eig[i].energy)
        {
            max[rep.eig[i].band+rep.num_band*rep.eig[i].spin]=rep.eig[i].energy;
        }
        if (min[rep.eig[i].band+rep.num_band*rep.eig[i].spin]>rep.eig[i].energy)
        {
            min[rep.eig[i].band+rep.num_band*rep.eig[i].spin]=rep.eig[i].energy;
        }
    }
    flag=0;
    for (i=0;i<rep.num_band*rep.spin;i++)
    {
        if ((max[i]-f)*(min[i]-f)<0)
        {
            
            if (flag==0)
            {
                printf("There is no band gap:\n");
            }
            flag=1;
            printf("Spin %d Band %d\n", i/rep.num_band, i%rep.num_band);
        }
    }
    if (flag==1)
    {
        printf("Go cross fermi level\n");
        printf("Notes: All the number counts from 0\n");
        free(max);
        free(min);
        return 1;
    }
    
    
    if (flag==0)
    {
        printf("Band Gap= %lf\n", rep.eig[rep.num_kpt*rep.num_electron*rep.spin/2].energy-rep.eig[rep.num_kpt*rep.num_electron*rep.spin/2-1].energy);
        printf("CBM on Band %d\tKPT %d\tSPIN %d\n", rep.eig[rep.num_kpt*rep.num_electron*rep.spin/2].band, rep.eig[rep.num_kpt*rep.num_electron*rep.spin/2].kpt, rep.eig[rep.num_kpt*rep.num_electron*rep.spin/2].spin);
        printf("VBM on Band %d\tKPT %d\tSPIN %d\n",rep.eig[rep.num_kpt*rep.num_electron*rep.spin/2-1].band, rep.eig[rep.num_kpt*rep.num_electron*rep.spin/2-1].kpt, rep.eig[rep.num_kpt*rep.num_electron*rep.spin/2-1].spin);
        printf("Notes: All the number counts from 0\n");
        
    }
    
    return 0;
}
