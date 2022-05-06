// CS Airline
// cs_airline.c
//
// This program was written by Arindam Mukherjee (z5421641)
// on 06/04/2022
//
// Version 1.0.0 2022-04-02: Initial Release.
//
/* This program emulates an airline program that is designed to map the routes  
of airlines and how we can use different commands to manipulate the routes.This
program tests our ability as a junior software developer by considering each 
flight route as linked lists of aiports with the struct flight holding the head
(flight->origin) and tail (flight->dest_airport) and as a result developing a 
functional flight route.*/

#include "manifest.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////  CONSTANTS  ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// The buffer length is used for reading a single line
#define MAX_STRING_LEN 100

#define COMMAND_PRINT_HELP "help"
#define COMMAND_PRINT_ROUTE "print"
#define COMMAND_SUBROUTE "subroute"
#define COMMAND_CHANGE_ORIGIN "change_origin"
#define COMMAND_BYPASS "bypass"
#define COMMAND_EMERGENCY "emergency"
#define COMMAND_CANCEL "cancel"
#define COMMAND_REVERSE "reverse"
#define COMMAND_ADD_PERSON "add_person"
#define COMMAND_PRINT_MANIFEST "print_manifest"
#define COMMAND_FLIGHT_STATS "stats"

// TODO: you may choose to add additional #defines here.
#define TRUE 0
#define FALSE 1
#define SAME 0
#define NO_EMERGENCY 0 
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////  STRUCTS  /////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Airports represent 'stops' along the flight path
// These are the 'nodes' of the linked list
struct airport {
    char code[MAX_STRING_LEN];
    int arrival_time;
    int departure_time;
    struct airport *next_airport;
    struct manifest *manifest;  
};

// Root flight structure
// This stores the state of the current flight
// (i.e. the head and tail of the linked list)
struct flight {
    struct airport *origin_airport;
    struct airport *dest_airport;
};

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////  FUNCTION PROTOTYPES  ///////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Helper Functions
void remove_newline(char input[MAX_STRING_LEN]);
void do_print_help(void);

void interpret_line(
    char buffer[MAX_STRING_LEN],
    int *val0,
    int *val1,
    char word[MAX_STRING_LEN]
);

// Stage 1 Functions
struct flight *setup_flight(void);
void do_print_airports(struct flight *flight);
void print_one_airport(struct airport *ap);
struct airport *create_airport(
    char code[MAX_STRING_LEN],
    int arrival_time,
    int departure
);

// Stage 2 Functions
void do_subroute(struct flight *flight);
void do_change_origin(struct flight *flight);

// TODO: Your functions prototypes here
struct airport *new_airport (
                            char code[MAX_STRING_LEN],
                            int arrival_time,
                            int departure_time); 
void append_airport (
                    struct flight *new_flight,
                    char code[MAX_STRING_LEN],
                    int arrival_time,
                    int departure_time); 
void time_difference (int start, int end, int *hour, int *min); 
void emergency_landing (struct flight *flight);
void cancel_flight (struct flight *flight);  
void bypass_airport (struct flight *flight);
void reverse_flight (struct flight *flight); 
void free_airport (struct flight *flight); 
void create_people (struct flight *flight);
void print_the_manifest (struct flight *flight); 
void check_conditions_subroute (struct flight *flight, 
                                int endloop_flag, 
                                int hours, 
                                int minutes); 
void check_conditions_emergency (struct flight *flight, int endloop_flag); 
void scan_person_details (
                        char icao_code[MAX_STRING_LEN], 
                        char person_name[MAX_NAME_LEN], 
                        char weight[MAX_NAME_LEN]); 
void emergency_zero (
                    struct flight *flight, 
                    int emergency_length,
                    struct airport *emergency); 
///////////////////////////////////////////////////////////////////////////////
//////////////////////////  FUNCTION IMPLEMENTATIONS  /////////////////////////
///////////////////////////////////////////////////////////////////////////////

