/**
*   Druhy projekt do IZP, iteracni vypocty.
*   Vytvoril: Tomas Kukan, xkukan00
*   Verze: 1.0
*
*   Popis programu a jeho funkce: https://wis.fit.vutbr.cz/FIT/st/cwk.php?title=IZP:Projekt2&csid=623120&id=11499
*   Funkce my_pow a my_log se spousti s argumentem --powx nebo --logx.
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define WRONG_ARGS 1
#define NEGATIVE_NUM 2
#define NOT_NUM 3
#define NOT_POSITIVE 4
#define IS_NEGATIVE 5

/*
*FUNKCI PROTOTYPY FUNKCI
*/

double taylor_log(double x, unsigned int n);
double cfrac_log(double x, unsigned int n);
double taylor_pow(double x, double y, unsigned int n);
double taylorcf_pow(double x, double y, unsigned int n);
double my_log(double x);
double my_pow(double x, double y);

int is_positive(double number);
int is_number(char *s);
void error(int err_num);
int num_comp(double a, double b, double c);
void help();

int main(int argc, char *argv[])
{
    double number, exp;
    unsigned int iteration_quantity;
    int is_log = 0, is_logx = 0, is_powx = 0, test_num;

    /*Prace s argumenty programu.*/
    if(argc < 2)
    {
        error(WRONG_ARGS);
        return 1;
    }

    if (strcmp(argv[1], "--log") == 0 || strcmp(argv[1], "--logx") == 0) /*testovani pokud je na vstupu --log*/
    {
        if(argc != 4)
        {
            error(WRONG_ARGS);
            return 1;
        }

        is_log = 1;

        if(strcmp(argv[1], "--logx") == 0) /*Pro pouziti funkce my_log*/
            is_logx = 1;

        for(int i = 2; i < 4; i++)
            if (!is_number(argv[i]))
            {
                error(WRONG_ARGS);
                return 1;
            }

        number = atof(argv[2]);
        iteration_quantity = atoi(argv[3]);
        test_num = atoi(argv[3]);

        if(test_num < 1)
        {
            error(IS_NEGATIVE);
            return 1;
        }
    }

    else if (strcmp(argv[1], "--pow") == 0 || strcmp(argv[1], "--powx") == 0) /*testovani pokud je na vstupu --pow*/
    {
        if(argc != 5)
        {
            error(WRONG_ARGS);
            return 1;
        }

        if(strcmp(argv[1], "--powx") == 0) /*Pro pouziti funkce my_pow*/
            is_powx = 1;


        for(int i = 2; i < 5; i++)
            if (!is_number(argv[i]))
            {
                error(WRONG_ARGS);
                return 1;
            }

        number = atof(argv[2]);
        exp = atof(argv[3]);
        iteration_quantity = atoi(argv[4]);
        test_num = atoi(argv[4]);

        if(test_num < 1)
        {
            error(IS_NEGATIVE);
            return 1;
        }
    }

    else /*Pokud neni --pow ani --log */
    {
        error(WRONG_ARGS);
        return 1;
    }

    if (is_log)
    {
        printf("       log(%g) = %.12g\n", number, log(number));
        printf(" cfrac_log(%g) = %.12g\n", number, cfrac_log(number, iteration_quantity));
        printf("taylor_log(%g) = %.12g\n", number, taylor_log(number,iteration_quantity));
        if(is_logx)
            printf("    my_log(%g) = %.7e\n", number, my_log(number));
    }
    else
    {
        printf("         pow(%g,%g) = %.12g\n", number, exp, pow(number, exp));
        printf("  taylor_pow(%g,%g) = %.12g\n", number, exp, taylor_pow(number, exp, iteration_quantity));
        printf("taylorcf_pow(%g,%g) = %.12g\n", number, exp, taylorcf_pow(number, exp, iteration_quantity));
        if(is_powx)
            printf("      my_pow(%g,%g) = %.7e\n", number, exp, my_pow(number, exp));
    }

    return 0;
}

