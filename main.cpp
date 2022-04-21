/*
 * Gabriel Talbert Bunt
 * CS 211
 * 04/18/2022
 *
 * Description:
 * This program reads a sequence of space delimited long long ints
 * from a file (testdata.txt), puts them into an array, and prints them.
 * It then sorts the array in ascending order using selection sort.
 * The user is then prompted for an int to search for,
 * (which is up to long long size, not that the user needs to know,)
 * which is searched for in the sorted array using binary search.
 * If the quarry is found, it's printed that it was found and
 * how many comparisons it took.
 * If the quarry is not found, it's printed that it was not found and
 * how many comparisons took place.
 * Then the program exits with no errors (0).
 *
 * Notes on execution:
 * I went ahead and took your advice from one of the lectures to
 * just use the largest data type since memory is cheap.
 * It makes some parts more difficult to read, but this should mostly
 * solve the issue of an input number being too large to search for.
 * In the future I'll actually decide how large a
 * number should need to be.
 *
 * Notes on style:
 * As promised, I've commented less in this assignment. I'd like to think that
 * my variable names serve as comments on their own and that commenting on what
 * things are doing would be redundant. However, I have left a small number of
 * comments on why things are a certain way, as well as a docstring for each function.
 * I've also switched over to function/when/for/if {
 * } formatting as I personally find it more concise and readable.
 *
 * Test Data (mostly the same as last time):
 * input                      | output                       | passed?
 * ---------------------------|------------------------------|---------
 * 5 4 6 2 1 3 9 7 8 10 11 12 | 5 4 6 2 1 3 9 7 8 10 11 12   | y (file)
 * ---------------------------|------------------------------|---------
 * 6                          |  6  found,     1 comparison  | y
 * 9                          |  9  found,     2 comparisons | y
 * 12                         |  12 found,     4 comparisons | y
 * 13                         |  13 NOT found, 4 comparisons | y
 * 0                          |  0  NOT found, 5 comparisons | y
 * -1                         | -1  NOT found, 5 comparisons | y
 * 1a                         |  1  found,     4 comparisons | y*1
 * 1 1                        |  1  found,     4 comparisons | y*1
 * a                          | Loops 64 times, then exits   | n*1
 * aa                         | Loops 64 times, then exits   | n*1
 * a1                         | Loops 64 times, then exits   | n*1
 * 9223372036854775808        | Loops 64 times, then exits   | n*1
 *
 * y*1: This means that the behavior is 'fine,' but wasn't intended or planned for.
 * n*1: This means that the behavior is unwanted but not as bad as it could be due to a
 *      shoddy patch I implemented to stop it from infinitely looping.
 */

#include <fstream>
#include <iostream>
#include <limits>
using namespace std;

long long CountLongLongElementsInFile(const string& file_name);
long long* InitializeLongLongArray(const long long& size);
void FillLongLongArray(long long* longlong_array, const long long& size, const string& file_name);
void PrintLongLongArray(const long long* longlong_array, const long long& size);
void SortLongLongArray(long long* longlong_array, const long long& size);
long long PromptLongLong();
bool SearchArrayForLongLong(const long long* longlong_array,
							const long long& size,
							const long long& quarry,
							long long& comparisons);
void PrintSearchResults(const long long& quarry, const bool& found, const long long& comparisons);

/**
 * Reads a sequence of space delimited long long ints
 * from a file (testdata.txt), puts them into an array, and prints them.
 * The array is then sorted in ascending order using selection sort.
 * The user is then prompted for an int to search for,
 * (which is up to long long size, not that the user needs to know,)
 * which is searched for in the sorted array using binary search.
 * If the quarry is found, it's printed that it was found and
 * how many comparisons it took.
 * If the quarry is not found, it's printed that it was not found and
 * how many comparisons took place.
 * @return 0 for no errors
 */
int main() {
	const string kFileName = "testdata.txt";
	const long long kSize = CountLongLongElementsInFile(kFileName);
	long long* longlong_array = InitializeLongLongArray(kSize);

	FillLongLongArray(longlong_array, kSize, kFileName);

	cout << "Input array:  ";
	PrintLongLongArray(longlong_array, kSize);
	cout << endl;

	SortLongLongArray(longlong_array, kSize);

	cout << "Sorted array: ";
	PrintLongLongArray(longlong_array, kSize);

	long long quarry = PromptLongLong();
	long long comparisons;
	bool search_result = SearchArrayForLongLong(longlong_array, kSize, quarry, comparisons);

	PrintSearchResults(quarry, search_result, comparisons);

	// This seems very easy to forget to include...
	// Especially the square brackets when we're using pointer notation throughout.
	// When it's not an assignment requirement I definitely prefer the syntactic sugar of array[] notation.
	delete[] longlong_array;
	return 0;
}

