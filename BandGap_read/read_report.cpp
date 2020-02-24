//
//  read_report.cpp
//  wannier_band
//
//  Created by 翁谋毅 on 2018/12/1.
//  Copyright © 2018 翁谋毅. All rights reserved.
//

#include "read_report.hpp"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include<algorithm>
int Comp(const void *p1,const void *p2)
{
    if (((struct E_E *)p1)->energy> ((struct E_E *)p2)->energy)
    {
        return 1;
    }
    if (((struct E_E *)p1)->energy< ((struct E_E *)p2)->energy)
    {
        return -1;
    }
    return 0;
}


void report::read_report(char *name_report)
{
    int i, j;
    struct E_E *temp;
    if (Read_N123(name_report, "N123", &n_x, &n_y, &n_z)==1)
    {
        return;
    }
    if (Read_int(name_report, "NUM_BAND", &num_band)==1)
    {
        return;
    }
    if (Read_int(name_report, "NUM_ELECTRON", &num_electron)==1)
    {
        return;
    }
    if (Read_int(name_report, "SPIN", &spin)==1)
    {
        return;
    }
    if (Read_kpt_num(name_report, "total number of K-point", &num_kpt)==1)
    {
        return;
    }
    if (spin==22 || spin ==222)
    {
        printf("Cases for spin = 22 and spin = 222 have not been written!\nThe results might be wrong.");
        spin=2;
    }
    eig = (struct E_E *)malloc(spin*num_kpt*num_band*sizeof(struct E_E ));
    if (Read_eig(name_report, eig, num_band, num_kpt, spin)==1)
    {
        return;
    }
    kp_p=(double **)malloc(num_kpt*sizeof(double *));
    for (i=0;i<num_kpt;i++)
    {
        kp_p[i]=(double *)malloc(3*sizeof(double));
    }
    Read_kpp(name_report, kp_p, num_kpt);
    Read_eig(name_report, eig, num_band, num_kpt, spin);
    qsort((void *)&eig[0], (size_t)num_band*num_kpt*spin, sizeof(struct E_E), Comp);
    return;
}



int Read_double(char *name, char *tag, double *out)
{
    FILE *in;
    char str[300];
    int i;
    int flag=1;
    in=fopen(name, "r");
    if (in==NULL)
    {
        printf("No inputfile: %s\n", name);
        return 1;
    }
    while(fgets(str, 300, in)!=NULL)
    {
        if (strstr(str, tag)!=NULL)
        {
            flag=0;
            for(i=0;i<strlen(str);i++)
            {
                if (str[i]=='=')
                {
                    break;
                }
            }
            fseek(in, -strlen(str)+i+1, SEEK_CUR);
            fscanf(in, "%lf", out);
            break;
        }
    }
    fclose(in);
    if (flag==0)
    {
        return 0;
    }
    else
    {
        printf("No %s in input file %s\n", tag, name);
        return 1;
    }
}



int Read_int(char *name, char *tag, int *out)
{
    FILE *in;
    char str[300];
    int i;
    int flag=1;
    in=fopen(name, "r");
    if (in==NULL)
    {
        printf("No inputfile: %s\n", name);
        return 1;
    }
    while(fgets(str, 300, in)!=NULL)
    {
        if (strstr(str, tag)!=NULL)
        {
            flag=0;
            for(i=0;i<strlen(str);i++)
            {
                if (str[i]=='=')
                {
                    break;
                }
            }
            fseek(in, -strlen(str)+i+1, SEEK_CUR);
            fscanf(in, "%d", out);
            break;
        }
    }
    fclose(in);
    if (flag==0)
    {
        return 0;
    }
    else
    {
        printf("No %s in input file %s\n", tag, name);
        return 1;
    }
}



int Read_N123(char *name, char *tag, int *n1, int *n2, int *n3)
{
    FILE *in;
    char str[300];
    int i;
    int flag=1;
    in=fopen(name, "r");
    if (in==NULL)
    {
        printf("No inputfile: %s\n", name);
        return 1;
    }
    while(fgets(str, 300, in)!=NULL)
    {
        if (strstr(str, tag)!=NULL)
        {
            flag=0;
            for(i=0;i<strlen(str);i++)
            {
                if (str[i]=='=')
                {
                    break;
                }
            }
            fseek(in, -strlen(str)+i+1, SEEK_CUR);
            fscanf(in, "%d", n1);
            fscanf(in, "%d", n2);
            fscanf(in, "%d", n3);
            break;
        }
    }
    fclose(in);
    if (flag==0)
    {
        return 0;
    }
    else
    {
        printf("No %s in input file %s\n", tag, name);
        return 1;
    }
}



int Read_kpt_num(char *name, char *tag, int *out)
{
    FILE *in;
    char str[300];
    int i;
    int flag=1;
    in=fopen(name, "r");
    if (in==NULL)
    {
        printf("No inputfile: %s\n", name);
        return 1;
    }
    while(fgets(str, 300, in)!=NULL)
    {
        if (strstr(str, tag)!=NULL)
        {
            flag=0;
            for(i=0;i<strlen(str);i++)
            {
                if (str[i]==':')
                {
                    break;
                }
            }
            fseek(in, -strlen(str)+i+1, SEEK_CUR);
            fscanf(in, "%d", out);
            break;
        }
    }
    fclose(in);
    if (flag==0)
    {
        return 0;
    }
    else
    {
        printf("No %s in input file %s\n", tag, name);
        return 1;
    }
}

int Read_eig(char *name, struct E_E *eig, int num_band, int num_kpt, int spin)
{
    FILE *in;
    char str[300];
    int flag=1;
    int i,j,k;
    in=fopen(name, "r");
    if (in==NULL)
    {
        printf("No inputfile: %s\n", name);
        return 1;
    }
    for (i=0;i<spin;i++)
    {
        for (j=0;j<num_kpt;j++)
        {
            flag=1;
            while(fgets(str, 300, in)!=NULL)
            {
                if (strstr(str, "eigen energies, in eV")!=NULL)
                {
                    for (k=0;k<num_band;k++)
                    {
                        fscanf(in, "%lf", &eig[k*num_kpt*spin+i*num_kpt+j].energy);
                        eig[k*num_kpt*spin+i*num_kpt+j].kpt=j;
                        eig[k*num_kpt*spin+i*num_kpt+j].spin=i;
                        eig[k*num_kpt*spin+i*num_kpt+j].band=k;
                    }
                    flag=0;
                    break;
                }
            }
        }
    }
    fclose(in);
    if (flag==0)
    {
        return 0;
    }
    else
    {
        printf("Not enough %s in input file %s\n", "eigen energies", name);
        return 1;
    }
}

int Read_kpp(char *name, double **kp_p, int num_kpt)
{
    FILE *in;
    in=fopen(name, "r");
    double temp;
    int i;
    char str[200];
    
    while(fgets(str, 200, in)!=NULL)
    {
        if (strstr(str, "total number of K-point:")!=NULL)
        {
            for (i=0;i<strlen(str);i++)
            {
                if (str[i]==':')
                {
                    break;
                }
            }
            fseek(in, -strlen(str)+i+1, SEEK_CUR);
            break;
        }
    }
    fgets(str, 200, in);
    for (i=0;i<num_kpt;i++)
    {
        fscanf(in, "%lf", &kp_p[i][0]);
        fscanf(in, "%lf", &kp_p[i][1]);
        fscanf(in, "%lf", &kp_p[i][2]);
        fscanf(in, "%lf", &temp);
    }
    
    fclose(in);
    
    return 1;
}
