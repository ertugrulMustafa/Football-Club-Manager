#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <semaphore.h>
#include <fcntl.h>


pthread_mutex_t checkAgent;
sem_t * negotiationSemaphore ;
sem_t * updateSemaphore ;




// Let us create a global variable to change it in threads
int g = 0;

int agents[4][4] = {
        {2, 2, 2, 1},
        {2, 2, 1, 1},
        {2, 1, 1, 2},
        {1, 1, 1, 1}
};

int agent0[4][10]; //Agent 1's players. Index 0 corresponds to forward players. index 3 corresponds to goalkeepers
// For example agent1[0][0] is the agent 1's first forward player's cost.
//  Another example agent1[1][1] is the agent 1's second midfielder player's cost.
int agent1[4][10];
int agent2[4][10];
int agent3[4][10];

//int *agentPointer = &agents;
struct arg_struct {
    char name; //club name
    int bud; //club budget
};

struct scoutStruct {
    int ID; //scout ID
};

struct transferData {
    int agentID;
    int position;
    int numberOfAttempts;
    char clubName;
};

int assignBudget();

int getRandomPlayerCost();

void setInitialCost(int agent[4][10], int agentNumber);

void addPlayer(int agent[4][10], int agentNumber);

void updateCost(int agent[4][10], int agentNumber);

void printArray(int ourArray[4][10]);

int transferPlayer(int clubBudget, struct transferData transferList[]);

void deletePlayer(int agentNumber, int position,int deleteIndex);

//this struct is needed for passing multiple arguments into a thread




void *createClub(void *arguments) {
    struct arg_struct *args = (struct arg_struct *)arguments;

    char clubName = args->name;
    int budget = args->bud;


    printf("\nCreated Club %c with budget=%d\n",clubName, budget);

    //struct transferData *args = NULL;
    int i,agentNumber, PlayerPosition;
    struct transferData transferList[5];

    for(i=0; i<5; i++) {
        struct transferData tempArg;

        agentNumber = rand()%4;
        PlayerPosition= rand()%4;
        tempArg.position = PlayerPosition;
        tempArg.agentID = agentNumber;
        tempArg.numberOfAttempts = 0;
        tempArg.clubName = clubName;
        transferList[i] = tempArg;
    }
    //sleep(0.5);

    budget = transferPlayer(budget, transferList);

    printf("\nClub Thread %c finished. Remaining budget: %d\n", clubName, budget);

    //print transfer list
//    for(i=0; i<5; i++) {
//        printf("\nClub %c's transfer list: Index->%d, AgentNumber:%d, Position:%d\n",clubName, i, transferList[i].agentID, transferList[i].position);
//    }

    //printf("Club Name: %c\n", clubName);



    //printf("Club Budget: %d\n", budget);
    return NULL;
}

void *createScout(void *arguments) {

    struct scoutStruct *args = (struct scoutStruct *)arguments;
    int scoutID = args->ID;
    //sleep(4-scoutID);
    int randomNumber, addCounter=0, updateCounter=0;
    printf("\nScout Created ID: %d\n", scoutID);

    while(addCounter<7 & updateCounter<4) {
        randomNumber = rand()%6;

        switch (scoutID) {
            case 0:
                //printArray(agent0);
                if(randomNumber==0 |randomNumber==4 | randomNumber==5) {
                    addPlayer(agent0, scoutID);
                    addCounter++;
                }
                //printArray(agent0);
                if(randomNumber==1 | randomNumber==2 | randomNumber==3) {
                    updateCost(agent0, scoutID);
                    updateCounter++;
                }
                //printArray(agent0);
                break;

            case 1:
                //printArray(agent1);
                if(randomNumber==0 |randomNumber==4 | randomNumber==5) {
                    addPlayer(agent1, scoutID);
                    addCounter++;
                }
                //printArray(agent1);
                if(randomNumber==1 | randomNumber==2 | randomNumber==3) {
                    updateCost(agent1, scoutID);
                    updateCounter++;
                }

                //printArray(agent1);
                break;

            case 2:
                //printArray(agent2);
                if(randomNumber==0 |randomNumber==4 | randomNumber==5) {
                    addPlayer(agent2, scoutID);
                    addCounter++;
                }

                //printArray(agent2);
                if(randomNumber==1 | randomNumber==2 | randomNumber==3) {
                    updateCost(agent2, scoutID);
                    updateCounter++;
                }

                //printArray(agent2);
                break;

            case 3:
                //printArray(agent3);
                if(randomNumber==0 |randomNumber==4 | randomNumber==5) {
                    addPlayer(agent3, scoutID);
                    addCounter++;
                }

                //printArray(agent3);
                if(randomNumber==1 | randomNumber==2 | randomNumber==3) {
                    updateCost(agent3, scoutID);
                    updateCounter++;
                }

                //printArray(agent3);
                break;
        }


    }



    //while (true) {
    //sleep((rand()%15+1));
    return NULL;
}

