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

#include "directory.h"

#include <stdexcept>

Directory::Directory(const std::string &directoryPath)
{
    mPDir = opendir(directoryPath.c_str());
}

std::list<dirent> Directory::getFiles() {
    dirent *directoryEntry = nullptr;
    std::list<dirent> files;

    while((directoryEntry = readdir(mPDir)) > 0)
    {
        files.push_front(*directoryEntry);
    }

    return files;
}

dirent Directory::find(const std::string &entryName) {
    dirent *directoryEntry = nullptr;

    while((directoryEntry = readdir(mPDir)) > 0) {
        if(entryName == directoryEntry->d_name)
            return *directoryEntry;
    }

    throw std::invalid_argument("find: Not Found");
}