/*
*DEFINICE FUNKCI
*/

/*
*   Funkce pro vypocet logaritmu pomoci taylotova polynomu.
*   prvni agument je samotne cislo a druhy je pocet iteraci.
*   Jeji navratova hodnota je vysledek.
*/
double taylor_log(double x, unsigned int n)
{
    /* 1 - X = A => X = 1 - A, KDE /A JE ZAKLAD*/

    if(x == INFINITY)
        return INFINITY;

    else if(x == 0.0)
        return -INFINITY;

    else if(x < 0.0)
        return NAN;

    double ans = 0;
    double a = 1 - x;
    double upper = 1;

    if (x > 0 && x < 1)
        for (unsigned int i = 1; n >= i ; i++) /*cyklus pro urceny pocet iteraci*/
        {
            upper *= a;
            if(isinf(upper))
            	return INFINITY;
            ans = ans - upper / i;
        }
    else
        for (unsigned int i = 1; n >= i ; i++) /*cyklus pro urceny pocet iteraci*/
        {
            upper *= (x - 1) / x;
            if(isinf(upper))
            	return INFINITY;
            ans += upper / i;
        }


    return ans;

}

/*
*   Funkce pro vypocet logaritmu pomoci zretezeneho zlomku.
*   prvni agument je samotne cislo a druhy je pocet iteraci.
*   Jeji navratova hodnota je vysledek.
*/
double cfrac_log(double x, unsigned int n)
{
    /*(1+z)/(1-z) = x => z = (x-1)/(x+1)*/

    if(x == INFINITY)
        return INFINITY;

    else if(x == 0.0)
        return -INFINITY;

    else if(x < 0.0)
        return NAN;

    double z = (x - 1)/(x + 1);
    double ans = 10;


    for (; n > 0 ;n--) /*cyklus pro urceny pocet iteraci*/
    {
        ans = (2*n - 1) - ((n*n*z*z) / ans);
    }

    return (2*z) / ans;

}


/*
*   Funkce pocitajici mocninu cisla x s exponentem
*   y. Pocita ji v n iteracich a pouziva tayloruv
*   polynom pro vypocet logaritmu.
*/
double taylor_pow(double x, double y, unsigned int n)
{

    if(y == 0)
        return 1;

    else if(x == 0)
        return 0;

    else if(x == INFINITY)
        return INFINITY;

    else if(x < 0)
        return NAN;

    double ans = 1,fac_num = 1;
    double log_x = taylor_log(x, n);
    double upper = 1;

    for (double i = 1; i < n + 1; i++)
    {
        fac_num *= i;
        upper *= y * log_x;
        ans += upper / fac_num;
	if(isinf(upper))
            return INFINITY;
    }
    return  ans;

}

/*
*   Funkce pocitajici mocninu cisla x s exponentem
*   y. Pocita ji v n iteracich a pouziva zretezeny
*   zlomek pro vypocet logaritmu.
*/
double taylorcf_pow(double x, double y, unsigned int n)
{
    double ans = 1, fac_num = 1;
    double log_x = cfrac_log(x, n), upper = 1;

    if(y == 0)
        return 1;

    else if(x == 0)
        return 0;

    else if(x == INFINITY)
        return INFINITY;

    else if(x < 0)
        return NAN;

    for (unsigned int i = 1; i < n ; i++)
    {
        fac_num *= i;
        upper *= y * log_x;
        ans +=  upper / fac_num;
        if(isinf(upper))
            return INFINITY;

    }
    return ans;
}

/*
*   Otestuje retezec zdali je cislo. Vrati 1 pokud ano, 0 pokud ne.
*/
int is_number(char *s)
{
    char *endptr;
    strtod(s, &endptr);

    if(*endptr != '\0')
    {
        error(NOT_NUM);
        return 0;
    }

    else
        return 1;
}

