//  Created by Karl Stenerud on 2012-01-28.
//
//  Copyright (c) 2012 Karl Stenerud. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall remain in place
// in this source code.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
//  GrowingCrashC.h
//  GrowingAnalytics
//
//  Created by YoloMao on 2022/9/19.
//  Copyright (C) 2022 Beijing Yishu Technology Co., Ltd.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.


/* Primary C entry point into the crash reporting system.
 */


#ifndef HDR_GrowingCrashC_h
#define HDR_GrowingCrashC_h

#ifdef __cplusplus
extern "C" {
#endif


#include "GrowingCrashMonitorType.h"
#include "GrowingCrashReportWriter.h"

#include <stdbool.h>


/** Install the crash reporter. The reporter will record the next crash and then
 * terminate the program.
 *
 * @param installPath Directory to install to.
 *
 * @return The crash types that are being handled.
 */
GrowingCrashMonitorType growingcrash_install(const char* appName, const char* const installPath);

/** Set the crash types that will be handled.
 * Some crash types may not be enabled depending on circumstances (e.g. running
 * in a debugger).
 *
 * @param monitors The monitors to install.
 *
 * @return The monitors that were installed. If GrowingCrash has been
 *         installed, the return value represents the monitors that were
 *         successfully installed. Otherwise it represents which monitors it
 *         will attempt to activate when GrowingCrash installs.
 */
GrowingCrashMonitorType growingcrash_setMonitoring(GrowingCrashMonitorType monitors);

/** Set the user-supplied data in JSON format.
 *
 * @param userInfoJSON Pre-baked JSON containing user-supplied information.
 *                     NULL = delete.
 */
void growingcrash_setUserInfoJSON(const char* const userInfoJSON);

/** Set the maximum time to allow the main thread to run without returning.
 * If a task occupies the main thread for longer than this interval, the
 * watchdog will consider the queue deadlocked and shut down the app and write a
 * crash report.
 *
 * Warning: Make SURE that nothing in your app that runs on the main thread takes
 * longer to complete than this value or it WILL get shut down! This includes
 * your app startup process, so you may need to push app initialization to
 * another thread, or perhaps set this to a higher value until your application
 * has been fully initialized.
 *
 * 0 = Disabled.
 *
 * Default: 0
 */
void growingcrash_setDeadlockWatchdogInterval(double deadlockWatchdogInterval);

/** If true, attempt to fetch dispatch queue names for each running thread.
 *
 * WARNING: There is a chance that this will crash on a growingcrashthread_getQueueName() call!
 *
 * Enable at your own risk.
 *
 * Default: false
 */
void growingcrash_setSearchQueueNames(bool searchQueueNames);

/** If true, introspect memory contents during a crash.
 * Any Objective-C objects or C strings near the stack pointer or referenced by
 * cpu registers or exceptions will be recorded in the crash report, along with
 * their contents.
 *
 * Default: false
 */
void growingcrash_setIntrospectMemory(bool introspectMemory);

/** List of Objective-C classes that should never be introspected.
 * Whenever a class in this list is encountered, only the class name will be recorded.
 * This can be useful for information security concerns.
 *
 * Default: NULL
 */
void growingcrash_setDoNotIntrospectClasses(const char** doNotIntrospectClasses, int length);

/** Set the callback to invoke upon a crash.
 *
 * WARNING: Only call async-safe functions from this function! DO NOT call
 * Objective-C methods!!!
 *
 * @param onCrashNotify Function to call during a crash report to give the
 *                      callee an opportunity to add to the report.
 *                      NULL = ignore.
 *
 * Default: NULL
 */
void growingcrash_setCrashNotifyCallback(const GrowingCrashReportWriteCallback onCrashNotify);

typedef void (*GrowingCrashReportWrittenCallback)(int64_t reportID);

/** Set the callback to invoke upon finishing writing a crash report.
 *
 * WARNING: Only call async-safe functions from this function! DO NOT call
 * Objective-C methods!!!
 *
 * @param onReportWrittenNotify Function to call after writing a crash report to
 *                      give the callee an opportunity to react to the report.
 *                      NULL = ignore.
 *
 * Default: NULL
 */
void growingcrash_setReportWrittenCallback(const GrowingCrashReportWrittenCallback onReportWrittenNotify);

/** Set if GrowingCrashLOG console messages should be appended to the report.
 *
 * @param shouldAddConsoleLogToReport If true, add the log to the report.
 */
void growingcrash_setAddConsoleLogToReport(bool shouldAddConsoleLogToReport);

/** Set if GrowingCrash should print the previous log to the console on startup.
 *  This is for debugging purposes.
 */
void growingcrash_setPrintPreviousLog(bool shouldPrintPreviousLog);

/** Set the maximum number of reports allowed on disk before old ones get deleted.
 *
 * @param maxReportCount The maximum number of reports.
 */
void growingcrash_setMaxReportCount(int maxReportCount);

/** Report a custom, user defined exception.
 * This can be useful when dealing with scripting languages.
 *
 * If terminateProgram is true, all sentries will be uninstalled and the application will
 * terminate with an abort().
 *
 * @param name The exception name (for namespacing exception types).
 *
 * @param reason A description of why the exception occurred.
 *
 * @param language A unique language identifier.
 *
 * @param lineOfCode A copy of the offending line of code (NULL = ignore).
 *
 * @param stackTrace JSON encoded array containing stack trace information (one frame per array entry).
 *                   The frame structure can be anything you want, including bare strings.
 *
 * @param logAllThreads If true, suspend all threads and log their state. Note that this incurs a
 *                      performance penalty, so it's best to use only on fatal errors.
 *
 * @param terminateProgram If true, do not return from this function call. Terminate the program instead.
 */
void growingcrash_reportUserException(const char* name,
                                 const char* reason,
                                 const char* language,
                                 const char* lineOfCode,
                                 const char* stackTrace,
                                 bool logAllThreads,
                                 bool terminateProgram);

/** Experimental feature. Works like LD_PRELOAD. Enable C++ exceptions catching with __cxa_throw swap,
 * by updating pointers in the indirect symbol table, which is located in the __LINKEDIT segment.
 * It supports getting a true stackstace even in dynamically linked libraries.
 * Also allows a user to override original __cxa_throw  with his implementation.
 */
void enableSwapCxaThrow(void);
    
#pragma mark -- Notifications --

/** Notify the crash reporter of GrowingCrash being added to Objective-C runtime system.
 */
void growingcrash_notifyObjCLoad(void);

/** Notify the crash reporter of the application active state.
 *
 * @param isActive true if the application is active, otherwise false.
 */
void growingcrash_notifyAppActive(bool isActive);

/** Notify the crash reporter of the application foreground/background state.
 *
 * @param isInForeground true if the application is in the foreground, false if
 *                 it is in the background.
 */
void growingcrash_notifyAppInForeground(bool isInForeground);

/** Notify the crash reporter that the application is terminating.
 */
void growingcrash_notifyAppTerminate(void);

/** Notify the crash reporter that the application has crashed.
 */
void growingcrash_notifyAppCrash(void);

    
#pragma mark -- Reporting --

/** Get the number of reports on disk.
 */
int growingcrash_getReportCount(void);

/** Get a list of IDs for all reports on disk.
 *
 * @param reportIDs An array big enough to hold all report IDs.
 * @param count How many reports the array can hold.
 *
 * @return The number of report IDs that were placed in the array.
 */
int growingcrash_getReportIDs(int64_t* reportIDs, int count);

/** Read a report.
 *
 * @param reportID The report's ID.
 *
 * @return The NULL terminated report, or NULL if not found.
 *         MEMORY MANAGEMENT WARNING: User is responsible for calling free() on the returned value.
 */
char* growingcrash_readReport(int64_t reportID);

/** Add a custom report to the store.
 *
 * @param report The report's contents (must be JSON encoded).
 * @param reportLength The length of the report in bytes.
 *
 * @return the new report's ID.
 */
int64_t growingcrash_addUserReport(const char* report, int reportLength);

/** Delete all reports on disk.
 */
void growingcrash_deleteAllReports(void);

/** Delete report.
 *
 * @param reportID An ID of report to delete.
 */
void growingcrash_deleteReportWithID(int64_t reportID);


#ifdef __cplusplus
}
#endif

#endif // HDR_GrowingCrashC_h
