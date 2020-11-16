/*
  ==============================================================================

    DataRace.cpp
    Created: 13 Nov 2020 6:05:08pm
    Author:  Lucio Westmoreland

  ==============================================================================
*/

#include "DataRace.h"

bool Data::operator==(const Data& other)
{
    return a == other.a && b == other.b;
}
bool Data::operator!=(const Data& other)
{
    return !(*this == other);
}

void A::run()
{
    while ( true )
    {
        
        if ( threadShouldExit() )
            break;
        x = a;
    
        //compare
        if ( x != a && x != b )
        {
            auto _local = x;
            
            std::cout << "A: " << "x = { .a=" << String::toHexString(_local.a) << ", .b=" << String::toHexString(_local.b) << std::endl;
        }
        
        wait (10);
    }
}

void B::run()
{
    while ( true )
    {
        
        if ( threadShouldExit() )
            break;
        x = b;
        
        //compare
        if ( x != a && x != b )
        {
            auto _local = x;
            std::cout << "B: " << "x = { .a=" << String::toHexString(_local.a) << ", .b=" << String::toHexString(_local.b) << std::endl;
        }
    }
}

//======================================
void LockingStruct::threadAFunc()
{
    {
    const ScopedLock sl (criticalSection);
    dataMember = 2;
    }
    std::cout << "thread A Func" << dataMember << std::endl;
}
void LockingStruct::threadBFunc()
{
    {
    const ScopedLock sl (criticalSection);
    dataMember = 0;
    }
    std::cout << "thread B Func" << dataMember << std::endl;
}


//======================================
//======================================
//======================================
//======================================
