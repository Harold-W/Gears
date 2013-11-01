//
// Blockingqueue.h - A generic blocking queue object built of std lib
// 
// by Hao
// 11/1/2013
//
// No rights reserved, use if you like.
//
#pragma once
#ifndef BLOCK_QUEUE_MYLIB
#include <queue>
#include <mutex>
#include <condition_variable>

namespace mylib {

template<typename T>
class BlockingQueue {
public:
	void push(T value);
	T pop();
	T front();
	size_t size();
private:
	std::mutex _mtx;
	std::queue<T> _q;	
	std::condition_variable _cv;
};

template<typename T>
void BlockingQueue<T>::push(T value) {
	std::unique_lock<std::mutex> push_lock(_mtx);
	_q.push(value);
	_cv.notify_all();
}

template<typename T>
T BlockingQueue<T>::pop() {
	std::unique_lock<std::mutex> lock(_mtx);
	while (_q.empty()) _cv.wait(lock);
	T front_value= _q.front();
	_q.pop();
	return front_value;
}

template<typename T>
T BlockingQueue<T>::front() {
	std::unique_lock<std::mutex> lock(_mtx);
	while (_q.empty()) _cv.wait(lock);
	return _q.front();
}

template<typename T>
size_t BlockingQueue<T>::size() {
	std::unique_lock<std::mutex> size_lock(_mtx);
	return _q.size();
}
}; // mylib namespace

#endif