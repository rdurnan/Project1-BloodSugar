// BloodSugar.cpp
// Ryan Durnan
// COSC-2030-01
// Project 1 : Blood Sugar


#include "stdafx.h"
#include <array>
#include <string>
#include <iostream>
#include <utility>
#include <sstream>

using std::array;
using std::string;
using std::stof;
using std::cin;
using std::cout;
using std::endl;
using std::istringstream;


// I referenced Node.h and Node.cpp from Lab2 to make this class.
// This class is an essential building block for making linked lists.
// Every iteration of a linked list is comprised of a Node to store values and address.
class Node
{
public:
	// This is where the actual value of a linked list is stored
	float entry_;

	// This is where the address to the next node is stored
	Node * next_;

public:
	// Constructor for a node object.
	// Takes the arguement value and stores it as part of a linked list.
	// Address half of the linked list is null.
	explicit Node(float entry);

	// Constructor for a node object.
	// Takes the arguements and stores a value and an address as parts of a linked list.
	Node(float entry, Node * next);

	// Destructor.
	~Node();

private:
	Node();
	Node(const Node &);
};

// I referenced LinkedList.h and LinkedList.cpp from Lab2 to make this class.
// This class actually builds linked lists.
// It uses the Node class to store values and addresses.
class List
{
public:
	// Default constructor
	List();

	// Destructor
	~List();

	// Returns true if linked list is empty
	bool empty() const;

	// Displays a linked list to the user
	void print() const;

	// Determines size of List object
	int size();

	// Calculates sum of floats in the list
	float sum();

	//Retrieves the count of overflows
	int getBuckets();

	// Finds the maximum value in the list
	float maxVal();

	// Finds the minimum value in the list
	float minVal();

	// Inserts an item at the front of the list
	void insertAsFirst(float x);

	// Deletes the first element from the linked list, returns element's value before closing
	float removeFirst();

private:
	static Node * clone(Node * ptr);

private:
	Node * first_;
	int bucketCount = 0;
};

// I found this function declaration in Stack Overflow
// I needed it to check if a string could be interpreted as a float.
// This is the exact place I got it from: https://stackoverflow.com/questions/447206/c-isfloat-function
bool isFloat(string myString)
{
	istringstream iss(myString);
	float f;
	iss >> f;
	return iss.eof() && !iss.fail();
};

