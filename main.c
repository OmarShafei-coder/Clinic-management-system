#include <stdio.h>
#include "types.h"

#define MAX_NUMBER_OF_PATIENT 5

typedef struct
{
    u8 name[20];
    u16 age;
    u16 gender; //1 for male, and 2 for female
    u16 ID;
}patient;

typedef struct
{
    f32 slot;
    u16 ID;
}reservation;

//Functions prototype
void correctPassword(patient[], reservation[], f32[], u16*, u16*);
void addPatient(patient[], u16*);
void inputData(patient[], u16*);
void checkID(patient[], u16*);
void editPatient(patient[]);
void reserveSlot(patient[], reservation[], f32[], u16*);
void cancelReservation(reservation[], f32[], u16*);
void incorrectPassword(u16,patient[], reservation[], f32[], u16*, u16*);
void displayPatientInformation(patient[]);
void DisplayTodayReservation(reservation[]);

void main(void)
{
    //Variables declaration :
    u16 mode1; // for choising between admin and user mode
    u16 mode2;// For choising the required feature  in user mode
    u16 password;
    u16 loopCounter;
    patient p[MAX_NUMBER_OF_PATIENT];  //Array of patients(array of structures)
    reservation r[MAX_NUMBER_OF_PATIENT]; //max number of slots = max number of patients
    u16 patientNumber = 0;  
    u16 numberOfAvailableSlots = MAX_NUMBER_OF_PATIENT;
    f32 slots[MAX_NUMBER_OF_PATIENT] = {2, 2.30, 3, 4, 4.30}; /*The available slots*/

    /*input the slots :*/
    for(loopCounter=0; loopCounter<MAX_NUMBER_OF_PATIENT; loopCounter++)
    {
        r[loopCounter].slot = slots[loopCounter];
        r[loopCounter].ID = 0;
    }

    while(1)
    {
        //Ask the user between admin mode and user mode:
        printf("For admin mode press 1, and for user mode press 2 : ");
        scanf("%hu",&mode1);

        //For admin mode :
        if(mode1 == 1)
        {
            //Ask the user to enter the password
            printf("Enter the password : ");
            scanf("%hu",&password);

            //If password is correct :
            if(password == 1234)
            {
                correctPassword(p,r,slots,&patientNumber,&numberOfAvailableSlots);
            }

            //If password is incorrect :
            else
            {
                incorrectPassword(password,p,r,slots,&patientNumber,&numberOfAvailableSlots);
            }
        }
        //For user mode :
        else if(mode1 == 2)
        {
            printf("press 1 for displaying a patient information or 2 for displaying today's reservation\n");
            scanf("%hu",&mode2);

            switch(mode2)
            {
                case 1  : displayPatientInformation(p); break;
                case 2  : DisplayTodayReservation(r);   break;
                default : printf("Wrong number!\n");    break;
            }
        }else;
    }
}

void correctPassword(patient p[], reservation r[], f32 slots[], u16 *ptrToPatientNumber, u16 *ptrToAvailableSlots)
{
    u16 mode; // for choising between adding, editing a patient data, reserving, or cancelling a slot

    printf("1.Press 1 for adding a new patient\n2.Press 2 for editing a patient data\n");
    printf("3.Press 3 for reserving a slot\n4.Press 4 for cancelling a slot\n")  ;
    scanf("%hu",&mode);

    switch(mode)
    {
        case 1  : addPatient(p,ptrToPatientNumber);               break;
        case 2  : editPatient(p);                                 break;
        case 3  : reserveSlot(p,r,slots,ptrToAvailableSlots);     break;
        case 4  : cancelReservation(r,slots,ptrToAvailableSlots); break;
        default : printf("Wrong number!\n");                      break;
    }
}

void addPatient(patient p[], u16 *ptrToPatientNumber)
{
    if(*ptrToPatientNumber < MAX_NUMBER_OF_PATIENT)
    {
        inputData(p,ptrToPatientNumber);
        printf("The process is successfully done\n");
        (*ptrToPatientNumber)++;
    }
    else
    {
        printf("There's no place for a new patient\n");
    }
}

