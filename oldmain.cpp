/*
 * Gabriel Talbert Bunt
 * CS 211
 * 04/19/2022
 * Description:
 *
 *
 *
 * Test Data:
 *
 */

// For working with files.
#include <fstream>
// For working with the console input.
#include <iostream>
// Theoretically for clearing the entire console input buffer.
#include <limits>
// Makes code cleaner.
using namespace std;

// See the definitions below main.
void ParseFile(int&, int*, string&);

void printIntegerArray(int*);

void integerSortSelection(int*);

int promptInteger();

bool integerSearchBinary(int*, int, int&);


int main() {
    /*
    This program reads a sequence of space delimited integers
    from a file (testdata.txt), puts them into an array, and prints them.
    It then sorts the array using selection sort and prints the new array.
    The user is then prompted for an integer to search for, which
    is then searched for in the sorted array using binary search.
    If the quarry is found, it's printed that it was found and
    how many comparisons it took.
    If the quarry is not found, it's printed that it was not found and
    how many comparisons that took.
    Then the program exits with no errors (0).
    */

    const string INPUT_FILE_NAME = "testdata.txt";
    const int MAX = 100;

    // Create the array that will hold all the integers.
    // The zero-th index is reserved for the number of
    // elements in the array, not including itself.
    int numbers[MAX + 1];

    // Put the integers (and size) into the array from a file.
  	ParseFile(MAX, numbers, INPUT_FILE_NAME);

    // Print the unsorted array.
    cout << "Input array:\n";
    printIntegerArray(numbers);

    // Sort the array from least to greatest (ascending order).
    integerSortSelection(numbers);

    // Print the newly sorted array.
    // Turns out it isn't necessary in the problem statement.
    // cout << "\nSorted array:\n";
    // printIntegerArray(numbers);

    // Get the number to search for from the user.
    int quarry = promptInteger();

    // Initialize a required variable for the assignment.
    int comparisons;

    // If the quarry is in the integer array,
    if (integerSearchBinary(numbers, quarry, comparisons)) {
        // print a message.
        cout << endl
             << '\"' << quarry << "\" found, taking " << comparisons
             << " comparison(s)!";
    }
        // Otherwise, (the quarry was not in the integer array)
    else {
        // print a different message.
        cout << endl
             << '\"' << quarry << "\" not found after " << comparisons
             << " comparison(s).";
    }
    // Spacing and
    cout << endl;
    // exit the program successfully.
    return 0;
}


int countFileElements(string fileName)


void ParseFile(int& MAX, int* integer_array, string& FILE_NAME) {
    /*
    ParseFile attempts to read space-delimited integers from a file (FILE_NAME)
    and put them into integer_array. MAX determines the maximum number of integers
    that can be put into the array from the file.
    */

    // Open the file.
    ifstream in_file;
    in_file.open(FILE_NAME);
    // Store the real number of elements in the array.
    int size = 0;
    // If the file successfully opened...
    if (in_file) {
        // Read input from the file while there is still stuff in the file.
        while (size < MAX && in_file >> integer_array[size + 1]) {
            // Increment size because a new element was just added.
            size++;
        }
        // When we are done working with it, close the file.
        in_file.close();
        integer_array[0] = size;
    }
        // If the file could not be opened...
    else {
        // Print the error,
        cout << "\nError Opening File.\n";
        // and exit the program.
        exit(1);
        // Note: It would probably be better to return whether the function was
        //       successful or not, that way the program could decide how to
        //       handle a fail (rather than the function exiting,) but I'm
        //       not too worried about function reusability right now,
        //       considering everything.
    }
}


void printIntegerArray(int* integer_array) {
    /*
    printIntegerArray just prints out every element (except the 0th) in
    an integer array, separated by a space and with a newline at the end.
    */

    // For each element of the array after the 0th...
    for (int index = 1 ; index < integer_array[0] + 1 ; index++) {
        // Print the element followed by a space
        cout << integer_array[index] << " ";
    }
    // Spacing.
    cout << endl;
}


