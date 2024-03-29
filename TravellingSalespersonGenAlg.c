/* Projeto 1(IIA-UnB), 22/09/2020
    Regina Emy Da nóbrega Kamada,190037351*/


#include<stdlib.h>
#include<stdio.h>
#include<time.h>

//chromossome has only 10 but fitness adds return
typedef struct  chromossome  {

int path[10];
int fitness;
int length;

struct chromossome* next;
int chromossomeKey;
}chromossome;

typedef struct population{
    struct chromossome** currentGeneration;
    int populationSize;
    int fitnessSum;
}population;



//A matrix containing the distances between every city
// Each index is a city as follows: SP BA RJ Lima Bog. Sant. Carac. BH PoA BsB
int distances [10][10] = {

    {  -1, 17, 3 , 35, 43, 26, 44, 5 , 8 , 9    }, // 0 = SP
    {  17, -1, 20, 31, 47, 11, 51, 22, 8 , 23   }, // 1 = Ba
    {  3 , 20, -1, 38, 45, 29, 45, 3 , 11, 9    }, // 2 = RJ
    {  35, 31, 38, -1, 19, 25, 27, 36, 33, 32   }, // 3 = Lima
    {  43, 47, 45, 19, -1, 43, 10, 43, 46, 37   }, // 4 = Bogotá
    {  26, 11, 29, 25, 43, -1, 49, 30, 19, 30   }, // 5 = Santiago
    {  44, 51, 45, 27, 10, 49, -1, 42, 48, 35   }, // 6 = Caracas
    {  5 , 22, 3 , 36, 43, 30, 42, -1, 13, 6    }, // 7 = BH
    {  8 , 8 , 11, 33, 46, 19, 48, 13, -1, 16   }, // 8 = Pot.Alegre
    {  9 , 23, 9 , 32, 37, 30, 35, 6 , 16, -1   }, // 9 = Brasília
    // 0   1   2   3   4   5   6   7   8
                            } ;

//creates given number of random chromossomes
void initiatePopulation(population *start);

//calculates specific fitness relative to the sum of all path lengths
//recursive, sum on the way, fitness on the back (specific/total length)
// int evaluateFitness(chromossome* path, int fitnessSum);

// prints chromossome as a path between cities
void printPath(chromossome* path);

int calculateLength( int *path);

chromossome* ANSWER = NULL;



// helper functions
        //An utility function to get a random number withn given range
        int randomInt( int minimum_number, int max_number){
           return ( rand() % (max_number + 1 - minimum_number) + minimum_number) ;
        }

        // An utility function to print an array
        void printArray (int* array) ;
        void printArray (int* array)
        {
            int size = sizeof(array)/ sizeof(array[0]);
            size = 10;
            for (int i = 0; i < size; i++)
                printf("%d ", array[i]);
            printf("\n");
        }

        void printChromossome (chromossome* chromossome) {

            printArray( chromossome->path );
            printf(" key:%d fitness:%d length:%d \n", chromossome->chromossomeKey,chromossome->fitness, chromossome->length);

        }


        void allocateCromossome( chromossome** new );
        void destroyCromossome();

        // An utility function to swap to integers
        void swapPlaces (int *indexA, int *indexB) ;
        void swapPlaces (int *indexA, int *indexB)
        {
            int aux = *indexA;
            *indexA = *indexB;
            *indexB = aux;
        }



        // A function to generate a random permutation of chromossome*


void setStandardChromossome(chromossome* chromossome, int key);

// Utility function to alocate chromossomes
        void allocateCromossome(chromossome** new){

            (*new) = (chromossome*)malloc(sizeof(chromossome)) ;

            if( (*new) == NULL )
                printf("Warning: Can't alocate struct. Is the memory sufficient?");

        }


//main functions

void setStandardChromossome(chromossome* chromossome, int key){

                chromossome->fitness = 0;
                chromossome->length = 0;
                chromossome->chromossomeKey = key;

                chromossome->next = NULL;

                for (int i = 0; i < 10; i++)
                {
                    chromossome->path[i] = i;
                }

}

void copyPath(chromossome* copy, chromossome* copied, int fromIndex, int toIndex);
void copyPath(chromossome* copy, chromossome* copied, int fromIndex, int toIndex){
    for (int i = fromIndex; i <= toIndex; i++)
    {
        copy->path[i] = copied->path[i] ;
    }
    
}

void initiatePopulation (population *start);
void initiatePopulation (population *start){

    chromossome** chromossome ;
    int i, j, chromossomeKey, size = 10;

    printf(">>Initiating population... \n");

    // Use a different seed value so that we don't get same
    // result each time we run this program
    srand ( time(NULL) );

    start->currentGeneration = malloc(sizeof(*chromossome) * start->populationSize);
    if ( start->currentGeneration == NULL)    
        printf("Erro na alocação de memória.");

    chromossome = (start->currentGeneration);
    //Alocates given number of chromossomes
    for ( chromossomeKey = 0; chromossomeKey < start->populationSize; chromossomeKey++)
    {
        allocateCromossome( chromossome );
        setStandardChromossome( *chromossome, chromossomeKey );

        // Start from the last element and swap one by one. We don't
        // need to run for the first element that's why i > 0
        for ( i = size-1; i > 0; i--)
        {
            // Pick a random index from 0 to i
             j = rand() % (i+1);

            // Swap chromossome[i] with the element at random index
            swapPlaces(&(*chromossome)->path[i], &(*chromossome)->path[j]);
        }

        printArray((*chromossome)->path);
        chromossome = &(*chromossome)->next;    /*CAN USE INDEX*/
    }


    printf(">>Population initiated \n");

}

