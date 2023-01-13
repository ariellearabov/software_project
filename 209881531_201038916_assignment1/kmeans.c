#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>


#define EPS (0.001)

int int_check (char * param);
void kmean_main_func (double **data, int n, int d, int k, int max_iter);
void print_centroids (double **centroids, int k, int d);
double euc_dist(double *v1,double *v2, int d);
void update_cent(double **vec, int *cluster_map, int n, int d, int k, double **cent, int *cluster_size);
void map_to_cluster(double **vec, int n, int d, int k, double **cent, int *cluster_map);
int check_conv(double **cent_prev, double **cent, int k, int d);
void zero_cent(double **cent, int k, int d, int *cluster_size);
void print_centroids (double **centroids, int k, int d);

/* last var */

void my_free(void *p)
{
    free(p);

}

void **mem_prev = NULL;

void my_free_all()
{
    while (mem_prev != NULL) {

        void *p = mem_prev[0];

        my_free(mem_prev);
        mem_prev = p;
    }

}

/* func that chains memory of program for easy release at the end or in case of failure;
for every alocation we add an additional 'slot', a pointer at the "begining" that will
point to the next alocation (next malloc) if there is one */
void *my_malloc(unsigned int s)
{
    void **p = malloc(s + sizeof(void*));

    if (p == NULL) {
        printf("An Error Has Occurred\n");
        my_free_all();
        exit(1);
    }



    if (mem_prev == NULL) {
        p[0] = NULL;
    } else {
        p[0] = mem_prev;
    }
    mem_prev = p;

    return &p[1];
}

struct cord
{
    double value;
    struct cord *next;
};
struct vector
{
    struct vector *next;
    struct cord *cords;
};


int main(int argc, char* argv[])
{

    struct vector *head_vec, *curr_vec;
    struct cord *head_cord, *curr_cord;
    double **data;
    int i, j, d, d_temp=0, np=0, k, max_iter=200;
    char c;
    double n;

     /* first we check correct num of args  */
    if(argc < 2 || argc > 3){
        printf("An Error Has Occurred\n");
        return 1;
    }
    /*  checks that k is netural num, if so we atoi  */
    if(!int_check(argv[1])){
        printf("Invalid number of clusters!\n");

        return 1;
    }

    k = atoi(argv[1]);

    /*  checks that we recievrd num of iter arg and if so, that it is a netural num, if it is we atoi  */

    if(argc == 3 && !int_check(argv[2])){
        printf("Invalid maximum iteration!\n");
        return 1;
    }
    if(argc == 3 && int_check(argv[2])){
        max_iter = atoi(argv[2]);

    }

    /*  now we now iter in an int we check that it is in the correct range  */
    if(max_iter <= 1 || max_iter >= 1000){
        printf("Invalid maximum iteration!\n");
        return 1;
    }

    head_cord = my_malloc(sizeof(struct cord));

    curr_cord = head_cord;
    curr_cord->next = NULL;

    head_vec = my_malloc(sizeof(struct vector));

    curr_vec = head_vec;
    curr_vec->next = NULL;

    /*  scanning data usung code we saw in class  */

    d=1;
    while (scanf("%lf%c", &n, &c) == 2)
    {


        if (c == '\n')
        {

            np++;
            curr_cord->value = n;
            curr_vec->cords = head_cord;
            curr_vec->next = my_malloc(sizeof(struct vector));

            curr_vec = curr_vec->next;
            if (curr_vec==NULL)
            {
                /* code */
            }

            curr_vec->next = NULL;
            head_cord = my_malloc(sizeof(struct cord));
            curr_cord = head_cord;
            curr_cord->next = NULL;


            continue;
        }

        d_temp = 1  ;
        curr_cord->value = n;

        curr_cord->next = my_malloc(sizeof(struct cord));
        curr_cord = curr_cord->next;
        curr_cord->next = NULL;
        d_temp++;

        if (np == 1) {
            d++; } /*  d counts my dimentions but only once becuse all vector are of the same size  */
    }

     /*  now we have num of points we can finilize correctness of the k input  */

     if (k<=1 || k>=np) {
         printf("Invalid number of clusters!\n");
         my_free_all();
        return 1;
     }



    /* coping from vector/cord struct to 2D array  */
     /* we use temo here so we can free data without losing the "next" pointer */
    data = my_malloc(np * sizeof(double*));
    curr_vec = head_vec;

    for ( i = 0; i < np; i++) {
        data[i] = my_malloc(d* sizeof(double));

        curr_cord = curr_vec->cords;


        for ( j = 0; j < d; j++)
        {
           /* temp_cord = curr_cord;*/
            data[i][j]=curr_cord->value;
            curr_cord = curr_cord->next;



        }

       /* temp_vec = curr_vec;*/
        curr_vec = curr_vec->next;
    }



    kmean_main_func(data, np,  d, k, max_iter);


    my_free_all ();
    return 0;
}



