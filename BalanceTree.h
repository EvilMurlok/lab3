#pragma once
#ifndef BALANCETREE_H_
#define BALANCETREE_H_

#include <iostream>
#include <queue>
#include "MyExceptions.h"

//создал АВЛ-дерево (2 очка), сделал все базовые операции (5 очков), организовал извлечение подерева по заданному элементу(2 очка), создал метод проерки на вхождение поддерева (3 очка),
//также организованы map(прямой обход), where(обход в ширину), reduce(обход в ширину) (3 очка) + курс на курсере (уже внес таблицу к Владимиру Владимировичу)

template <typename T>
class BalanceTree
{
private:
	T data;
	int height;
	BalanceTree<T>* leftChild;
	BalanceTree<T>* rightChild;

	void deleteTree();
	void fixHeight();
	BalanceTree<T>* removeMin();
	BalanceTree<T>* balance();
	BalanceTree<T>* findMin()  { return leftChild ? leftChild->findMin() : this; }
	BalanceTree<T>* rotateRight();
	BalanceTree<T>* rotateLeft();
	int Height() const { return this ? height : 0; }
	int bfactor() const { return rightChild->Height() - leftChild->Height(); }
public:
	BalanceTree() : data{  }, height{ 1 }, rightChild{ nullptr }, leftChild{ nullptr } {};
	BalanceTree(T value) : data{ value }, height{ 1 }, rightChild{ nullptr }, leftChild{ nullptr } {};
	~BalanceTree() { deleteTree(); };
	BalanceTree<T>* copyTree();
	BalanceTree<T>* insert(T itemToInsert);
	BalanceTree<T>* deleteNode(T itemToDelete);
	bool SearchPreOrder(T valueToFind) const;
	bool SearchInOrder(T valueToFind) const;
	bool SearchPostOrder(T valueToFind) const;
	size_t len() const;
	void print() const;
	bool checkTree(const BalanceTree<T>* treeForCheck) const;
	T getMax() const { return rightChild ? rightChild->getMax() : this->data; }
	T getMin() const { return leftChild ? leftChild->getMin() : this->data; }
	BalanceTree<T>* getSubTreeByValue(T valueToGetSubTree);
	template<typename fcn>
	void map(fcn func) {
		if (leftChild) {
			leftChild->map(func);
		}
		func(data);
		if (rightChild) {
			rightChild->map(func);
		}
	}
	template <typename fcn>
	T reduce(T zeroValue, fcn func) {
		if (!this) {
			throw TreeExceptions("tree is empty!");
		}
		std::queue< BalanceTree<T>*> queueForReduce;
		queueForReduce.push(this);
		T temp = func(zeroValue, data);
		bool flag = false;
		while (queueForReduce.size()) {
			BalanceTree<T>* copy = queueForReduce.front();
			queueForReduce.pop();
			if (!flag) {
				flag = true;
			}
			else {
				temp = func(temp, copy->data);
			}
			if (copy->leftChild) {
				queueForReduce.push(copy->leftChild);
			}
			if (copy->rightChild) {
				queueForReduce.push(copy->rightChild);
			}
		}
		return temp;
	}
	template<typename fcn>
	BalanceTree<T>* where(fcn func) {
		if (!this) {
			throw TreeExceptions("tree is empty!");
		}
		BalanceTree<T>* temp = nullptr;
		std::queue<BalanceTree<T>*> queueForWhere;
		queueForWhere.push(this);
		while (queueForWhere.size()){
			BalanceTree<T>* copy = queueForWhere.front();
			queueForWhere.pop();
			if (func(copy->data)) {
				temp = temp->insert(copy->data);
			}
			if (copy->leftChild) {
				queueForWhere.push(copy->leftChild);
			}
			if (copy->rightChild) {
				queueForWhere.push(copy->rightChild);
			}
		}
		return temp;
	}
};

template<typename T>
void BalanceTree<T>::fixHeight(){
	int hl = leftChild->Height();
	int hr = rightChild->Height();
	height = (hl > hr ? hl : hr) + 1;
}