//Reads chromossome->path, seeks data on distances matrix and sums path distance
// sets individuals fitness ( 1/length ) on the way to the last chromossome
// sets idividual probability on the way back
int evaluateFitness(chromossome* chromossome, int fitnessSum );
int evaluateFitness(chromossome* chromossome, int fitnessSum ){

    if (chromossome != NULL){

        chromossome->length = calculateLength( chromossome->path );       

        chromossome->fitness = 1000000 /chromossome->length ;
        fitnessSum += chromossome->fitness;

        fitnessSum = evaluateFitness( chromossome->next, fitnessSum );


//chromossome is marked as answer if it's the smallest one
        if ( chromossome->length < ANSWER->length || ANSWER->length == 0) {
            ANSWER->length = chromossome->length;
            *ANSWER = *chromossome;

        }
        printf(">>Evaluated fitness of:\n");
        printChromossome(chromossome);
    }

 return fitnessSum;
}


void freeMemory( population* population);
void freeMemory( population* population) {
    chromossome** auxi = population->currentGeneration;
    chromossome** auxii;

    if ( population->currentGeneration && (*population->currentGeneration)->next != NULL)
        auxii = population->currentGeneration;

    while ( *auxi != NULL){
        free(*auxi);
        auxi = auxii;

        if(*auxii != NULL)
            auxii = &(*auxii)->next;
    }

    free ( population->currentGeneration );
}

void reverseArray(int* arr, int start, int end)
{
    int temp;

    while (start < end)
    {
        temp = arr[start];
        arr[start] = arr[end];
        arr[end] = temp;
        start++;
        end--;
    }
}

void crossover(chromossome* parent1, chromossome* parent2);
void crossover(chromossome* parent1, chromossome* parent2){

}

//first a very varied breeding, then selected
// mutates chromossome according to option
// since the first city must be counted twice in the path, 
// a paticular option is given to check wether it'd be bes
void mutation( chromossome** chromossome,int opt);
void mutation( chromossome** chromossome,int opt){
    int indexA, indexB;
    indexA = indexB;


    switch (opt)
    {
    case 0:

        reverseArray( (*chromossome)->path, 0, 9);
        break;
    case 1:

        indexA = randomInt( 0, 9);
        indexB = randomInt( 0, 9);

        swapPlaces( &((*chromossome)->path[indexA]), &((*chromossome)->path[indexB]) );

        break;
    case 3:

       while ( indexA >= indexB){
        indexA = randomInt( 0, 9);
        indexB = randomInt( 0, 9);}

        reverseArray( (*chromossome)->path, indexA, indexB);
        break;
    case 4:

        for (int i = 0; i < randomInt( 1, 4); i++)
            mutation( chromossome, randomInt( 0, 3) );

        break;
    }

}

//stimulates population quality by replacing
// remarkably unfitting idividuals
//with permutations of desirable ones

//check if new values are according, fitness, NEXT CHROMOSSOME
void  rehabilitation( population* population );
void  rehabilitation( population* population ){

    chromossome** aux = population->currentGeneration;
    chromossome* keepInLine;



    while( *aux != NULL){
        
        if ( (*aux)->fitness <= (ANSWER->fitness /2) )
        {
            if ( *aux == NULL)
            {
                return;
            }
            
            printf("\n                In Need Of Correction\n");
            
            keepInLine = (*aux)->next;
            setStandardChromossome( *aux, (*aux)->chromossomeKey );
            copyPath( *aux, ANSWER, 0, 9);
            
            
            (*aux)->next = keepInLine; 
            mutation( aux, randomInt( 0, 4));

            printChromossome( *aux );
        }

        aux = &(*aux)->next;
    }


    printf("reformed");

}

int setProbabilityWheel( chromossome** chromossome, int probabilitySum);
int setProbabilityWheel( chromossome** chromossome, int probabilitySum){
    probabilitySum = 0;

    if ( *chromossome != NULL){
        probabilitySum = setProbabilityWheel( &(*chromossome)->next, probabilitySum);
        probabilitySum += (*chromossome)->fitness;
        (*chromossome)->fitness = probabilitySum ;
    }

    return probabilitySum ;
}

chromossome** searchByFitness( chromossome** head, int fitness, int wheel){
    chromossome** aux = head;

    while ( (*aux)->fitness >= fitness)
    {
        if ( (*aux)->next != NULL && (*aux)->fitness > fitness)
        aux = &(*aux)->next;
    }
 return aux;
}

