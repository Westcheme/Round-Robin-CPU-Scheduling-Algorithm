#pragma once

#include <iostream>
#include <string>
using namespace std;

#define MAX 100

struct Job
{
	int arrivalTime;
	int No_CPUbursts;
	int currentCPUburst = 0;
	int CPUbursts[10];
	int IObursts[10];
	string id;
	string status = "NEW";
	int turn_around_time = 0;
	int wait_time = 0;
	int io_wait = 0;
};


class Queue
{
private:
	Job queue[MAX];
	int front;
	int rear;
	int length;

public:
	Queue();

	void enqueue(Job item);
	Job dequeue();
	Job peek();
	int size();
	bool isEmpty();
	bool isFull();
	string contents();
	void isIdle();
	int averageWait();
	int averageIoWait();
	int averageTAT();
	void print();
};