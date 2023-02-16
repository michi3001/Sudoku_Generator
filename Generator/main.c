#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "stack.h"

// ******************************** GLOBAL VARIABLES **********************************************************
char cField[9][9];      //Multidimesional Array: 1st dim=row, 2nd dim=column !!!!!define Size 


// ******************************* FUNCTIONPROTOTYPES *********************************************************
void printBoard(char*);
void changeFieldValue(char[], int);
void clearField();
void fillFieldRandom(char*);
int checkRules(char, int, int);
void userInputPlayerName(char*);
void userInputCoordinateAndValue();
int getRandomInteger(int, int);
int checkNumberIsNotInArray(char[], char);


// ************************************** MAIN ***************************************************************
int main(int argc, char** argv)
{
    srand((unsigned) time(NULL));   //has to be called only once for generating random numbers
    char cPlayername[20];
    
    clearField();
    userInputPlayerName(cPlayername);       //Array is already a pointer due to this you musn't give the adress of it with &cPlayername
    printBoard(cPlayername);

    fillFieldRandom(cPlayername);
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
}//END printBoard()



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
}//END changeFieldValue()



void clearField() {  //Fills the Array with *
    for(int i = 0; i < 9; i++) {
        for(int x = 0; x < 9; x++) cField[i][x] = '*';
    }
    return;
}//END fillField()



void fillFieldRandom(char* p_Playername) {    //Fills the Array with Random Numbers
    int StackCoordinate;
    myStack_t* Stack = StackNew(sizeof(StackCoordinate), 81);
    if (IsStackEmpty(Stack) == 0) printf("\n!!!Error the Stack is not empty!!!\nPlease restart the program");
    int iCountNumbersInField = 0;
    int iCountBacktracks = 0;
    
    while(iCountNumbersInField < 81) {
        int iRandomCoordinate = getRandomInteger(0, 80);
        int iRandomRow = iRandomCoordinate / 9;
        int iRandomCol = iRandomCoordinate % 9;

        if (cField[iRandomRow][iRandomCol] == '*') {   //Find Empty Fields         
            char cFieldValue = '0';
            do {
                cFieldValue++;
            } while ((checkRules(cFieldValue, iRandomRow, iRandomCol) == 0) && cFieldValue <= (9+'0')); //break if Rules are correct and the Value <= 9

            if (cFieldValue > (9+'0')) {   //Backtrack Case
                iCountBacktracks++;

                for(int u=0; u < iCountBacktracks; u++) {   //Clear the last x fields (x = Number of Backtracks until now)
                    Pop(Stack, &StackCoordinate);         
                    iRandomRow = StackCoordinate / 9;
                    iRandomCol = StackCoordinate % 9;
                    cField[iRandomRow][iRandomCol] = '*';
                    iCountNumbersInField--;
                        
                    if (IsStackEmpty(Stack) == 1) {     //If the Stack is Empty clear the field
                    clearField();
                    iCountBacktracks = 0;
                    }
                }   
            }
            else {
                StackCoordinate = iRandomCoordinate;
                iCountNumbersInField++;                          //Counter which shows how much numbers are on the field at the moments
                cField[iRandomRow][iRandomCol] = cFieldValue;
                Push(Stack, &StackCoordinate);
            }               
        }//END if empty field
    }//END while(iCountNumbersInField < 81)
    StackDestroy(Stack);
}//END fillFieldRandom()



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

    if((checkNumberIsNotInArray(cRow, pFieldValue) == 1) && (checkNumberIsNotInArray(cCol, pFieldValue) == 1) && (checkNumberIsNotInArray(cMatrix, pFieldValue) == 1)) bool = 1;   

    return bool;
}//END checkRules()



void userInputPlayerName(char *p_Playername) {    //Read the players name
    printf("Please give your name: ");
    scanf("%s", p_Playername);
    return;
}//END userInputPlayerName()



void userInputCoordinateAndValue() { //User inputs a Coordinate and the Value for that field
    char cCoordinate[2];                //1st element=column, 2nd element=row
    int iValue = 0;                     //This Integer stores the value which has to be changed in field xy

    printf("Give the Coordinate and the value (Format ColRow  value  e.g. b5 7): ");
    scanf("%s %d", cCoordinate, &iValue);
    changeFieldValue(cCoordinate, iValue);
    return;
}//END userInputCoordinateAndValue()



int getRandomInteger(int pLower, int pUpper) {    //Random Integer Generator
    int num = (rand() % (pUpper - pLower + 1)) + pLower;
    return num;
}//END getRandomInteger()



int checkNumberIsNotInArray(char arr[9], char cNumber) { //Tests if all numbers of the array are unique
    int bool=1; //True = the number isn't already in the array
    
    for(int i=0; i<9; i++) {
        if(arr[i] == cNumber) bool = 0;
    }
    return bool;
}//END checkNumberIsUnique()