int main (void) {

    // Stage 1.1
    // TODO: Complete the setup_flight function below
    struct flight *flight = setup_flight(); 
    // TODO: Fill out the while loop with relevant commands & functions
    // Move into and stay in operational mode until CTRL+D
    printf("Enter Command: ");
    char command_line[MAX_STRING_LEN];
    while (fgets(command_line, MAX_STRING_LEN, stdin) != NULL) 
    {
        remove_newline(command_line);

        if (strcmp(COMMAND_PRINT_HELP, command_line) == SAME) {
            // A help command we have implemented for you.
            do_print_help();
        } 
        else if (strcmp(COMMAND_PRINT_ROUTE, command_line) == SAME) 
        {
            // Stage 1.2 - This command prints the route.
            printf("ROUTE:\n"); 
            do_print_airports(flight);
        } 
        else if (strcmp(COMMAND_SUBROUTE, command_line) == SAME) 
        {
            // Stage 2.1 - This command calculates the subroute.
            do_subroute(flight);
        } 
        else if (strcmp(COMMAND_CHANGE_ORIGIN, command_line) == SAME) 
        {
            // Stage 2.2 - This command chages the origin information.
            do_change_origin(flight);
        } 
        // TODO: Add more conditions here for the remaining commands.
        else if (strcmp(COMMAND_EMERGENCY, command_line) == SAME)
        {
            // Stage 3.1 - This command initiates emergency sequence. 
            emergency_landing(flight); 
        }   
        else if (strcmp(COMMAND_CANCEL, command_line) == SAME)
        {
            // Stage 3.2 - This command cancels flights.
            cancel_flight(flight); 
            flight = setup_flight(); 
        } 
        else if (strcmp(COMMAND_BYPASS, command_line) == SAME)
        {
            // Stage 3.3 - This command removes an airport from the route.
            bypass_airport(flight); 
        }
        else if (strcmp(COMMAND_REVERSE, command_line) == SAME)
        {
            // Stage 3.3 - This command reverses the flight route. 
            reverse_flight(flight); 
        }
        else if (strcmp(COMMAND_ADD_PERSON, command_line) == SAME)
        {
            // Stage 4.1 - This command adds people to the manifest.
            create_people(flight); 
        }
        else if (strcmp(COMMAND_PRINT_MANIFEST, command_line) == SAME)
        {
            // Stage 4.2 - This command prints the manifest.
            print_the_manifest(flight); 
        }
        printf("Enter Command: ");
    } 
    printf("Goodbye!\n"); 
    // After CTRL+D is pressed this will free the aiport, manifest and flight.
    free_airport(flight);
    free(flight);  
    return 0;
}

// Stage 1.1
// Creates and initialises a flight by asking the user for input.
// Parameters:
//     None
// Returns:
//     Pointer to the malloc'd flight
struct flight *setup_flight(void) 
{
    // Create a new, empty flight
    struct flight *new_flight = malloc(sizeof(struct flight));
    new_flight->origin_airport = NULL;
    new_flight->dest_airport = NULL;
    // Read number of stops using fgets
    printf("How many stops will be enroute? ");
    char input_line[MAX_STRING_LEN] = {0};
    fgets(input_line, MAX_STRING_LEN, stdin);
    // Convert the string to a number
    int num_stops = 0;
    num_stops = atoi(input_line);

    // TODO: For each airport Use fgest to read a line and then use the
    // provided interpret_line() function to extract information
    char buffer[MAX_STRING_LEN]; 
    int arrival_time = 0; 
    int departure_time = 0;
    char icao_code[MAX_STRING_LEN]; 
    int i = 0; 
    int previous_departure = 0; 
    