template<typename T>
BalanceTree<T>* BalanceTree<T>::removeMin()
{
	if (!leftChild) {
		return rightChild;
	}
	leftChild = leftChild->removeMin();
	return balance();
}

template<typename T>
BalanceTree<T>* BalanceTree<T>::rotateRight(){
	BalanceTree<T>* temp = leftChild;
	leftChild = temp->rightChild;
	temp->rightChild = this;
	fixHeight();
	temp->fixHeight();
	return temp;
}

template <typename T>
BalanceTree<T>* BalanceTree<T>::rotateLeft() {
	BalanceTree<T>* temp = rightChild;
	rightChild = temp->leftChild;
	temp->leftChild = this;
	fixHeight();
	temp->fixHeight();
	return temp;
}

template <typename T>
BalanceTree<T>* BalanceTree<T>::copyTree() {
	std::queue<const BalanceTree<T> *> queueOfNodes;
	BalanceTree<T>* temp = nullptr;
	queueOfNodes.push(this);
	while (queueOfNodes.size()) {
		const BalanceTree<T> * copy = queueOfNodes.front();
		queueOfNodes.pop();
		temp = temp->insert(copy->data);
		if (copy->leftChild) {
			queueOfNodes.push(copy->leftChild);
		}
		if (copy->rightChild) {
			queueOfNodes.push(copy->rightChild);
		}
	}
	return temp;
}

template <typename T>
BalanceTree<T>* BalanceTree<T>::balance() {
	fixHeight();
	if (bfactor() == 2) {
		if (rightChild->bfactor() < 0) {
			rightChild = rightChild->rotateRight();
		}
		return rotateLeft();
	}
	if (bfactor() == -2) {
		if (leftChild->bfactor() > 0) {
			leftChild = leftChild->rotateLeft();
		}
		return rotateRight();
	}
	return this;
}

template<typename T>
BalanceTree<T>* BalanceTree<T>::insert(T itemToInsert)
{
	if (!this) {
		return new BalanceTree<T>(itemToInsert);
	}
	//инвариант именно такой (далее применяем его везде)
	if (itemToInsert < data) {
		leftChild = leftChild->insert(itemToInsert);
	}
	else{
		rightChild = rightChild->insert(itemToInsert);
	}
	return balance();
}

template<typename T>
BalanceTree<T>* BalanceTree<T>::deleteNode(T itemToDelete)
{
	if (!this) {
		return nullptr;
	}
	if (itemToDelete < data) {
		leftChild = leftChild->deleteNode(itemToDelete);
	}
	else if (itemToDelete > data) {
		rightChild = rightChild->deleteNode(itemToDelete);
	}
	else {
		BalanceTree<T>* temp1 = leftChild;
		BalanceTree<T>* temp2 = rightChild;
		delete this;
		if (!temp2) {
			return temp1;
		}
		BalanceTree<T>* min = temp2->findMin();
		min->rightChild = temp2->removeMin();
		min->leftChild = temp1;
		return min->balance();
	}
	return balance();
}

//обход в прямом порядке
template<typename T>
bool BalanceTree<T>::SearchPreOrder(T valueToFind) const {
	if (data == valueToFind) {
		return true;
	}
	else {
		if (valueToFind < data) {
			if (leftChild == nullptr) {
				return false;
			}
			return leftChild->SearchPreOrder(valueToFind);
		}
		else {
			if (rightChild == nullptr) {
				return false;
			}
			return rightChild->SearchPreOrder(valueToFind);
		}
	}
}

//симметричный обход
template <typename T>
bool BalanceTree<T>::SearchInOrder(T valueToFind) const {
	if (valueToFind < data && leftChild) {
		return leftChild->SearchInOrder(valueToFind);
	}
	if (data == valueToFind) {
		return true;
	}
	if (valueToFind >= data && rightChild) {
		return rightChild->SearchInOrder(valueToFind);
	}
	return false;
}

