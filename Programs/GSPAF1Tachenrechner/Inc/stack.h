/**
  * @file stack.h
  * @author Maksym Zadoya
  * @date 2025/10/27 #1
  * @brief Header file of stack module. 
  */

#include <stdbool.h>

#ifndef STACK_H
#define STACK_H

void init();

int top();

int getAll(int* all);

void clear();

int dupeTop();

int swapTop();

int push(int v);

int pop(int *v);

int getPos();

int getSecondValue();

bool safetyCheckOne();

 bool safetyCheckTwo();

 bool overflowCheck();

#endif