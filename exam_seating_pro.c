#include <stdio.h>
#include <stdlib.h> // For malloc (memory allocation)
#include <string.h>

// --- 1. DATA STRUCTURES (Structs, Nodes, Queues) ---

// Define the blueprint for a Student
struct Student {
    char rollNumber[20];
    char branch[10];
};

// Define a Node for the Linked List (used by the Queue)
struct Node {
    struct Student data;
    struct Node* next;
};

// Define the Queue structure (holds the front and rear of the list)
struct Queue {
    struct Node *front;
    struct Node *rear;
};

// Define room layout
#define NUM_ROOMS 5
#define ROOM_ROWS 4
#define ROOM_COLS 5
// Define the input file name
#define INPUT_FILE "students.txt"


// --- 2. QUEUE HELPER FUNCTIONS ---

// Creates and returns a new, empty queue
struct Queue* createQueue() {
    struct Queue* q = (struct Queue*)malloc(sizeof(struct Queue));
    q->front = q->rear = NULL;
    return q;
}

// Checks if a queue is empty
int isEmpty(struct Queue* q) {
    return (q->front == NULL);
}

// Adds a student to the back of the queue
void enqueue(struct Queue* q, struct Student student) {
    // Create a new node
    struct Node* temp = (struct Node*)malloc(sizeof(struct Node));
    temp->data = student;
    temp->next = NULL;

    // If queue is empty, new node is both front and rear
    if (q->rear == NULL) {
        q->front = q->rear = temp;
        return;
    }

    // Add the new node at the end of queue and change rear
    q->rear->next = temp;
    q->rear = temp;
}

// Removes and returns a student from the front of the queue
struct Student dequeue(struct Queue* q) {
    // If queue is empty, return an "EMPTY" student
    if (isEmpty(q)) {
        struct Student emptyStudent;
        strcpy(emptyStudent.rollNumber, "EMPTY");
        strcpy(emptyStudent.branch, "---");
        return emptyStudent;
    }

    // Store previous front and move front one node ahead
    struct Node* temp = q->front;
    struct Student studentData = temp->data;

    q->front = q->front->next;

    // If front becomes NULL, then change rear also as NULL
    if (q->front == NULL) {
        q->rear = NULL;
    }

    free(temp); // Free the old front node
    return studentData;
}

// --- 3. EXAM HALL FUNCTIONS ---

// Initializes all 5 rooms with "EMPTY" seats
void initializeAllRooms(struct Student rooms[NUM_ROOMS][ROOM_ROWS][ROOM_COLS]) {
    struct Student emptyStudent;
    strcpy(emptyStudent.rollNumber, "EMPTY");
    strcpy(emptyStudent.branch, "---");

    for (int room = 0; room < NUM_ROOMS; room++) {
        for (int r = 0; r < ROOM_ROWS; r++) {
            for (int c = 0; c < ROOM_COLS; c++) {
                rooms[room][r][c] = emptyStudent;
            }
        }
    }
}

// Prints the final seating plan for all 5 rooms
void printAllRooms(struct Student rooms[NUM_ROOMS][ROOM_ROWS][ROOM_COLS]) {
    printf("\n--- Exam Hall Seating Plan ---\n");
    for (int room = 0; room < NUM_ROOMS; room++) {
        printf("\n--- ROOM %d ---\n", room + 1);
        for (int r = 0; r < ROOM_ROWS; r++) {
            for (int c = 0; c < ROOM_COLS; c++) {
                // --- CHANGE: Improved spacing ---
                printf("%-10s (%-4s)\t", rooms[room][r][c].rollNumber, rooms[room][r][c].branch);
            }
            printf("\n");
        }
    }
    printf("\n---------------------------------\n");
}

// FEATURE: Search for a student by roll number
void searchStudent(struct Student rooms[NUM_ROOMS][ROOM_ROWS][ROOM_COLS], const char* rollToFind) {
    for (int room = 0; room < NUM_ROOMS; room++) {
        for (int r = 0; r < ROOM_ROWS; r++) {
            for (int c = 0; c < ROOM_COLS; c++) {
                if (strcmp(rooms[room][r][c].rollNumber, rollToFind) == 0) {
                    // --- CHANGE: "Bench" and "+ 1" added ---
                    printf("--> FOUND: Student %s is in ROOM %d, Row %d, Bench %d\n", rollToFind, room + 1, r + 1, c + 1);
                    return;
                }
            }
        }
    }
    printf("--> NOT FOUND: Student %s is NOT in the seating plan.\n", rollToFind);
}

// FEATURE: Export the plan to "seating_plan.txt"
void exportHall(struct Student rooms[NUM_ROOMS][ROOM_ROWS][ROOM_COLS]) {
    FILE *file = fopen("seating_plan.txt", "w");
    if (file == NULL) {
        printf("Error: Could not open file to export.\n");
        return;
    }

    fprintf(file, "--- Exam Hall Seating Plan ---\n");
     for (int room = 0; room < NUM_ROOMS; room++) {
        fprintf(file, "\n--- ROOM %d ---\n", room + 1);
        for (int r = 0; r < ROOM_ROWS; r++) {
            for (int c = 0; c < ROOM_COLS; c++) {
                // --- CHANGE: Improved spacing ---
                fprintf(file, "%-10s (%-4s)\t", rooms[room][r][c].rollNumber, rooms[room][r][c].branch);
            }
            fprintf(file, "\n");
        }
    }

    fclose(file);
    printf("Success: Seating plan exported to 'seating_plan.txt'\n");
}