void inputData(patient p[], u16 *ptrToPatientNumber) //p is the patient variable name
{
    printf("Enter the name of the patient : ");
    scanf("%s",&p[*ptrToPatientNumber].name);
    printf("Enter the age : ");
    scanf("%hu",&p[*ptrToPatientNumber].age);
    printf("Enter the gender(1 fo male & 2 for female) : ");
    scanf("%hu",&p[*ptrToPatientNumber].gender);
    printf("Enter the ID : ");
    scanf("%hu",&p[*ptrToPatientNumber].ID);
    //Check if ID is already exist
    checkID(p,ptrToPatientNumber);
}

void checkID(patient p[], u16 *ptrToPatientNumber)
{
    u16 loopCounter;
    /*check the ID values for the previous patients*/
    for(loopCounter=0; loopCounter < *ptrToPatientNumber; loopCounter++)
    {
        while(p[*ptrToPatientNumber].ID == p[loopCounter].ID)
        {
            printf("This ID is already exist. Please Enter the patient's ID again! : ");
            scanf("%hu",&p[*ptrToPatientNumber].ID);
        }
    }
}

void editPatient(patient p[])
{
    u16 ID;
    u16 loopCounter;
    u16 edit; /*variable to ask the user which field want to edit*/
    u16 flag;
    printf("Please enter the patient's ID : ");
    scanf("%hu",&ID);
    /*Check if the ID exists or not*/
    for(loopCounter=0; loopCounter<MAX_NUMBER_OF_PATIENT; loopCounter++)
    {
        if(ID == p[loopCounter].ID)
        {
            flag = 0;
            printf("What do you want to edit ?\n");
            printf("Enter 1 for name, 2 for age , or 3 for gender : ");
            scanf("%hu",&edit);

            switch(edit)
            {
                case 1 :
                    printf("Enter the name of the patient : ");
                    scanf("%s",&p[loopCounter].name);
                    break;
                case 2 :
                    printf("Enter the age of the patient : ");
                    scanf("%hu",&p[loopCounter].age);
                    break;
                case 3 :
                    printf("Enter the gender of the patient(1 for male and 2 for female) : ");
                    scanf("%hu",&p[loopCounter].gender);
                    break;
                default  :
                    break;
            }
            printf("Edited successfully\n");
            break; // if ID is exist it'll take an action and break
        }
        else
        {
           flag = 1;
        }
    }
    /*If ID is not exist*/
    if(flag == 1)
    {
        printf("Incorrect ID!\n");

    }
}

void reserveSlot(patient p[], reservation r[], f32 slots[], u16 *ptrToAvailableSlots)
{
    u16 ID;
    u16 reservedSlotNumber;/*the number of slot the user will enter*/
    u16 flag1;
    u16 flag2;
    u16 loopCounter;
    u16 lc; //loop counter
    printf("Please enter the patient's ID : ");
    scanf("%hu",&ID);
    /*Check if This ID has already reserved a slot*/
    for(loopCounter=0; loopCounter<MAX_NUMBER_OF_PATIENT; loopCounter++)
    {
        if(ID == r[loopCounter].ID)
        {
            flag2 = 1;
            break;
        }
        else
        {
            flag2 = 0;
        }
    }
    /*Check if the ID exists or not*/
    for(loopCounter=0; loopCounter<MAX_NUMBER_OF_PATIENT; loopCounter++)
    {
        if(ID == p[loopCounter].ID && flag2 == 0 && (*ptrToAvailableSlots) > 0)
        {
            flag1 = 0;
            printf("there're %d available slots :\n",*ptrToAvailableSlots);
            for(loopCounter=0; loopCounter<*ptrToAvailableSlots; loopCounter++)
            {
                printf("%hu- %0.2fPM.\n",loopCounter+1, slots[loopCounter]); /*Ex: 1- 2.00PM*/
            }
            printf("Please enter the number of the slot you want : ");
            scanf("%hu",&reservedSlotNumber);
            printf("Reserved Successfully\n");
            /*store the ID with slot in the same struct*/
            r[reservedSlotNumber-1].ID = ID;
            /*if the number is valid*/
            if(reservedSlotNumber <= *ptrToAvailableSlots)
            {
                /*Removing the slot the user choised from the available slots*/
                for(lc = reservedSlotNumber-1; lc < (*ptrToAvailableSlots)-1; lc++) //lc->loop counter
                {
                    slots[lc] = slots[lc + 1];
                }
                /*Decrement the number of available slots*/
                (*ptrToAvailableSlots)--;
            }

            /*if the number is invalid*/
            else
            {
                printf("The number you entered is invalid\n");
            }
            break; // if ID is exist it'll take an action and break
        }
        else
        {
            flag1 = 1;
        }/*Wait until the loop ends and then check flag value*/
    }

    /*If ID is not exist*/
    if(flag1 == 1)
    {
        printf("Invalid ID!\n");
    }else;
}

