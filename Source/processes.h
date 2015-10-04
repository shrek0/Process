/**
 * @file
 * @author shrek0 (shrek0.tk@gmail.com)
 * @class
 * @section LICENSE
 *
 * Process copyright (C) 2015 shrek0
 *
 * Process is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Process is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @section DESCRIPTION
 *
 */

#ifndef PROCESSES_H
#define PROCESSES_H

template<typename _BasicType, typename _ContainerType, typename _PositionType=unsigned int>
class BasicIterator
{
public:
    typedef _BasicType BasicType;
    typedef _ContainerType ContainerType;
    typedef _PositionType PositionType;

    BasicIterator(ContainerType &container, const PositionType &position)
        : mContainer(container), mPosition(position)
    {
    }

    bool operator !=(const BasicIterator &anotherIterator)
    {
        return mPosition != anotherIterator.mPosition;
    }

    const BasicIterator<BasicType, ContainerType> operator ++() {
        ++mPosition;

        return *this;
    }

    BasicType &operator *()
    {
        return mContainer.at(mPosition);
    }

    BasicType &operator ->()
    {
        return mContainer.at(mPosition);
    }

private:
    ContainerType &mContainer;
    PositionType mPosition;
};

#include <string>
#include <list>
#include <process.h>

//class Processes
//{
//public:
//    Processes();

//    typedef BasicIterator<Process, Processes, Process::ProcessID> IteratorType;

//    IteratorType begin() { return IteratorType(*this, getFirstPID()); }
//    IteratorType end() { return IteratorType(*this, getLastPID()); }

//    std::list<Process> filterByName(const std::string &name);
//    Process getProcess(const Process::ProcessID &processID);

//private:

//};

#endif // PROCESSES_H
