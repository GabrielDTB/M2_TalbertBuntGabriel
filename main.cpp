/*
 * Gabriel Talbert Bunt
 * CS 211
 * 04/18/2022
 *
 * Description:
 * This program reads a sequence of space delimited long long ints
 * from a file (testdata.txt), puts them into an array, and prints them.
 * It then sorts the array using selection sort.
 * The user is then prompted for an int to search for,
 * (which is up to long long size, not that the user needs to know,)
 * which is searched for in the sorted array using binary search.
 * If the quarry is found, it's printed that it was found and
 * how many comparisons it took.
 * If the quarry is not found, it's printed that it was not found and
 * how many comparisons took place.
 * Then the program exits with no errors (0).
 *
 * Note on execution:
 * I went ahead and took your advice from one of the lectures to
 * just use the largest data type since memory is cheap.
 * It makes some parts more annoying to read, but this should mostly
 * solve the issue of an input number being too large to search for.
 * To be honest, I replaced via search and replace post-haste,
 * but in the future I'll actually decide how large a
 * number should need to be.
 *
 * Notes on style:
 * As promised, I've commented less in this assignment. I'd like to think that
 * my variable names serve as comments on their own and that commenting on what
 * things are doing would be redundant. However, I have left a small number of
 * comments on why things are a certain way, as well as docstrings.
 * I've also switched over to function() {
 * } formatting as I personally find it more concise and readable.
 *
 *
 * Test Data (same as last time):
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
 * 9999999999                 | Loops 64 times, then exits   | n*2
 */

#include <fstream>
#include <iostream>
#include <limits>
using namespace std;

long long CountLongLongElementsInFile(const string& file_name);
long long* InitializeLongLongArray(const long long& size);
void FillLongLongArray(const long long& size, long long* longlong_array, const string& file_name);
void PrintLongLongArray(const long long* longlong_array, const long long& size);
void SortLongLongArray(long long* longlong_array, const long long& size);
long long PromptLongLong();
bool SearchArrayForLongLong(const long long* longlong_array,
							const long long& size,
							const long long& quarry,
							long long& comparisons);
void PrintSearchResults(const bool& found, const long long& comparisons, const long long& quarry);

int main() {
	const string kFileName = "testdata.txt";
	const long long kSize = CountLongLongElementsInFile(kFileName);
	long long* longlong_array = InitializeLongLongArray(kSize);
	FillLongLongArray(kSize, longlong_array, kFileName);

	PrintLongLongArray(longlong_array, kSize);
	cout << endl;

	SortLongLongArray(longlong_array, kSize);
	PrintLongLongArray(longlong_array, kSize);

	long long quarry = PromptLongLong();
	long long comparisons;
	bool search_result = SearchArrayForLongLong(longlong_array, kSize, quarry, comparisons);

	PrintSearchResults(search_result, comparisons, quarry);

	// This seems very easy to forget to include...
	// Especially the square brackets when we're using pointer notation throughout.
	// When it's not an assignment requirement I definitely prefer the syntactic sugar of array[] notation.
	delete[] longlong_array;
	return 0;
}

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

long long* InitializeLongLongArray(const long long& size) {
	return (new long long[size]);
}

void FillLongLongArray(const long long& size, long long* longlong_array, const string& file_name) {
	ifstream in_file;
	in_file.open(file_name);
	for (long long index = 0; index < size; index++) {
		in_file >> *(longlong_array + index);
	}
	in_file.close();
}

void PrintLongLongArray(const long long* longlong_array, const long long& size) {

	for (long long index = 0; index < size; index++) {
		cout << *(longlong_array + index) << " ";
	}
}

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

long long PromptLongLong() {
	// Here as a failsafe to an infinite loop
	long long counter = 0;

	long long user_number;
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

void PrintSearchResults(const bool& found, const long long& comparisons, const long long& quarry) {
	if (found) {
		cout << '\"' << quarry << "\" found, taking " << comparisons << " comparison(s)!";
	} else {
		cout << '\"' << quarry << "\" not found after " << comparisons << " comparison(s).";
	}
}