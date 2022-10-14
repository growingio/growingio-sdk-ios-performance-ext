//  Created by Karl Stenerud on 2012-02-18.
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
//  GrowingCrashReportFilter.h
//  GrowingAnalytics
//
//  Created by YoloMao on 2022/9/23.
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

#import <Foundation/Foundation.h>

/** Callback for filter operations.
 *
 * @param filteredReports The filtered reports (may be incomplete if "completed"
 *                        is false).
 * @param completed True if filtering completed.
 *                  Can be false due to a non-erroneous condition (such as a
 *                  user cancelling the operation).
 * @param error Non-nil if an error occurred.
 */
typedef void(^GrowingCrashReportFilterCompletion)(NSArray* filteredReports, BOOL completed, NSError* error);


/**
 * A filter receives a set of reports, possibly transforms them, and then
 * calls a completion method.
 */
@protocol GrowingCrashReportFilter <NSObject>

/** Filter the specified reports.
 *
 * @param reports The reports to process.
 * @param onCompletion Block to call when processing is complete.
 */
- (void) filterReports:(NSArray*) reports
          onCompletion:(GrowingCrashReportFilterCompletion) onCompletion;

@end


/** Conditionally call a completion method if it's not nil.
 *
 * @param onCompletion The completion block. If nil, this function does nothing.
 * @param filteredReports The parameter to send as "filteredReports".
 * @param completed The parameter to send as "completed".
 * @param error The parameter to send as "error".
 */
static inline void growingcrash_callCompletion(GrowingCrashReportFilterCompletion onCompletion,
                                            NSArray* filteredReports,
                                            BOOL completed,
                                            NSError* error)
{
    if(onCompletion)
    {
        onCompletion(filteredReports, completed, error);
    }
}