void addPlayer(int agent[4][10], int agentNumber) {
    sleep(1);
    sem_wait(updateSemaphore);
    int forward, midfielder, defender, goalkeeper, playerCost, arrayIndex;
    forward= (rand() %4);
    midfielder= (rand() %4);
    defender= (rand() %4);
    goalkeeper= (rand() %4);

    if (forward == 1) {
        playerCost = getRandomPlayerCost();
        //printf("\nadding forward to %d with player cost %d", agentNumber, playerCost);
        agents[agentNumber][0] = agents[agentNumber][0] + 1;
        arrayIndex = agents[agentNumber][0];
        agent[0][arrayIndex-1] = playerCost;
    }
    if (midfielder == 1) {
        playerCost = getRandomPlayerCost();
        printf("\nadding midfielder to %d with player cost %d", agentNumber, playerCost);
        agents[agentNumber][1] = agents[agentNumber][1] + 1;
        arrayIndex = agents[agentNumber][1];
        agent[1][arrayIndex-1] = playerCost;
    }
    if (defender == 1) {
        playerCost = getRandomPlayerCost();
        printf("\nadding defender to %d with player cost %d", agentNumber, playerCost);
        agents[agentNumber][2] = agents[agentNumber][2] + 1;
        arrayIndex = agents[agentNumber][2];
        agent[2][arrayIndex-1] = playerCost;
    }
    if (goalkeeper == 1) {
        playerCost = getRandomPlayerCost();
        printf("\nadding goalkeeper to %d with player cost %d", agentNumber, playerCost);
        agents[agentNumber][3] = agents[agentNumber][3] + 1;
        arrayIndex = agents[agentNumber][3];
        agent[3][arrayIndex-1] = playerCost;
    }
    sem_post(updateSemaphore);
    sleep(1);

}

void updateCost(int agent[4][10], int agentNumber) {
    sem_wait(updateSemaphore);
    int i, j, playerCount=0, counter=0, playerCost = 0, increaseAmount;
    int increaseSize[] = {-500000, 1000000, 0};
    for (i=0; i<4; i++) {
        playerCount = agents[agentNumber][i];

        while(playerCount != 0) {
            //printf("\nBefore update %d ", agent[i][counter]);
            increaseAmount = increaseSize[rand() % 3];
            //printf("increase size %d ", increaseAmount);
            playerCost = agent[i][counter];

            playerCost += increaseAmount;

            if(playerCost <3000000 | playerCost > 22000000 ) {
                //printf("skipped update at this point %d", playerCount);
            }
            else{
                agent[i][counter] = playerCost;
                //printf("after update %d\n", agent[i][counter]);
            }
            playerCount--;
            counter++;
        }
        counter = 0;
    }
    printf("\nUpdating costs of Agent %d. \n", agentNumber);
    sem_post(updateSemaphore);
    sleep(1);
}

void printArray(int ourArray[4][10]) {
    int i,j,counter=0;
    for(i=0; i<4; i++) {
        printf("\nIndex = %d -> ", i);
        for(j=0; j<10; j++) {
            printf("%d, ",ourArray[i][j]);
        }
    }
    printf("\n");
}

void setInitialCost(int agent[4][10], int agentNumber) {
    int j, playerCount, playerCost, counter;
    counter = 0;

    for (j = 0; j < 4; j++) {
        playerCount = agents[agentNumber][j];
        while(playerCount != 0) {
            playerCost = getRandomPlayerCost();
            agent[j][counter] = playerCost;
            playerCount--;
            counter++;
        }
        counter = 0;
    }
    //printArray(agent);
    //printf("Agent Number %d and value is %d\n", agentNumber, agent[3][1]);
}

//return a random budget value
int assignBudget() {
    int budgetList[] = {10000000, 20000000,30000000, 40000000, 50000000 };
    int randomNumber = rand() % 5;

    return budgetList[randomNumber];
}


//generates a random num between 4m and 20m
int getRandomPlayerCost() {
    //
    int num = (rand() % 17) + 4;
    num *= 1000000;
    return num;
}


int findCheapest(int agentNumber, int position) {
    int i, playerCount, cheapest, currentPrice, cheapestIndex;
    playerCount = agents[agentNumber][position];

    switch(agentNumber) {
        case 0  :
            cheapest = agent0[position][0];
            for(i=0; i<playerCount; i++) {
                currentPrice = agent0[position][i];
                if (currentPrice<cheapest) {
                    cheapest = currentPrice;
                    cheapestIndex = i;
                }
            }
            break; /* optional */

        case 1  :
            cheapest = agent1[position][0];
            for(i=0; i<playerCount; i++) {
                currentPrice = agent1[position][i];
                if (currentPrice<cheapest) {
                    cheapest = currentPrice;
                    cheapestIndex = i;
                }
            }
            break; /* optional */
        case 2  :
            cheapest = agent2[position][0];
            for(i=0; i<playerCount; i++) {
                currentPrice = agent2[position][i];
                if (currentPrice<cheapest) {
                    cheapest = currentPrice;
                    cheapestIndex = i;
                }
            }
            break; /* optional */
        case 3  :
            cheapest = agent3[position][0];
            for(i=0; i<playerCount; i++) {
                currentPrice = agent3[position][i];
                if (currentPrice<cheapest) {
                    cheapest = currentPrice;
                    cheapestIndex = i;
                }
            }
            break;
    }
    return cheapest;


}
// special comparison function will determine the order of the sorted array
int compare(const void *x_void, const void *y_void) {
    int x = *(int *)x_void;
    int y = *(int *)y_void;
    return y - x;
}

