#ifndef MLFQ_H
#define MLFQ_H

#include<fstream>
#include<queue>
#include<iostream>
#include<string>
#include<iomanip>
#include<vector>
#include<windows.h>
#include<ctime>
#include<cmath>
#include<cstdlib>
#include<string>
#include<conio.h>

using namespace std;

//All details are also mentioned in Readme file
//this Class will be used for queue, queue will store details of process
struct Process
{
	int processId;
	int arrivalTime;
	int burstTime;
	int TimeWaiting;
	Process(int pid,int at,int bt, int tw)
	{
		processId = pid;
		burstTime = bt;
		arrivalTime = at;
		TimeWaiting = tw;
	}
};
//this class will be used in for vector, grant chart will used this class 
struct Node
{
	int pid;
	int start;
	int end;
	Node(int p,int s,int e)
	{
		pid = p;
		start = s;
		end = e;
	}
};


class MLFQ
{
	int * pid;
	int * arrival_time;
	int * burst_time;
	int * waitingTime;
	int * responseTime;
	int * turnAroundTime;
	double avgWaitingTime;
	int waitTimer1, waitTimer2;
	double avgTurnAroundTime;
	double avgResponseTime;
	int totalPids = 0;
	queue<Process*> queue0;
	queue<Process*> queue1;
	queue<Process*> queue2;
	queue<Process*> tempQueue;
	queue<Process*> tempQueue1;
	queue<Process*> tempQueue2;
	vector<Node> granttChart;
	int counter;
	int currentQueue;
	int QUANTUM0, QUANTUM1, queue1WaitTime, queue2WaitTime;
	bool stop;
	string fileName;
private:
	//this method will calculte total process
	int findTotalPids(const char* fName)
	{
		ifstream ifs(fName);
		int count = 0;
		int num = 0;
		while ((ifs >> num))
		{
			count++;
		}
		ifs.close();
		return count / 3;
	}
	//this method will initalize all the data from file 
	void initalizeData(const char * fName)
	{
		stop = false;
		counter = 0;
		if (totalPids != 0)
		{
			this->~MLFQ();
		}
		totalPids = findTotalPids(fName);
		if (totalPids > 0)
		{
			pid = new int[totalPids];
			arrival_time = new int[totalPids];
			burst_time = new int[totalPids];
			waitingTime = new int[totalPids];
			responseTime = new int[totalPids];
			turnAroundTime = new int[totalPids];
			ifstream ifs(fileName);
			for (int i = 0; i < totalPids; i++)
			{
				ifs >> pid[i];
				ifs >> arrival_time[i];
				ifs >> burst_time[i];
				waitingTime[i] = 0;
				responseTime[i] = 0;
				turnAroundTime[i] = 0;
			}
			ifs.close();
		}
		else
		{
			pid = NULL;
			arrival_time = NULL;
			burst_time = NULL;
			waitingTime = NULL;
			responseTime = NULL;
			turnAroundTime = NULL;
		}
	}
	// this method will display all queues, arrivat time, burst time, 
	// waiting time, response time, and grant chartt
	void display()
	{
		system("cls");		//clear screen
		//display table of all processes
		cout << "Process ID" << setw(15) << "Arrival Time" << setw(15) << "Burst Time" << setw(15) << "Waiting Time" << setw(20) << "Turn-Around Time" << setw(15) << "Response Time";
		for (int i = 0; i < totalPids; i++)
		{
			cout << '\n' << setw(5) << pid[i] << setw(15) << arrival_time[i] << setw(15) << burst_time[i];
			if (stop == true)
			{
				cout << setw(15) << waitingTime[i] << setw(18) << turnAroundTime[i] << setw(15) << responseTime[i];
			}
			else
			{
				cout << setw(15) << "-" << setw(18) << "-" << setw(15) << "-";
			}
		}
		//display counter, current queue, and current operation
		cout << '\n';
		cout << "\n " << right << setw(50) << "Quantum0 : " << QUANTUM0;
		cout << "\n " << right << setw(50) << "Quantum1 : " << QUANTUM1;
		cout << "\n " << right << setw(50) << "queue1WaitTime : " << queue1WaitTime;
		cout << "\n " << right << setw(50) << "queue2WaitTime : " << queue2WaitTime;
		cout << "\n " << right << setw(50) << "Counter : " << counter;
		cout << "\n " << right << setw(50) << "Current Queue : " << currentQueue;
		if (stop == true)
		{
			cout << " (Last Used)";
		}
		if (currentQueue == 0)
		{
			cout << "\n " << right << setw(50) << "Current Algorithm : " << "RR";
			if (stop == true)
			{
				cout << " (Last Used)";
			}
			cout << "\n " << right << setw(50) << "Current Quantum : " << QUANTUM0;
			if (stop == true)
			{
				cout << " (Last Used)";
			}
		}
		else if (currentQueue == 1)
		{
			cout << "\n " << right << setw(50) << "Current Algorithm : " << "RR";
			if (stop == true)
			{
				cout << " (Last Used)";
			}
			cout << "\n " << right << setw(50) << "Current Quantum : " << QUANTUM1;
			if (stop == true)
			{
				cout << " (Last Used)";
			}
		}
		else
		{
			cout << "\n " << right << setw(50) << "Current Algorithm : " << "SRTF";
			if (stop == true)
			{
				cout << " (Last Used)";
			}
		}
		cout << '\n';
		//display all queues states
		cout << "\n     Queue (0) : ";
		if (queue0.empty() == true)
		{
			cout << " Empty";
		}
		queue<Process* > temp;
		while (queue0.empty() == false)
		{
			Process* p = queue0.front();
			cout << " |P" << p->processId<<"  "; // queue 0: p1 | P2 trong hang doi
			// temp: hang doi ao, lay proc dau tu q0, r q0 pop tien trinh day ra, lan luot
			// temp se thanh q0
			temp.push(queue0.front());// xoa tien trinh day di, coi nhu dang trong cpu
			queue0.pop();
			
		}
		//cout << "\n Time quantum: " << QUANTUM0;
		cout << "\n Remaining Time: ";
		
		if (temp.empty() == true) {
			cout << " -----";
		}
		else {
			while (temp.empty() == false)
			{
				queue0.push(temp.front());
				Process* p = temp.front();
				if (p->burstTime >= 100 && p->burstTime <= 999) {
					cout << " |" << p->burstTime << " ";
				}
				else if (p->burstTime < 10) {
					cout << " |" << p->burstTime << "   ";
				}
				else if (p->burstTime >= 1000) {
					cout << " |" << p->burstTime;
				}
				else {
					cout << " |" << p->burstTime << "  ";
				}
				temp.pop();
			}
		}
		
		
		cout << '\n';
		cout << "\n     Queue (1) : ";
		if (queue1.empty() == true)
		{
			cout << " Empty";
		}
		while (queue1.empty() == false)
		{
			Process* p = queue1.front();
			cout << " |P" << p->processId<<"  ";
			temp.push(queue1.front());
			queue1.pop();
			
		}
		//cout << "\n Time quantum: " << QUANTUM1;
		cout << "\n Remaining Time: ";
		
		if (temp.empty() == true) {
			cout << " -----";
		}
		else {
			while (temp.empty() == false)
			{
				queue1.push(temp.front());
				Process* p = temp.front();
				if (p->burstTime >= 100 && p->burstTime <= 999) {
					cout << " |" << p->burstTime << " ";
				}
				else if (p->burstTime < 10) {
					cout << " |" << p->burstTime << "   ";
				}
				else if (p->burstTime >= 1000) {
					cout << " |" << p->burstTime;
				}
				else {
					cout << " |" << p->burstTime << "  ";
				}
				temp.pop();

			}
		}

		cout << "\n    TimeWaiting: ";
		if (queue1.empty() == true) {
			cout << " -----";
		}
		else {
			while (queue1.empty() == false)
			{
				Process* p = queue1.front();
				if (p->TimeWaiting >= 100) {
					cout << " |" << p->TimeWaiting<< " ";
				}
				else if ((p->TimeWaiting < 99)&&(p->TimeWaiting>=10)) {
					cout << " |" << p->TimeWaiting << "  ";
				}
				else {
					cout << " |" << p->TimeWaiting << "   ";
				}
				temp.push(queue1.front());
				queue1.pop();

			}
		}
		// trả lại q1
		while (temp.empty() == false)
		{
			Process* p = temp.front();
			queue1.push(temp.front());
			temp.pop();
		}
		

		cout << '\n';
		cout << "\n     Queue (2) : ";
		if (queue2.empty() == true)
		{
			cout << " Empty";
		}
		while (queue2.empty() == false)
		{
			Process* p = queue2.front();
			cout << " |P" << p->processId<<"  ";
			temp.push(queue2.front());
			queue2.pop();
			
		}
		cout << "\n Remaining Time: ";
		if (temp.empty() == true) {
			cout << " -----";
		}
		else {
			while (temp.empty() == false)
			{
				queue2.push(temp.front());
				Process* p = temp.front();
				if (p->burstTime >= 100 && p->burstTime <= 999) {
					cout << " |" << p->burstTime << " ";
				}
				else if (p->burstTime < 10) {
					cout << " |" << p->burstTime << "   ";
				}
				else if (p->burstTime >= 1000) {
					cout << " |" << p->burstTime;
				}
				else {
					cout << " |" << p->burstTime << "  ";
				}
				temp.pop();
			}
		}

		cout << "\n    TimeWaiting: ";
		if (queue2.empty() == true) {
			cout << " -----";
		}
		else {
			while (queue2.empty() == false)
			{
				Process* p = queue2.front();
				if (p->TimeWaiting >= 100) {
					cout << " |" << p->TimeWaiting << " ";
				}
				else if ((p->TimeWaiting < 99) && (p->TimeWaiting >= 10)) {
					cout << " |" << p->TimeWaiting << "  ";
				}
				else {
					cout << " |" << p->TimeWaiting << "   ";
				}
				temp.push(queue2.front());
				queue2.pop();

			}
		}
		//trả lại q2
		while (temp.empty() == false)
		{
			Process* p = temp.front();
			queue2.push(temp.front());
			temp.pop();
		}
		



		cout << "\n";
		//display gantt chart
		cout << "\n    Gannt Chart: \n";
		
		int n = granttChart.size();
		int step = 15;						//display max 15 process in one line
		int size = ceil(n / (double)step);	// total no of lines of grantt chart
		int i = 0;
		for (int j = 0; j < size; j++)
		{
			//display process id
			cout << " ";
			for (i = (j)*step ;i < (j+1) * step && i < n; i++)
			{
				int  s = granttChart[i].start;
				if ( s >= 10 && s< 100)
				{
					cout << " ";
				}
				else if (s >= 100 && s < 1000)
				{
					cout << "  ";
				}
				else if (s >= 1000)
				{
					cout << "   ";
				}
				if (granttChart[i].pid == -1)
				{
					cout << "  " << "i" << " ";
				}
				else
				{
					cout << " P" << granttChart[i].pid << " ";
				}
			}
			//display line with use of - and | 
			cout << '\n';
			for (i = (j)*step; i < (j + 1) * step && i < n; i++)
			{
				if (granttChart[i].start >= 10)
				{
					int  s = granttChart[i].start;
					if (s >= 10 && s < 100)
					{
						cout << "-|---";
					}
					else if (s >= 100 && s < 1000)
					{
						cout << "--|---";
					}
					else if (s >= 1000)
					{
						cout << "--|---";
					}
					
				}
				else if (granttChart[i].pid == -1)
				{
					cout << "-|-" << "-";
				}
				else
				{
					cout << "-|-" << "-";
				}
			}
			cout << "-|-";
			cout << '\n';
			cout << " ";
			
			// display the values of starting and position of process in grantt chart
			for (i = (j)*step; i < (j + 1) * step && i < n; i++)
			{
				cout << granttChart[i].start << "   ";
			}
			cout << granttChart[i-1].end << "   ";
			if (j != size)
			{
				cout << "\n\n";
			}
		}
		cout << '\n';
		//Sleep(4000);		//for delay of 0.5 sec
		_getch();
	}
	//check all queues are empty or not
	bool isEmptyAllQueue()
	{
		if (queue0.empty()==true && queue1.empty()==true && queue2.empty()==true && tempQueue.empty()==true)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	//sort the process acording to arrival time
	void sortProcesses(Process** p)
	{
		int n = totalPids;
		for (int i = 0; i < n; i++)
		{
			int min = i;
			for (int j = i + 1;j<n; j++)
			{
				if (p[j]->arrivalTime < p[min]->arrivalTime)
				{
					min = j;
				}
			}
			Process* temp = p[min];
			p[min] = p[i];
			p[i] = temp;
		}
	}
	//calculate the waiting time of all process
	void calcWaitingTime()
	{
		
		for (int i = 0; i < totalPids; i++)
		{
			int finishTime = 0;
			int n = granttChart.size();
			for (int j = 0; j < n; j++)
			{
				if (granttChart[j].pid == pid[i])
				{
					finishTime = granttChart[j].end;
				}
			}
			waitingTime[i] = finishTime - arrival_time[i] - burst_time[i] ;
		}
	}
	//calculte the turnaround time of all processes
	void calcTurnAroundTime()
	{
		for (int i = 0; i < totalPids; i++)
		{
			int finishTime = 0;
			int n = granttChart.size();
			for (int j = 0; j < n; j++)
			{
				if (granttChart[j].pid == pid[i])
				{
					finishTime = granttChart[j].end;
				}
			}
			turnAroundTime[i] = finishTime - arrival_time[i];
		}
	}
	//calculate the response time of all processes
	void calcResponseTime()
	{
		for (int i = 0; i < totalPids; i++)
		{
			int n = granttChart.size();
			int j = 0;
			while (j < n && granttChart[j].pid!=pid[i])
			{
				j++;
			}
			responseTime[i] = granttChart[j].start - arrival_time[i];
		}
	}
	// calculate average waiting time
	void calcAvgWaitingTime()
	{
		avgWaitingTime = 0;
		for (int i = 0; i < totalPids; i++)
		{
			avgWaitingTime = avgWaitingTime+waitingTime[i];
		}
		avgWaitingTime = avgWaitingTime / totalPids;
	}
	// calculate average turn around time
	void calcAvgTurnAroundTime()
	{
		avgTurnAroundTime = 0;
		for (int i = 0; i < totalPids; i++)
		{
			avgTurnAroundTime = avgTurnAroundTime + turnAroundTime[i];
		}
		avgTurnAroundTime = avgTurnAroundTime / totalPids;
	}
	// calculate average response time
	void calcAvgResponseTime()
	{
		avgResponseTime = 0;
		for (int i = 0; i < totalPids; i++)
		{
			avgResponseTime = avgResponseTime + responseTime[i];
		}
		avgResponseTime = avgResponseTime / totalPids;
	}
public:
	//constructor of MLFQ
	MLFQ()
	{
		stop = false;
		totalPids = 0;
		pid = NULL;
		arrival_time = NULL;
		burst_time = NULL;
		waitingTime = NULL;
		responseTime = NULL;
		turnAroundTime = NULL;
		avgResponseTime = 0;
		avgWaitingTime = 0;
		avgTurnAroundTime = 0;
		counter = 0;
		currentQueue = 0;
	}
	//Destructor of MLFQ
	~MLFQ()
	{
		if (totalPids != 0)
		{
			delete[] pid;
			delete[] arrival_time;
			delete[] burst_time;
			delete[] waitingTime;
			delete[] responseTime;
			pid = NULL;
			arrival_time = NULL;
			burst_time = NULL;
			waitingTime = NULL;
			responseTime = NULL;
			totalPids = 0;
		}
		else
		{
			totalPids = 0;
			pid = NULL;
			arrival_time = NULL;
			burst_time = NULL;
			waitingTime = NULL;
			responseTime = NULL;
		}
	}
	//set file name for input data
	void setFileName(string fName)
	{
		fileName = fName;
	}
	//change quantum for round robbin for queue0 or queue1 if necessary 
	//void setQuantum(double quant0, double quant1)
	//{
	//	QUANTUM0 = quant0;
	//	QUANTUM1 = quant1;
	//}
	// 
	 void setQuantum(int a, int b,int c,int d)
		{
		 QUANTUM0 = a;
		 QUANTUM1 = b;
		 queue1WaitTime = c;
		 queue2WaitTime = d;
		}
	//start which will handle all mlfq algortihm , 
	// updates queus , and display grantt chart
	// at the end display avg waiting time, avg response time, and avg turn arround time
	void start()
	{	
		initalizeData(fileName.data());
		if (totalPids == 0)
		{
			cout << "\nNo Process";
			exit(0);
		}
		else {
			cout << "\nFile input succeed";
		}
		int a, b, c, d;
		cout << "\nQuantum0 ="; cin >> a;
		cout << "\nQuantum1 ="; cin >> b;
		cout << "\nqueue1WaitTime ="; cin >> c;
		cout << "\nqueue2WaitTime2 ="; cin >> d;
		setQuantum(a, b, c, d);
		currentQueue = 0;
		Process ** proc = new Process * [totalPids];
		for (int i = 0; i < totalPids; i++)
		{
			proc[i] = new Process(pid[i],arrival_time[i],burst_time[i],0);
		}
		sortProcesses(proc);
		for (int i = 0; i < totalPids; i++)
		{
			tempQueue.push(proc[i]);
		}
		bool stopMLFQ = isEmptyAllQueue();	
		//int bTime = 0;
		int idleTime = 0;
		cout << '\n';
		 waitTimer1 = queue1WaitTime;
		 waitTimer2 = queue2WaitTime;
		while (stopMLFQ!=true)
		{
			while (tempQueue.empty()==false && tempQueue.front()->arrivalTime<=counter)
			{
				queue0.push(tempQueue.front());
				tempQueue.pop();
				currentQueue = 0;
			}
			display();
			// run round robin on Queue0 with quantum QUANT0
			if (queue0.empty() == false)
			{	
				Process * p = queue0.front();
				if (counter >= p->arrivalTime)
				{	
					if (p->burstTime > QUANTUM0)
					{
						while (queue1.empty() == false) {
							//Process* p = queue1.front();
							queue1.front()->TimeWaiting = queue1.front()->TimeWaiting + QUANTUM0;
							tempQueue1.push(queue1.front());
							queue1.pop();
						}
						while (tempQueue1.empty() == false) {
							//Process* p = tempQueue1.front();
							//tempQueue1.front()->TimeWaiting = tempQueue1.front()->TimeWaiting + QUANTUM0;
							queue1.push(tempQueue1.front());
							tempQueue1.pop();
						}
						queue0.front()->burstTime = queue0.front()->burstTime - QUANTUM0;
						granttChart.push_back(Node(queue0.front()->processId, counter, counter+QUANTUM0));
						queue0.front()->TimeWaiting = 0;
						queue1.push(queue0.front());
						//queue1-> Q1time=?;
						queue0.pop();
						counter = counter + QUANTUM0;
						//TW
					
						while (queue2.empty() == false) {
							//Process* p = queue2.front();
							queue2.front()->TimeWaiting = queue2.front()->TimeWaiting + QUANTUM0;
							tempQueue2.push(queue2.front());
							queue2.pop();
						}
						//Tra q1,q2
						/*if (queue1.empty() == false) {
							queue1.back()->TimeWaiting = 0;
						}*/
						while (tempQueue2.empty() == false) {
							//Process* p = tempQueue1.front();
							//tempQueue2.front()->TimeWaiting = tempQueue2.front()->TimeWaiting + QUANTUM0;
							queue2.push(tempQueue2.front());
							tempQueue2.pop();
						}
					}
					else if (p->burstTime == QUANTUM0)
					{
						granttChart.push_back(Node(queue0.front()->processId, counter, counter + QUANTUM0));
						counter = counter + QUANTUM0;
						queue0.pop();

						//WT
						while (queue1.empty() == false) {
							//Process* p = queue1.front();
							queue1.front()->TimeWaiting = queue1.front()->TimeWaiting + QUANTUM0;
							tempQueue1.push(queue1.front());
							queue1.pop();
						}
						while (queue2.empty() == false) {
							//Process* p = queue2.front();
							queue2.front()->TimeWaiting = queue2.front()->TimeWaiting + QUANTUM0;
							tempQueue2.push(queue2.front());
							queue2.pop();
						}
						//Tra q1,q2
						while (tempQueue1.empty() == false) {
							//Process* p = tempQueue1.front();
							//tempQueue1.front()->TimeWaiting = tempQueue1.front()->TimeWaiting + QUANTUM0;
							queue1.push(tempQueue1.front());
							tempQueue1.pop();
						}
						while (tempQueue2.empty() == false) {
							//Process* p = tempQueue1.front();
							//tempQueue2.front()->TimeWaiting = tempQueue2.front()->TimeWaiting + QUANTUM0;
							queue2.push(tempQueue2.front());
							tempQueue2.pop();
						}
					}
					else
					{
						granttChart.push_back(Node(queue0.front()->processId, counter, counter + queue0.front()->burstTime));
						counter = counter + queue0.front()->burstTime;
						int c = queue0.front()->burstTime;
						queue0.pop();

						while (queue1.empty() == false) {
							//Process* p = queue1.front();
							queue1.front()->TimeWaiting = queue1.front()->TimeWaiting + c;
							tempQueue1.push(queue1.front());
							queue1.pop();
						}
						while (queue2.empty() == false) {
							//Process* p = queue2.front();
							queue2.front()->TimeWaiting = queue2.front()->TimeWaiting + c;
							tempQueue2.push(queue2.front());
							queue2.pop();
						}
						//Tra q1,q2
						while (tempQueue1.empty() == false) {
							//Process* p = tempQueue1.front();
							//tempQueue1.front()->TimeWaiting = tempQueue1.front()->TimeWaiting + QUANTUM0;
							queue1.push(tempQueue1.front());
							tempQueue1.pop();
						}
						while (tempQueue2.empty() == false) {
							//Process* p = tempQueue1.front();
							//tempQueue2.front()->TimeWaiting = tempQueue2.front()->TimeWaiting + QUANTUM0;
							queue2.push(tempQueue2.front());
							tempQueue2.pop();
						}

					}

					while (queue1.empty() == false) {
						//Process* p = queue1.front();
						//queue1.front()->TimeWaiting = queue1.front()->TimeWaiting + e;
						if (queue1.front()->TimeWaiting > queue1WaitTime) {
							queue1.front()->TimeWaiting = 0;
							queue0.push(queue1.front());
							queue1.pop();
						}
						else {
							tempQueue1.push(queue1.front());
							queue1.pop();
						}
					}
					while (tempQueue1.empty() == false) {
						//Process* p = tempQueue1.front();
						//tempQueue1.front()->TimeWaiting = tempQueue1.front()->TimeWaiting + QUANTUM0;
						queue1.push(tempQueue1.front());
						tempQueue1.pop();
					}
					while (queue2.empty() == false) {
						//Process* p = queue1.front();
						//queue1.front()->TimeWaiting = queue1.front()->TimeWaiting + e;
						if (queue2.front()->TimeWaiting > queue2WaitTime) {
							queue2.front()->TimeWaiting = 0;
							queue1.push(queue2.front());
							queue2.pop();
						}
						else {
							tempQueue2.push(queue2.front());
							queue2.pop();
						}
					}
					while (tempQueue2.empty() == false) {
						//Process* p = tempQueue1.front();
						//tempQueue1.front()->TimeWaiting = tempQueue1.front()->TimeWaiting + QUANTUM0;
						queue2.push(tempQueue2.front());
						tempQueue2.pop();
					}
				}
				else	//idle time
				{
					granttChart.push_back(Node(-1, counter, counter + 1));
					counter++;
				}
				if (queue0.empty() == true)
				{
					if (queue1.empty() == false)
					{
						waitTimer1 = queue1WaitTime;
						currentQueue = 1;
					}
					else if (queue2.empty() == false)
					{
						waitTimer2 = queue2WaitTime;
						currentQueue = 2;
					}
				}

			}
			// run round robin on Queue1 with quantum QUANT1
			else if (queue1.empty() == false)
			{
				Process* p = queue1.front();
				if (counter >= p->arrivalTime)
				{
					if (p->burstTime > QUANTUM1)
					{
						while (queue2.empty() == false) {
							//Process* p = queue2.front();
							queue2.front()->TimeWaiting = queue2.front()->TimeWaiting + QUANTUM1;
							tempQueue2.push(queue2.front());
							queue2.pop();
						}
						//Tra q1,q2
						while (tempQueue2.empty() == false) {
							//Process* p = tempQueue1.front();
							//tempQueue2.front()->TimeWaiting = tempQueue2.front()->TimeWaiting + QUANTUM0;
							queue2.push(tempQueue2.front());
							tempQueue2.pop();
						}

						queue1.front()->burstTime = queue1.front()->burstTime - QUANTUM1;
						granttChart.push_back(Node(queue1.front()->processId, counter, counter + QUANTUM1));
						queue1.front()->TimeWaiting = 0;
						queue2.push(queue1.front());
						queue1.pop();
						waitTimer1 = waitTimer1 - QUANTUM1;
						counter = counter + QUANTUM1;

						//i=1,n-1
						//if(queue.emty==false){

						while (queue1.empty() == false) {
							//Process* p = queue1.front();
							//if (queue1.front()->TimeWaiting == queue1.back()->TimeWaiting) break;
							queue1.front()->TimeWaiting = queue1.front()->TimeWaiting + QUANTUM1;
							tempQueue1.push(queue1.front());
							queue1.pop();
						}
						while (tempQueue1.empty() == false) {
							//Process* p = tempQueue1.front();
							//tempQueue1.front()->TimeWaiting = tempQueue1.front()->TimeWaiting + QUANTUM0;
							queue1.push(tempQueue1.front());
							tempQueue1.pop();
						}


					 
					}
					else if (p->burstTime == QUANTUM1)
					{	
						granttChart.push_back(Node(queue1.front()->processId, counter, counter + QUANTUM1));
						counter = counter + QUANTUM1;
						waitTimer1 = waitTimer1 - QUANTUM1;
						queue1.pop();

						//TW
						while (queue1.empty() == false) {
							//Process* p = queue1.front();
							queue1.front()->TimeWaiting = queue1.front()->TimeWaiting + QUANTUM1;
							tempQueue1.push(queue1.front());
							queue1.pop();
						}
						while (queue2.empty() == false) {
							//Process* p = queue2.front();
							queue2.front()->TimeWaiting = queue2.front()->TimeWaiting + QUANTUM1;
							tempQueue2.push(queue2.front());
							queue2.pop();
						}
						//Tra q1,q2
						while (tempQueue1.empty() == false) {
							//Process* p = tempQueue1.front();
							//tempQueue1.front()->TimeWaiting = tempQueue1.front()->TimeWaiting + QUANTUM0;
							queue1.push(tempQueue1.front());
							tempQueue1.pop();
						}
						while (tempQueue2.empty() == false) {
							//Process* p = tempQueue1.front();
							//tempQueue2.front()->TimeWaiting = tempQueue2.front()->TimeWaiting + QUANTUM0;
							queue2.push(tempQueue2.front());
							tempQueue2.pop();
						}
					}
					else
					{
						granttChart.push_back(Node(queue1.front()->processId, counter, counter + queue1.front()->burstTime));
						counter = counter + queue1.front()->burstTime;
						waitTimer1 = waitTimer1 - queue1.front()->burstTime;
						int e = queue1.front()->burstTime;
						queue1.pop();

						//wt
						while (queue1.empty() == false) {
							//Process* p = queue1.front();
							queue1.front()->TimeWaiting = queue1.front()->TimeWaiting + e;
							tempQueue1.push(queue1.front());
							queue1.pop();
						}
						while (queue2.empty() == false) {
							//Process* p = queue2.front();
							queue2.front()->TimeWaiting = queue2.front()->TimeWaiting + e;
							tempQueue2.push(queue2.front());
							queue2.pop();
						}
						//Tra q1,q2
						while (tempQueue1.empty() == false) {
							//Process* p = tempQueue1.front();
							//tempQueue1.front()->TimeWaiting = tempQueue1.front()->TimeWaiting + QUANTUM0;
							queue1.push(tempQueue1.front());
							tempQueue1.pop();
						}
						while (tempQueue2.empty() == false) {
							//Process* p = tempQueue1.front();
							//tempQueue2.front()->TimeWaiting = tempQueue2.front()->TimeWaiting + QUANTUM0;
							queue2.push(tempQueue2.front());
							tempQueue2.pop();
						}
					} 
					// sau khi thuc hien proc xong, thi het tg cho WT <1

					/*if (waitTimer1 < 1)
					{	
						// neu q1 ma con proc thi day len q0, ko con thi ko lam gi
						if (queue1.empty() == false)
						{	
							queue1.front()->TimeWaiting = 0;
							queue0.push(queue1.front());
							queue1.pop();
							currentQueue = 0;
							waitTimer1 = queue1WaitTime;
						}
						// ??
						if (queue0.empty())
						{
							currentQueue = 2;
							waitTimer2 = queue2WaitTime;
							waitTimer1 = queue1WaitTime;
						}
					} // con chua het WT, Q1 con proc thi tiep tuc
					else if (queue1.empty() == true)
					{
						if (queue0.empty() == true)
						{
							currentQueue = 2;
							waitTimer2 = queue2WaitTime;
							waitTimer1 = queue1WaitTime;
						}
						else
						{
							currentQueue = 1;
							waitTimer1 = 0;
						}
					}*/
					// nang cấp các tiến trình
					while (queue1.empty() == false) {
						//Process* p = queue1.front();
						//queue1.front()->TimeWaiting = queue1.front()->TimeWaiting + e;
						if (queue1.front()->TimeWaiting > queue1WaitTime) {
							queue1.front()->TimeWaiting = 0;
							queue0.push(queue1.front());
							queue1.pop();
						}
						else{
							tempQueue1.push(queue1.front());
							queue1.pop();
						}
					}
					while (tempQueue1.empty() == false) {
						//Process* p = tempQueue1.front();
						//tempQueue1.front()->TimeWaiting = tempQueue1.front()->TimeWaiting + QUANTUM0;
						queue1.push(tempQueue1.front());
						tempQueue1.pop();
					}
					while (queue2.empty() == false) {
						if (queue2.front()->TimeWaiting > queue2WaitTime) {
							queue2.front()->TimeWaiting = 0;
							queue1.push(queue2.front());
							queue2.pop();
						}
						else {
							tempQueue2.push(queue2.front());
							queue2.pop();
						}
					}
					while (tempQueue2.empty() == false) {
						//Process* p = tempQueue1.front();
						//tempQueue1.front()->TimeWaiting = tempQueue1.front()->TimeWaiting + QUANTUM0;
						queue2.push(tempQueue2.front());
						tempQueue2.pop();
					}
				}
				else
				{
					granttChart.push_back(Node(-1, counter, counter + 1));
					counter++;
				}
				if (queue0.empty() == false) {
					currentQueue = 0;
				}
				else if (queue0.empty() == true && queue1.empty() == true) {
					currentQueue = 2;
				}
				else {
					currentQueue = 1;
				}
			}
			// run SRTF on Queue2
			else if (queue2.empty() == false)
			{
				vector<Process*> temp;
				while (queue2.empty()==false)
				{
					temp.push_back(queue2.front());
					queue2.pop();// thuc hien tren giao dien P1|P2|P3
				}
				int n = temp.size();
				int min = 0;
				int dem;
				// sap xep theo burst time
				for (int i = 1; i < n; i++)
				{
					if (temp[i]->burstTime < temp[min]->burstTime)
					{
						min = i;
					}
				}
				
				dem = temp[min]->burstTime;
				temp[min]->burstTime = 0;
				waitTimer2 = waitTimer2 - dem;
				granttChart.push_back(Node(temp[min]->processId, counter, counter + dem));
				counter = counter + dem;

				for (int i = 0; i < n; i++)
				{	
					if (i == min) continue;
					temp[i]->TimeWaiting = temp[i]->TimeWaiting + dem;
				}

				while (queue1.empty() == false) {
					//Process* p = queue1.front();
					queue1.front()->TimeWaiting = queue1.front()->TimeWaiting + dem;
					tempQueue1.push(queue1.front());
					queue1.pop();
				}
				//Tra q1,q2
				while (tempQueue1.empty() == false) {
					//Process* p = tempQueue1.front();
					//tempQueue1.front()->TimeWaiting = tempQueue1.front()->TimeWaiting + QUANTUM0;
					queue1.push(tempQueue1.front());
					tempQueue1.pop();
				}

							
				for (int i = 0; i < n; i++)
				{
					if (temp[i]->burstTime > 0)
					{
						queue2.push(temp[i]);
					}
				}
				temp.clear();
				

				// nang cấp các tiến trình
				while (queue1.empty() == false) {
					//Process* p = queue1.front();
					//queue1.front()->TimeWaiting = queue1.front()->TimeWaiting + e;
					if (queue1.front()->TimeWaiting > queue1WaitTime) {
						queue1.front()->TimeWaiting = 0;
						queue0.push(queue1.front());
						queue1.pop();
					}
					else {
						tempQueue1.push(queue1.front());
						queue1.pop();
					}
				}
				while (tempQueue1.empty() == false) {
					//Process* p = tempQueue1.front();
					//tempQueue1.front()->TimeWaiting = tempQueue1.front()->TimeWaiting + QUANTUM0;
					queue1.push(tempQueue1.front());
					tempQueue1.pop();
				}
				while (queue2.empty() == false) {
					//Process* p = queue1.front();
					//queue1.front()->TimeWaiting = queue1.front()->TimeWaiting + e;
					if (queue2.front()->TimeWaiting > queue2WaitTime) {
						queue2.front()->TimeWaiting = 0;
						queue1.push(queue2.front());
						queue2.pop();
					}
					else {
						tempQueue2.push(queue2.front());
						queue2.pop();
					}
				}
				while (tempQueue2.empty() == false) {
					//Process* p = tempQueue1.front();
					//tempQueue1.front()->TimeWaiting = tempQueue1.front()->TimeWaiting + QUANTUM0;
					queue2.push(tempQueue2.front());
					tempQueue2.pop();
				}

				if (queue0.empty() == false) {
					currentQueue = 0;
				}
				else if (queue0.empty() == true && queue1.empty() == true) {
					currentQueue = 2;
				}
				else {
					currentQueue = 1;
				}

			}
			else
			{
				granttChart.push_back(Node(-1, counter, counter + 1));
				counter++;
			}
			stopMLFQ = isEmptyAllQueue();
		}
		stop = true;
		calcWaitingTime();
		calcAvgWaitingTime();
		calcTurnAroundTime();
		calcResponseTime();
		calcAvgTurnAroundTime();
		calcAvgResponseTime();
		display();
		cout << "\n"<<setw(50)<<"Avg Waiting Time :" << setw(10) << avgWaitingTime;
		cout << "\n" << setw(50) << "Avg Turn Around TIme :" << setw(10) << avgTurnAroundTime;
		cout << "\n" << setw(50) << "Avg Response Time :"<<setw(10)<< avgResponseTime<<'\n';
	}
	
};

#endif
