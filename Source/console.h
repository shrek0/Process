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

#ifndef CONSOLE_H
#define CONSOLE_H

#include <iostream>

#include <map>

#include <list>

#include <functional>

class Console
{
public:
    typedef std::function<void(const std::string &optionName)> NotFoundHandlerType;
    typedef std::function<void(const std::list<std::string> &arguments)> OptionHandler;

    Console();

    void stop();
    void run();

    void setOptionNotFoundHandler(const NotFoundHandlerType &handler);

    static std::string readWord();
    static void write(const std::string &string);

    /**
     * @brief addOption
     * @param optionName
     * @param argumentsNumber
     */
    void addOption(const std::string &optionName, uint8_t argumentsNumber, const OptionHandler &optionHandler);

    /**
     * @brief getOption  Wait until the user will send something relevant.
     * @return
     */
    void processOption();

private:
    struct OptionEntry{
        uint8_t argumentsNumber;
        OptionHandler optionHandler;
    };

    static void defaultNotFoundHandler(const std::string &optionName);

    std::map<std::string, OptionEntry> mOptions;
    NotFoundHandlerType notFoundHandler;

    bool stopFlag = false;
};

#endif // CONSOLE_H