    while (i < num_stops)
    { 
        //fgets reads in a line and interprets it as information
        fgets(buffer, MAX_STRING_LEN, stdin); 
        remove_newline(buffer); 
        interpret_line(buffer, &arrival_time, &departure_time, icao_code);
        
        if (arrival_time > departure_time)
        {
            printf("Departure time cannot be before the arrival time!\n"); 
        }
        else if (arrival_time < previous_departure)
        {
            printf("New arrival time cannot be before the previous departure" 
            " time\n"); 
        }
        else 
        {  
            // If no errors in input, append the airport. 
            append_airport(
                    new_flight, 
                    icao_code, 
                    arrival_time, 
                    departure_time);
        }

        previous_departure = departure_time; 
        i++; 
    }  
    return new_flight; 
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////// MY FUNCTIONS ////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//||APPEND AIRPORTS||
// This function helped me to seamlessly traverse the linked list using a while 
// loop to NULL and then updating where the tail points to. 
void append_airport (
                    struct flight *new_flight, char code[MAX_STRING_LEN],
                    int arrival_time,
                    int departure_time)
{
    struct airport *new_airport = create_airport(
                                                code, 
                                                arrival_time, 
                                                departure_time); 
    //for every new aiport created, create a manifest
    new_airport->manifest = create_manifest();
    struct airport *current = new_flight->origin_airport; 
    
    //if list is empty 
    if (current == NULL)
    {
        new_flight->origin_airport = new_airport; 
        return; 
    }
    //if list is not empty
    while (current->next_airport != NULL)
    {   
        current = current->next_airport;  
    }
    
    current->next_airport = new_flight->dest_airport;
    new_flight->dest_airport = new_airport;   
    current->next_airport = new_airport;  
    return; 
}

//||TIME DIFFERENCE||
// This functions uses the pseudocode and logic to calculate the time
// difference between the departure time of the origin airport
// and the arrival time of the destination airport 
void time_difference (int start, int end, int *hour, int *min)
{
    *hour = (end / 100) - (start / 100); 
    *min = (end % 100) - (start % 100); 
    
    int difference_min = (*hour) * 60 + *min; 
    
    if (difference_min < 0)
    {
        difference_min *= -1; 
    }
    
    *hour = difference_min / 60; 
    *min = difference_min % 60; 
    
    return; 
}

// ||FREE AIRPORTS||
// This function loops through all the airports in the route and free's them 
// it also free's all manifests in doing so that there are no memory leaks 
// within the code!
void free_airport (struct flight *flight)
{
    struct airport *current = flight->origin_airport;   
    //traverse the linked list and keep freeing airports and delete manifests.
    while (current != NULL)
    { 
        struct airport *new_next = current->next_airport;
        delete_manifest(current->manifest); 
        free(current);
        current = new_next; 
        new_next = NULL;
    }  
    return; 
}
// ||CHECK CONDITIONS FOR SUBROUTE||
// This function helps in check what the value of the flag is and as a result
// prints an error message if it changes and prints the correct output if the 
// flag remains unchanged!
void check_conditions_subroute (struct flight *flight, 
int endloop_flag, 
int hours, 
int minutes)
{
    if (endloop_flag == FALSE)
    {
        printf("Desired subroute destination does not exist!\n");
    }
 
    if (endloop_flag == TRUE)
    {
        printf("Time for subroute: %d hrs & %d mins\n", hours, minutes);
    }
    
    return;
}

// ||CHECK CONDITIONS FOR EMERGENCY||
// The function checks what the value of the flag is after it has traversed the
// loop after the condition. This as a result prints out a "flight is safe" if 
// the condition becomes false.  
void check_conditions_emergency (struct flight *flight, int endloop_flag)
{
    //error checking 
    if (endloop_flag == FALSE)
    {
        printf("Flight was safe!\n");
    } 
}

// ||SCANNING PERSON DETAILS||
// This function helps in entering people's details in stage 4 to develop a 
// manifest using the inputted information.
void scan_person_details (
                        char icao_code[MAX_STRING_LEN], 
                        char person_name[MAX_NAME_LEN], 
                        char weight[MAX_NAME_LEN])
{
    printf("Enter the airport code: "); 
    fgets(icao_code, MAX_STRING_LEN, stdin); 
    remove_newline(icao_code);

    printf("Enter the passenger name: ");
    fgets(person_name, MAX_STRING_LEN, stdin);
    remove_newline(person_name); 

    printf("Enter the passenger weight: "); 
    fgets(weight, MAX_NAME_LEN, stdin); 
    remove_newline(weight);
}

// ||CONDITIONS IF EMERGENCY IS ZERO||
// This function helps reduce the length of 3.1 by testing the condition if 
// the user enters zero as the emergency length. This then prints out the 
// the origin and sets the departure time as zero.
void emergency_zero (
                    struct flight *flight, 
                    int emergency_length,
                    struct airport *emergency)
{
    if (emergency_length == NO_EMERGENCY)
    {
        emergency = emergency->next_airport; 
        flight->origin_airport->departure_time = 0; 
        flight->origin_airport->next_airport = NULL;
        free(emergency);  
        return; 
    }
}

// Stage 1.2
// Given a pointer to a flight struct, prints all the info about the route
// Parameters:
//     flight = the flight which contains the flight route to print
// Returns:
//     None
void do_print_airports (struct flight *flight) 
{

    // TODO: Loop through the flight route and call 
    //       the provided print_one_airport function
    struct airport *new_airport = flight->origin_airport; 
    //traverse the list of airports and keep printing them until you reach the 
    //end. 
    while (new_airport != NULL)
    {
        print_one_airport(new_airport); 
        new_airport = new_airport->next_airport; 
    }
    return; 
   
}

// Stage 1.2
// PROVIDED FUNCTION - DO NOT CHANGE
// Given a pointer to an airport struct,
// prints all the info about the airport
// Parameters:
//     ap = the airport to print
// Returns:
//     None
void print_one_airport (struct airport *ap) 
{
    printf(
        "%8s:    %04d -> %04d\n",
       ap->code,
       ap->arrival_time,
       ap->departure_time
    );
}

// Stage 1.1
// Given the information about a new airport,
// Uses `malloc` to create memory for it and returns a pointer to
// that memory.
// Parameters:
//     code            = the airport ICAO code
//     arrival_time    = the arrival time of the new airport
//     departure_time  = the departure time of the new airport
// Returns:
//     A pointer to the malloc'd struct airport
struct airport *create_airport (
    char code[MAX_STRING_LEN],
    int arrival_time,
    int departure_time)
{
    // Malloc new struct
    struct airport *new_airport = malloc(sizeof(struct airport));
    new_airport->manifest = NULL; 

    // initialise airport fields
    strcpy(new_airport->code, code);
    new_airport->arrival_time = arrival_time;
    new_airport->departure_time = departure_time;
    new_airport->next_airport = NULL;
    
    return new_airport;
}

// STAGE 2.1
// ||SUBROUTE||
// This function allows the user to input an airport which it will calculate 
// the time difference between the departure time of the origin aiport and 
// the arrival time of the destination airport!
void do_subroute (struct flight *flight) 
{
    struct airport *current = flight->origin_airport; 
    printf("Enter airport code: ");
    char new_code[MAX_STRING_LEN];
    fgets(new_code, MAX_STRING_LEN, stdin);
    remove_newline(new_code); 
    int hours = 0; 
    int minutes = 0; 
    int endloop_flag = FALSE; 
    
    
    while (current != NULL && endloop_flag == FALSE)
    {    
        //if the current code is same as the code scanned in, run the logic
        if (strcmp(current->code, new_code) == SAME)
        {
            int start = flight->origin_airport->departure_time;
            int end = current->arrival_time;  
            time_difference(start, end, &hours, &minutes); 
            endloop_flag = TRUE; 
        }
        //if the current code is different set the flag to false and give an
        //error.
        if (strcmp(current->code, new_code) != SAME)
        {
            endloop_flag = FALSE; 
        }
        current = current->next_airport;     
    }

    check_conditions_subroute(flight, endloop_flag, hours, minutes); 
   
    return;
}

// STAGE 2.2
// ||CHANGE THE ORIGIN||
// This function adds a unique airport to the head of the flight route by 
// asking the user to enter a code and the arrival and departure times. 
// Additionally it checks conditions to see if the new airport is unique or 
// not!
void do_change_origin (struct flight *flight) 
{
    //make a current pointer which is temporary 
    //assign a new origin that becomes the new head 
    struct airport *current = flight->origin_airport;  
    //defining variables 
    char icao_code[MAX_STRING_LEN]; 
    int new_arrival_time = 0; 
    int new_departure_time = 0; 
    char new_origin_info[MAX_STRING_LEN];
    int endloop_flag = TRUE; 
    
    //code for scanning in info and then removing newline by calling function 
    printf("Enter new origin info: "); 
    fgets(new_origin_info, MAX_STRING_LEN, stdin); 
    remove_newline(new_origin_info);
    interpret_line(new_origin_info, &new_arrival_time, &new_departure_time, icao_code);

    if (new_arrival_time > new_departure_time)
    {
        printf("Departure time cannot be before the arrival time!\n"); 
        return; 
    }
    else if (flight->origin_airport->arrival_time < new_departure_time)
    {
        printf("Departure of new origin cannot be after the arrival time of the"
        " next airport!\n"); 
        return; 
    }

    while (current != NULL)
    {
        if (strcmp(current->code, icao_code) == SAME)
        { 
            endloop_flag++; 
        }
        current = current->next_airport;   
    }
    //error checking 
    if (endloop_flag > TRUE)
    {
        printf("New airport code is not unique!\n");
        return; 
    }
    else 
    {
        //if no errors add an airport and make it the new origin
        struct airport *new_origin = create_airport(
                                            icao_code, 
                                            new_arrival_time, 
                                            new_departure_time);

        new_origin->next_airport = flight->origin_airport;
        flight->origin_airport = new_origin; 
        printf("Added: %s\n", icao_code);
    }

    return;
}

// STAGE 2.3
// ||BYPASS THE AIRPORT||
// This function removes an airport from the flight route if it exists!
void bypass_airport (struct flight *flight)
{
    struct airport *current = flight->origin_airport; 
    struct airport *previous_airport = NULL; 
    printf("Enter airport code: "); 
    char bypass[MAX_STRING_LEN] = {0}; 
    fgets(bypass, MAX_STRING_LEN, stdin); 
    remove_newline(bypass); 

    while (current != NULL)
    {
        if (strcmp(current->code, bypass) == SAME)
        {
            if (previous_airport == NULL)
            {
                flight->origin_airport = current->next_airport; 
            }
            else 
            {
                previous_airport->next_airport = current->next_airport; 
            }
            free(current); 
            return; 
        }
        previous_airport = current; 
        current = current->next_airport; 
    }
    printf("No airport of that code exists!\n"); 
}

// STAGE 3.1 
// ||EMERGENCY LANDING||
// This function reads in the number of hours until an emergency landing and 
// as a result it will initiate an emergency landing at a new airport called 
// ALT0 which will have its arrival time to be added on to the departure of 
// the origin airport and will delete all the flights after!
void emergency_landing (struct flight *flight)
{
    struct airport *current = flight->origin_airport; 
    char icao_code[MAX_STRING_LEN]; 
    char input_line[MAX_STRING_LEN]; 
    int arrival_time = 0; 
    int departure_time = 0; 

    printf("How long until emergency: "); 
    fgets(input_line, MAX_STRING_LEN, stdin);
    remove_newline(input_line); 
    //converts string to int 
    int emergency_length = 0; 
    emergency_length = atoi(input_line); 
    //logic to find the new arrival time 
    arrival_time = flight->origin_airport->departure_time + 
    (emergency_length * 100); 

    strcpy(icao_code, "ALT0"); 

    struct airport *emergency = create_airport(
                                    icao_code, 
                                    arrival_time, 
                                    departure_time); 

    emergency_zero(flight, emergency_length, emergency);
    int endloop_flag = TRUE; 
    //insert emergency in the middle 
    while (current->next_airport != NULL)
    { 
        if (current->arrival_time < arrival_time && 
        current->next_airport->arrival_time >= arrival_time)
        {
            emergency->next_airport = current->next_airport; 
            current->next_airport = emergency;
        }  

        if (flight->dest_airport->arrival_time < arrival_time)
        {
            endloop_flag = FALSE; 
        }

        current = current->next_airport;  
    }  
    //updating to point to tail
    emergency->next_airport = flight->dest_airport; 
    emergency->next_airport = NULL;  
    check_conditions_emergency(flight, endloop_flag); 

    return; 
}

// STAGE 3.2
// ||CANCEL FLIGHT||
// This function deletes all flights stored in the linked list after the 
// command is executed. As a result this gives the user a chance to input new
// airports!
void cancel_flight (struct flight *new_flight)
{     
    free_airport(new_flight);
    printf("Flight cancelled. Now accepting a new flight:\n"); 
    free(new_flight); 
    return; 
}

// STAGE 3.3 
// ||REVERSE FLIGHT||
// This function allows the user to have their flight route reversed after 
// entering the command!
void reverse_flight (struct flight *flight)
{
    struct airport *current = flight->origin_airport; 

    struct airport *previous_airport = NULL; 
    struct airport *new_next_airport = NULL; 

    while (current != NULL)
    { 
        new_next_airport = current->next_airport; 
        current->next_airport = previous_airport; 
        previous_airport = current; 
        current = new_next_airport; 
    }
    current = previous_airport; 
    flight->origin_airport = current; 
    return; 
}

// STAGE 4.1
// ||CREATE PEOPLE||
// This function helps create people that will be added into the manifest, 
// given their details.  
void create_people (struct flight *flight)
{ 
    struct airport *current = flight->origin_airport;
    char person_name[MAX_NAME_LEN] = {0}; 
    char weight[MAX_STRING_LEN] = {0}; 
    char icao_code[MAX_STRING_LEN] = {0}; 
    int endloop_flag = TRUE; 
    
    scan_person_details (icao_code, person_name, weight); 
    double person_weight = atof(weight); 
    
    while (current != NULL && endloop_flag == TRUE)
    {
        if (strcmp(current->code, icao_code) == SAME) 
        {
            add_person(current->manifest, person_name, person_weight); 
            endloop_flag++;
        } 
        else if (strcmp(current->code, icao_code) != SAME)
        {
            endloop_flag = TRUE;
        }
        current = current->next_airport; 
    }

    if (endloop_flag == TRUE)
    {
        printf("No airport of that code exists!\n"); 
    }
    return; 
}

// STAGE 4.2 
// ||PRINTING THE MANIFEST||
// This function helps print the manifest after entering the details of the 
// people!
void print_the_manifest (struct flight *flight)
{
    struct airport *current = flight->origin_airport;
    printf("Enter the airport code: "); 
    char icao_code[MAX_STRING_LEN] = {0}; 
    fgets(icao_code, MAX_STRING_LEN, stdin); 
    remove_newline(icao_code); 
    int endloop_flag = TRUE; 
            
    while (current != NULL && endloop_flag == TRUE)
    {
        if (strcmp(current->code, icao_code) == SAME)
        {
            print_manifest(current->manifest); 
            endloop_flag++;
        } 
        else if (strcmp(current->code, icao_code) != SAME)
        {
            endloop_flag = TRUE; 
        }
        current = current->next_airport; 
    }

    if (endloop_flag == TRUE)
    {
        printf("No airport of that code exists!\n");
    }
}

///////////////////////////////////////////////////////////////////////////////
//////////////////////////////  HELPER FUNCTIONS  /////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Helper Function
// You likely do not need to change this function.
//
// Given a raw string will remove and first newline it sees.
// The newline character wil be replaced with a null terminator ('\0')
// Parameters:
//     flight  = the flight to move people along of
// Returns:
//     None
void remove_newline(char input[MAX_STRING_LEN]) {

    // Find the newline or end of string
    int index = 0;
    while (input[index] != '\n' && input[index] != '\0') {
        index++;
    }
    // Goto the last position in the array and replace with '\0'
    // Note: will have no effect if already at null terminator
    input[index] = '\0';
}


// Helper Function
// You DO NOT NEED TO UNDERSTAND THIS FUNCTION, and will not need to change it.
//
// Given a raw string in the following foramt: [integer0] [integer1] [string]
// This function will extract the relevant values into the given pointer 
//variables. The function will also remove any newline characters.
// For example, if given: "0135 0545 YSSY"
// The function will put the integer values
//     135 into the val1 pointer
//     545 into the val2 pointer
// And will copy a null terminated string
//     "YSSY" into the 'word' array
//
// If you are interested, `strtok` is a function which takes a string,
// and splits it up into before and after a "token" (the second argument)
//
// Parameters:
//     buffer  = A null terminated string in the following format
//               [integer0] [integer1] [string]
//     val0    = A pointer to where [integer0] should be stored
//     val1    = A pointer to where [integer1] should be stored
//     word    = An array for the [string] to be copied into
// Returns:
//     None
void interpret_line(
    char buffer[MAX_STRING_LEN],
    int *val0,
    int *val1,
    char word[MAX_STRING_LEN]
) {

    // Remove extra newline
    remove_newline(buffer);

    // Extract value 1 as int
    char *val0_str = strtok(buffer, " ");
    if (val0_str != NULL) {
        *val0 = atoi(val0_str);
    } else {
        *val0 = -1;
    }

    // Extract value 2 as int
    char *val1_str = strtok(NULL, " ");
    if (val1_str != NULL) {
        *val1 = atoi(val1_str);
    } else {
        *val1 = -1;
    }

    char *word_str = strtok(NULL, " ");
    if (word_str != NULL) {
        // Extract word
        strcpy(word, word_str);
    }

    if (val0_str == NULL || val1_str == NULL || word_str == NULL) {
        // If any of these are null, there were not enough words.
        printf("Could not properly interpret line: %s.\n", buffer);
    }

}

void do_print_help(void) {
    printf("+-----------------+-------------------------------------------+\n");
    printf("|Command Name     | How to Use                                |\n");
    printf("+=================+===========================================+\n");
    printf("|PRINT ROUTE      | Enter command: print                      |\n");
    printf("+-----------------+-------------------------------------------+\n");
    printf("|SUBROUTE         | Enter command: subroute                   |\n");
    printf("|                 | Enter airport code: [AIRPORT CODE]        |\n");
    printf("+-----------------+-------------------------------------------+\n");
    printf("|CHANGE ORIGIN    | Enter command: change_origin              |\n");
    printf("|                 | Enter new origin info: [ARRIVAL TIME]...  |\n");
    printf("|                 |     ... [DEPARTURE TIME] [AIRPORT CODE]   |\n");
    printf("+-----------------+-------------------------------------------+\n");
    printf("|BYPASS           | Enter command: bypass                     |\n");
    printf("|                 | Enter airport code: [AIRPORT CODE]        |\n");
    printf("+-----------------+-------------------------------------------+\n");
    printf("|EMERGENCY        | Enter command: emergency                  |\n");
    printf("|                 | How long until emergency: [TIME IN HOURS] |\n");
    printf("+-----------------+-------------------------------------------+\n");
    printf("|CANCEL           | Enter command: cancel                     |\n");
    printf("+-----------------+-------------------------------------------+\n");
    printf("|REVERSE          | Enter command: reverse                    |\n");
    printf("+-----------------+-------------------------------------------+\n");
    printf("|                 | Enter command: add_person                 |\n");
    printf("|ADD PERSON       | Enter the airport code: [AIRPORT CODE]    |\n");
    printf("|                 | Enter the passenger name: [NAME]          |\n");
    printf("|                 | Enter the passenger weight: [WEIGHT]      |\n");
    printf("+-----------------+-------------------------------------------+\n");
    printf("|PRINT MANIFEST   | Enter command: print_manifest             |\n");
    printf("|                 | Enter the airport code: [AIRPORT CODE]    |\n");
    printf("+-----------------+-------------------------------------------+\n");
    printf("|FLIGHT STATISTICS| Enter command: stats                      |\n");
    printf("+-----------------+-------------------------------------------+\n");
}