/*
*   Funkce pro vypis chyboveho hlaseni.
*/
void error(int err_num)
{
    switch(err_num)
    {
        case WRONG_ARGS:
            fprintf(stderr,"Chybne argumenty, ukonceni programu.\n");
            help();
            break;

        case NEGATIVE_NUM:
            fprintf(stderr,"Argument nemuze byt zaporny.\n");
            help();
            break;

        case NOT_NUM:
            fprintf(stderr,"Argument neni cislo.\n");
            help();
            break;

        case NOT_POSITIVE:
            fprintf(stderr,"Argument X neni kladne cislo!\n");
            help();
            break;

        case IS_NEGATIVE:
            fprintf(stderr,"Argument pocet iteraci neni kladne cislo.\n");
            help();
            break;

    }

    return ;
}

/*
*   Funkce vypisujici napovedu.
*/
void help()
{
    fprintf(stderr,"*   Druhy projekt do IZP, iteracni vypocty.\n"
                    "*   Vytvoril: Tomas Kukan, xkukan00\n"
                    "*   Verze: 1.0\n"
                    "*\n"
                    "*   Popis programu a jeho funkce: https://wis.fit.vutbr.cz/FIT/st/cwk.php?title=IZP:Projekt2&csid=623120&id=11499\n "
                    "*   Funkce my_pow a my_log se spousti s argumentem --powx nebo --logx.\n");

}


/*
*   Vlastni funkce my_log, vraci vysledek s presnosti na 8 platnych cislic, s co nejmene iteracema.
*/
double my_log(double x)
{
    /*(1+z)/(1-z) = x => z = (x-1)/(x+1)*/

    if(x == INFINITY)
        return INFINITY;

    else if(x == 0.0)
        return -INFINITY;

    else if(x < 0.0)
        return NAN;


    double z = (x - 1)/(x + 1);
    double ans = 0, ans1 = 0, ans2 = 0;
    int n = 1, i;

    do
    {
        ans2 = ans1;
        ans1 = ans;
        ans = 1;
        for (i = n; i > 0 ; i--) /*cyklus pro urceny pocet iteraci*/
        {
            ans = (2*i - 1) - ((i*i*z*z) / ans);
            if(isinf(i*i*z*z) && isinf(ans))
                return NAN;
        }
        n*=2;

    }while(num_comp(ans,ans1,ans2));
    /*Testuji zde posledni tri posledni, protoze se obcas stane ze posledi 2 cleny jsou naprosto stejne.
    * (treba pro log 0.2)
    */

    return (2*z) / ans;

}

/*
*   Moje unkce pocitajici mocninu cisla x s exponentem
*   y. Pocita ji na presnost 6ti platnych cislic a pouziva zretezeny
*   zlomek pro vypocet logaritmu.
*/
double my_pow(double x, double y)
{

    if(y == 0)
        return 1;

    else if(x == 0)
        return 0;

    else if(x == INFINITY)
        return INFINITY;

    else if(x < 0)
        return NAN;

    double ans = 1, fac_num = 1,ans2 = 0;
    double log_x = my_log(x), upper = 1;

    unsigned int i;

    for (i = 1; num_comp(ans, ans2, ans2); i++)
    {
        ans2 = ans;
        fac_num *= i;
        upper *= y * log_x;
        if(isinf(upper))
            return INFINITY;

        ans +=  upper / fac_num;
    }

    return ans;
}


/*
*   Funkce porovna 3 cisla na 7 platnych cislic.
*   Vraci 0 pokud jsou stejne.
*/
int num_comp(double a, double b, double c)
{
    char arrayA[16]; /*15 mist, protoze 1.3456789e+9999\0*/
    char arrayB[16];
    char arrayC[16];

    int test;

    sprintf(arrayA, "%.7e", a);
    sprintf(arrayB, "%.7e", b);
    sprintf(arrayC, "%.7e", c);

    test = strcmp(arrayA,arrayB);

    if(test == 0)
        test = strcmp(arrayB,arrayC);

    return test == 0 ? 0 : 1;
}