//обход в обратном порядке
template <typename T>
bool BalanceTree<T>::SearchPostOrder(T valueToFind) const {
	if (valueToFind < data && leftChild) {
		return leftChild->SearchPostOrder(valueToFind);
	}
	if (valueToFind > data && rightChild) {
		return rightChild->SearchPostOrder(valueToFind);
	}
	return data == valueToFind;
}

//практика обхода в ширину для подсчета узлов
template<typename T>
size_t BalanceTree<T>::len() const{
	if (!this) {
		return 0;
	}
	size_t length = 0;
	std::queue<const BalanceTree<T>*> queueOfNodes;
	queueOfNodes.push(this);
	while (!queueOfNodes.empty()) {
		auto temp = queueOfNodes.front();
		queueOfNodes.pop();
		++length;
		if (temp->leftChild) {
			queueOfNodes.push(temp->leftChild);
		}
		if (temp->rightChild) {
			queueOfNodes.push(temp->rightChild);
		}
	}
	return length;
}

template<typename T>
void BalanceTree<T>::print() const {
	if (!this) {
		std::cout << "Tree is empty!";
		return;
	}
	if (leftChild != nullptr) {
		leftChild->print();
	}
	std::cout << data << ' ';
	if (rightChild != nullptr) {
		rightChild->print();
	}
}

template<typename T>
void BalanceTree<T>::deleteTree(){
	if (leftChild) {
		leftChild->deleteTree();
	}
	if (rightChild) {
		rightChild->deleteTree();
	}
	if (!this) {

		delete this;
	}
}

template <typename T>
BalanceTree<T>* BalanceTree<T>::getSubTreeByValue(T value) {
	if (!SearchPreOrder(value)) {
		std::cout << "There is no such element in the tree!" << std::endl;
		return nullptr;
	}
	else {
		if (data == value) {
			BalanceTree<T>* temp = nullptr;
			std::queue<const BalanceTree<T>*> queueOfNodes;
			queueOfNodes.push(this);
			while (queueOfNodes.size()) {
				const BalanceTree<T>* copy = queueOfNodes.front();
				queueOfNodes.pop();
				temp = temp->insert(copy->data);
				if (copy->leftChild) {
					queueOfNodes.push(copy->leftChild);
				}
				if (copy->rightChild) {
					queueOfNodes.push(copy->rightChild);
				}
			}
			return temp;
		}
		else {
			if (value < data) {
				return leftChild->getSubTreeByValue(value);
			}
			else {
				return rightChild->getSubTreeByValue(value);
			}
		}
	}
}

template <typename T>
bool BalanceTree<T>::checkTree(const BalanceTree<T>* treeForCheck) const {
	if (!SearchPreOrder(treeForCheck->data) || len() < treeForCheck->len()) {
		return false;
	}
	else
	{
		if (data == treeForCheck->data) {
			std::queue<const BalanceTree<T>*> queueOfNodes1;
			std::queue<const BalanceTree<T>*> queueOfNodes2;
			queueOfNodes1.push(treeForCheck);
			queueOfNodes2.push(this);
			while (queueOfNodes1.size()) {
				const BalanceTree<T>* copy1 = queueOfNodes1.front();
				const BalanceTree<T>* copy2 = queueOfNodes2.front();
				if (copy1->data != copy2->data) {
					return false;
				}
				queueOfNodes1.pop();
				queueOfNodes2.pop();
				if (copy1->leftChild && copy2->leftChild) {
					queueOfNodes1.push(copy1->leftChild);
					queueOfNodes2.push(copy2->leftChild);
				}
				if (copy1->rightChild && copy2->rightChild) {
					queueOfNodes1.push(copy1->rightChild);
					queueOfNodes2.push(copy2->rightChild);
				}
			}
			return true;
		}
		if (treeForCheck->data < data) {
			return leftChild->checkTree(treeForCheck);
		}
		else {
			return rightChild->checkTree(treeForCheck);
		}
	} 
}



#endif // !BALANCETREE_H_


