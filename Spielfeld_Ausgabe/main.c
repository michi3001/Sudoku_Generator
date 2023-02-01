/* 
 * File:   main.c
 * Baden-Wuerttemberg Cooperative State University
 * Created on 23. Januar 2023, 18:58
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ******************************** GLOBAL VARIABLES **********************************************************
char cField[9][9];      //Multidimesional Array: 1st dim=row, 2nd dim=column !!!!!
char cPlayername[20];


// ******************************* FUNCTIONPROTOTYPES *********************************************************
void printBoard();
void changeFieldValue(char[], int);
void fillField();
void fillFieldRandom();
void userInputPlayerName();
void userInputCoordinateAndValue();
int getRandomInteger(int, int);

// ************************************** MAIN ***************************************************************
int main(int argc, char** argv)
{  
    srand((unsigned) time(NULL));   //has to be called only once

    fillField();
    userInputPlayerName();
    printBoard();
    
    fillFieldRandom();
    printBoard();
    
    for(int i = 0; i < 1; i++) {
        userInputCoordinateAndValue();    
        printBoard();
    } 
  
    return (EXIT_SUCCESS);
}



// **************************************** FUNCTIONS ********************************************************
void printBoard() {
    int iArrayRowCount = 0;         //Counter which shows in which Row of the Field Array we are
    
    printf("\n\nSpielername: %s\n", cPlayername);
    printf("   abc def ghi\n");                                                     //Prints the col letters
    for(int i = 0; i < 13; i++) {
        if(i==0 || i==4 || i==8 || i==12) printf("   --- --- --- \n");              //Prints the -----------------
        else {
            printf("%d ", iArrayRowCount + 1);                                      //Prints the row numbers
            for(int x = 0; x < 9; x++) {
                if(x==0 || x==3 || x==6) printf("|%c", cField[iArrayRowCount][x]);  //Prints the lines with the field values
                else printf("%c", cField[iArrayRowCount][x]);          
            }            
        printf("|\n");  
        iArrayRowCount++;       //increment the ArrayCounter if one field value line was printed (else statement)
        }
    }
    return;
}

void changeFieldValue(char pCoordinate[2], int pValue) {
    int iColumn = 0, iRow = 0;
    //pCoordinate[0] = Column;       pCoordinate[1] = Row 
    //Get Column
    if(pCoordinate[0] >= 65 && pCoordinate[0] <= 73) iColumn = pCoordinate[0] - 65;
    else if(pCoordinate[0] >= 97 && pCoordinate[0] <= 105) iColumn = pCoordinate[0] - 97;
    else printf("\n!!!Your input was incorrect!!!");   
    
    //Get Row
    if(pCoordinate[1] >= 49 && pCoordinate[1]<=57) iRow = pCoordinate[1] - 49;
    else printf("\n!!!Your input was incorrect!!!");
    
    cField[iRow][iColumn] = pValue + 48;
    return;
}

void fillField() {
    for(int i = 0; i < 9; i++) {
        for(int x = 0; x < 9; x++) {
            //cField[i][x]= x+49;
            cField[i][x] = '*';
        }
    } 
    return;
}

void fillFieldRandom() {
    for(int i = 0; i < 81; i++) {
        int bool = 0;
        do {
            int iRandom = getRandomInteger(0, 80);
            int iRandomRow = iRandom / 9;
            int iRandomCol = iRandom % 9;
            if (cField[iRandomRow][iRandomCol] == '*') {
                cField[iRandomRow][iRandomCol] = (getRandomInteger(1, 9) + 48);
                bool = 1;
            }
        } while(bool==0);
    }
}

void userInputPlayerName() {
    printf("Please give your name: ");
    scanf("%s", cPlayername);
    return;
}

void userInputCoordinateAndValue() {
    char cCoordinate[2];                //1st element=column, 2nd element=row
    int iValue = 0;                     //This Integer stores the value which has to be changed in field xy
    
    printf("Give the Coordinate and the value (Format ColRow  value  e.g. b5 7): ");
    scanf("%s %d", cCoordinate, &iValue);
    changeFieldValue(cCoordinate, iValue);
    return;
}

int getRandomInteger(int pLower, int pUpper) {    
    int num = (rand() % (pUpper - pLower + 1)) + pLower;
    return num;    
}
