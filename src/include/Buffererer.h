/*
 * Buffererer.h
 *
 *  Created on: May 7, 2013
 *      Author: eric
 */

#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/thread/mutex.hpp>
#include <string>
#include <cmath>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>
#include "RaytraceBuffer.hpp"
#include "mat.hpp"
#include "Engine.hpp"
#include "Object.hpp"
#include "Util.hpp"

#ifndef BUFFERERER_H_
#define BUFFERERER_H_

typedef boost::function<float (float)> timeParamaterizedThing;

class Buffererer
{
  public:
    Buffererer();
    virtual ~Buffererer();
  private:
    boost::thread _thread;
    boost::mutex padlock;
    bool die;
    void kill();
    void threadFunc();
};

class BufferererManager
{
  public:
    static BufferererManager *instance( void );
    void Displayed();
    boost::mutex birthlock;
    std::map<int, Buffererer*> _instances;
    int _maxTextures;
    void add(Buffererer*);
    void remove(Buffererer*);
};

#endif /* BUFFERERER_H_ */
