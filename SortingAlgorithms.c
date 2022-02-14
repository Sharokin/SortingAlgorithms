#define _CRT_SECURE_NO_WARNINGS
#include "SortingAlgorithms.h"
#include "Statistics.h"
#include "Array.h"
#include <assert.h>
#include <string.h>

int isImplemented(SortingAlgorithm algorithm)
{
	switch (algorithm)
	{
  //    case BUBBLE_SORT:
//      case INSERTION_SORT:
 //     case SELECTION_SORT:
      case QUICK_SORT:
//      case MERGE_SORT:
            return 1;
        default:
            return 0;
	}
}

/*Antalet byten i en och samma algoritm kan vara olika beroende pŒ implementationen. Ibland ligger datat redan pŒ rŠtt plats och dŒ kan man vŠlja att testa det och inte gšra ett byte (vilket ger extra jŠmfšrelse) eller sŒ kan man ŠndŒ gšra ett byte (med sig sjŠlv). Fšljer man de algoritmer som vi gŒtt igenom pŒ fšrelŠsningarna exakt sŒ gšr man en swap Šven pŒ ett element som ligger pŒ rŠtt plats
 
    NŠr du analyserar det data som genereras (result.txt) sŒ behšver du ha detta i Œtanke */

/******************************************************************************************/
/* Era algoritmer har: */

static void bubbleSort(ElementType* arrayToSort, unsigned int size, Statistics* statistics)
{
    int checker, j = 0;  //j tells us that the last elements are already sorted
    do{
        checker = 0;
        for(int i = 0; lessThan(i, size - 1 - j, statistics); i++)
            {
                if(greaterThan(arrayToSort[i], arrayToSort[i+1], statistics))
                {
                    swapElements(&arrayToSort[i], &arrayToSort[i+1], statistics);
                    checker = 1;
                }
            }
        j++;
    }while(equalTo(checker, 1, statistics));
    
}

static void insertionSort(ElementType* arrayToSort, unsigned int size, Statistics* statistics)
{
    for(int i = 1; lessThan(i, size, statistics); i++)
    {
        int current = arrayToSort[i];
        int j;
        
        for(j = i -1; arrayToSort[j] > current && j >= 0; j--)
        {
            arrayToSort[j+1] = arrayToSort[j];
        }
        arrayToSort[j+1] = current;
    }
}

static void selectionSort(ElementType* arrayToSort, unsigned int size, Statistics* statistics)
{
    int indexOfReplaceElement = 0, j = 0;
    for(int i = 0; lessThan(i, size, statistics); i++)
    {
        int lowest = arrayToSort[i];
        
        for(j = i; lessThan(j, size, statistics); j++)
        {
            if(lessThan(arrayToSort[j], lowest, statistics))
            {
                lowest = arrayToSort[j];
                indexOfReplaceElement = j;
            }
        }
        ElementType temp = arrayToSort[i];
        arrayToSort[i] = lowest;
        arrayToSort[j] = temp;
    }
}

static void mergeSort(ElementType* arrayToSort, unsigned int size, Statistics* statistics)
{
}

static void quickSort(ElementType* arrayToSort, unsigned int size, Statistics* statistics)
{
    quickSortFunc1(arrayToSort, 0, size - 1, statistics);
}

void quickSortFunc1(ElementType* arrayToSort, int low, int high, Statistics* statistics)
{
    if(lessThan(low, high, statistics))
    {
        int indexFromPartition = partition(arrayToSort, low, high, statistics);
        quickSortFunc1(arrayToSort, low, indexFromPartition - 1, statistics);
        quickSortFunc1(arrayToSort, indexFromPartition + 1, high, statistics);
        
    }
}

int partition(ElementType* arrayToSort, int low, int high, Statistics* statistics)
{
    int pivot = arrayToSort[low]; //first element is our pivot, always
    int i = low, j = high;
    
    while(lessThan(i, j, statistics))
    {
        while(lessThanOrEqualTo(arrayToSort[i], pivot, statistics) && lessThan(i, j, statistics))
            i++; //keep looking for a element bigger than pivot
        while(greaterThanOrEqualTo(arrayToSort[j], pivot, statistics) && greaterThanOrEqualTo(j, i, statistics))
            j--; //keep looking for a element smaller than pivot
        if(lessThan(i, j, statistics))
            swapElements(&arrayToSort[i], &arrayToSort[j], statistics); 
    }
    swapElements(&arrayToSort[low], &arrayToSort[j], statistics);
    return j;  // returns what index the pivot is at
}


