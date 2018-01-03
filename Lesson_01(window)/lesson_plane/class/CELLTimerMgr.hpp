#pragma once

#include    <vector>
#include    <algorithm>

namespace   CELL
{
    class   CELLTimerInterface
    {
    public:
        virtual void    update(float elapsed)   =   0;
    }; 

    class   CELLTimerMgr :public std::vector<CELLTimerInterface*>
    {
    public:
        CELLTimerMgr()
        {
            reserve(1024);
        }
        /**
        *   ¸üÐÂº¯Êý
        */
        void    update(float elapsed)
        {
            iterator    itr =   begin();
            for ( ; itr != end() ; ++ itr )
            {
                (*itr)->update(elapsed);
            }
        }

        void    registerTimer(CELLTimerInterface* timer)
        {
            iterator    itr =   std::find(begin(),end(),timer) ;
            if(itr== end())
            {
                push_back(timer);
            }
        }
        void    unRegisterTimer(CELLTimerInterface* timer)
        {
            iterator    itr =   std::find(begin(),end(),timer);
            assert(itr != end());
            
            if (itr != end())
            {
                erase(itr);
            }
        }

    };
}