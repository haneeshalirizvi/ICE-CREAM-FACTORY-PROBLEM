#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>
#include <time.h>

#define SMALL 5.05
#define MEDIUM 6.55
#define LARGE 8.75
#define CHOCOLATE 2.00
#define VANILLA 2.00
#define STRAWBERRY 2.00
#define CHOCOLATECHIP 1.58
#define SPRINKLES 0.98
#define CUP 2.24
#define CONE 1.12

typedef struct {
    int id;
    char *flav;
    char *top;
    char *size;
    char *choice;
    double price;
} icecream;

int tickets = 50;
int sizes[3] = {25, 20, 15};
int flavors[3] = {31, 28, 24};
int toppings[2] = {26, 28};
int choices[2] = {23, 30};
double revenue = 0.0;
int cust_count = 0;
int count = 0;

sem_t ticket_sem, size_sem, flavor_sem, topping_sem, choice_sem, payment_sem;

void displayMenu();
void *takeOrders(void *arg);
void deliverIcecream(icecream *ID);
void *processOrders(void *arg);
void invoice();
void writeCustomerHistory(icecream *c);

int main() {
    // Start timing
    clock_t start = clock();

    srand(time(NULL));
    int N;
    int choice;
    printf("icecream parlour\n\n");
    do {
        choice = 0;
        printf("Enter Number of Cones you want [1 - %d]:  ", tickets);
        scanf("%d", &N);
        fflush(stdin);

        if (N > tickets || N <= 0) {
            printf("\nWarning: Invalid input\nDo you want to enter again? (1 for yes): ");
            fflush(stdin);
            scanf("%d", &choice);
            fflush(stdin);
        }

    } while (choice == 1);
    displayMenu();
    icecream c[N];
    pthread_t th[N];
    sem_init(&ticket_sem, 0, 1);
    sem_init(&size_sem, 0, 1);
    sem_init(&flavor_sem, 0, 1);
    sem_init(&topping_sem, 0, 1);
    sem_init(&choice_sem, 0, 1);
    sem_init(&payment_sem, 0, 1);

    for (int i = 0; i < N; i++) {
        pthread_create(&th[i], NULL, &takeOrders, (void *)&c[i]);
    }

    for (int i = 0; i < N; i++) {
        pthread_join(th[i], NULL);
    }

    printf("Ice-Cream Making Process Started\n\n");
    
    pthread_t cust[N];
    for (int i = 0; i < N; i++) {
        pthread_create(&cust[i], NULL, &processOrders, (void *)&c[i]);
        sleep(1);
    }

    for (int i = 0; i < N; i++) {
        pthread_join(cust[i], NULL);
    }

    invoice();

    // End timing
    clock_t end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Execution Time: %f seconds\n", cpu_time_used);

    return 0;
}
void displayMenu() {
    printf("\n\n*MENU*\n\n\n");
    printf("SIZE      || FLAVOURS          || TOPPINGS         || CHOICES\n");
    printf("\n\n");
    printf("1.Small   || 1.Chocolate       || 1.chocolateChips || 1.cup\n");
    printf("2.Medium  || 2.Vanilla         || 2.sprinkles      || 2.cone\n");
    printf("3.Large   || 3.Strawberry\n\n\n");
}

float totalRevenue = 0.0;

void *takeOrders(void *arg) {
    int a;
    icecream *c = (icecream *)arg;
    long int threadID = pthread_self();
    cust_count++;
    c->id = cust_count;

    fflush(stdin);
    printf("\n---Worker Thread %ld taking order---\n", threadID);
    fflush(stdin);

    // Choose size
    printf("Choose size for ice cream %d (1: Small, 2: Medium, 3: Large): ", c->id);
    scanf("%d", &a);
    if (a == 1)
        c->size = "small";
    else if (a == 2)
        c->size = "medium";
    else if (a == 3)
        c->size = "large";

    // Calculate price based on size
    if (a == 1)
        c->price += SMALL;
    else if (a == 2)
        c->price += MEDIUM;
    else if (a == 3)
        c->price += LARGE;

    fflush(stdin);
    // Choose flavor
    printf("Choose flavor for ice cream %d (1: Chocolate, 2: Vanilla, 3: Strawberry): ", c->id);
    scanf("%d", &a);
    if (a == 1)
        c->flav = "chocolate";
    else if (a == 2)
        c->flav = "vanilla";
    else if (a == 3)
        c->flav = "strawberry";

    // Calculate price based on flavor
    c->price += CHOCOLATE + VANILLA + STRAWBERRY;

    fflush(stdin);
    // Choose toppings
    printf("Choose toppings for ice cream %d (1: Chocolate Chip, 2: Sprinkles): ", c->id);
    scanf("%d", &a);
    if (a == 1)
        c->top = "chocolateChip";
    else if (a == 2)
        c->top = "sprinkles";

    // Calculate price based on toppings
    if (a == 1)
        c->price += CHOCOLATECHIP;
    else if (a == 2)
        c->price += SPRINKLES;

    fflush(stdin);
    // Choose container
    printf("Choose container for ice cream %d (1: Cup, 2: Cone): ", c->id);
    scanf("%d", &a);
    if (a == 1)
        c->choice = "cup";
    else if (a == 2)
        c->choice = "cone";

    // Calculate price based on container
    if (a == 1)
        c->price += CUP;
    else if (a == 2)
        c->price += CONE;

    printf("\n---Customer [%d] entered into Order Queue---\n", c->id);
    printf("Total price for ice cream %d: $%.2f\n\n", c->id, c->price);

    // Add the price to total revenue
    totalRevenue += c->price;
    sem_wait(&payment_sem);
    revenue=totalRevenue;
    sem_post(&payment_sem);
    writeCustomerHistory(c);

    pthread_exit(NULL);
}

