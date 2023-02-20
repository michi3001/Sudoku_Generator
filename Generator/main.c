#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "stack.h"

struct Coordinate {
    int row;
    int column;
};

// ******************************** GLOBAL VARIABLES **********************************************************
char cField[9][9];      //Multidimesional Array: 1st dim=row, 2nd dim=column !!!!!define Size 
char cCopyOfField[9][9];
char cPlayerInputField[9][9];   //This field contains the fields which the Player has filled


// ******************************* FUNCTIONPROTOTYPES *********************************************************
void printBoard(char*);
void printSolution(char*);
void changeFieldValue(char[], int, myStack_t*, int*);
void clearField();
void fillFieldRandom(char*);
int checkRules(char, int, int);
void userInputPlayerName(char*);
void userInputCoordinateAndValue(myStack_t*, int*);
int getRandomInteger(int, int);
int checkNumberIsNotInArray(char[], char);
void makeCopyOfField();
void hideFields();
int UserInputMenu(char*, int*, myStack_t*, struct Coordinate*, int*);


// ************************************** MAIN ***************************************************************
int main(int argc, char** argv)
{
    srand((unsigned) time(NULL));   //has to be called only once for generating random numbers
    char cPlayername[20];
    int bCopiedOrOriginal = 1;  //0=Solution, 1=normal field 
    int bRulesActivated = 1;    //0=No, 1=Yes
    
    struct Coordinate sPlayerCoordinate;
    myStack_t* PlayerStack = StackNew(sizeof(sPlayerCoordinate), 81);
    
    clearField();
    userInputPlayerName(cPlayername);       //Array is already a pointer due to this you musn't give the adress of it with &cPlayername
    fillFieldRandom(cPlayername);    
    makeCopyOfField();    
    hideFields();
    
    int bool = 0;
    while(bool == 0) {
        bool = UserInputMenu(cPlayername, &bCopiedOrOriginal, PlayerStack, &sPlayerCoordinate, &bRulesActivated);     //irgendein Problem beim Backtrack, entweder Struct Übergabeproblem oder Stackübergabeproblem
    } 
    printf("\n\n!!!GAME ENDED!!!");    
    return (EXIT_SUCCESS);
}


// **************************************** FUNCTIONS ********************************************************
void printBoard(char* p_Playername) { //Outputs the Sudoku Board on the Console
    int iArrayRowCount = 0;            //Counter which shows in which Row of the Field Array we are

    printf("\n\n\n\n\n\nPlayername: %s\n", p_Playername);
    printf("   abc def ghi\n");                                                     //Prints the col letters
    for(int i = 0; i < 13; i++) {
        if(i==0 || i==4 || i==8 || i==12) printf("   --- --- --- \n");              //Prints the -----------------
        else {
            printf("%d ", iArrayRowCount + 1);                                      //Prints the row numbers
            for(int x = 0; x < 9; x++) {
                if(x==0 || x==3 || x==6) printf("|\x1B[32m%c\x1B[0m", cField[iArrayRowCount][x]);  //Prints the lines with the field values
                else printf("\x1B[32m%c\x1B[0m", cField[iArrayRowCount][x]);        //Original fields other colour than the fields which the 
                
                if(cPlayerInputField[iArrayRowCount][x] != '*') {                   //player put
                    if(x==0 || x==3 || x==6)  printf("|\b\b\x1B[33m%c\x1B[0m", cPlayerInputField[iArrayRowCount][x]);
                    else printf("\b\x1B[33m%c\x1B[0m", cPlayerInputField[iArrayRowCount][x]); 
                }
            }
        printf("|\n");
        iArrayRowCount++;       //increment the ArrayCounter if one field value line was printed (else statement)
        }
    }
}//END printOriginalBoard()

void printSolution(char* p_Playername) { //Outputs the Sudoku Board on the Console
    int iArrayRowCount = 0;            //Counter which shows in which Row of the Field Array we are

    printf("\n\n\n\n\n\nPlayername: %s\n", p_Playername);
    printf("   abc def ghi\n");                                                     //Prints the col letters
    for(int i = 0; i < 13; i++) {
        if(i==0 || i==4 || i==8 || i==12) printf("   --- --- --- \n");              //Prints the -----------------
        else {
            printf("%d ", iArrayRowCount + 1);                                      //Prints the row numbers
            for(int x = 0; x < 9; x++) {
                if(x==0 || x==3 || x==6) printf("|\x1B[32m%c\x1B[0m", cCopyOfField[iArrayRowCount][x]);  //Prints the lines with the field values
                else printf("\x1B[32m%c\x1B[0m", cCopyOfField[iArrayRowCount][x]);      //Belegte Felder Farbig ausgeben
            }
        printf("|\n");
        iArrayRowCount++;       //increment the ArrayCounter if one field value line was printed (else statement)
        }
    }
}//END printCopiedBoard()