/******************************************************************************************/


char* getAlgorithmName(SortingAlgorithm algorithm)
{
	/* Ar inte strangen vi allokerar lokal for funktionen?
	   Nej, inte i detta fall. Strangkonstanter ar ett speciallfall i C */
	switch (algorithm)
	{
        case BUBBLE_SORT:	 return "  Bubble sort ";
        case SELECTION_SORT: return "Selection sort";
        case INSERTION_SORT: return "Insertion sort";
        case MERGE_SORT:	 return "  Merge sort  ";
        case QUICK_SORT:	 return "  Quick sort  ";
        default: assert(0 && "Ogiltig algoritm!"); return "";
	}
}

// Sorterar 'arrayToSort' med 'algorithmToUse'. Statistik for antal byten och jamforelser hamnar i *statistics
static void sortArray(ElementType* arrayToSort, unsigned int size, SortingAlgorithm algorithmToUse, Statistics* statistics)
{
	if(statistics != NULL)
		resetStatistics(statistics);

	switch (algorithmToUse)
	{
	case BUBBLE_SORT:	 bubbleSort(arrayToSort, size, statistics); break;
	case SELECTION_SORT: selectionSort(arrayToSort, size, statistics); break;
	case INSERTION_SORT: insertionSort(arrayToSort, size, statistics); break;
	case MERGE_SORT:	 mergeSort(arrayToSort, size, statistics); break;
	case QUICK_SORT:	 quickSort(arrayToSort, size, statistics); break;
	default:
		assert(0 && "Ogiltig algoritm!");
	}
}

// Forbereder arrayer for sortering genom att allokera minne for dem, samt intialisera dem
static void prepareArrays(SortingArray sortingArray[], SortingAlgorithm algorithm, const ElementType* arrays[], const unsigned int sizes[])
{
	assert(isImplemented(algorithm));

	int i;
	int totalArraySize;

	for (i = 0; i < NUMBER_OF_SIZES; i++)
	{
		totalArraySize = sizeof(ElementType)*sizes[i];
		sortingArray[i].arrayToSort = malloc(totalArraySize);
		memcpy(sortingArray[i].arrayToSort, arrays[i], totalArraySize);

		sortingArray[i].algorithm = algorithm;
		sortingArray[i].arraySize = sizes[i];
		resetStatistics(&sortingArray[i].statistics);
	}
}

// Sorterar arrayerna
static void sortArrays(SortingArray toBeSorted[])
{
	int i;
	for (i = 0; i < NUMBER_OF_SIZES; i++)
	{
		SortingArray* current = &toBeSorted[i];
		sortArray(current->arrayToSort, current->arraySize, current->algorithm, &current->statistics);
	
		if (!isSorted(current->arrayToSort, current->arraySize))
		{
			printf("Fel! Algoritmen %s har inte sorterat korrekt!\n", getAlgorithmName(current->algorithm));
			printf("Resultatet är: \n");
			printArray(current->arrayToSort, current->arraySize, stdout);
			assert(0); // Aktiveras alltid
		}
	}
}

void printResult(SortingArray sortedArrays[], FILE* file)
{
	assert(file != NULL);

	int i;
	for (i = 0; i < NUMBER_OF_SIZES; i++)
	{
		fprintf(file, "%4d element: ", sortedArrays[i].arraySize);
		printStatistics(&sortedArrays[i].statistics, file);
		fprintf(file, "\n");
	}
	fprintf(file, "\n");
}

void sortAndPrint(SortingArray sortingArray[], SortingAlgorithm algorithm, const ElementType* arrays[], unsigned int sizes[], FILE* file)
{
	assert(file != NULL);

	prepareArrays(sortingArray, algorithm, arrays, sizes);
	sortArrays(sortingArray);
	printResult(sortingArray, file);
}

void freeArray(SortingArray sortingArray[])
{
	int i;
	for (i = 0; i < NUMBER_OF_SIZES; i++)
	{
		if (sortingArray[i].arrayToSort != NULL)
			free(sortingArray[i].arrayToSort);
		sortingArray[i].arrayToSort = NULL;
		sortingArray[i].arraySize = 0;
		resetStatistics(&sortingArray[i].statistics);
	}
}
