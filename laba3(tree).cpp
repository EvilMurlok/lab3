#include <iostream>
#include <string>
#include <ctime>
#include "Student.h"
#include "BalanceTree.h"

using std::cout;
using std::endl;
using std::cin;
using std::string;

const size_t AMOUNT = 20;

//превращение массива в бинарную кучу
void MakeHeap(int* arr, size_t size, bool& flag) {
    for (size_t i = 0; i < size; ++i) {
        size_t index = i;
        while (index != 0) {
            size_t parent = (index - 1) / 2;
            if (arr[index] <= arr[parent]) {
                break;
            }
            flag = true;
            int temp = arr[index];
            arr[index] = arr[parent];
            arr[parent] = temp;
            index = parent;
        }
    }
    if (flag) {
        flag = false;
        MakeHeap(arr, 15, flag);
    }
    return;
}

int main()
{
   // srand(time(0));
    bool flag = false;
    int arr[] = { 7, 1, 10, 4, 6, 9, 2, 11, 3, 5, 12, 8, 13, 15, 14 };
    MakeHeap(arr, 15, flag );
    for (size_t i = 0; i < 15; ++i)
    {
        cout << arr[i] << ' ';
    }
    cout << endl;                                                                                   //          дерево было восстановлено при помощи метода извлечения поддерева из основного дерева

    BalanceTree<int>* firstObject = nullptr;
    for (size_t i = 1; i <= AMOUNT; ++i)                                                            //          для случая если строить "вырожденное" (т е натуральные значения 1..20) дерево,
                                                                                                    //          блансировка работает нормально
    {                                                                                               //          из-за нехватки места пришлось расширить дерево (предок у 16 и 4 - 8)
        firstObject = firstObject->insert(i);                                                       //          дерево           8 
    }                                                                                               //                 /                    \
    
    cout << endl;                                                                                   //                4                      16
    firstObject->print();                                                                           //               /  \                   /   \
    
    cout << endl;                                                                                   //              2    6                12       18
    for (size_t i = 1; i <= AMOUNT + 5; ++i) {   
                                                                                                    //             / \     \              / \     /   \
       
        cout << firstObject->SearchPreOrder(i) << ' ';                                              //            1   3     7            10  14  17    20
    }
    
                                                                                                    //                                /     /   \     /   \

    cout << endl;                                                                                   //                              9     13    15 19     324
    for (size_t i = 1; i <= AMOUNT + 5; ++i) {
        cout << firstObject->SearchInOrder(i) << ' ';
    }
    cout << endl;

    for (size_t i = 1; i <= AMOUNT + 5; ++i) {
        cout << firstObject->SearchPostOrder(i) << ' ';
    }
    cout << endl;

    cout << firstObject->len() << endl;
    
    firstObject = firstObject->deleteNode(5);
    firstObject->print();
    cout << endl;
    firstObject->map([](int& temp) {
        temp += 10; });
    firstObject->print();
    cout << endl;
    cout << firstObject->reduce(0, [](int& zeroValue, int& value) {
        return zeroValue + value;
        }) << endl;
    BalanceTree<int>* secondObject = firstObject->where([](int& temp) { return temp > 17; });
    secondObject->print();
    cout << endl;   

    BalanceTree<Student>* studentsTree = nullptr;
    studentsTree = studentsTree->insert({ "Petrov", 'M', 72.432 });
    studentsTree = studentsTree->insert({ "Pupkin", 'M', 84.32 });
    studentsTree = studentsTree->insert({ "Vasechkin", 'M', 89.23 });
    studentsTree = studentsTree->insert({ "Panin", 'M', 99.432 });
    studentsTree->print();
    cout << endl;
    cout << studentsTree->SearchPostOrder({ "Vasechkin", 'M', 72.432 });
    cout << studentsTree->SearchPostOrder({ "Pupkin", 'M', 84.32 });
    cout << studentsTree->SearchPostOrder({ "Petrov", 'M', 89.23 });
    cout << studentsTree->SearchPostOrder({ "Panin", 'M', 99.432 });
    BalanceTree<Student>* temp = studentsTree->getSubTreeByValue({ "Petrov", 'M', 89.23 });
    temp->print();
    cout << endl;
    temp = studentsTree->getSubTreeByValue({ "Pupkin", 'M', 84.32 });
    temp->print();
    cout << endl;
    temp = studentsTree->getSubTreeByValue({ "Vasechkin", 'M', 72.432 });
    temp->print();
    cout << endl;
    temp = studentsTree->getSubTreeByValue({ "Panin", 'M', 99.432 });
    temp->print();
    cout << endl;
    BalanceTree<Student>* stud2 = nullptr;
    stud2 = stud2->insert({ "Pupkin", 'M', 84.32 });
    stud2 = stud2->insert({ "Panin", 'M', 89.23 });
    cout << studentsTree->checkTree(stud2) << endl; //выведет ноль
    studentsTree = studentsTree->deleteNode({ "Pupkin", 'M', 84.32 });
    studentsTree->print();
    return EXIT_SUCCESS;
}

