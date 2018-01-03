#pragma once

#include    "class/CELLTimerMgr.hpp"
#include    "class/CELLDelegate.hpp"
namespace   CELL
{
    class   CELLTimer 
        :public CELLTimerInterface
        ,public Delegate2<int,CELLTimer*,float>
    {
        CELLTimerMgr&   _timerMgr;
    public:
        CELLTimer(CELLTimerMgr& timerMgr,float timer = 1,void* user = 0) 
            :_timerMgr(timerMgr)
            ,_timer(timer)
            ,_elapsed(0)
        {
            _timerMgr.registerTimer(this);
        }
        virtual ~CELLTimer()
        {
            _timerMgr.unRegisterTimer(this);
        }

        virtual void    update(float elapsed)
        {
            if (_delegate == 0)
            {
                return;
            }
            if (_elapsed >= _timer)
            {
                _delegate->invoke( this, _elapsed );
                _elapsed    =   0;
            }
            _elapsed    +=  elapsed;
        }
        virtual void   setTimer(float timer)
        {
            _timer  =   timer;
        }

        virtual float   getTimer() const
        {
            return  _timer;
        }
    protected:
        float   _timer;
        float   _elapsed;
        
    };
}