void changeFieldValue(char pUserCoordinate[2], int pValue, myStack_t *p_PlayerStack, int* p_bRulesActivated) {    //Decodes the User Coordinate Input and changes the Value of the Field
    int iColumn = 0, iRow = 0;
    int bCorrect = 1;
    struct Coordinate sUserCoordinate;
    //pCoordinate[0] = Column;       pCoordinate[1] = Row
    //Get Column
    if(pUserCoordinate[0] >= 65 && pUserCoordinate[0] <= 73) iColumn = pUserCoordinate[0] - 65;
    else if(pUserCoordinate[0] >= 97 && pUserCoordinate[0] <= 105) iColumn = pUserCoordinate[0] - 97;
    else {
        printf("\n!!!Your input was incorrect!!!");
        bCorrect = 0;
    }

    //Get Row
    if(pUserCoordinate[1] >= 49 && pUserCoordinate[1]<=57) iRow = pUserCoordinate[1] - 49;
    else {
        printf("\n!!!Your input was incorrect!!!");
        bCorrect = 0;
    }
    
    
    if(pValue < 1 || pValue > 9) {
        printf("\n!!!Your input was incorrect!!!");
        bCorrect = 0;
    }
    
    if(bCorrect == 1) {
        if(cField[iRow][iColumn] == '*') {
            if(*p_bRulesActivated == 1) {
                if (checkRules((pValue+'0'), iRow, iColumn) == 0) printf("\n\nYour given value was incorrect");
                else if(checkRules((pValue+'0'), iRow, iColumn) == 1) {
                    cPlayerInputField[iRow][iColumn] = pValue + '0';
                    sUserCoordinate.row = iRow;
                    sUserCoordinate.column = iColumn;
                    Push(p_PlayerStack, &sUserCoordinate);
                }
            } 
            else if(*p_bRulesActivated == 0) {
                cPlayerInputField[iRow][iColumn] = pValue + '0';
                sUserCoordinate.row = iRow;
                sUserCoordinate.column = iColumn;
                Push(p_PlayerStack, &sUserCoordinate);
            }
        }
        else printf("\n\nYour given Coordinate is already filled");
    }
}//END changeFieldValue()

void clearField() {  //Fills the Array with *
    for(int i = 0; i < 9; i++) {
        for(int x = 0; x < 9; x++) {
            cField[i][x] = '*';
            cPlayerInputField[i][x] = '*';
        }
    }
}//END fillField()

void fillFieldRandom(char* p_Playername) {    //Fills the Array with Random Numbers
    struct Coordinate sStackCoordinate;
    myStack_t* Stack = StackNew(sizeof(sStackCoordinate), 81);

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
                    Pop(Stack, &sStackCoordinate);
                    iRandomRow = sStackCoordinate.row;
                    iRandomCol = sStackCoordinate.column;
                    cField[iRandomRow][iRandomCol] = '*';
                    iCountNumbersInField--;
                        
                    if (IsStackEmpty(Stack) == 1) {     //If the Stack is Empty clear the field
                    clearField();
                    iCountBacktracks = 0;
                    }
                }   
            }
            else {
                iCountNumbersInField++;                          //Counter which shows how much numbers are on the field at the moments
                cField[iRandomRow][iRandomCol] = cFieldValue;
                sStackCoordinate.row = iRandomRow;
                sStackCoordinate.column = iRandomCol;
                Push(Stack, &sStackCoordinate);
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
}//END userInputPlayerName()

void userInputCoordinateAndValue(myStack_t *p_PlayerStack, int* p_bRulesActivated ) { //User inputs a Coordinate and the Value for that field
    char cCoordinate[2];                //1st element=column, 2nd element=row
    int iValue = 0;                     //This Integer stores the value which has to be changed in field xy

    printf("Give the Coordinate and the value (Format ColRow  value  e.g. b5 7): ");
    scanf("%s %d", cCoordinate, &iValue);
    
    changeFieldValue(cCoordinate, iValue, p_PlayerStack, p_bRulesActivated);
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

void makeCopyOfField(){
    for(int i = 0; i < 9; i++) {
        for(int x = 0; x < 9; x++) cCopyOfField[i][x] = cField[i][x];
    }
}

void hideFields() {
    int iNumberHidden = getRandomInteger(17, 36);
    int bool;
    for(int i=0; i < iNumberHidden; i++) {
        bool = 0;
        do {
            int iRandomCoordinate = getRandomInteger(0, 80);
            int iRandomRow = iRandomCoordinate / 9;
            int iRandomCol = iRandomCoordinate % 9;
        
            if (cField[iRandomRow][iRandomCol] != '*') {   //Find Empty Fields         
                cField[iRandomRow][iRandomCol] = '*';
                bool = 1;
            }
        } while(bool == 0);     

    }  
}

int UserInputMenu(char* p_Playername, int* p_CopiedOrOriginal, myStack_t* p_PlayerStack, struct Coordinate* p_sPlayerCoordinate, int* p_bRulesActivated) {
    int bGameEnd = 0;
    char cSelector;
        
    if(*p_CopiedOrOriginal == 0) printSolution(p_Playername);
    else if(*p_CopiedOrOriginal == 1) printBoard(p_Playername);
    else printBoard(p_Playername);
    
    printf("Type what you want to do\n(i=Change a fieldvalue; b=Delete last described; r=Turn rules on; n=Turn rules off; s=show solution; d=solution disapears; #=EXIT GAME)\nINPUT:   ");
    scanf("%c", &cSelector);
    switch(cSelector) {
        case 'i': {
            userInputCoordinateAndValue(p_PlayerStack, p_bRulesActivated); 
            bGameEnd = 0;
            break;
        }
        case 'b': {
            if (IsStackEmpty(p_PlayerStack) == 1) printf("\n!!!Error the Stack is empty!!!");
            else {
                Pop(p_PlayerStack, p_sPlayerCoordinate);
                cPlayerInputField[(*p_sPlayerCoordinate).row][(*p_sPlayerCoordinate).column] = '*';
            }
            break;
        }
        case 'r': *p_bRulesActivated = 1; break;
        case 'n': *p_bRulesActivated = 0; break;
        case 's': *p_CopiedOrOriginal = 0; break;
        case 'd': *p_CopiedOrOriginal = 1;break;
        case '#': bGameEnd = 1; break;      //End game
    }
    return bGameEnd;
}