#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <cassert>
#include <algorithm>
#include <deque>
#include <cmath>
#include "MaxHeap.h"


//Method to swap two element of a same array
template<typename T>
void swap(std::vector<T> &array, const int lhs_index, const int rhs_index)
{
    const T temp {array[lhs_index]};
    array[lhs_index] = array[rhs_index];
    array[rhs_index] = temp;
}

//Returns random number in [min, max]
int random_number(const int min, const int max)
{
    return rand() % (max-min+1) + min;
}

//Check if the array is correctly sorted
template<typename T>
bool is_sorted(const std::vector<T> &array)
{
    bool sorted {true};

    for(int i {0}; i < array.size() - 1; i++)
    {
        if(array[i+1] < array[i])
            sorted = false;
    }

    return sorted;
}

//Print the array for debugging purpose
template<typename T>
void debug_array(const std::vector<T> &array)
{
    std::cout << '[';

    for(int i {0}; i < array.size(); i++)
    {
        std::cout << array[i] << (i+1 < array.size() ? "," : "");
    }

    std::cout << ']' << std::endl;
}

//Print the array for debugging purpose
template<typename T>
void debug_array(const std::vector<T> &array, const int first, const int last)
{
    assert(first >= 0 && first < array.size() && last >= 0 && last < array.size());
    std::cout << '[';

    for(int i {first}; i <= last; i++)
    {
        std::cout << array[i] << (i+1 <= last ? "," : "");
    }

    std::cout << ']' << std::endl;
}

//Quick sort algorithm - efficient algorithm
void quick_sort(std::vector<int> &array, const int first_index, const int last_index)
{
    const int pivot_value = array[last_index];
    int wall_index = first_index;

    //Walking through each element from first_index to last_index
    for(int i { first_index }; i <= last_index; ++i)
    {
        if(array[i] < pivot_value)
        {
            //If the element is < to the pivot value, put that element just after the wall
            swap(array, i, wall_index);
            ++wall_index; //And put the wall just after this value. Now this value is to the left of the wall.
        }
    }

    //Put the pivot value to the position of the wall, and the wall value to the last_index position.
    swap(array, wall_index, last_index);

    //Now all elements before the pivot are inferior to the pivot and all elements after are superior to the pivot.

    /*Now we will apply the previous method to the sub-array from first_index to just before the last pivot,
    and from just after the last pivot to the last_index.*/

    //We will just check if there is a sub-array to work on.
    if(wall_index-1 > first_index)
        quick_sort(array, first_index, wall_index-1);

    if(wall_index+1 < last_index)
    quick_sort(array, wall_index+1, last_index);
}

//Bubble sort algorithm - inefficient algorithm
void bubble_sort(std::vector<int> &array)
{
    bool swapped {};

    do
    {
        swapped = false; //For now, no swap were made.
        for(int i {0}; i < array.size() - 1; ++i)
        {
            if(array[i+1] < array[i]) //If twe contiguous elements are out of order, swap them.
            {
                swap(array, i, i+1);
                swapped = true; //We did a swap.
            }
        }
    } while(swapped == true); //While we did at least one swap during last iteration. If no swap occured then the array is sorted.
}

//Insertion sort algorithm - inefficient algorithm
void insertion_sort(std::vector<int> &array)
{
    //For each iteration, elements from 0 to i - 1 are sorted.
    for(int i {1}; i < array.size(); ++i)
    {
        int j {i};

        const int value_to_insert { array[i] }; //Storing value to insert

        //Finding the right place to insert the value in array[0...i-1]
        while(j > 0 && array[j - 1] > value_to_insert)
        {
            array[j] = array[j-1]; //Shift current element to the right to prepare insertion
            --j;
        }

        //We shifted elements to the right so there is a hole at array[j]. This is where the value must be inserted.
        array[j] = value_to_insert;
    }
}

//Shaker sort algorithm - inefficient algorithm
void shaker_sort(std::vector<int> &array)
{
    bool swapped {};

    do
    {
        swapped = false; // No swap occured for now.

        //Perform one iteration of bubble sort from left to right
        for(int i {0}; i < array.size()-1; ++i)
        {
            if(array[i+1] < array[i])
            {
                swap(array, i, i+1);
                swapped = true;
            }
        }

        //Perform one iteration of bubble sort, but now from right to left
        for(int i {array.size()-2}; i >= 0; --i)
        {
            if(array[i+1] < array[i])
            {
                swap(array, i, i+1);
                swapped = true;
            }
        }

    } while(swapped == true); //While we swapped two elements during last iteration. If no swap occured then array is sorted.
}