/**
 * Opens a file and sequentially counts the number of elements that fit in a long long.
 *
 * Returns size if it reaches the end of the file a value that will not go into a long long.
 *
 * @param file_name the name of the file to be read from
 * @throw exit(1) if unable to open the file
 * @return the number of long long elements in the file
 */
long long CountLongLongElementsInFile(const string& file_name) {
	ifstream in_file;
	in_file.open(file_name);
	long long size = 0;
	long long garbage;
	if (in_file) {
		while (in_file >> garbage) {
			size++;
		}
		in_file.close();
	} else {
		cout << "\nError Opening File.\n";
		exit(1);
	}
	return size;
}

/**
 * Initializes an array of a specified size.
 *
 * @param size the size of the array that's being initialized
 * @return a pointer to the new array
 */
long long* InitializeLongLongArray(const long long& size) {
	return (new long long[size]);
}

/**
 * Reads values from a file and puts them into a provided array.
 *
 * @param longlong_array the array to be filled
 * @param size the size of the array
 * @param file_name the name of the file to be read from
 */
void FillLongLongArray(long long* longlong_array, const long long& size, const string& file_name) {
	ifstream in_file;
	in_file.open(file_name);
	for (long long index = 0; index < size; index++) {
		in_file >> *(longlong_array + index);
	}
	in_file.close();
}

/**
 * Prints all of the values in a provided array, separated by a space
 *
 * @param longlong_array the array to be read from
 * @param size the size of the array
 */
void PrintLongLongArray(const long long* longlong_array, const long long& size) {

	for (long long index = 0; index < size; index++) {
		cout << *(longlong_array + index) << " ";
	}
}

/**
 * Sorts a provided array in descending order using selection sort.
 *
 * @param longlong_array the array to be sorted
 * @param size the size of the array
 */
void SortLongLongArray(long long* longlong_array, const long long& size) {
	long long smallest;
	for (long long elements_sorted = 0; elements_sorted < size; elements_sorted++) {
		smallest = elements_sorted;
		for (long long prospective = elements_sorted + 1; prospective < size; prospective++) {
			if (*(longlong_array + prospective) < *(longlong_array + smallest)) {
				smallest = prospective;
			}
		}
		if (smallest != elements_sorted) {
			long long temp = *(longlong_array + elements_sorted);
			*(longlong_array + elements_sorted) = *(longlong_array + smallest);
			*(longlong_array + smallest) = temp;
		}
	}
}

/**
 * Prompts the user for an 'integer' which is then returned.
 * The current method for persistently prompting correct input is non-functional.
 *
 * @return the user input as long long
 */
long long PromptLongLong() {
	bool entered_number = false;
	long long user_number;
	while (!entered_number) {
		cout << "\nPlease enter an integer to search for: ";
		cin >> user_number;
		if (cin.fail()) {
			cout << "Invalid entry.";
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		} else {
			entered_number = true;
		}
	}
	return user_number;
}

/**
 * Searches the provided array for a specified long long value using binary search.
 *
 * @param[in] longlong_array the array to be searched
 * @param[in] size the size of the array
 * @param[in] quarry the long long value to be searched for
 * @param[out] comparisons the number of comparison passes it took to reach the result
 * @return whether the quarry was found (1) or not (0)
 */
bool SearchArrayForLongLong(const long long* longlong_array,
							const long long& size,
							const long long& quarry,
							long long& comparisons) {
	long long lower_bound = 0;
	long long upper_bound = size - 1;
	long long current_index = (lower_bound + upper_bound) / 2;
	comparisons = 1;
	while (*(longlong_array + current_index) != quarry) {
		if (lower_bound == upper_bound) {
			return false;
		}
		if (*(longlong_array + current_index) > quarry) {
			upper_bound = current_index;
		} else {
			lower_bound = current_index + 1;
		}
		current_index = (lower_bound + upper_bound) / 2;
		comparisons++;
	}
	return true;
}

/**
 * Simple function that prints out the results of the binary search.
 *
 * @param quarry the value that was searched for
 * @param found whether or not the quarry was found
 * @param comparisons how many comparison passes the search performed
 */
void PrintSearchResults(const long long& quarry, const bool& found, const long long& comparisons) {
	if (found) {
		cout << '\"' << quarry << "\" found, taking " << comparisons << " comparison(s)!";
	} else {
		cout << '\"' << quarry << "\" not found after " << comparisons << " comparison(s).";
	}
}