void integerSortSelection(int* integer_array) {
    /*
    integerSortSelection takes in an integer array,
    with index 0 being its size,
    and sorts it using selection sort.
    */

    // Extract the size of the array.
    int size = integer_array[0];
    // Initialize important variable.
    int small_index;
    // For each element in the array, begin walking up the elements.
    for (int pass = 1 ; pass < size - 1 ; pass++) {
        // Set the index that keeps track of the current smallest element.
        small_index = pass;
        // For each element above the first, begin walking up the elements.
        for (int index = pass + 1 ; index < size ; index++) {
            // If this index holds the smallest element so far,
            if (integer_array[index] < integer_array[small_index]) {
                // update the small_index to reflect that knowledge.
                small_index = index;
            }
        }
        // Temporarily hold the value of the current index.
        int temp = integer_array[pass];
        // Set the value of the current index equal to the smallest element.
        integer_array[pass] = integer_array[small_index];
        // Set the value of the index that held the smallest element equal to
        // the value that was held from the current index.
        integer_array[small_index] = temp;
    }
}


int promptInteger() {
    /*
    promptInteger asks the user for an
    integer and returns it if entered.
    Otherwise, keeps trying until it hits an
    error or a correct value is entered.
    */

    // Here as a failsafe to an infinite loop
    int counter = 0;
    // Holds the number that the user inputs
    int user_number;

    cout << "\nPlease enter an integer to search for: ";
    // While the user number cannot be grabbed from the console input,
    while (!(cin >> user_number)) {
        // ask the to try again.
        cout << "\nInvalid entry.";
        cout << "\nPlease enter an integer to search for: ";
        // I don't know why this causes an infinite loop,
        // as it should just ignore the remainder of the buffer.
        // I'm guessing that it's related to the '\n',
        // but cin in general gives me conniptions so idk.
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        // This is here just as a failsafe
        // so the program doesn't loop for
        // too long due to the above bug.
        if (counter++ > 63) {
            // Print error and exit with an error code.
            cout << "\nThe program has fallen into an infinite loop.\n";
            exit(1);
        }
    }
    return user_number;
}


bool integerSearchBinary(int* integer_array, int quarry, int& comparisons) {
    /*
    integerSearchBinary searches for an integer (quarry) in
    an integer array (integer_array) using binary search and
    keeps track of how many comparisons it took (in comparisons).
    If the integer is found, the function returns true.
    If the integer is not found, the function returns false.
    In either case, the comparisons are kept track in the passed
    in variable (comparisons).
    */

    // The lower bound starts at 1 because
    // that is the index of the first element.
    int lower_bound = 1;
    // The upper bound is equal to the number of
    // elements in the array because the indices start at 1.
    int upper_bound = integer_array[0];
    // The current index is set to the
    // midpoint between the lower and upper, rounded down.
    int current_index = (lower_bound + upper_bound) / 2;
    // The number of comparisons is initialized to 1 because
    // at least 1 comparison is going to happen no matter what.
    comparisons = 1;
    // While the quarry has not been found...
    while (integer_array[current_index] != quarry) {
        // If the lower and upper bound are
        // equal (quarry not in array),
        if (lower_bound == upper_bound) {
            // return not found.
            return false;
        }
        // If the current element is larger than the quarry,
        if (integer_array[current_index] > quarry) {
            // shrink the upper bound.
            upper_bound = current_index;
            // (Used to be
            // upper_bound = current_index - 1;
            // however this would cause an infinite loop
            // when searching for values smaller
            // than the smallest in the array.)
        }
            // Otherwise (the current element is smaller than the quarry),
        else {
            // raise the lower bound.
            lower_bound = current_index + 1;
        }
        // Update the current index to be
        // in between the new lower and upper bounds,
        current_index = (lower_bound + upper_bound) / 2;
        // and make note of the next comparison.
        comparisons++;
    }
    // Only returns true if the while loop is escaped,
    // which means that the element was in the array.
    return true;
}