//Gnome sort algorithm
void gnome_sort(std::vector<int> &array)
{
    int i {0};

    while(i+1 < array.size())
    {
        if(array[i+1] >= array[i]) //If the contiguous elements array[i] array[i+1] are in order, search for next contiguous elements. (i++)
        {
            ++i;
        }

        else //if the elements are unsorted
        {
           swap(array, i, i+1); //Swap the elements to make them sorted

            if(i >= 1) //Go back in iteration (i--).
                --i;
        }
    }
}

//Part of merge-sort algorithm.
//Merge two sorted sub-arrays into one bigger sorted array.
void merge(std::vector<int> &array, std::vector<int> &temp_array, const int left_start_index, const int right_end_index)
{
    temp_array.clear(); //Make the temporary empty to prepare the output sorted array

    const int middle { (left_start_index + right_end_index)/2 }; //Middle index between left_start_index and right_end_index
    const int left_end_index {middle}; //Index of the end of the left sub-array
    const int right_start_index {middle + 1}; //Index of the start of the right sub-array

    int left_cur_index { left_start_index };
    int right_cur_index { right_start_index };

    /*
    To obtain a sorted array from two sorted sub-arrays, compare the front values of the sub-arrays.
    Add the min value to an empty array.
    Remove this value from the sub-array.
    Do this until one array is empty.
    Add all the remaining elements of the other array to the global array we are building.

    This algorithm is implemented without removing the front values but adding 1 to the current searching index of the
    concerned sub-array.
    */

    while(left_cur_index <= left_end_index && right_cur_index <= right_end_index)
    {
        int cur_left_value {array[left_cur_index]};
        int cur_right_value {array[right_cur_index]};

        if(cur_left_value <= cur_right_value)
        {
            temp_array.push_back(cur_left_value);
            ++left_cur_index;
        }
        else
        {
            temp_array.push_back(cur_right_value);
            ++right_cur_index;
        }
    }

    //Now it is not possible anymore to compare the front values of the sub-arrays because at least one is empty.

    //If the left sub-array is not empty
    if(left_cur_index <= left_end_index)
    {
        for(int i {left_cur_index}; i <= left_end_index; ++i) //Add all the values of this sub-array to the final sorted array
        {
            temp_array.push_back(array[i]);
        }
    }

    //Same method for right sub-array.
    else if(right_cur_index <= right_end_index)
    {
        for(int i {right_cur_index}; i <= right_end_index; ++i)
        {
            temp_array.push_back(array[i]);
        }
    }

    //Copy the sorted array we obtained from the 2 sub-arrays into the great array, at the correct index.
    std::copy(temp_array.begin(), temp_array.end(), array.begin() + left_start_index);
}

//Merge-sort algorithm - efficient algorithm
void merge_sort(std::vector<int> &array, std::vector<int> &temp_array, const int start_index, const int end_index)
{
    //If we cannot continue recursively
    if(start_index >= end_index)
        return;

    const int middle_index { (start_index+end_index)/2 }; //Index of the middle of the array [start_index, end_index]

    merge_sort(array, temp_array, start_index, middle_index); //Call merge_sort on left half of the current array
    merge_sort(array, temp_array, middle_index+1, end_index); //Call merge_sort on right half of the current array

    merge(array, temp_array, start_index, end_index); //Merge the two sub-arrays.
}

//User interface for call to sort an array using array
void merge_sort(std::vector<int> &array)
{
    std::vector<int> temp_array {};
    merge_sort(array, temp_array, 0, array.size()-1);
}



//Selection sort algorithm - inefficient algorithm
void selection_sort(std::vector<int> &array)
{
    for(int i {0}; i < array.size() - 1; ++i)
    {
        int lowest_index {i};

        for(int j {i+1}; j < array.size(); ++j) //Iterating array[i+1...end]
        {
            if(array[j] < array[lowest_index]) //If array[j] is < to the minimum found
                lowest_index = j; //Then update the index of the lowest value
        }

        swap(array, i, lowest_index); //Put the lowest value of the array from [i+1...end] at i
    }
}

