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

#ifndef PROCESS_H
#define PROCESS_H

#include <string>

#include <unistd.h>
#include <signal.h>

#include <sys/ptrace.h>
#include <sys/user.h>

#include "directory.h"

#include <cstdint>

class std::vector;

class Process
{
public:
    typedef pid_t ProcessID;
    typedef user_regs_struct ProcessRegisters;

    typedef uint8_t Byte;
    typedef uint16_t Word;
    typedef uint32_t DoubleWord;
    typedef uint64_t QuadWord;

    typedef register_t Register;
    typedef Register MemoryAddress;

    Process(const std::string &programName);
    Process(ProcessID processID);

    ~Process();

    /**
     * @brief step  Run one assembly and stop.
     * @note Don't call this function when the process is running.
     */
    void step();

    /**
     * @brief stop  Stop the process.
     */
    void stop();

    /**
     * @brief cont  Continue the process.
     * @param signal  signal to send on the start (optional).
     * @note Don't call this function when the process is running.
     */
    void cont(int signal=0);

    /**
     * @brief continueAndStopOnSystemCall  Continue the process as for cont() and stop the process before and after an interrupting of a system call.
     * @note Don't call this function when the process is running.
     */
    void continueAndStopOnSystemCall();

    /**
     * @brief wait  Wait for the process.
     * @param options
     * @return
     */
    int wait(int options=0);

    /**
     * @brief kill  Send a signal to the process.
     * @param signal
     */
    void kill(int signal=SIGKILL);

    /**
     * @brief getProcessRegisters  Get the process's registers.
     * @return
     */
    ProcessRegisters getProcessRegisters();

    /**
     * @brief setProcesssRegisters  Set the process's registers.
     * @param processRegisters
     */
    void setProcesssRegisters(const ProcessRegisters &processRegisters);

    /**
     * @brief jump  Jump to an address.
     * @param ip
     */
    void jump(MemoryAddress ip);

    /**
     * @brief call  Push the current address and jump to @arg address.
     * @param address  A valid virtual address (in the process's space).
     */
    void call(MemoryAddress address);

    void push(Byte byte);
    void push(Word word);
    void push(DoubleWord doubleWord);
    void push(QuadWord quadWord);

    template<typename T>
    void push(T value) {

        auto registers = getProcessRegisters();

        // Allocate some memory in the stack.
        registers.rsp -= sizeof(T);

        setProcesssRegisters(registers);

        Word *wordArray = reinterpret_cast<Word*>(&value);

        size_t bytesToWrite=sizeof(T);

        for(size_t bytesWritten=0; bytesWritten < sizeof(T)-1; bytesWritten+=sizeof(Register))
        {
            move(*wordArray++, registers.rsp+bytesWritten);
        }

        if(bytesToWrite == sizeof(T)-1) {
            Register lastWord = *reinterpret_cast<Byte *>(&value) + sizeof(T)-1;

            move(lastWord, registers.rsp+sizeof(T))
        }

    }

    /**
     * @brief movePointer  Copy a Word from the process's memory to another place in the process's memory.
     * @param sourceAddress
     * @param destinationAddress
     */
    void movePointer(MemoryAddress sourceAddress, MemoryAddress destinationAddress);

    /**
     * @brief move  Move a Word to the process's memory.
     * @param source  Any word.
     * @param destinationAddress  Valid virtual process address.
     */
    void move(Word source, MemoryAddress destinationAddress);

    void write(const std::vector<Byte> &bytesToWrite, MemoryAddress destinationAddresss);

    std::vector<Byte> read(MemoryAddress sourceAddress, const std::vector::size_type &bytesCount);

    /**
     * @brief copyFrom  Copy a Word from sourceAddress and return it.
     * @param sourceAddress
     * @return
     */
    Word copyFrom(MemoryAddress sourceAddress);

    /**
     * @brief getProcessID
     * @return
     */
    ProcessID getProcessID();
    std::string getProgramName();

    std::string getCmdline();

    struct ProcessInfo
    {
        ProcessID parentProcessID;
        std::string cmdline;
        uid_t userID;
    };

private:
    static long ptrace(enum __ptrace_request request, void *addr, void *data, pid_t pid);
    long ptrace(__ptrace_request request, void *addr, void *data)
    {
        return ptrace(request, addr, data, processID);
    }

    long ptrace(__ptrace_request request, void *addr, int data)
    {
        return ptrace(request, addr, reinterpret_cast<void *>(data));
    }

    Word ptrace(__ptrace_request request, MemoryAddress address, Register data)
    {
        return ptrace(request, reinterpret_cast<void*>(address), reinterpret_cast<void*>(data));
    }

    /**
     * @brief processID  The process's ID.
     */
    ProcessID processID = 0xdeadbeef;

    static ProcessID programNameToProcessID(const std::string &programName);

    static std::string getProgramNameByCmdLine(const std::string &cmdline);
    static std::string gedCmdlineByProcessID(ProcessID processID);

    static std::string processIDToProgramName(ProcessID processID);

    static const char procPath[];
};

#endif // PROCESS_H
