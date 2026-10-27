//  gcc -std=c17 -Wall -Wextra -Wpedantic -Wconversion -g main.c -o main.exe
//  .\main.exe

#include <stdio.h>

#define MAX_HOURS 23
#define MIN_SURGE 1.0
#define DENIED_CAPACITY 5


int scanf_data(double* dist, int* dur, int* hour, int* pass, int* lugg, double* sur, double* payment) {

    printf("distance:\t");
    if(scanf("%lf", dist) != 1) return 0;

    printf("duration:\t");
    if(scanf("%d", dur) != 1) return 0;

    printf("start_hour:\t");
    if(scanf("%d", hour) != 1) return 0;

    printf("passengers:\t");
    if(scanf("%d", pass) != 1) return 0;

    printf("luggage:\t");
    if(scanf("%d", lugg) != 1) return 0;

    printf("surge:\t\t");
    if(scanf("%lf", sur) != 1) return 0;

    printf("payment:\t");
    if(scanf("%lf", payment) != 1) return 0;

    return 1;

}


int validate(double dist, int dur, int hour, int pass, int lugg, double sur, double payment) {

    if(dist < 0 || dur < 0 || hour > MAX_HOURS || hour < 0 || pass <= 0 || 
        lugg < 0 || sur < MIN_SURGE || payment < 0) {
        return 0;
    }

    return 1;

}


double basic_tariff(double dist, int dur) {

    return 3.5 + dist * 0.9 + dur * 0.12;

}


double night_coeff_foo(int hour) {

    return (hour < 6 || hour >= 23) ? 1.2 : 1.0;

}

double total_foo(double base, double n_coeff, double sur, double lugg_fee) {
    return  base * n_coeff * sur + lugg_fee; 
}

int get_total(int pass, double payment, double total, double surge) {
    if (pass >= DENIED_CAPACITY) return 0; //для DENIED_CAPACITY
    if (payment < total)         return 1; //для PAYMENT_REQUIRED
    if (surge > 1.5)             return 2; //для HIGH_SURGE
    return 3;                              //для COMPLETE
}


void print_all_data(double total, double per_pers, double change, int flag) {

    if(flag == 0) { 
        printf("DENIED_CAPACITY\n"); 
        return; 
    }

    printf("\nRESULT OF PROGRAMM:\t");
    if(flag == 1) {
        printf("PAYMENT_REQUIRED\n");
    }
    else if(flag == 2) {
        printf("HIGH_SURGE\n");
    }
    else if(flag == 3) {
        printf("COMPLETE\n");
    }

    printf("Data after the trip:\ntotal:\t\t%.2f\nper_person:\t%.2f\nchange:\t\t%.2f\n\n", 
        total, per_pers, change);

}

int main(void) {

    printf("Enter input data:\n");

    double distance, surge, payment;
    int duration, start_hour, passengers, luggage;

    if(!scanf_data(&distance, &duration, &start_hour, &passengers, &luggage, &surge, &payment)) {
        printf("Input error!"); 
        return 1; 
    }

    int first_flag = validate(distance, duration, start_hour, passengers, luggage, surge, payment);
    if(!first_flag) {
        printf("Bad data. Try again!\n");
        return 1;
    }

    double base = basic_tariff(distance, duration);

    double night_coeff = night_coeff_foo(start_hour);

    double luggage_fee = luggage * 1.5;

    double total = total_foo(base, night_coeff, surge, luggage_fee);

    double per_person = total / (double)passengers;

    double change = payment - total;

    int second_flag = get_total(passengers, payment, total, surge);

    //если пассажиров слишком много, нет смысла печатать данные
    if (second_flag == 0) {
        print_all_data(0.0, 0.0, 0.0, second_flag);
        return 0;
    }

    print_all_data(total, per_person, change, second_flag);

    return 0;
}