//
//
////
////
//////
int main()
{
	int currDay = 0;					// Tracks current day. Mon=0,7; Tue=1,8; Wed=2,9; Thu=3,10; Fri=4,11; Sat=5,12; Sun=6,13
	int currWeek = 0;					// Tracks current week. First=0; Second=1
	string input;						// User's input. If it can be converted to float using stof function from stl, then the number should be stored in linked list
	float fullBucket = FLT_MAX;			// I do not manipulate this variable. I use it to print out how many overflows occur
	array<List, 14> dayInfo;			// Array of linked lists. Each element contains the linked list for a day
	array<int, 14> fullBuckets;			// Counts how many overflows are encountered when calculating sum for a day
	array<float, 14> remaining;			// Tracks the last sum that did not overflow.

	// Display a welcoming statement, briefly inform user how to use interface
	cout << "\nWelcome to your blood sugar storage interface!" << endl << endl;
	cout << "This interface will store two weeks worth of your blood sugar measurements." << endl << endl;
	cout << "Valid commands:" << endl;
	cout << "\tAny positive number will be stored" << endl;
	cout << "\tD or Day  : Displays summary of the current day" << endl;
	cout << "\tW or Week : Displays summary of the current week" << endl;
	cout << "\tN or Next : Continues to the next day's storage" << endl << endl;
	
	// Begin collecting data
	while (currDay != 14)
	{
		cout << "Please enter a command:  ";	// Prompt for a command
		cin >> input;	// Get a string from user. Input should be one of the valid commands.
		cout << endl;
		//string inFlt = input;	// This is a copy I use later to convert input's string into a float without affecting input

		// Check if the user wants to continue to the next day.
		if ((input == "N") || (input == "Next") || (input == "n") || (input == "next"))
		{
			currDay++;	// Increment the day count
			if (currDay == 7)	// Check if we've made it to the next week
			{
				currWeek++;		// Increment the week count if necessary
			}
		}

		// Check if the user wants a daily summary
		else if ((input == "D") || (input == "Day") || (input == "d") || (input == "day"))
		{
			float dispSum = dayInfo[currDay].sum();		// Calculate the sum before displaying anything. Need to do this because it updates overflow count (bucketCount in List class)
			fullBuckets[currDay] = dayInfo[currDay].getBuckets();	// Update the array containing overflow counts
			cout << "Daily Summary:" << endl;

			if (dispSum == 0)		// If the sum is zero, I know that there's no entries in the list; hence, the following declarations.
			{
				cout << "\tSum of entries    :  No entries." << endl;
				cout << "\tMax of entries    :  No entries." << endl;
				cout << "\tMin of entries    :  No entries." << endl;
				cout << "\tNumber of entries :  No entries." << endl << endl;
			}
			else
			{						// If there's data in the linked list, I can display information.

				cout << "\tSum of entries    :  ( " << fullBuckets[currDay] << " * " << fullBucket << " ) + " << dispSum << endl;
				cout << "\tMax of entries    :  " << dayInfo[currDay].maxVal() << endl;
				cout << "\tMin of entries    :  " << dayInfo[currDay].minVal() << endl;
				cout << "\tNumber of entries :  " << dayInfo[currDay].size() << endl << endl;
			}
		}

		// Check if the user wants a weekly summary
		else if ((input == "W") || (input == "Week") || (input == "w") || (input == "week"))
		{
			cout << "Weekly Summary:" << endl;
			float tempSum = 0;		// Temporary storage location. Used to check for overflow
			int tempBuckets = 0;	// Temporary storage location. Used to track overflow encounters for the week
			float weekSum = 0;		// Stores the week's sum. Gets reset anytime overflow occurs
			float weekMax = 0;		// Stores the week's max value
			float weekMin = 0;		// Stores the week's min value
			int weekCount = 0;		// Tracks how many values have been stored
			string deltaDay = "Monday";		// Says which day had the largest day-to-day delta
			int largestDelta = 0;	// Tracks the largest day-to-day delta

			if (currWeek == 0)		// Check if we're looking at the first week
			{
				for (int i = 0; i <= currDay; i++)		// Look at linked lists for each day up to (and including) current day
				{
					tempBuckets += dayInfo[i].getBuckets(); // Make sure to get an accurate count the overflows of each day
					tempSum += dayInfo[i].sum();		// Add the sum of the current day to the temporary week sum
					if (isinf(tempSum))					// Check if the temporary week sum overflowed
					{
						tempSum = 0;					// Reset the temporary week sum if necessary
						tempSum += dayInfo[i].sum();	// Temporary sum should equal week sum after the if-statement closes
						weekSum = 0;					// Reset the actual week sum if necessary
						tempBuckets += 1;				// Increment overflow count
					}
					weekSum += dayInfo[i].sum();		// Week sum should not overflow given previous if statment (assuming input isn't too large)
					if (dayInfo[i].maxVal() > weekMax)	// Check if current day's max is greater than week max
					{
						weekMax = dayInfo[i].maxVal();	// Update week max if necessary
					}
					if (dayInfo[i].minVal() < weekMin)	// Check if current day's min is less than week min
					{
						weekMin = dayInfo[i].minVal();	// Update week min if necessary
					}
					weekCount += dayInfo[i].size();		// Add the number of entries of the current day to the week's entry count
				}
				for (int i = 1; i <= currDay; i++)		// Start checking for Day-to-Day deltas on the second day
				{
					int tempDelta = (dayInfo[i].size() - dayInfo[i-1].size());	// Temporary delta equals the difference between the size of the current day and the size of the previous day
					if (tempDelta < 0)		// Check if temporary delta is negative
					{
						tempDelta = (tempDelta * (-1));		// Update temporary delta if necessary
					}
					if (tempDelta > largestDelta)			// Check if temporary delta is greater than current largest delta
					{
						largestDelta = tempDelta;			// Update largest delta if necessary
						// Below if-statements decide what day it is based on i. Can't check Monday's delta because there's no prior data to work with
						if (i == 1)
						{
							deltaDay = "Tuesday";
						}
						if (i == 2)
						{
							deltaDay = "Wednesday";
						}
						if (i == 3)
						{
							deltaDay = "Thursday";
						}
						if (i == 4)
						{
							deltaDay = "Friday";
						}
						if (i == 5)
						{
							deltaDay = "Saturday";
						}
						if (i == 6)
						{
							deltaDay = "Sunday";
						}
					}
				}
			}
			if (currWeek == 1)		// Check if we're looking at the second week
			{
				for (int i = 7; i <= currDay; i++)		// Look at linked lists for each day up to (and including) current day
				{
					tempBuckets += dayInfo[i].getBuckets(); // Make sure to get an accurate count the overflows of each day
					tempSum += dayInfo[i].sum();		// Add the sum of the current day to the temporary week sum
					if (isinf(tempSum))					// Check if the temporary week sum overflowed
					{
						tempSum = 0;					// Reset the temporary week sum if necessary
						tempSum += dayInfo[i].sum();	// Temporary sum should equal week sum after the if-statement closes
						weekSum = 0;					// Reset the actual week sum if necessary
						tempBuckets += 1;				// Increment overflow count
					}
					weekSum += dayInfo[i].sum();		// Week sum should not overflow given previous if statment (assuming input isn't too large)
					if (dayInfo[i].maxVal() > weekMax)	// Check if current day's max is greater than week max
					{
						weekMax = dayInfo[i].maxVal();	// Update week max if necessary
					}
					if (dayInfo[i].minVal() < weekMin)	// Check if current day's min is less than week min
					{
						weekMin = dayInfo[i].minVal();	// Update week min if necessary
					}
					weekCount += dayInfo[i].size();		// Add the number of entries of the current day to the week's entry count
				}
				for (int i = 8; i <= currDay; i++)		// Start checking for Day-to-Day deltas on the second day (second week)
				{
					int tempDelta = (dayInfo[i].size() - dayInfo[i - 1].size());	// Temporary delta equals the difference between the size of the current day and the size of the previous day
					if (tempDelta < 0)		// Check if temporary delta is negative
					{
						tempDelta = (tempDelta * (-1));		// Update temporary delta if necessary
					}
					if (tempDelta > largestDelta)			// Check if temporary delta is greater than current largest delta
					{
						largestDelta = tempDelta;			// Update largest delta if necessary
						// Below if-statements decide what day it is based on i. Can't check Monday's delta because there's no prior data to work with
						if (i == 8)
						{
							deltaDay = "Tuesday";
						}
						if (i == 9)
						{
							deltaDay = "Wednesday";
						}
						if (i == 10)
						{
							deltaDay = "Thursday";
						}
						if (i == 11)
						{
							deltaDay = "Friday";
						}
						if (i == 12)
						{
							deltaDay = "Saturday";
						}
						if (i == 13)
						{
							deltaDay = "Sunday";
						}
					}
				}
			}

			if (weekSum == 0)		// If the sum is zero, I know that there's no entries in the list; hence, the following declarations
			{
				cout << "\tSum of entries            :  No entries." << endl;
				cout << "\tMax of entries            :  No entries." << endl;
				cout << "\tMin of entries            :  No entries." << endl;
				cout << "\tNumber of entries         :  No entries." << endl;
				cout << "\tLargest day-to-day delta  :  No entries." << endl;
				cout << "\tDay holding largest delta :  No entries." << endl << endl;
			}
			else
			{						// If there's data in the linked list, I can display information
				cout << "\tSum of entries            :  ( " << tempBuckets << " * " << fullBucket << " ) + " << weekSum << endl;
				cout << "\tMax of entries            :  " << weekMax << endl;
				cout << "\tMin of entries            :  " << weekMin << endl;
				cout << "\tNumber of entries         :  " << weekCount << endl;
				cout << "\tLargest day-to-day delta  :  " << largestDelta << endl;
				cout << "\tDay holding largest delta :  " << deltaDay << endl << endl;
			}
		}

		// Check if the user's input can be stored as a float
		else if (isFloat(input)) // I use the function from Stack Overflow here
		{
			float value = stof(input);
			if (value > 0)	// If the user's number is greater than 0...
			{
				dayInfo[currDay].insertAsFirst(value);	// ...add that number to the current day's list
			}
		}

		// The user's input was invalid if none of the above if-elseif statements were used
		else
		{
			cout << "Command Invalid:  " << input << endl << endl;
		}
	}

	cout << "Your 2 weeks are over! Calculating final summary..." << endl << endl;

	float tempFinSum = 0;							// Final sum (to check for overflow)
	float FinSum = 0;								// Final sum
	int FinBuckets = 0;								// Total overflow over the two weeks
	float FinMax = dayInfo[0].maxVal();				// Max value over the two weeks
	float FinMin = dayInfo[0].minVal();				// Min value over the two weeks
	int FinEntryCount = 0;							// Total number of entries
	string FinDeltaDay = "Monday, Week 1";					// Day with largest day-to-day delta
	int FinDelta = 0;								// Largest delta

	for (int i = 0; i < 14; i++)					// Search through all the days one more time
	{
		remaining[i] = dayInfo[i].sum();			// Store final sum for each day in array
		fullBuckets[i] = dayInfo[i].getBuckets();	// Store final overflow count for each day in array
		tempFinSum += remaining[i];					// Store temporary for all days
		if (isinf(tempFinSum))						// If the temporary sum overflows...
		{
			tempFinSum = 0;							// Reset temporary sum.
			tempFinSum += remaining[i];				// Store the last number added. This needs to be done so tempFinSum == FinSum after this if-statement.
			FinSum = 0;								// Reset final sum.
			FinBuckets += 1;						// Increment final overflow count
		}
		FinSum += remaining[i];						// Store the last number added
		FinBuckets += fullBuckets[i];				// Update final overflow count
		if (dayInfo[i].maxVal() > FinMax)
		{
			FinMax = dayInfo[i].maxVal();			// Update max value
		}
		if (dayInfo[i].minVal() < FinMin)
		{
			FinMin = dayInfo[i].minVal();			// Update min value
		}
		FinEntryCount += dayInfo[i].size();			// Update entry count
	}
	
	for (int i = 1; i < 14; i++)
	{
		int tempDelta = (dayInfo[i].size() - dayInfo[i - 1].size());	// Temporary delta equals the difference between the size of the current day and the size of the previous day
		if (tempDelta < 0)		// Check if temporary delta is negative
		{
			tempDelta = (tempDelta * (-1));		// Update temporary delta if necessary
		}
		if (tempDelta > FinDelta)			// Check if temporary delta is greater than current largest delta
		{
			FinDelta = tempDelta;			// Update largest delta if necessary
			// Below if-statements decide what day it is based on i. Can't check Monday's delta because there's no prior data to work with
			if (i == 1)
			{
				FinDeltaDay = "Tuesday, Week 1";
			}
			if (i == 2)
			{
				FinDeltaDay = "Wednesday, Week 1";
			}
			if (i == 3)
			{
				FinDeltaDay = "Thursday, Week 1";
			}
			if (i == 4)
			{
				FinDeltaDay = "Friday, Week 1";
			}
			if (i == 5)
			{
				FinDeltaDay = "Saturday, Week 1";
			}
			if (i == 6)
			{
				FinDeltaDay = "Sunday, Week 1";
			}
			if (i == 7)
			{
				FinDeltaDay = "Monday, Week 2";
			}
			if (i == 8)
			{
				FinDeltaDay = "Tuesday, Week 2";
			}
			if (i == 9)
			{
				FinDeltaDay = "Wednesday, Week 2";
			}
			if (i == 10)
			{
				FinDeltaDay = "Thursday, Week 2";
			}
			if (i == 11)
			{
				FinDeltaDay = "Friday, Week 2";
			}
			if (i == 12)
			{
				FinDeltaDay = "Saturday, Week 2";
			}
			if (i == 13)
			{
				FinDeltaDay = "Sunday, Week 2";
			}
		}
	}

	cout << "Calculations complete. Here's your final summary:" << endl;
	if (FinSum == 0)	// If the final sum is zero, I know that all linked lists were empty
	{
		cout << "\tSum of entries            :  No entries." << endl;
		cout << "\tMax of entries            :  No entries." << endl;
		cout << "\tMin of entries            :  No entries." << endl;
		cout << "\tNumber of entries         :  No entries." << endl;
		cout << "\tLargest day-to-day delta  :  No entries." << endl;
		cout << "\tDay holding largest delta :  No entries." << endl << endl;
	}
	else
	{
		cout << "\tSum of entries            :  ( " << FinBuckets << " * " << fullBucket << " ) + " << FinSum << endl;
		cout << "\tMax of entries            :  " << FinMax << endl;
		cout << "\tMin of entries            :  " << FinMin << endl;
		cout << "\tNumber of entries         :  " << FinEntryCount << endl;
		cout << "\tLargest day-to-day delta  :  " << FinDelta << endl;
		cout << "\tDay holding largest delta :  " << FinDeltaDay << endl << endl;
	}
	
    return 0;
}
//////
//////
////
////
//
//