void cancelReservation(reservation r[], f32 slots[], u16 *ptrToAvailableSlots)
{
    u16 ID;
    u16 flag;
    u16 loopCounter;
    u16 lc;/*loop counter*/

    printf("Please enter the patient's ID that you want to cancel its reservation : ");
    scanf("%hu",&ID);
    /*Check if the ID exists or not*/
    for(loopCounter=0; loopCounter<MAX_NUMBER_OF_PATIENT; loopCounter++)
    {
        if(ID == r[loopCounter].ID) //&& *ptrToAvailableSlots <= MAX_NUMBER_OF_PATIENT)
        {
            flag = 0;

            /*Shifting the slots*/
            for(lc = (*ptrToAvailableSlots-1); lc >= loopCounter; lc--)
            {
                slots[lc+1] = slots[lc];
            }
            
            /*returning the slot to the available slots*/
            slots[loopCounter] = r[loopCounter].slot;
            r[loopCounter].ID = 0; //removing the ID value from the array of reserevd IDs
            /*increment the number of available slots*/
            (*ptrToAvailableSlots)++;   

            break; // if ID is exist it'll take an action and break
        }
        else
        {
            flag = 1;
        }/*Wait until the loop ends and then check flag value*/
    }

    /*If ID is not exist*/
    if(flag == 1)
    {
        printf("Incorrect ID!\n");
    }else;
}

void incorrectPassword(u16 password,patient p[], reservation r[], f32 slots[], u16 *ptrToPatientNumber,u16*ptrToAvailableSlots)
{
    u16 flag = 0;
    u16 loopCounter;

    for(loopCounter=0; loopCounter<2; loopCounter++)
    {
        printf("Wrong password\nPlease enter the password again! : ");
        scanf("%hu",&password);

        if(password == 1234)
        {
            correctPassword(p,r,slots,ptrToPatientNumber, ptrToAvailableSlots);
            break;
        }
        else
        {
            flag++;
        }

        //If the entered password was incorrect for 3 consecutive times, the system shall close :
        if(flag == 2)
        {
            break;
        }else;
    }
}
/********User mode********/
void displayPatientInformation(patient p[])
{
    u16 ID;
    u16 loopCounter;
    u16 flag;
    printf("Please enter the patient's ID : ");
    scanf("%hu",&ID);
    
    for(loopCounter=0; loopCounter<MAX_NUMBER_OF_PATIENT;loopCounter++)
    {
        flag = 0;
        if(ID == p[loopCounter].ID)
        {
            printf("Name   : %s\n",p[loopCounter].name);
            printf("Age    : %hu\n",p[loopCounter].age);

            if(p[loopCounter].gender == 1)
            {
                printf("Gender : Male\n");
            }
            else if(p[loopCounter].gender == 2)
            {
                printf("Gender : Female\n");
            }
            else
            {
                printf("Invalid number!\n");
            }
            break;
        }
        
        else
        {
            flag = 1;
        }
    }

    /*Check if ID is not exist*/
    if(flag == 1)
    {
        printf("Invalid ID!\n");
    }else;
}

void DisplayTodayReservation(reservation r[])
{
    u16 loopCounter;
    u16 flag;
    
    printf("        Time : ID\n");
    for(loopCounter=0; loopCounter<MAX_NUMBER_OF_PATIENT; loopCounter++)
    {
        if(r[loopCounter].ID != 0)
        {
            flag = 0;
            printf("        %0.2f : %hu\n",r[loopCounter].slot, r[loopCounter].ID); //Ex: 2.30 : 55
        }else;
    }

    if(flag != 0)
    {
        printf("There 're no reservations yet\n");
    }else;
}