void deliverIcecream(icecream *ID) {
    // Simulate delivery with a random time between 01 and 20 minutes
    int time = rand() % 16 + 5;
    // payments counter - ENTER
    printf("\n**Delivering Order for Customer %d \n\n", ID->id);
    printf("Delivering %s icecream with %s topping in %s\n\n", ID->flav, ID->top, ID->choice);
    printf("Order for Customer %d Delivered\n", ID->id);
    count++;
}

void *processOrders(void *arg) {
    icecream *ID = (icecream *)arg;
    sem_wait(&ticket_sem);
    tickets--;
    sem_post(&ticket_sem);
    if (strcmp(ID->size, "small") == 0) {
        sem_wait(&size_sem);
        sizes[0]--;
        sem_post(&size_sem);
    } else if (strcmp(ID->size, "medium") == 0) {
        sem_wait(&size_sem);
        sizes[1]--;
        sem_post(&size_sem);
    } else {
        sem_wait(&size_sem);
        sizes[2]--;
        sem_post(&size_sem);
    }
    if (strcmp(ID->flav, "chocolate") == 0) {
        sem_wait(&flavor_sem);
        flavors[0]--;
        sem_post(&flavor_sem);
    } else if (strcmp(ID->flav, "vanilla") == 0) {
        sem_wait(&flavor_sem);
        flavors[1]--;
        sem_post(&flavor_sem);
    } else {
        sem_wait(&flavor_sem);
        flavors[2]--;
        sem_post(&flavor_sem);
    }
    if (strcmp(ID->top, "chocolateChip") == 0) {
        sem_wait(&topping_sem);
        toppings[0]--;
        sem_post(&topping_sem);
    } else {
        sem_wait(&topping_sem);
        toppings[1]--;
        sem_post(&topping_sem);
    }
    if (strcmp(ID->choice, "cup") == 0) {
        sem_wait(&choice_sem);
        choices[0]--;
        sem_post(&choice_sem);
    } else {
        sem_wait(&choice_sem);
        choices[1]--;
        sem_post(&choice_sem);
    }
    sem_wait(&payment_sem);
    revenue += ID->price;
    sem_post(&payment_sem);
    deliverIcecream(ID);
    sleep(2);
    pthread_exit(NULL);
}

void invoice() {
    time_t t;
    time(&t);

    printf("\n\n\t\t******** INVOICE ********\n\n");
    printf("Date & Time: %s\n\n", ctime(&t));
    printf("Total Customers:     %d\n", cust_count);
    printf("Successful Customers:%d\n", count);
    printf("Total Revenue:       $%.2f\n\n", revenue / 2);
    printf("___________________________________________\n");
    printf("              ITEM              |  QUANTITY\n");
    printf("-------------------------------------------\n");
    printf("Total Cup left                   |     %d\n", choices[0]);
    printf("Total Cone left                  |     %d\n", choices[1]);
    printf("Small size left                  |     %d\n", sizes[0]);
    printf("Medium size left                 |     %d\n", sizes[1]);
    printf("Large size left                  |     %d\n", sizes[2]);
    printf("Chocolate flavor left            |     %d\n", flavors[0]);
    printf("Vanilla flavor left              |     %d\n", flavors[1]);
    printf("Strawberry flavor left           |     %d\n", flavors[2]);
    printf("Chocolate Chip left              |     %d\n", toppings[0]);
    printf("Sprinkles left                   |     %d\n", toppings[1]);
    printf("-------------------------------------------\n");
    printf("Tickets Destroyed                |     %d\n", cust_count);
    printf("Tickets Left                     |     %d\n\n", tickets);
}
void writeCustomerHistory(icecream *ID) {
    FILE *file = fopen("customerhistorysem.txt", "a");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }
    fprintf(file, "Customer ID: %d\n", ID->id);
    fprintf(file, "Size: %s\n", ID->size);
    fprintf(file, "Flavor: %s\n", ID->flav);
    fprintf(file, "Toppings: %s\n", ID->top);
    fprintf(file, "Container: %s\n", ID->choice);
    fprintf(file, "Price: $%.2f\n\n", ID->price);
    fclose(file);
}
