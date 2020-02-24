//
//  read_report.hpp
//  wannier_band
//
//  Created by 翁谋毅 on 2018/12/1.
//  Copyright © 2018 翁谋毅. All rights reserved.
//

#ifndef read_report_hpp
#define read_report_hpp

#include <stdio.h>
struct E_E{
    int spin;
    int kpt;
    int band;
    double energy;
};

class report{
public:
    void read_report(char *name_report);
    int n_x, n_y, n_z;
    struct E_E *eig;
    int num_band;
    int num_kpt;
    double **kp_p;
    int spin;
    int num_electron;
};
int Read_N123(char *name, char *tag, int *n1, int *n2, int *n3);
int Read_double(char *name, char *tag, double *out);
int Read_int(char *name, char *tag, int *out);
int Read_kpt_num(char *name, char *tag, int *out);
int Read_eig(char *name, struct E_E *eig, int num_band, int num_kpt, int spin);
int Read_kpp(char *name, double **kp_p, int num_kpt);

#endif /* read_report_hpp */
