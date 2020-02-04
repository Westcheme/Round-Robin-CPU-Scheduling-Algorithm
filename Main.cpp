//Program Name: Round Robin CPU Scheduling Simulation
//Programmer Name: Jacob Westerbeek
//Description:	The program will simulate the round-robin scheduling technique 
//				used for scheduling jobs for the CPU to process as well as assigning an order 
//				in which the CPU will take processes to make sure the CPU is idle as little as possible.
//Date Created:	1-18-2020



#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Queue.h"
using namespace std;



void printProcess(Job job);



int main()
{
	int systemClock = 0, quantumTime, CPUcount;
	float CPU_utilization = 0;
	bool CPUidle = true, IOidle = true, sendToIO = false, eventOccured = false, preempt = false;
	Job CPUprocess, IOprocess;
	Queue entryQueue, readyQueue, IOQueue, exitQueue;

	string input;
	Job inputProcess;

	cout << "Choose an output file: ";
	cin >> input;
	ofstream outputFile("output_files/" + input + ".txt");

	cout << "Choose an input file: ";
	cin >> input;
	ifstream inputFile("input_files/" + input + ".txt");

	cout << "Choose a log file: ";
	cin >> input;
	ofstream logFile("output_files/" + input + ".txt");

	while (true)
	{
		cout << "Choose a Quantum Time: ";
		cin >> quantumTime;
		if (quantumTime <= 0) cout << "Quantum Time must be greater than 0" << endl;
		else goto done;
	}
	done:

	//READ FROM FILE
	if (inputFile.is_open())
	{
		for (int i = 0; !inputFile.eof(); i++)
		{
			inputFile >> inputProcess.arrivalTime;
			inputFile >> inputProcess.No_CPUbursts;
			for (int j = 0; j < inputProcess.No_CPUbursts; j++)
			{
				inputFile >> inputProcess.CPUbursts[j];
				if (j != inputProcess.No_CPUbursts - 1) inputFile >> inputProcess.IObursts[j];
			}
			inputProcess.id = "P" + to_string(i);
			entryQueue.enqueue(inputProcess);
		}
	}
	else
	{
		cout << "File is not open";
		goto end;
	}
	inputFile.close();

	outputFile << "System Clock\tCPU\tIO\tRQ\t\tIOQ" << endl;

	//SIMULATION BEGINS
	while (!entryQueue.isEmpty() || !readyQueue.isEmpty() || !IOQueue.isEmpty() || CPUidle == false || IOidle == false)
	{
		eventOccured = false;
		logFile << systemClock << ": ";

		if (systemClock % 5 == 0)
		{
			logFile << "\tRQ:  " << readyQueue.contents() << endl;
			logFile << "\tIOQ: " << IOQueue.contents() << endl;
		}

		//=======================================---NEW PROCESS---=========================================//
		if (entryQueue.peek().arrivalTime == systemClock)
		{
			entryQueue.peek().status = "READY";
			readyQueue.enqueue(entryQueue.dequeue());
			logFile << "\t" << readyQueue.peek().id << " NEW sent to Ready Queue" << endl;
			eventOccured = true;
		}

		//=======================================---CPU---==========================================//
		if (CPUidle == true && !readyQueue.isEmpty())
		{
			eventOccured = true;
			CPUcount = 0;
			CPUprocess = readyQueue.dequeue();
			logFile << "\t" << CPUprocess.id << " RUNNING in CPU" << endl;
			CPUidle = false;
			CPUprocess.status = "RUNNING";
			CPUprocess.CPUbursts[CPUprocess.currentCPUburst]--;
			CPU_utilization++;
			CPUcount++;
			if (CPUprocess.CPUbursts[CPUprocess.currentCPUburst] == 0) goto processFinished;
		}
		else if (CPUidle == false)
		{
			CPU_utilization++;
			CPUprocess.CPUbursts[CPUprocess.currentCPUburst]--;
			CPUcount++;
			if (CPUprocess.CPUbursts[CPUprocess.currentCPUburst] == 0)
			{
				eventOccured = true;
				processFinished:
				CPUprocess.currentCPUburst++;
				if (CPUprocess.currentCPUburst == CPUprocess.No_CPUbursts)
				{
					logFile << "\t" << CPUprocess.id << " TERMINATED" << endl;
					CPUprocess.turn_around_time = systemClock - CPUprocess.arrivalTime;
					CPUprocess.status = "EXIT";
					cout << systemClock << ": " << CPUprocess.id << " terminated: Turn-Around-Time = " << CPUprocess.turn_around_time+1 << ", Wait Time = " << CPUprocess.wait_time << ", I/O Wait = " << CPUprocess.io_wait << endl;
					exitQueue.enqueue(CPUprocess);
					CPUidle = true;
					goto CPUfinish;
				}
				logFile << "\t" << CPUprocess.id << " BLOCKED CPUbursts finished and Sent to IOQueue" << endl;
				CPUprocess.status = "BLOCKED";
				sendToIO = true;
				CPUidle = true;
			}
			else if (CPUcount == quantumTime)
			{
				eventOccured = true;
				preempt = true;
				logFile << "\t" << CPUprocess.id << " has been Preempted" << endl;
				CPUprocess.status = "READY";
				CPUidle = true;
			}
		}
		CPUfinish:
		
		//========================================---IO---=========================================//
		if (IOidle == true && !IOQueue.isEmpty())
		{
			eventOccured = true;
			IOprocess = IOQueue.dequeue();
			logFile << "\t" << IOprocess.id << " BLOCKED and Running in IO" << endl;
			IOidle = false;
			IOprocess.IObursts[IOprocess.currentCPUburst - 1]--;
		}
		else if (IOidle == false)
		{
			IOprocess.IObursts[IOprocess.currentCPUburst - 1]--;
			if (IOprocess.IObursts[IOprocess.currentCPUburst - 1] == 0)
			{
				eventOccured = true;
				logFile << "\t" << IOprocess.id << " READY IObursts finished and Sent to readyQueue" << endl;
				IOprocess.status = "READY";
				readyQueue.enqueue(IOprocess);
				IOidle = true;
			}
		}
		
		if (sendToIO == true)
		{
			IOQueue.enqueue(CPUprocess);
			sendToIO = false;
		}

		if (preempt == true)
		{
			readyQueue.enqueue(CPUprocess);
			preempt = false;
		}

		outputFile << systemClock << "\t\t";
		if (CPUidle == false) outputFile << CPUprocess.id;
		outputFile << "\t";
		if (IOidle == false) outputFile << IOprocess.id;
		outputFile << "\t" << readyQueue.contents() << "\t\t" << IOQueue.contents() << endl;

		readyQueue.isIdle();
		IOQueue.isIdle();

		if (eventOccured == false) logFile << "\tNO EVENT" << endl;
		logFile << endl;
		systemClock++;
	}
	//SIMULATION ENDS

	exitQueue.averageTAT();
	exitQueue.averageWait();
	exitQueue.averageIoWait();

	CPU_utilization = CPU_utilization / systemClock * 100;
	cout << "Results for Quantum = " << quantumTime << "  ";
	cout << "CPU Utilization = " << CPU_utilization << "%" << endl;
	cout << "Avg. Turn-Around-Time = " << exitQueue.averageTAT() << ", Avg. Wait Time = " << exitQueue.averageWait() << ", Avg. I/O Wait = " << exitQueue.averageIoWait() << endl;

	end:
	cout << endl << endl;
	outputFile.close();
	logFile.close();
	system("PAUSE");
	return 0;
}


//Description: Prints information associated with the given job
//Pre-condition: The input is a job
//Post-condition: Prints the information to the terminal
void printProcess(Job job)
{
	cout << "ArrivalTime = " << job.arrivalTime << endl;
	cout << "No_CPUbursts = " << job.No_CPUbursts << endl;
	cout << "CurrentCPUburst = " << job.currentCPUburst << endl;
	cout << "CPU_bursts: ";
	for (int i = 0; i < job.No_CPUbursts; i++)
	{
		cout << job.CPUbursts[i] << " ";
	}
	cout << endl;
	cout << "IO_bursts: ";
	for (int i = 0; i < job.No_CPUbursts - 1; i++)
	{
		cout << job.IObursts[i] << " ";
	}
	cout << endl;
	cout << "Status = " << job.status << endl;
	cout << "Process Number: " << job.id << endl;
}