/* func checking if input is a netural number  */

int int_check (char * param){
    char x, point = '.';
    int len, pp, point_cnt =0;
    len = strlen(param);
    for (pp = 0; pp < len; pp++){
        x= param[pp];
        if (x == point) {
            point_cnt = point_cnt +1    ;
        }
        if (!isdigit(x) && (x != point))
            return 0;
    }
    if ((atoi(param)>0) && ((atof(param)- atoi(param))==0) && (point_cnt <=1)) {
        return 1;
    }

    return 0;

    }

/* func checking Euclidean Distance  */
double euc_dist(double *v1,double *v2, int d)
{
    int i;
    double dist = 0;

    for (i = 0; i < d; i++) {
        double diff = (v1[i] - v2[i]);

        dist += (diff * diff);
    }
    return sqrt(dist);

}


/* func updeting centroids - cent vect array and cluster_size is all zeros  */
void update_cent(double **vec, int *cluster_map, int n, int d, int k, double **cent, int *cluster_size)
{
    int i, j, vec_clust;

    for (i = 0; i < n; i++) {
        vec_clust = cluster_map[i];
        cluster_size[vec_clust]++;

        for ( j = 0; j < d; j++) {
            cent[vec_clust][j] += vec[i][j];
        }
    }

    for (i = 0; i < k; i++) {
        for ( j = 0; j < d; j++) {
            cent[i][j] /= cluster_size[i];
        }
    }
}

/* assigning each point to the correct cluster  */

void map_to_cluster(double **vec, int n, int d, int k, double **cent, int *cluster_map)
{
    int i,j, curr_cluster_index = 0;

    for ( i = 0; i < n; i++) {
        double min_dis = -1 ,min_temp;

        for ( j = 0; j < k; j++) {
           min_temp = euc_dist (vec[i], cent[j], d);
           if (min_temp < min_dis || min_dis < 0) {
            min_dis = min_temp;
            curr_cluster_index = j;

           }
        }
        cluster_map[i] = curr_cluster_index;
    }
}

/* checking convergence  */

int check_conv(double **cent_prev, double **cent, int k, int d)
{   int i;
    double dis;

    for (i = 0; i < k; i++) {
        dis = euc_dist(cent_prev[i], cent[i], d);

        if (dis >= EPS) {
            return 0;
        }

    }

    return 1;
}

 /** gets the centroids and size of clausters array **/
void zero_cent(double **cent, int k, int d, int *cluster_size)
{   int i,j;
    for (i = 0; i < k; i++)
    {
        cluster_size[i] = 0;
         for ( j = 0; j < d; j++)
         {
            cent[i][j] = 0;
         }

    }

}

 /** Prints final centroids**/
void print_centroids (double **centroids, int k, int d) {
    int i,j;

    for ( i = 0; i < k; i++)
    {
       for ( j = 0; j < d; j++)
       {
        printf("%.4f", centroids[i][j]);

        if(j< (d-1)){
            printf(",");
       }

    }
    printf("\n");
    }
}

 /** this is where the magic happens **/
void kmean_main_func (double **data, int n, int d, int k, int max_iter){
    double ** centroids1 = my_malloc (k * sizeof(double *));
    double ** centroids2 = my_malloc (k * sizeof(double *));
    int *cluster_size = my_malloc (k * sizeof(int));
    int *cluster_map = my_malloc (n * sizeof(int));
    int i,j, iter_cnt=0;
    double **cur_cent, **prev_cent;


     /** initializing centroids with first k vectors **/
    for ( i = 0; i < k; i++)
    {
       centroids1[i] = my_malloc (d * sizeof(double));
       centroids2[i] = my_malloc (d * sizeof(double));

       for (j = 0; j < d; j++)
       {
        centroids1[i][j] = data[i][j];
       }
    }


    while (iter_cnt< max_iter) {
        if ((iter_cnt % 2) == 0) {
            cur_cent = centroids2;
            prev_cent = centroids1;
        } else {
            cur_cent = centroids1;
            prev_cent = centroids2;
        }

        zero_cent(cur_cent,  k,  d, cluster_size);
        map_to_cluster(data, n,  d,  k, prev_cent, cluster_map);
        update_cent(data, cluster_map,  n,  d,  k, cur_cent, cluster_size);

        if (check_conv(cur_cent, prev_cent, k, d)) {
            break;
        }

        iter_cnt++;
    }
    print_centroids (cur_cent, k, d);


    }
