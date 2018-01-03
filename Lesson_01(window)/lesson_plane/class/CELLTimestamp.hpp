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
            ::QueryPerformanceFrequency(&_frequency);  //������û����ڲ���ʱ����ʱ��Ƶ��
            ::QueryPerformanceCounter(&_start);  //ÿ�λ�ô���
        }

        void    update()
        {
            ::QueryPerformanceCounter(&_start);
        }
        

        double   getElapsedSecond() const   //Elapse  ���� ʱ���ȥ
        {
            LARGE_INTEGER   cur;
            QueryPerformanceCounter(&cur);
			//������������� ��ǰ������ȥ��һ�εĴ�������_frequencyƵ�ʾ���һ���ӵĴ���  ����͵õ�ʱ����
            return  double(cur.QuadPart-_start.QuadPart)/double(_frequency.QuadPart);
        }

    };
}