Node::Node(float entry)
{
	entry_ = entry;		// Take the entry from the user. Store it as part of a Node object.
	next_ = NULL;		// Address part of the node is null to signify the end of the linked list.
}

Node::Node(float entry, Node * next)
{
	entry_ = entry;		// Take the entry from the user. Store it as part of a Node object.
	next_ = next;		// Make sure the address of the next linked list element is stored in the current linked list element.
}

Node::~Node()
{

}


List::List()
{
	first_ = NULL;
}

List::~List()
{
	while (!empty())
	{
		removeFirst();
	}
}

// Determines if linked list is empty
bool List::empty() const
{
	return first_ == NULL;
}

// Adds a new element to beginning of linked list
void List::insertAsFirst(float x)
{
	first_ = new Node(x, first_);
}

// Removes the first element of linked list, returns element's value
float List::removeFirst()
{
	float item = first_->entry_;
	Node * tempPtr = first_;
	first_ = first_->next_;
	delete tempPtr;
	return item;
}

// Probably not needed.
void List::print() const
{
	cout << "[ ";
	if (!empty())
	{
		// The first entry is printed separately because no comma
		// is needed.
		cout << first_->entry_;
		Node * ptr = first_->next_;
		while (ptr != NULL)
		{
			cout << ", " << ptr->entry_;
			ptr = ptr->next_;
		}
	}
	cout << " ]";
}

