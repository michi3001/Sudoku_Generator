/*
 * File:   main.c
 * Baden-Wuerttemberg Cooperative State University
 * Created on 23. Januar 2023, 18:58
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "stack.h"

// ******************************** GLOBAL VARIABLES **********************************************************
char cField[9][9];      //Multidimesional Array: 1st dim=row, 2nd dim=column !!!!!define Size 

// ******************************* FUNCTIONPROTOTYPES *********************************************************
void printBoard(char*);
void changeFieldValue(char[], int);
void fillField();
void fillFieldRandom();
int checkRules(char, int, int);
void userInputPlayerName(char*);
void userInputCoordinateAndValue();
int getRandomInteger(int, int);
int checkNumberIsUnique(char[], char);

// ************************************** MAIN ***************************************************************
int main(int argc, char** argv)
{
    srand((unsigned) time(NULL));   //has to be called only once
    char cPlayername[20];

    fillField();
    userInputPlayerName(cPlayername);       //Array is already a pointer due to this you musn't give the adress of it with &cPlayername
    printBoard(cPlayername);

    fillFieldRandom();
    printBoard(cPlayername);

    /* for(int i = 0; i < 1; i++) {
        userInputCoordinateAndValue();
        printBoard(cPlayername);
    } */

    return (EXIT_SUCCESS);
}


// **************************************** FUNCTIONS ********************************************************
void printBoard(char* p_Playername) { //Outputs the Sudoku Board on the Console
    int iArrayRowCount = 0;            //Counter which shows in which Row of the Field Array we are

    printf("\n\nSpielername: %s\n", p_Playername);
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

void changeFieldValue(char pCoordinate[2], int pValue) {    //Decodes the User Coordinate Input and changes the Value of the Field
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

void fillField() {  //Fills the Array with *
    for(int i = 0; i < 9; i++) {
        for(int x = 0; x < 9; x++) cField[i][x] = '*';
    }
    return;
}

void fillFieldRandom() {    //Fills the Array with Random Numbers
    for(int i = 0; i < 81; i++) {
        int bool = 0;
        do {
            int iRandom = getRandomInteger(0, 80);
            int iRandomRow = iRandom / 9;
            int iRandomCol = iRandom % 9;
            if (cField[iRandomRow][iRandomCol] == '*') {
                char cFieldValue = '1';
                while((checkRules(cFieldValue, iRandomRow, iRandomCol) == 0) && cFieldValue < (9+'0')) {
                //Problem hier ist dass er die Schleife beendet sobald 9 erreicht wurde und dann einfach x eingetragen wird
                //dies geschieht weil er irgendwann keine Zahl mehr findet, die den Regeln entspricht
                //Dann wird aktuell noch ein 'x' eingetragen aber hier kommt dann Backtracking ins Spiel
                    cFieldValue++;
                 }
                if (cFieldValue == (9+'0')) cField[iRandomRow][iRandomCol] = 'x';
                else cField[iRandomRow][iRandomCol] = cFieldValue;

                bool = 1;
            }
        } while(bool==0);
        printBoard(&"foo");         //Just for the Test Case
    }
}

int checkRules(char pFieldValue, int pRandomRow, int pRandomCol) {
    int bool = 0;
    char cRow[9], cCol[9], cMatrix[9];

    //make an array for all the numbers in the same row
    for(int z=0; z<9;z++) cRow[z] = cField[pRandomRow][z];

    //make an array for all the numbers in the same column
    for(int x=0; x<9; x++) cCol[x] = cField[x][pRandomCol];

    //make an array for all the numbers in the same 3x3 field
    int iFieldIdxHorizontal, iFieldIdxVertical;
    if(pRandomRow >= 0 && pRandomRow <= 2) iFieldIdxVertical = 0;
    else if(pRandomRow >= 3 && pRandomRow <= 5) iFieldIdxVertical = 3;
    else if(pRandomRow >= 6 && pRandomRow <= 8) iFieldIdxVertical = 6;

    if(pRandomCol >= 0 && pRandomCol <= 2) iFieldIdxHorizontal = 0;
    else if(pRandomCol >= 3 && pRandomCol <= 5) iFieldIdxHorizontal = 3;
    else if(pRandomCol >= 6 && pRandomCol <= 8) iFieldIdxHorizontal = 6;


    int iCounter = 0; //is needed to fill the array with the correct indexes
    for(int m=0; m<3; m++) {    //The 3x3 Matrix, in which the chosen random field lays, is picked here from the 9x9 Field and converted into an array
        for(int n=0; n<3; n++) {
            cMatrix[iCounter] = cField[iFieldIdxVertical+m][iFieldIdxHorizontal+n];
            iCounter++;
        }
    }

    if((checkNumberIsUnique(cRow, pFieldValue) == 1) && (checkNumberIsUnique(cCol, pFieldValue) == 1) && (checkNumberIsUnique(cMatrix, pFieldValue) == 1)) bool = 1;   

    return bool;
}

void userInputPlayerName(char *p_Playername) {    //Read the players name
    printf("Please give your name: ");
    scanf("%s", p_Playername);
    return;
}

void userInputCoordinateAndValue() { //User inputs a Coordinate and the Value for that field
    char cCoordinate[2];                //1st element=column, 2nd element=row
    int iValue = 0;                     //This Integer stores the value which has to be changed in field xy

    printf("Give the Coordinate and the value (Format ColRow  value  e.g. b5 7): ");
    scanf("%s %d", cCoordinate, &iValue);
    changeFieldValue(cCoordinate, iValue);
    return;
}

int getRandomInteger(int pLower, int pUpper) {    //Random Integer Generator
    int num = (rand() % (pUpper - pLower + 1)) + pLower;
    return num;
}

int checkNumberIsUnique(char arr[9], char cNumber) { //Tests if all numbers of the array are unique
    int i, j;
    int bool=1; //True = the number isn't already in the array

    for(int i=0; i<9; i++) {
        if(arr[i] == cNumber) bool = 0;
    }

    return bool;
}