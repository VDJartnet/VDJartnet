//
//  CSUndoManager.hpp
//  CppStep
//
//  Copyright � 2017-18 Jonathan Tanner. All rights reserved.
//
//This file is part of CppStep.
//
//CppStep is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//CppStep is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with CppStep.  If not, see <http://www.gnu.org/licenses/>.

#ifndef CSUndoManager_hpp
#define CSUndoManager_hpp

#include <functional>

/** An undo manager */
class CSUndoManager {
public:
    CSUndoManager(); /**< Construct an undo manager */

    void registerUndoFunc(std::function<void()> f); /**< Register an undo to the stack with the given function object */

    /** Register an undo to the stack with the given function and argument to pass it */
    template<typename T>
    void registerUndoFuncArg(std::function<void(T)> f, T arg) {
        registerUndoFunc([f, arg]() { f(arg); });
    }

    /** Register an undo to the stack with the given function and pair of arguments to pass it */
    template<typename S, typename T>
    void registerUndoFuncpArg2(void(*f)(S, T), S arg1, T arg2) {
        registerUndoFunc([f, arg1, arg2]() { f(arg1, arg2); });
    }

    /** Register an undo to the stack that restores the value of the given target */
    template<typename T>
    void registerUndoTarget(T* target) {
        T value = *target;
        registerUndoFunc([target, value]() { *target = value; });
    }

    bool canUndo(); /**< There is an undo on the stack */
    void undo(); /**< Undo the last value from the undo stack */
    bool canRedo(); /**< There is a redo on the stack */
    void redo(); /**< Redo the last value from the redo stack */
private:
    /** An event that can be undone */
    class UndoItem {
    public:
        std::function<void()> f; /**< The function to be used to undo the event */
        UndoItem* next; /**< The next event to undo */
        /** Construct an event with the given function and next event */
        UndoItem(std::function<void()> fTMP, UndoItem* nextTMP) {
            this->f = fTMP;
            this->next = nextTMP;
        }
        /** Destruct the event and next event */
        ~UndoItem() {
            delete this->next;
        }
    };

    UndoItem* undoStack; /**< The stack of undos */
    UndoItem* redoStack; /**< The stack of redos */

    bool isUndoing; /**< Is undoing an event. While undoing a new event registered is a redo. */
    bool isRedoing; /**< Is redoing an event. While redoing a new event registered should not clear the redo stack. */
};


#endif /* CSUndoManager_hpp */
