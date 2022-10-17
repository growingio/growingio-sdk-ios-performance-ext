// swift-tools-version:5.3
// The swift-tools-version declares the minimum version of Swift required to build this package.

//
//  Package.swift
//  GrowingAnalytics
//
//  Created by YoloMao on 2022/10/17.
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

import PackageDescription

let package = Package(
    name: "GrowingAPM",
    platforms: [.iOS(.v9)],
    products: [
        .library(
            name: "GrowingAPM",
            targets: ["GrowingAPM_Wrapper"]
        ),
        .library(
            name: "GrowingAPMCrashMonitor",
            targets: ["GrowingAPMCrashMonitor_Wrapper"]
        ),
        .library(
            name: "GrowingAPMUIMonitor",
            targets: ["GrowingAPMUIMonitor_Wrapper"]
        ),
        .library(
            name: "GrowingAPMLaunchMonitor",
            targets: ["GrowingAPMLaunchMonitor_Wrapper"]
        ),
    ],
    dependencies: [
        .package(
            name: "GrowingUtils",
            url: "https://github.com/growingio/growingio-sdk-ios-utilities.git",
            .revision("af1ec1304d71a5a7467616cafa5fad79ca8e01bc")
        ),
    ],
    targets: [
        
        // MARK: - GrowingAPM Wrapper
        
        .target(
            name: "GrowingAPM_Wrapper",
            dependencies: [
                "GrowingAPMCore",
                "GrowingAPMCrashMonitor",
                "GrowingAPMUIMonitor",
                "GrowingAPMLaunchMonitor"
            ],
            path: "SwiftPM-Wrap/GrowingAPM_Wrapper"
        ),
        .target(
            name: "GrowingAPMCrashMonitor_Wrapper",
            dependencies: [
                "GrowingAPMCore",
                "GrowingAPMCrashMonitor"
            ],
            path: "SwiftPM-Wrap/GrowingAPMCrashMonitor_Wrapper",
            cxxSettings: [
                .define("GCC_ENABLE_CPP_EXCEPTIONS", to: "YES"),
            ],
            linkerSettings: [
                .linkedLibrary("c++"),
                .linkedLibrary("z"),
            ]
        ),
        .target(
            name: "GrowingAPMUIMonitor_Wrapper",
            dependencies: [
                "GrowingAPMCore",
                "GrowingAPMUIMonitor"
            ],
            path: "SwiftPM-Wrap/GrowingAPMUIMonitor_Wrapper"
        ),
        .target(
            name: "GrowingAPMLaunchMonitor_Wrapper",
            dependencies: [
                "GrowingAPMCore",
                "GrowingAPMLaunchMonitor"
            ],
            path: "SwiftPM-Wrap/GrowingAPMLaunchMonitor_Wrapper"
        ),
        
        // MARK: - GrowingAPM Core
        
        .target(
            name: "GrowingAPMCore",
            dependencies: [
                .product(name: "GrowingUtilsAutotrackerCore", package: "GrowingUtils"),
            ],
            path: "Core"
        ),
        
        // MARK: - GrowingAPM Modules
        
        .binaryTarget(
            name: "GrowingAPMCrashMonitor",
            path: "CrashMonitor/GrowingAPMCrashMonitor.xcframework"
        ),
        .binaryTarget(
            name: "GrowingAPMUIMonitor",
            path: "UIMonitor/GrowingAPMUIMonitor.xcframework"
        ),
        .binaryTarget(
            name: "GrowingAPMLaunchMonitor",
            path: "LaunchMonitor/GrowingAPMLaunchMonitor.xcframework"
        ),
    ]
)