// Returns number of elements in linked list
int List::size()
{
	int count = 0;									// Make a variable to count each entry.
	if (!empty())									// If the List object is not empty...
	{
		if (first_ != NULL)							// ...if the first entry has information...
		{
			count++;								// ...increment the count...
			Node * ptr = first_->next_;				// ...make a pointer to the next address.
			while (ptr != NULL)						// While the pointer continues to point to an address...
			{
				count += 1;							// ...increment the count...
				ptr = ptr->next_;					// ...make the pointer point to the next address.
			}
		}
	}
	return count;
}

// Calculates sum of linked list values, returns 0 if list is empty
float List::sum()
{
	if (!empty())
	{
		bucketCount = 0;								// This is the count of overflows. If sum functions is used multiple times on the same list, I need to be sure I calculate overflows over again.
		float tempsum = 0;								// Make a variable to help determine overflow.
		float sum = 0;									// Make a variable to keep track of the sum.
		if (!empty())									// If the List object is not empty...
		{
			if (first_ != NULL)							// ...if the first entry has information...
			{
				tempsum += first_->entry_;				// ...add the value of the entry to the temporary sum...
				sum += first_->entry_;					// ...add the value of the entry to the sum...
				Node * ptr = first_->next_;				// ...make a pointer to the next address.
				while (ptr != NULL)						// While the pointer continues to point to an address...
				{
					tempsum += ptr->entry_;				// ...add the next value to the temporary sum...
					if (isinf(tempsum))					// If temporary sum overflows...
					{
						tempsum = 0;					// ...reset temporary sum...
						tempsum += ptr->entry_;			// ...add the last value to tempsum before it overflowed...
						sum = 0;						// ...reset sum...
						bucketCount += 1;				// ...increment count of overflows...
					}
					sum += ptr->entry_;					// ...add the next value to the summ...
					ptr = ptr->next_;					// ...make the pointer point to the next address.
				}
			}
		}
		return sum;
	}
	else
	{
		return 0;
	}
}