int findMissingGene( chromossome* chromossome){
    int total, i;
    total = (9 + 1) * (9 + 2) / 2;
    for (i = 0; i < 9; i++)
        total -= chromossome->path[i];
    return total;
 
}
void correctPath( chromossome* child, chromossome* parent, int index ){
    int i,j,k;
    for ( i = 0; i < 10; i++)
    {
        for ( j = 0; j < 10; j++)
        {
            if (child->path[j] == child->path [i]){
                child->path[j] = 
                
                for ( k = 0; k < 10; k++)
                {
                    if ( parent->path[k] == child->path[j]){
                        
                    }
                }
                
            }
        }
        
    }
    
}

void fitnessProportionateCrossover( chromossome** generationHead, int fitnessSum){
    chromossome** parentOne;
    chromossome** parentTwo;
    chromossome childOne;
    chromossome childTwo;
    int index = randomInt( 1, 9);

    int wheel = setProbabilityWheel( generationHead, 0);

    parentOne = searchByFitness( generationHead, randomInt(0, wheel), wheel);
    parentTwo = searchByFitness( generationHead, randomInt(0, wheel), wheel);

    copyPath( &childOne, (*parentOne), 0, index);
    copyPath( &childTwo, (*parentTwo), 0, index);

    copyPath( &childOne, (*parentTwo), index, 9);
    copyPath( &childTwo, (*parentOne), index, 9);
    
    correctPath( &childOne, (*parentOne), index);
    copyPath( (*parentOne), &childOne, 0, 9);
    copyPath( (*parentTwo), &childTwo, 0, 9);



}

void selectByProbability();

void newGeneration( population* population);
void newGeneration( population* population){
    chromossome** aux;
    aux = population->currentGeneration;
    chromossome** nextGeneration;
      
    fitnessProportionateCrossover( aux, population->fitnessSum );



    // setProbabilities( population);
    
    for (int i = 0; i < population->populationSize / 2; i++)
    {
        mutation( aux, randomInt(0,4));
        aux = &(*aux)->next;
    }

    // for ( i = 0; i < population->populationSize; i++)
    // {
    //     /* code */
    // }
    

    // turns remarkably undesirable idivuduals into mutated copies of ANSWER    
        rehabilitation( population );

}


int calculateLength( int *path){
    int length = 0;
    int fromCity;
    int toCity;

            for (int i = 0; i < 9; i ++ )
        {
            fromCity = path[i] ;
            toCity = path[i + 1] ;
            length += distances [ fromCity ] [ toCity ] ;

        }
        //considers return to original city
        fromCity = path[0];
        length += distances [toCity] [fromCity];
       
        return length;
}

int main ()
{
    population population;
    allocateCromossome( &ANSWER );
  
    population.populationSize = 20;
    // population.populationSize = 79999;
// for (int i = 0; i < 10; i++)
// {                            //FOR BRUTE RANDOM ASSESSING
//         initiatePopulation( &population);
// }


    
    // int  aTest[ 10 ] = { 8, 1, 5, 3, 4, 6, 9, 7, 2, 0 };
    // int aTestq[ 10 ] = { 6, 4, 3, 5, 1, 8, 0, 2, 7, 9 };
    // int aTestw[ 10 ] = { 7, 9, 6, 4, 3, 5, 1, 8, 0, 2 };
    // int aTeste[ 10 ] = { 2, 0, 8, 1, 5, 3, 4, 6, 9, 7 };
    // int aTestr[ 10 ] = { 1, 8, 0, 2, 7, 9, 6, 4, 3, 5 };
    // int aTestt[ 10 ] = { 3, 4, 6, 9, 7, 2, 0, 8, 1, 5 };
    // int aTesty[ 10 ] = { 8, 1, 5, 3, 4, 6, 9, 7, 2, 0 };
    // int aTestu[ 10 ] = { 4, 6, 9, 7, 2, 0, 8, 1, 5, 3 };
    // int aTesti[ 10 ] = { 8, 0, 2, 7, 9, 6, 4, 3, 5, 1 };
    // printf(" %d \n", calculateLength(aTest ) );
    // printf(" %d \n", calculateLength(aTestq)  );
    // printf(" %d \n", calculateLength(aTestw)  );
    // printf(" %d \n", calculateLength(aTeste)  );
    // printf(" %d \n", calculateLength(aTestr)  );
    // printf(" %d \n", calculateLength(aTestt)  );
    // printf(" %d \n", calculateLength(aTesty)  );
    // printf(" %d \n", calculateLength(aTestu)  );
    // printf(" %d \n", calculateLength(aTesti)  );


    initiatePopulation( &population);


    for (int i = 0; i < 1000; i++)
    {
        population.fitnessSum = evaluateFitness( *(population.currentGeneration), 0 );

        newGeneration( &population);

    }


    // printf("            @@ ANSWER %f \n", rand()/(double)RAND_MAX);
    printf("         SHORTEST PATH: ");
    printChromossome(ANSWER);
    
    
    printf(" %d KM \n", calculateLength(ANSWER->path) *100  );
    


    freeMemory(&population);
    free(ANSWER);


    return 0;
}
