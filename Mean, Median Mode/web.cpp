/*
 * web.cpp
 *
 *  Created on: Dec 17, 2014
 *      Author: Shurjo
 */
#include <cstdlib>
#include <iostream>

using namespace std;

//define the 'comparison function' used by qsort() for the sort
int CompareValues(const void *a, const void *b){
    const int *val1 = (const int *)a;
    const int *val2 = (const int *)b;
    if(*val1 < *val2) return -1;
    else if (*val1 == *val2) return 0;
    else return 1;
}

//define the CalcMean() , CalcMedian(), CalcMode() and CalcRange(). CalcMedian() and CalcMode() work on a pre-sorted array!
float CalcMean(const int values[] , int num){
    if (num<=0) return 0;
    int count=0;
    for (int i=0;i<num;i++) count += values[i];
    return (float)count / num;
}

float CalcMedian(const int values[],int num){
    if(num<=0) return 0;
    if(num%2) return (float)values[(num+1)/2];
    else{
        int pos = num/2;
        return (float)(values[pos] + values[pos+1]) / 2;
    }
}

int CalcMode(const int values[],int num){
    if(num<=0)return 0;
    int max_count=0 , pos=0,max_nums=0;
    int count=1;
    for(int i=1;i<num;i++){
        if(values[i] != values[pos]){
            if(count > max_count){
                max_count=count;
                max_nums=0;
            }else if (count == max_count) max_nums++;
            pos= i;
            count=0;
        }else count++;
    }
    if(max_nums) return 0;
    else return values[pos];
}

int CalcRange(const int values[] , int num){
    if(num<=0) return -1;
    int max,min;
    max=min=values[0];
    for(int i=1;i<num;i++){
        if(values[i]>max)max=values[i];
        else if (values[i]<min) min=values[i];
    }
    return max-min;
}

#define NUM 10

int main_(){
    int numbers[NUM];
    int count=0, val;
    do{
        cout << "enter a number ("<< count + 1<<" of "<<NUM<<" - digit 5 to stop input) : ";
        cin >> val;
        if(val == 5){
            cout<<"\nInput stopped! Digited numbers: "<<count<<".\n";
            break;
        }
        numbers[count] = val;
        count++;
    }while(count<NUM);

    int mean=CalcMean(numbers,count);
    int range=CalcRange(numbers,count);
    qsort( (void*)numbers, count, sizeof(int),CompareValues);
    float mode=CalcMode(numbers,count);
    float median=CalcMedian(numbers,count);

    cout<<"\n  Mean: "<<mean;
    cout<<"\n  Median: "<<median;
    cout<<"\n  Mode: "<<mode;
    cout<<"\n  Range: "<<range;
    cout<<endl;

    return 0;
}
