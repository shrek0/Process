/**
 * @file process.cpp
 * @author shrek0 (shrek0.tk@gmail.com)
 * @class Process
 * @section LICENSE
 *
 * This file is part of Process.
 *
 * Process copyright (C) 2015 shrek0
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
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

#include "process.h"

#include <fstream>
#include <sys/types.h>
#include <sys/wait.h>

#include <stdexcept>
#include <vector>

const char Process::procPath[] = "/proc/";

Process::Process(const std::string &programName)
    : Process(programNameToProcessID(programName))
{
}

Process::Process(Process::ProcessID processID)
    : processID(processID)
{
    ptrace(PTRACE_ATTACH, nullptr, nullptr);

    wait(WUNTRACED);
}

Process::~Process()
{
    ptrace(PTRACE_DETACH, nullptr, nullptr);
}

void Process::step()
{
    ptrace(PTRACE_SINGLESTEP, nullptr, nullptr);
}

void Process::stop()
{
    kill(SIGSTOP); // In the end it's just sending SIGTRAP (and stop the )
}

void Process::cont(int signal)
{
    ptrace(PTRACE_CONT, nullptr, signal);
}

void Process::continueAndStopOnSystemCall()
{
    ptrace(PTRACE_SYSCALL, nullptr, 0);
}

int Process::wait(int options) {
    int ret = 0;

    ::waitpid(processID, &ret, options);

    return ret;
}

void Process::kill(int signal)
{
    ::kill(processID, signal);
}

Process::ProcessRegisters Process::getProcessRegisters() {
    ProcessRegisters processRegisters;

    ptrace(PTRACE_GETREGS, nullptr, &processRegisters);

    return processRegisters;
}

void Process::setProcesssRegisters(const Process::ProcessRegisters &processRegisters)
{
    ptrace(PTRACE_SETREGS, nullptr, const_cast<Process::ProcessRegisters *>(&processRegisters));
}

void Process::jump(MemoryAddress ip) {
    auto registers = getProcessRegisters();

#ifdef __x86_64__
    registers.rip = ip;
#elif defined __i386__
    registers.eip = ip;
#endif

    setProcesssRegisters(registers);
}

void Process::call(MemoryAddress address) {
    auto registers = getProcessRegisters();

    // Push the current ip register into the stack (the return address).
    // Jump to the address (by changing ip register).
#ifdef __x86_64__
//    registers.rsp -= sizeof(register_t);

    push(registers.rip);

    registers.rip = address;
#elif defined __i386__
//    registers.esp -= sizeof(register_t);

    push(registers.rip);

    registers.eip = address;
#else
# error "Your arch is not supported by Process"
#endif

    setProcesssRegisters(registers);
}

void Process::movePointer(MemoryAddress sourceAddress, MemoryAddress destinationAddress)
{
    move(copyFrom(sourceAddress), destinationAddress);
}

void Process::move(Word source, MemoryAddress destinationAddress)
{
    ptrace(PTRACE_POKEDATA, destinationAddress, source);
}

void Process::write(const std::vector<Process::Byte> &bytesToWrite, Process::MemoryAddress destinationAddresss) {
    const auto &bytesToWriteLength = bytesToWrite.size();

    if(bytesToWriteLength % sizeof(Word) != 0) {
        Word word = bytesToWrite[0];

        move(word, destinationAddresss++);
    }

    for(const Byte *pBytesToWrite = bytesToWrite.data(); pBytesToWrite < bytesToWrite.data()+bytesToWriteLength-1; pBytesToWrite+=2) {
        Word word = *reinterpret_cast<const Word *>(pBytesToWrite);

        move(word, destinationAddresss);

        destinationAddresss+=2;
    }
}

std::vector<Process::Byte> Process::read(Process::MemoryAddress sourceAddress, const std::vector<Byte>::size_type &bytesCount) {
    std::vector<Byte> vector;
    vector.resize(bytesCount);

    Word *data = reinterpret_cast<Register *>(vector.data());

    while(bytesCount > 0) {
        *data++ = copyFrom(sourceAddress);

        sourceAddress+=sizeof(Register);
        bytesCount-=sizeof(Register);
    }
}

Process::Word Process::copyFrom(MemoryAddress sourceAddress)
{
    return ptrace(PTRACE_PEEKDATA, sourceAddress, 0); // @arg data is ignored here.
}

long Process::ptrace(__ptrace_request request, void *addr, void *data, ProcessID pid) {
    long ret = ::ptrace(request, pid, addr, data);

    // ret may contain -1 and it will be okay (PTRACE_PEEK*)
    if(ret == -1 && errno)
        throw std::invalid_argument("ptrace() returned < 0");

    return ret;
}

Process::ProcessID Process::programNameToProcessID(const std::string &programName) {
    Directory directory(procPath);
    auto entries = directory.getFiles();

    for(const auto &entry : entries) {
        // Convert the directory's name to a process id.
        ProcessID processID = std::atoi(entry.d_name);

        // If it's not a numeric name.
        if(processID < 0)
            continue;

        // Check if the process's program name is the requested name.
        if(processIDToProgramName(processID) == programName)
            return processID;
    }

    throw std::invalid_argument("Process Name not found");
}

std::string Process::getProgramNameByCmdLine(const std::string &cmdline) {
    std::string::size_type position = cmdline.find('\0');

    if(position == std::string::npos)
        return std::string("-1");

    // /proc/*/cmdline 's format is FILENAME%00parameters.
    // Therefore, get the first part of the string (before the '\0')
    std::string programName = cmdline.substr(0, position);

    // If this is a path, keep only the file's name.
    position = programName.find('/');

    if(position != std::string::npos)
    {
        programName = programName.substr(position+1);
    }

    return programName;
}

std::string Process::gedCmdlineByProcessID(Process::ProcessID processID) {
    std::string cmdline = procPath;
    cmdline += processID + "/cmdline";

    std::ifstream file(cmdline.c_str());

    // Get the first line of /proc/processID/cmdline.
    std::getline(file, cmdline);

    return cmdline;
}

std::string Process::processIDToProgramName(Process::ProcessID processID)
{
    return getProgramNameByCmdLine(gedCmdlineByProcessID(processID));
}

