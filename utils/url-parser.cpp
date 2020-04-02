/**
 * @file url-parser.cpp
 * @author Mingxuan Fan
*/

#include <stdio.h>
#include <iostream>
#include <string>

std::string parseUrl(std::string reqStr) {
    std::string firstLine = reqStr.substr(0, reqStr.find("\r\n"));
    firstLine = firstLine.substr(firstLine.find(" ") + 1);
    return firstLine.substr(0, firstLine.find(" "));
}