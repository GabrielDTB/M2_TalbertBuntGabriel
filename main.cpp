/*
 * Gabriel Talbert Bunt
 * CS 211
 * 04/18/2022
 * Description:
 *
 *
 *
 *
 * Test Data:
 *
 */

#include <fstream>
#include <iostream>
#include <limits>
using namespace std;

int CountIntegerElementsInFile(const string& file_name);
int* InitializeIntegerArray(const int& size);
void FillIntegerArray(const int& size, int* integer_array, const string& file_name);
void PrintIntegerArray(const int* integer_array, const int& size);
void SortIntegerArray(int* integer_array, const int& size);
int PromptInteger();
bool SearchArrayForInteger(const int* integer_array, const int& size, const int& quarry, int& comparisons);
void PrintSearchResults(const bool& found, const int& comparisons, const int& quarry);

int main() {
	const string kFileName = "testdata.txt";
	const int kSize = CountIntegerElementsInFile(kFileName);
	int* integer_array = InitializeIntegerArray(kSize);
	FillIntegerArray(kSize, integer_array, kFileName);

	PrintIntegerArray(integer_array, kSize);
	cout << endl;

	SortIntegerArray(integer_array, kSize);
	PrintIntegerArray(integer_array, kSize);

	int quarry = PromptInteger();
	int comparisons;
	bool search_result = SearchArrayForInteger(integer_array, kSize, quarry, comparisons);

	PrintSearchResults(search_result, comparisons, quarry);

	// This seems very easy to forget to include...
	// Especially the square brackets when we're using pointer notation throughout.
	// When it's not an assignment requirement I definitely prefer the syntactic sugar of array[] notation.
	delete[] integer_array;
	return 0;
}

int CountIntegerElementsInFile(const string& file_name) {
	ifstream in_file;
	in_file.open(file_name);
	int size = 0;
	int garbage;
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

int* InitializeIntegerArray(const int& size) {
	return (new int[size]);
}

void FillIntegerArray(const int& size, int* integer_array, const string& file_name) {
	ifstream in_file;
	in_file.open(file_name);
	for (int index = 0; index < size; index++) {
		in_file >> *(integer_array + index);
	}
	in_file.close();
}

void PrintIntegerArray(const int* integer_array, const int& size) {

	for (int index = 0; index < size; index++) {
		cout << *(integer_array + index) << " ";
	}
}

void SortIntegerArray(int* integer_array, const int& size) {
	int smallest;
	for (int elements_sorted = 0; elements_sorted < size; elements_sorted++) {
		smallest = elements_sorted;
		for (int prospective = elements_sorted + 1; prospective < size; prospective++) {
			if (*(integer_array + prospective) < *(integer_array + smallest)) {
				smallest = prospective;
			}
		}
		if (smallest != elements_sorted) {
			int temp = *(integer_array + elements_sorted);
			*(integer_array + elements_sorted) = *(integer_array + smallest);
			*(integer_array + smallest) = temp;
		}
	}
}

int PromptInteger() {
	// Here as a failsafe to an infinite loop
	int counter = 0;

	int user_number;
	cout << "\nPlease enter an integer to search for: ";
	while (!(cin >> user_number)) {
		cout << "\nInvalid entry.";
		cout << "\nPlease enter an integer to search for: ";
		// I don't know why this causes an infinite loop,
		// as it should just ignore the remainder of the buffer.
		// I'm guessing that it's related to the '\n',
		// but cin in general gives me conniptions...
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		// This is here just as a failsafe
		// so the program doesn't loop for
		// too long due to the above bug.
		if (counter++ > 63) {
			cout << "\nThe program has fallen into an infinite loop.\n";
			exit(1);
		}
	}
	return user_number;
}

bool SearchArrayForInteger(const int* integer_array, const int& size, const int& quarry, int& comparisons) {
	int lower_bound = 0;
	int upper_bound = size - 1;
	int current_index = (lower_bound + upper_bound) / 2;
	comparisons = 1;
	while (lower_bound != upper_bound) {
		if (*(integer_array + current_index) == quarry) {
			return true;
		} else if (*(integer_array + current_index) > quarry) {
			upper_bound = current_index;
		} else {
			lower_bound = current_index + 1;
		}
		current_index = (lower_bound + upper_bound) / 2;
		comparisons++;
	}
	return false;
}

void PrintSearchResults(const bool& found, const int& comparisons, const int& quarry) {
	if (found) {
		cout << '\"' << quarry << "\" found, taking " << comparisons << " comparison(s)!";
	} else {
		cout << '\"' << quarry << "\" not found after " << comparisons << " comparison(s).";
	}
}