#include "Queue.h"


Queue::Queue()
{
	front = 0;
	rear = -1;
	length = 0;
}

void Queue::enqueue(Job item)
{
	if (isFull())
	{
		cout << "Queue is full";
	}
	else
	{
		rear++;
		queue[rear] = item;
		length++;
	}
}

Job Queue::dequeue()
{
	if (isEmpty())
	{
		cout << "Queue is empty";
	}
	else
	{
		length--;
		return queue[front++];
	}
}

Job Queue::peek()
{
	return queue[front];
}

int Queue::size()
{
	return length;
}

bool Queue::isEmpty()
{
	return (size() == 0);
}

bool Queue::isFull()
{
	return (size() == MAX);
}

string Queue::contents()
{
	string temp = "";
	for (int i = front; i < rear+1; i++)
	{
		temp += queue[i].id + " ";
	}
	return temp;
}

void Queue::isIdle()
{
	for (int i = front; i < rear + 1; i++)
	{
		if (queue[i].status == "READY") queue[i].wait_time++;
		if (queue[i].status == "BLOCKED") queue[i].io_wait++;
	}
}

int Queue::averageWait()
{
	int temp = 0;
	for (int i = front; i < rear + 1; i++)
	{
		temp += queue[i].wait_time;
	}
	return temp / size();
}

int Queue::averageIoWait()
{
	int temp = 0;
	for (int i = front; i < rear + 1; i++)
	{
		temp += queue[i].io_wait;
	}
	return temp / size();
}

int Queue::averageTAT()
{
	int temp = 0;
	for (int i = front; i < rear + 1; i++)
	{
		temp += queue[i].turn_around_time;
	}
	return temp / size();
}

void Queue::print()
{
	for (int i = front; i < rear+1; i++)
	{
		cout << "ArrivalTime = " << queue[i].arrivalTime << endl;
		cout << "No_CPUbursts = " << queue[i].No_CPUbursts << endl;
		cout << "CurrentCPUburst = " << queue[i].currentCPUburst << endl;
		cout << "CPU_bursts: ";
		for (int j = 0; j < queue[i].No_CPUbursts; j++)
		{
			cout << queue[i].CPUbursts[j] << " ";
		}
		cout << endl;
		cout << "CPU_bursts: ";
		for (int j = 0; j < queue[i].No_CPUbursts-1; j++)
		{
			cout << queue[i].IObursts[j] << " ";
		}
		cout << endl;
		cout << "Status = " << queue[i].status << endl << endl;
	}
}