// Returns the number of overflows encountered when calculating sum
int List::getBuckets()
{
	return bucketCount;
}

// Finds the maximum value in the linked list, returns maximum
float List::maxVal()
{
	if (!empty())							// If the list is not empty...
	{
		Node * ptr = first_;			// ...point to the first element of the linked list...
		float currMax = ptr->entry_;	// ...set current maximum to value of first entry.
		while (ptr->next_ != NULL)		// While not at the end of the list...
		{
			if (ptr->entry_ > currMax)	// ...check to see if the current value in the linked list is greater than current maximum...
			{
				currMax = ptr->entry_;	// ...update current maximum if necessary...
			}
			ptr = ptr->next_;			// ...point to the next element of the linked list.
		}
		if (ptr->entry_ > currMax)		// Make sure to compare the last element of the linked list.
		{
			currMax = ptr->entry_;		// Update current maximum if necessary.
		}
		return currMax;					// Return current maximum.
	}
	else
	{
		return 0;
	}
}

// Finds the minimum value in the linked list, returns minimum
float List::minVal()
{
	if (!empty())						// If the list is not empty...
	{
		Node * ptr = first_;			// ...point to the first element of the linked list...
		float currMin = ptr->entry_;	// ...set current minimum to value of first entry.
		while (ptr->next_ != NULL)		// While not at the end of the list...
		{
			if (ptr->entry_ < currMin)	// ...check to see if the current value in the linked list is less than current minimum...
			{
				currMin = ptr->entry_;	// ...update current minimum if necessary...
			}
			ptr = ptr->next_;			// ...point to the next element of the linked list.
		}
		if (ptr->entry_ < currMin)		// Make sure to compare the last element of the linked list.
		{
			currMin = ptr->entry_;		// Update current minimum if necessary.
		}
		return currMin;					// Return minimum.
	}
	else 
	{
		return 0;
	}
}

// Iterative version of clone.
// This version walks down the linked structure making a
// new Node for each double in the structure.
Node * List::clone(Node * ptr)
{
	if (ptr == NULL)
	{
		return NULL;
	}
	Node * first = new Node(ptr->entry_);
	Node * last = first;
	ptr = ptr->next_;
	while (ptr != NULL)
	{
		last->next_ = new Node(ptr->entry_);
		last = last->next_;
		ptr = ptr->next_;
	}
	return first;
}

