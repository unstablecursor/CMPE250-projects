/*
Student Name: Recep Deniz Aksoy
Student Number: 2014400150
Project Number: #1
Operating System: written in OSX, compiled and tested in xubuntu.
Compile Status:
Program Status:
Notes: Anything you want to say about your code that will be helpful in the grading process.

*/

#include "BigInteger.h"
#include <algorithm>

BigInteger::BigInteger(int number){
    num = new LinkedList;
    int numberdec = number;
    if(number == 0){
        num->pushTail(0);
    }
    for (int i = 1; i <= numberdec; i = i * 10) {
        num->pushTail(number % 10);
        number = number / 10;
    }

}

BigInteger::BigInteger(const string& bigInteger){
    num = new LinkedList;
    for (int i = 0; i < bigInteger.length(); i++) {
        num->pushHead(bigInteger[i] - '0');
    }
}

BigInteger BigInteger::operator+(const BigInteger &list){
    BigInteger result(0);                   //Creating a result
    Node *pointer1 = this->num->head;
    Node *pointer2 = list.num->head;
    Node *resPointer = result.num->head;
    while(pointer1 != nullptr && pointer2 != nullptr){
        if((resPointer->data + pointer1->data + pointer2->data) > 9) {
            if(resPointer->next == nullptr){
                result.num->pushTail(0);
            }
            resPointer->next->data = resPointer->next->data + 1;
            Node *traverse = resPointer->next;
            while (traverse->data > 9 && traverse->next != nullptr) {
                traverse->data = traverse->data % 10;
                traverse->next->data = traverse->next->data + 1;
                traverse = traverse->next;
            }
        }
        resPointer->data = (resPointer->data + (pointer1->data + pointer2->data)) % 10;
        pointer1 = pointer1->next;
        pointer2 = pointer2->next;
        if(resPointer->next == nullptr){
            result.num->pushTail(0);
        }
        resPointer = resPointer->next;

    }
    Node *nonEmpty = nullptr;
    if (pointer2 != nullptr){
        nonEmpty = pointer2;
    }
    else if(pointer1 != nullptr) {
        nonEmpty = pointer1;
    }
    while (nonEmpty != nullptr){
        if(nonEmpty->next == nullptr){
            resPointer->data = nonEmpty->data;
        }
        else {
            if(resPointer->next == nullptr){result.num->pushTail(0);}
            resPointer->data = resPointer->data + nonEmpty->data;
            resPointer = resPointer->next;
        }
        nonEmpty = nonEmpty->next;
    }

    if(result.num->tail->data == 0) {
        Node *tmp = result.num->head;
        while(tmp->next != result.num->tail){
            tmp = tmp->next;
        }
        tmp->next = nullptr;
        result.num->tail = tmp;
    }
    return result;
}
BigInteger BigInteger::operator*(const BigInteger &list){
    BigInteger result(0);
    Node *pointer1;
    Node *pointer2 = list.num->head;
    Node *resPointer;
    Node *resPointerDec = result.num->head;
    while(pointer2 != nullptr){
        resPointer = resPointerDec;
        pointer1 = this->num->head;
        while(pointer1!= nullptr){
            int addRes = resPointer->data + pointer2->data * pointer1->data;
            resPointer->data = (addRes) % 10;
            if(resPointer->next == nullptr){result.num->pushTail(0);}
            resPointer->next->data = resPointer->next->data + addRes / 10;
            Node *traverse = resPointerDec->next;
            while(traverse->data > 9 && traverse->next != nullptr){
                traverse->data = traverse->data % 10;
                traverse->next->data = traverse->next->data + 1;
                traverse = traverse->next;
            }
            resPointer = resPointer->next;
            pointer1 = pointer1->next;
        }
        pointer2 = pointer2->next;
        resPointerDec = resPointerDec->next;
    }

    return result;

}

BigInteger BigInteger::operator*(int i){
    BigInteger result(0);
    Node *pointer1 = this->num->head;
    Node *resPointer = result.num->head;
    while(pointer1!= nullptr){
        int addRes = resPointer->data + i * pointer1->data;
        resPointer->data = addRes % 10;
        if(resPointer->next == nullptr){result.num->pushTail(0);}
        resPointer->next->data = resPointer->next->data + addRes / 10;
        Node *traverse = resPointer->next;
        while(traverse->data >9 && traverse->next != nullptr){
            traverse->data = traverse->data % 10;
            traverse->next->data = traverse->next->data + 1;
            traverse = traverse->next;
        }
        resPointer = resPointer->next;
        pointer1 = pointer1->next;
    }
    return result;
}

BigInteger::BigInteger(const BigInteger &other){
    num = new LinkedList(*other.num);
}
BigInteger& BigInteger::operator=(const BigInteger &list){
    if(num){
        delete num;
    }
    num = new LinkedList(*list.num);
    return *this;
}

BigInteger::~BigInteger(){
    if(num){
        delete num;
    }
}