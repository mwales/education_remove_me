#pragma once

#include <iostream>
#include <vector>
#include <map>


template<typename printableType>
void printVector(std::vector<printableType> v);

template<typename keyType, typename valueType>
void printMap(std::map<keyType, valueType> m);


template<typename sortableType>
void insertOrdered(std::vector<sortableType>& origList, sortableType x);

template<typename t>
std::vector<t> append(std::vector<t> a, std::vector<t> b);

template<typename keyType, typename valueType>
std::map<keyType, valueType> mergeMaps(std::map<keyType, valueType> m1,
                             std::map<keyType, valueType> m2);

template<typename keyType, typename valueType>
std::map<keyType, valueType> multiplyMap(std::map<keyType, valueType> m1, int val);

template<typename keyType, typename valueType>
valueType sumMap(std::map<keyType, valueType> m1);


std::vector<std::string> stringSplit(std::string const & input, char delimeter);

std::string replaceChar(std::string orig, char before, char after);

