#include "blockingqueue.h"
#include <iostream>
#include <thread>
#include <chrono>

std::mutex iomtx;

void dequeuer(mylib::BlockingQueue<int> *q, int id) {
	for (int i=0; i<3; ++i) {
		int msg=q->pop();
		std::lock_guard<std::mutex> lock(iomtx);
		std::cout<<" DeQ "<<id<<" : - "<<msg<<std::endl;
	}
}

void enqueuer(mylib::BlockingQueue<int> *q, int id) {
	for (int i=(id+1)*100; i<(id+1)*100+3; ++i) {
		q->push(i);
		std::lock_guard<std::mutex> lock(iomtx);
		std::cout<<" EnQ "<<id<<" : + "<<i<<std::endl;
		//std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}

void main() {
	mylib::BlockingQueue<int> bq;
	std::vector<std::thread> enq_thrds(5), deq_thrds(5);
	for (int i=0; i<deq_thrds.size(); ++i) {
		deq_thrds[i]=std::thread(dequeuer, &bq, i);
	}
	for (int i=0; i<enq_thrds.size(); ++i) {
		enq_thrds[i]=std::thread(enqueuer, &bq, i);
	}
	for (size_t i=0; i<enq_thrds.size(); ++i) enq_thrds[i].join();
	for (size_t i=0; i<deq_thrds.size(); ++i) deq_thrds[i].join();
	system("pause");
}