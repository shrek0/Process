/**
 * @file
 * @author shrek0 (shrek0.tk@gmail.com)
 * @class
 * @section LICENSE
 *
 * ProtocolLearn copyright (C) 2015 shrek0
 *
 * ProtocolLearn is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ProtocolLearn is distributed in the hope that it will be useful,
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

#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <dirent.h>

#include <stdlib.h>
#include <stdio.h>

#include <string>
#include <list>

#include <exception>

class Directory
{
public:
    Directory(const std::string &directoryPath);

    std::list<dirent> getFiles();

    dirent find(const std::string &entryName);

private:
    DIR *mPDir;

    std::string directoryName;
};

#endif // DIRECTORY_H