int transferPlayer(int clubBudget, struct transferData transferList[]) {

    int i, j, cheapestPlayer, deleteIndex;
    for (i = 0; i < 5; i++) {
        sem_wait(updateSemaphore);
        sem_wait(negotiationSemaphore);

        for (j = 0; j < 2; j++) {
            int agentID = transferList[i].agentID;
            int position = transferList[i].position;

            if (transferList[i].numberOfAttempts > 2 | agents[agentID][position] == 0) {
                sem_post(updateSemaphore);
                sleep(3);
                sem_wait(updateSemaphore);
                printf("\nCouldn't transfer this player to Club %c because agent %d doesn't have a player in position %d. \n", transferList[i].clubName, agentID, position);
                transferList[i].numberOfAttempts = 0;
                continue;
            }

            transferList[i].numberOfAttempts++;
            cheapestPlayer = findCheapest(agentID, position);
            deleteIndex = agents[agentID][position] - 1;

            if (cheapestPlayer < clubBudget) {
                deletePlayer(agentID, position, deleteIndex);
                agents[agentID][position]--;
                clubBudget -= cheapestPlayer; //budgetı kulüpte de güncelle ya da çok da önemli değil
                transferList[i].numberOfAttempts = 0;
                sem_post(updateSemaphore);
                printf("\nTransferred Player with cost of %d to Club %c from Agent %d and position is %d\n", cheapestPlayer, transferList[i].clubName, agentID, position);
                break;

                sleep(0.5);
            }
            else {
                printf("\nCouldn't transfer player with cost of %d to Club %c from Agent %d and position is %d\n", cheapestPlayer, transferList[i].clubName, agentID, position);
                sem_post(updateSemaphore);
                sleep(4);
            }
        }
        sem_post(negotiationSemaphore);
        sleep(1);

    }
    return clubBudget;
}

void deletePlayer(int agentNumber, int position,int deleteIndex) {
    switch(agentNumber) {
        case 0:
            agent0[position][deleteIndex] = 0;
            qsort(agent0[position], 10, sizeof(int), compare);
            break;
        case 1:
            agent1[position][deleteIndex] = 0;
            qsort(agent1[position], 10, sizeof(int), compare);
            break;
        case 2:
            agent2[position][deleteIndex] = 0;
            qsort(agent2[position], 10, sizeof(int), compare);
            break;
        case 3:
            agent3[position][deleteIndex] = 0;
            qsort(agent3[position], 10, sizeof(int), compare);
            break;
    }
}



int main() {
    int i, budget, error;
    char clubNames[] = {'A', 'B', 'C', 'D', 'E'};
    srand(time(0));
    negotiationSemaphore = malloc(sizeof(sem_t));
    updateSemaphore = malloc(sizeof(sem_t));
    sem_init(negotiationSemaphore, 0, 2);
    sem_init(updateSemaphore, 0, 1);



    pthread_t clubThreads[5];
    pthread_t scoutThreads[4];

    pthread_attr_t attr; /* set of attributes for the thread */
    pthread_attr_init(&attr);

    /* Intializes random number generator */



    setInitialCost(agent0, 0);
    setInitialCost(agent1, 1);
    setInitialCost(agent2, 2);
    setInitialCost(agent3, 3);


    for(i = 0; i<4; i++) {
        qsort(agent0[i], 10, sizeof(int), compare);
        qsort(agent1[i], 10, sizeof(int), compare);
        qsort(agent2[i], 10, sizeof(int), compare);
        qsort(agent3[i], 10, sizeof(int), compare);
    }

//    //addPlayer(agent0, 0);
//    updateCost(agent0, 0);
    //printf("Agent Number %d and value is %d\n", 0, agent1[3][0]);
    struct arg_struct args; //to pass multiple arguments into a thread
    struct scoutStruct scoutArguments;





    for (i = 0; i<4; i++) {

    }
    //create 5 threads for 5 football clubs
    for (i = 0; i < 5; i++) {
        budget = assignBudget();
        args.name = clubNames[i];
        args.bud = budget;
        error = pthread_create(&clubThreads[i], NULL, createClub, (void *)&args);
        sleep(1);

        if(i!=4) {
            scoutArguments.ID = i;
            pthread_create(&scoutThreads[i], NULL, createScout, (void *)&scoutArguments);
            sleep(1);
        }
        if (error != 0)
            printf("\nThread can't be created :[%s]", strerror(error));

    }

//    for(i=0; i<5; i++) {
//        pthread_join(clubThreads[i], NULL);
//    }
//
//    for(i=0; i<4; i++) {
//        pthread_exit(&scoutThreads[i]);
//    }

    pthread_exit(NULL);
    //sem_close()
    return 0;
}
