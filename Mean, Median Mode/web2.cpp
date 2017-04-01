/*
 * web2.cpp
 *
 *  Created on: Dec 19, 2014
 *      Author: Shurjo
 */
#include <iostream>

double GetMedian(double daArray[], int iSize) {
    // Allocate an array of the same size and sort it.
    double* dpSorted = new double[iSize];
    for (int i = 0; i < iSize; ++i) {
        dpSorted[i] = daArray[i];
    }
    for (int i = iSize - 1; i > 0; --i) {
        for (int j = 0; j < i; ++j) {
            if (dpSorted[j] > dpSorted[j+1]) {
                double dTemp = dpSorted[j];
                dpSorted[j] = dpSorted[j+1];
                dpSorted[j+1] = dTemp;
            }
        }
    }

    // Middle or average of middle values in the sorted array.
    double dMedian = 0.0;
    if ((iSize % 2) == 0) {
        dMedian = (dpSorted[iSize/2] + dpSorted[(iSize/2) - 1])/2.0;
    } else {
        dMedian = dpSorted[iSize/2];
    }
    delete [] dpSorted;
    return dMedian;
}

double GetMode(double daArray[], int iSize) {
    // Allocate an int array of the same size to hold the
    // repetition count
    int* ipRepetition = new int[iSize];
    for (int i = 0; i < iSize; ++i) {
        ipRepetition[i] = 0;
        int j = 0;
        while ((j < i) && (daArray[i] != daArray[j])) {
            if (daArray[i] != daArray[j]) {
                ++j;
            }
        }
        ++(ipRepetition[j]);
    }
    int iMaxRepeat = 0;
    for (int i = 1; i < iSize; ++i) {
        if (ipRepetition[i] > ipRepetition[iMaxRepeat]) {
            iMaxRepeat = i;
        }
    }
    delete [] ipRepetition;
    return daArray[iMaxRepeat];
}

double GetMean(double daArray[], int iSize) {
    double dSum = daArray[0];
    for (int i = 1; i < iSize; ++i) {
        dSum += daArray[i];
    }
    return dSum/iSize;
}

int main()
{
    double dValues[] = {3.4, 4.8, 8.4, 9.6, 2.3, 9.6, 5.6, 9.6, 4.8, 2.2};
    int iArraySize = 10;

    std::cout << "Median = " << GetMedian(dValues, iArraySize) << std::endl;
    std::cout << "Mode = " << GetMode(dValues, iArraySize) << std::endl;
    std::cout << "Mean = " << GetMean(dValues, iArraySize) << std::endl;

    return 0;
}



