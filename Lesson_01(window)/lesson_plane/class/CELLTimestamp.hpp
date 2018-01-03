#pragma once

namespace   CELL
{
    class   CELLTimestamp
    {
    protected:
        LARGE_INTEGER   _start;
        LARGE_INTEGER   _frequency;
    public:
        CELLTimestamp()
        {
            ::QueryPerformanceFrequency(&_frequency);  //函数获得机器内部计时器的时钟频率
            ::QueryPerformanceCounter(&_start);  //每次获得次数
        }

        void    update()
        {
            ::QueryPerformanceCounter(&_start);
        }
        

        double   getElapsedSecond() const   //Elapse  消逝 时间过去
        {
            LARGE_INTEGER   cur;
            QueryPerformanceCounter(&cur);
			//可以这样子理解 当前次数减去上一次的次数，而_frequency频率就是一秒钟的次数  相除就得到时间了
            return  double(cur.QuadPart-_start.QuadPart)/double(_frequency.QuadPart);
        }

    };
}