GrowingIO APM
======
![GrowingIO](https://www.growingio.com/vassets/images/home_v3/gio-logo-primary.svg) 

### SDK 简介

**GrowingAPM** 提供移动端性能采集分析功能，包括崩溃分析、启动分析、页面加载分析等。

### 如何集成

- 集成默认监控模块

```ruby
pod 'GrowingAPM'
```

- 或者，按需集成对应模块

```ruby
# 按照所需模块自由组合
pod 'GrowingAPM/UIMonitor'
pod 'GrowingAPM/CrashMonitor'
# ...
```

### 初始化SDK

- 在 main.m 以及 AppDelegate.m 导入 GrowingAPM：

```objc
#import "GrowingAPM.h"
```

- 在 main 函数中，添加如下代码 (此操作仅为了尽可能地在 App 运行前 Swizzle，除必要的崩溃日志本地缓存外，不会生成、上报数据)：

```objc
int main(int argc, char * argv[]) {
    // GrowingAPM Swizzle
    [GrowingAPM swizzle:GrowingAPMMonitorsCrash | GrowingAPMMonitorsUserInterface];
    NSString * appDelegateClassName;
    @autoreleasepool {
        // Setup code that might create autoreleased objects goes here.
        appDelegateClassName = NSStringFromClass([AppDelegate class]);
    }
    return UIApplicationMain(argc, argv, nil, appDelegateClassName);
}
```

- 在 AppDelegate.m application:didFinishLaunchingWithOptions: 中初始化 GrowingAPM：

```objc
// 添加 GrowingAPM 初始化配置
GrowingAPMConfig *config = GrowingAPMConfig.config;
// 根据您需要的监控类型
config.monitors = GrowingAPMMonitorsCrash | GrowingAPMMonitorsUserInterface;
// 初始化 GrowingAPM
[GrowingAPM startWithConfig:config];
```