// --- 4. MAIN PROGRAM ---

int main() {
    
    // Create the 3D array for all 5 rooms
    struct Student examHalls[NUM_ROOMS][ROOM_ROWS][ROOM_COLS];
    initializeAllRooms(examHalls);

    // --- FEATURE: File Input & Queue Loading ---
    
    // 1. Create one queue for each new branch
    struct Queue* cseQueue = createQueue();
    struct Queue* itQueue = createQueue();
    struct Queue* excpQueue = createQueue();
    struct Queue* raiQueue = createQueue();
    struct Queue* entcQueue = createQueue();
    
    // 2. Open the student list file
    FILE *file = fopen(INPUT_FILE, "r");
    if (file == NULL) {
        printf("Error: Could not open file '%s'.\n", INPUT_FILE);
        printf("Please make sure 'students.txt' is in the same folder!\n");
        return 1; // Exit with an error
    }

    // 3. Read the file and sort students into queues
    // printf("\n--- Loading Students from file... ---\n"); // DEBUG LINE REMOVED
    struct Student tempStudent;
    while (fscanf(file, "%s %s", tempStudent.rollNumber, tempStudent.branch) == 2) {
        
        // --- DEBUG LINE REMOVED ---
        // printf("Reading: %-10s | Branch: %s\n", tempStudent.rollNumber, tempStudent.branch);
        // --------------------------

        if (strcmp(tempStudent.branch, "CSE") == 0) {
            enqueue(cseQueue, tempStudent);
        } else if (strcmp(tempStudent.branch, "IT") == 0) {
            enqueue(itQueue, tempStudent);
        } else if (strcmp(tempStudent.branch, "EXCP") == 0) {
            enqueue(excpQueue, tempStudent);
        } else if (strcmp(tempStudent.branch, "RAI") == 0) {
            enqueue(raiQueue, tempStudent);
        } else if (strcmp(tempStudent.branch, "ENTC") == 0) {
            enqueue(entcQueue, tempStudent);
        }
    }
    fclose(file); // Close the file
    // printf("--- Finished loading students. ---\n"); // DEBUG LINE REMOVED

    // --- FEATURE: Automatic Seating Generation (Round-Robin) ---

    // Create an array of our 5 branch queues to cycle through
    struct Queue* branchQueues[] = { cseQueue, itQueue, excpQueue, raiQueue, entcQueue };
    int numBranches = 5;
    int branchTurn = 0; // 0=CSE, 1=IT, 2=EXCP, 3=RAI, 4=ENTC
    
    // Check if all queues are empty
    int allEmpty = isEmpty(cseQueue) && isEmpty(itQueue) && isEmpty(excpQueue) && isEmpty(raiQueue) && isEmpty(entcQueue);

    // Loop through 5 rooms
    for (int room = 0; room < NUM_ROOMS; room++) {
        
        // --- BUG FIX: Changed loop order to fill ROW BY ROW ---
        // This now matches the print and search functions.
        for (int r = 0; r < ROOM_ROWS; r++) {
            for (int c = 0; c < ROOM_COLS; c++) {
                
                // If all queues are empty, stop filling
                if (allEmpty) {
                    break; 
                }
                
                // Find the next non-empty queue
                while (isEmpty(branchQueues[branchTurn])) {
                    // This queue is empty, try the next one
                    branchTurn = (branchTurn + 1) % numBranches;
                }

                // We found a queue with students, so dequeue and place the student
                examHalls[room][r][c] = dequeue(branchQueues[branchTurn]);
                
                // Move to the next branch for the next seat
                branchTurn = (branchTurn + 1) % numBranches;

                // Re-check if all queues are now empty
                allEmpty = isEmpty(cseQueue) && isEmpty(itQueue) && isEmpty(excpQueue) && isEmpty(raiQueue) && isEmpty(entcQueue);
            }
            if (allEmpty) {
                break; // Stop filling columns if all students are seated
            }
        }
        if (allEmpty) {
            break; // Stop filling rooms if all students are seated
        }
    }

    // --- 5. RUN ALL OTHER FEATURES ---
    
    // 1. Display the final plan on screen
    printAllRooms(examHalls);
    
    // 2. Interactive Search
    char rollToSearch[20]; // Variable to store user's input
    printf("\n--- Student Search Tool ---\n");
    
    while (1) { // Loop forever
        printf("Enter Roll Number to find (or type 'exit' to quit): ");
        scanf("%s", rollToSearch); // Read the user's text input

        // Check if the user wants to quit
        if (strcmp(rollToSearch, "exit") == 0) {
            printf("Exiting search tool.\n");
            break; // Exit the while loop
        }
        
        // If not "exit", run the search with the user's input
        searchStudent(examHalls, rollToSearch);
    }
    
    // 3. Save the plan to a file
    exportHall(examHalls);

    return 0; // Success!
}

