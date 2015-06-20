#pragma once;



//const double to_ms_factor = 1000.0 / CLOCKS_PER_SEC;//win32上获取到的是毫秒，linux上面是微秒,用这个因子将时钟转换为毫秒单位

//获取程序运行了多少秒
double getApplicationSeconds();
//获取程序运行了多少毫秒
double getApplicationMilliSeconds();