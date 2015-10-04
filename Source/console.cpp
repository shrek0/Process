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

#include "console.h"

#include <iostream>

Console::Console()
    : notFoundHandler(defaultNotFoundHandler)
{
}

void Console::stop()
{
    stopFlag = true;
}

void Console::run() {
    // An option handler can change the status of stop flag, threads is for weaks!
    while(stopFlag == false)
    {
        processOption();
    }
}

std::string Console::readWord() {
    std::string tmp;

    std::cin >> tmp;

    return tmp;
}

void Console::addOption(const std::string &optionName, uint8_t argumentsNumber, const OptionHandler &optionHandler)
{
    mOptions[optionName] = OptionEntry{ argumentsNumber, optionHandler };
}

void Console::processOption() {
    const auto word = readWord();

    if(mOptions.count(word) == 0)
        notFoundHandler(word);

    std::list<std::string> stringList;
    stringList.push_back(word);

    // Read the required args.
    for(uint8_t i = 0; i < mOptions.at(word).argumentsNumber; ++i)
    {
        stringList.push_back(readWord());
    }

    mOptions.at(word).optionHandler(stringList);
}

void Console::defaultNotFoundHandler(const std::string &optionName) {
    std::string message = "The option " + optionName + " not found";

    write(message);
}