//Comb sort algorithm - inefficient algorithm
void comb_sort(std::vector<int> &array)
{
    const float reduction_factor { 1.3f }; //The reduction factor (efficient in [1.25; 1.33]). 1.3 is often chosen.
    int gap {array.size() - 1}; //The current gap between two elements to swap

    bool swapped {};

    do
    {
        swapped = false; //Currently, no elements were swapped
        gap/=reduction_factor; //Reduce the gab taking in account reduction_factor

        if(gap < 1) //Gap cannot be 0, because we would swap two same positions.
            gap = 1;

        for(int i {0}; i+gap < array.size(); ++i)
        {
            if(array[i+gap] < array[i]) //If the element array[i+gap] is < to array[i] then they are unsorted
            {
                swap(array, i, i+gap); //Swap them

                swapped = true; //We swapped
            }
        }

    } while(swapped == true || gap > 1); //Continue while we swapped at least two elements in last iteration or the gap is > to 1
}

//Heap-sort algorithm - efficient sort
void heap_sort(std::vector<int> &array)
{
    MaxHeap heap(array); //Class handling a max heap.

    for(int i {0}; i < array.size(); ++i) //Remove the root array.size() times to sort the array
        heap.remove_root();

    //Now array of heap is sorted. You can access to it by heap.get_heap_array();
}

//Shell sort algorithm - inefficient algorithm
void shell_sort(std::vector<int> &array)
{
    const std::vector<int> gaps {701, 301, 132, 57, 23, 10, 4, 1}; //Marcin Ciuras's gap sequence

    for(const int gap : gaps) // For each gap in the gap sequence
    {
        for(int i { gap }; i < array.size(); ++i)
        {
            //Perform an insertion sort
            const int value_to_insert { array[i] };

            int j {i};

            while(j >= gap && array[j - gap] > value_to_insert)
            {
                array[j] = array[j - gap];
                j-=gap;
            }

            array[j] = value_to_insert;
        }
    }
}

//void intro_sort(std::vector<int> &array, const int left_index, const int right_index, const int recursion_depth)
{
    int pivot_value {array[right_index]};
    int wall_index {left_index};

    for(int i {left_index}; i <= right_index; ++i)
    {
        if(array[i] < pivot_value)
        {
            swap(array, i, wall_index);
            ++wall_index;
        }
    }

    swap(array, wall_index, right_index);

    if(wall_index - 1 > left_index)
        {
                intro_sort(array, left_index, wall_index-1, recursion_depth-1);
        }

    if(wall_index +1 < right_index)
        intro_sort(array, wall_index+1, right_index, recursion_depth-1);
}

void pseudo_radix_sort(std::vector<int> &array)
{
    std::vector<std::deque<std::string>> buckets(10);

    std::vector<std::string> to_string_array {};

    int max_digit_number {0};

    for(int i {0}; i < array.size(); i++)
    {
        std::string current_number_str {std::to_string(array[i])};
        int digit_number {current_number_str.size()};

        if(digit_number > max_digit_number)
            max_digit_number = digit_number;

        to_string_array.push_back(current_number_str);

    }

    for(int i {0}; i < to_string_array.size(); i++)
    {
        while(to_string_array[i].size() < max_digit_number)
        {
            to_string_array[i].insert(0, 1, '0');
        }
    }

    //Ready to perform radix-sort.

    for(int d {0}; d < max_digit_number; d++)
    {
        for(int i {0}; i < array.size(); i++)
        {
            std::string string_number {to_string_array[i]};
            buckets[string_number[max_digit_number - d-1] - '0'].push_back(string_number);
        }

        to_string_array.clear();

        for(std::deque<std::string> &bucket : buckets)
        {
            while(!bucket.empty())
            {
                to_string_array.push_back(bucket.front());
                bucket.pop_front();
            }
        }
    }

    for(int i {0}; i < array.size(); i++)
    {
        array[i] = std::stoi(to_string_array[i]);
    }
}

typedef std::chrono::high_resolution_clock Clock;

int main()
{
    srand(time(NULL));

    std::vector<int> array {};

    for(int i {0}; i < 100000000; i++)
    {
        array.push_back(rand()%100000000);
    }

    auto start_time = Clock::now();

    intro_sort(array, 0, array.size()-1, 2 * (std::log2(array.size())));

    auto end_time = Clock::now();

    std::cout << std::boolalpha << is_sorted(array) << std::endl;

    std::cout << "Took " << static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(end_time-start_time).count())/1000000000 << "s.";

    return 0;
}
