#pragma once
#include <assert.h>
#include <new>          // For placement new
namespace   CELL
{
    /**
    *   构件该类的目的是不需要再次的使用堆内存
    *   高效率，弱点：占用更多的内存
    */

    //////////////////////////////////////////////////////////////////////////
    //! one argument
    //////////////////////////////////////////////////////////////////////////
    template<typename R,typename A>
    class IDelegate1
    {
    public:
        virtual R   invoke(A a) =   0;
    };

    /**
    *   类成员函数
    */
    template<typename T,typename R,typename A>
    class IDelegate1Imp :public IDelegate1<R,A>
    {
    public:
        typedef R   (T::*Method)(A);

        IDelegate1Imp(T* object,Method method)
        {
            _object =   object;
            _method =   method;
        }
        virtual R   invoke(A a)
        {
            return  (_object->*_method)(a);
        }
    protected:
        T*      _object;
        Method  _method;
    };
    /**
    *   全局函数,静态函数
    */
    template<typename R,typename A>
    class IDelegate1ImpStatic :public IDelegate1<R,A>
    {
    public:
        typedef R   (*Method)(A);

        IDelegate1ImpStatic(Method method)
        {
            _method =   method;
        }
        virtual R   invoke(A a)
        {
            return  (*_method)(a);
        }
    protected:
        Method  _method;
    };

    /**
    *   使用该类完成对象的声明使用
    */
    template<typename R,typename A>
    class Delegate1
    {
    public:
        Delegate1()
        {
            memset(this,0,sizeof(Delegate1));
        }
        Delegate1(const Delegate1& right)
        {
            memcpy(_realObject,right._realObject,sizeof(_realObject));
            if (right._delegate)
            {
                _delegate   =   (IDelegate1<R,A>*)(_realObject);
            }
            else
            {
                _delegate   =   0;
            }
        }
        Delegate1&  operator = (const Delegate1& right)
        {
            memcpy(_realObject,right._realObject,sizeof(_realObject));
            if (right._delegate)
            {
                _delegate   =   (IDelegate1<R,A>*)(_realObject);
            }
            return  *this;
        }
        
        /**
        *   通过参数设置代理函数
        */
        template<typename T>
        void    bind(T* object, R (T::*method)( A ) )
        {
            if (object != 0 && method != 0)
            {
                /**
                *   在给定的内存上进行构造，而不是使用堆内存
                */
                assert(sizeof(IDelegate1Imp<T,R,A>(object, method)) <= sizeof(_realObject));
                _delegate   =   new(_realObject) IDelegate1Imp<T,R,A>(object, method);
            }
            else
            {
                _delegate   =   0;
            }
        }
        /**
        *   通过参数设置代理函数
        */
        void    bind(R (*method)( A ) )
        {
            if (method != 0)
            {
                /**
                *   在给定的内存上进行构造，而不是使用堆内存
                */
                _delegate   =   new(_realObject) IDelegate1ImpStatic<R,A>(method);
            }
            else
            {
                _delegate   =   0;
            }
        }
        /**
        *   调用代理函数
        */
        R operator()( A a )
        {
            if (_delegate)
            {
                return  _delegate->invoke( a );
            }
            else
            {
                return  R(0);
            }
            
        }
    protected:
        IDelegate1<R,A>*    _delegate;

        char                _realObject[32];
    };

















    //////////////////////////////////////////////////////////////////////////
    //! 2 argument
    //////////////////////////////////////////////////////////////////////////
	/*依然是代理模式  IDelegate2是虚基类接口     IDelegate2Imp 是接口实现     Delegate2是几个参数对应不同的类*/
    template<typename R,typename A1,typename A2>
    class IDelegate2  
    {
    public:
        virtual R   invoke(A1, A2) =   0;
    };
    

	/*接口实现*/
    template<typename T,typename R,typename A1,typename A2>
    class IDelegate2Imp :public IDelegate2<R,A1,A2>     
    {
    public:
        typedef R   (T::*Method)(A1,A2);

        IDelegate2Imp(T* object,Method method)
        {
            _object =   object;
            _method =   method;
        }
        virtual R   invoke(A1 a1,A2 a2)
        {
            return  (_object->*_method)(a1,a2);
        }
    protected:
        T*      _object;
        Method  _method;
    };



    /**
    *   使用该类完成对象的声明使用  名字很容易混淆  前面I字母是接口   imp是实现
    */
    template<typename R,typename A1,typename A2>
    class Delegate2
    {
        typedef IDelegate2<R,A1,A2>     TIDelegate2;  
    public:
        Delegate2()
        {
            memset(this,0,sizeof(Delegate2));
        }
        Delegate2(const Delegate2& right)
        {
            memcpy(_realObject,right._realObject,sizeof(_realObject));
            if (right._delegate)
            {
                _delegate   =   (TIDelegate2*)(_realObject);
            }
            else
            {
                _delegate   =   0;
            }
        }
        Delegate2&  operator= (const Delegate2& right)
        {
            memcpy(_realObject,right._realObject,sizeof(_realObject));
            if (right._delegate)
            {
                _delegate   =   (TIDelegate2*)(_realObject);
            }
            return  *this;
        }
        /**
        *   通过参数设置代理函数
        */
        template<typename T>
        void    bind(T* object, R (T::*method)( A1,A2 ) )
        {
            if (object != 0 && method != 0)
            {
                /**
                *   在给定的内存上进行构造，而不是使用堆内存
                */
                assert(sizeof(IDelegate2Imp<T,R,A1,A2>(object, method)) <= sizeof(_realObject));
                _delegate   =   new(_realObject) IDelegate2Imp<T,R,A1,A2>(object, method);
            }
            else
            {
                _delegate   =   0;
            }
        }
        /**
        *   调用代理函数
        */
        R operator()( A1 a1,A2 a2)
        {
            if (_delegate)
            {
                return  _delegate->invoke( a1,a2 );
            }
            else
            {
                return  R(0);
            }
            
        }
    protected:
        TIDelegate2*_delegate;
        char        _realObject[32];
    };















    //////////////////////////////////////////////////////////////////////////
    //! 3 argument
    //////////////////////////////////////////////////////////////////////////
    template<typename R,typename A1,typename A2,typename A3>
    class IDelegate3
    {
    public:
        virtual R   invoke(A1,A2,A3) =   0;
    };

    template<typename T,typename R,typename A1,typename A2,typename A3>
    class IDelegate3Imp :public IDelegate3<R,A1,A2,A3>
    {
    public:
        typedef R   (T::*Method)(A1 a1,A2 a2,A3 a3);

        IDelegate3Imp(T* object,Method method)
        {
            _object =   object;
            _method =   method;
        }
        virtual R   invoke(A1 a1,A2 a2,A3 a3)
        {
            return  (_object->*_method)(a1,a2,a3);
        }
    protected:
        T*      _object;
        Method  _method;
    };

    /**
    *   使用该类完成对象的声明使用
    */
    template<typename R,typename A1,typename A2,typename A3>
    class Delegate3
    {
        typedef IDelegate3<R,A1,A2,A3>     TIDelegate3;
    public:
        Delegate3()
        {
            memset(this,0,sizeof(Delegate3));
        }
        Delegate3(const Delegate3& right)
        {
            memcpy(_realObject,right._realObject,sizeof(_realObject));
            if (right._delegate)
            {
                _delegate   =   (TIDelegate3*)(_realObject);
            }
            else
            {
                _delegate   =   0;
            }
        }
        Delegate3&  operator= (const Delegate3& right)
        {
            memcpy(_realObject,right._realObject,sizeof(_realObject));
            if (right._delegate)
            {
                _delegate   =   (TIDelegate3*)(_realObject);
            }
            return  *this;
        }

        /**
        *   通过参数设置代理函数
        */
        template<typename T>
        void    bind(T* object, R (T::*method)( A1,A2,A3 ) )
        {
            if (object != 0 && method != 0)
            {
                /**
                *   在给定的内存上进行构造，而不是使用堆内存
                */
                assert(sizeof(IDelegate3Imp<T,R,A1,A2,A3>(object, method)) <= sizeof(_realObject));
                _delegate   =   new(_realObject) IDelegate3Imp<T,R,A1,A2,A3>(object, method);
            }
            else
            {
                _delegate   =   0;
            }
        }
        /**
        *   调用代理函数
        */
        R operator()( A1 a1,A2 a2,A3 a3 )
        {
            if (_delegate)
            {
                return  _delegate->invoke( a1,a2,a3 );
            }
            else
            {
                return  R(0);
            }
            
        }
    private:
        TIDelegate3*_delegate;
        char        _realObject[32];
    };



    //////////////////////////////////////////////////////////////////////////
    //! 4 argument
    //////////////////////////////////////////////////////////////////////////
    template<typename R,typename A1,typename A2,typename A3,typename A4>
    class IDelegate4
    {
    public:
        virtual R   invoke(A1,A2,A3,A4) =   0;
    };

    template<typename T,typename R,typename A1,typename A2,typename A3,typename A4>
    class IDelegate4Imp :public IDelegate4<R,A1,A2,A3,A4>
    {
    public:
        typedef R   (T::*Method)(A1 a1,A2 a2,A3 a3,A4 a4);

        IDelegate4Imp(T* object,Method method)
        {
            _object =   object;
            _method =   method;
        }
        virtual R   invoke(A1 a1,A2 a2,A3 a3,A4 a4)
        {
            return  (_object->*_method)(a1,a2,a3,a4);
        }
    protected:
        T*      _object;
        Method  _method;
    };

    /**
    *   使用该类完成对象的声明使用
    */
    template<typename R,typename A1,typename A2,typename A3,typename A4>
    class Delegate4 
    {
        typedef IDelegate4<R,A1,A2,A3,A4>     TIDelegate4;
    public:
        Delegate4()
        {
            memset(this,0,sizeof(Delegate4));
        }
        Delegate4(const Delegate4& right)
        {
            memcpy(_realObject,right._realObject,sizeof(_realObject));
            if (right._delegate)
            {
                _delegate   =   (TIDelegate4*)(_realObject);
            }
            else
            {
                _delegate   =   0;
            }
        }
        Delegate4&  operator= (const Delegate4& right)
        {
            memcpy(_realObject,right._realObject,sizeof(_realObject));
            if (right._delegate)
            {
                _delegate   =   (TIDelegate4*)(_realObject);
            }
            return  *this;
        }
        /**
        *   通过参数设置代理函数
        */
        template<typename T>
        void    bind(T* object, R (T::*method)( A1,A2,A3,A4 ) )
        {
            if (object != 0 && method != 0)
            {
                /**
                *   在给定的内存上进行构造，而不是使用堆内存
                */
                assert(sizeof(IDelegate4Imp<T,R,A1,A2,A3,A4>(object, method)) <= sizeof(_realObject));
                _delegate   =   new(_realObject) IDelegate4Imp<T,R,A1,A2,A3,A4>(object, method);
            }
            else
            {
                _delegate   =   0;
            }
        }
        /**
        *   调用代理函数
        */
        R operator()( A1 a1,A2 a2,A3 a3,A4 a4 )
        {
            return  _delegate->invoke( a1,a2,a3,a4);
        }
    private:
        TIDelegate4*_delegate;
        char        _realObject[32];
    };

    //////////////////////////////////////////////////////////////////////////
    //! 5 argument
    //////////////////////////////////////////////////////////////////////////
    template<typename R,typename A1,typename A2,typename A3,typename A4,typename A5>
    class IDelegate5
    {
    public:
        virtual R   invoke(A1,A2,A3,A4, A5) =   0;
    };

    template<typename T,typename R,typename A1,typename A2,typename A3,typename A4,typename A5>
    class IDelegate5Imp :public IDelegate5<R,A1,A2,A3,A4,A5>
    {
    public:
        typedef R   (T::*Method)(A1,A2,A3,A4,A5);

        IDelegate5Imp(T* object,Method method)
        {
            _object =   object;
            _method =   method;
        }
        virtual R   invoke(A1 a1,A2 a2,A3 a3,A4 a4,A5 a5)
        {
            return  (_object->*_method)(a1,a2,a3,a4,a5);
        }
    protected:
        T*      _object;
        Method  _method;
    };

    /**
    *   使用该类完成对象的声明使用
    */
    template<typename R,typename A1,typename A2,typename A3,typename A4, typename A5>
    class Delegate5
    {
        typedef IDelegate5<R,A1,A2,A3,A4,A5>     TIDelegate5;
    public:
        Delegate5()
        {
            memset(this,0,sizeof(Delegate5));
        }
        Delegate5(const Delegate5& right)
        {
            memcpy(_realObject,right._realObject,sizeof(_realObject));
            if (right._delegate)
            {
                _delegate   =   (TIDelegate5*)(_realObject);
            }
            else
            {
                _delegate   =   0;
            }
        }
        Delegate5&  operator= (const Delegate5& right)
        {
            memcpy(_realObject,right._realObject,sizeof(_realObject));
            if (right._delegate)
            {
                _delegate   =   (TIDelegate5*)(_realObject);
            }
            return  *this;
        }
        /**
        *   通过参数设置代理函数
        */
        template<typename T>
        void    bind(T* object, R (T::*method)( A1,A2,A3,A4,A5 ) )
        {
            if (object != 0 && method != 0)
            {
                /**
                *   在给定的内存上进行构造，而不是使用堆内存
                */
                assert(sizeof(IDelegate5Imp<T,R,A1,A2,A3,A4,A5>(object, method)) <= sizeof(_realObject));
                _delegate   =   new(_realObject) IDelegate5Imp<T,R,A1,A2,A3,A4,A5>(object, method);
            }
            else
            {
                _delegate   =   0;
            }
        }
        /**
        *   调用代理函数
        */
        R operator()( A1 a1,A2 a2,A3 a3,A4 a4,A5 a5)
        {
            return  _delegate->invoke( a1,a2,a3,a4,a5);
        }
    private:
        TIDelegate5*_delegate;
        char        _realObject[32];
    };

    //////////////////////////////////////////////////////////////////////////
    //! 6 argument
    //////////////////////////////////////////////////////////////////////////
    template<typename R,typename A1,typename A2,typename A3,typename A4,typename A5,typename A6>
    class IDelegate6
    {
    public:
        virtual R   invoke(A1,A2,A3,A4,A5,A6) =   0;
    };

    template<typename T,typename R,typename A1,typename A2,typename A3,typename A4,typename A5,typename A6>
    class IDelegate6Imp :public IDelegate6<R,A1,A2,A3,A4,A5,A6>
    {
    public:
        typedef R   (T::*Method)(A1,A2,A3,A4,A5,A6);

        IDelegate6Imp(T* object,Method method)
        {
            _object =   object;
            _method =   method;
        }
        virtual R   invoke(A1 a1,A2 a2,A3 a3,A4 a4,A5 a5,A6 a6)
        {
            return  (_object->*_method)(a1,a2,a3,a4,a5,a6);
        }
    protected:
        T*      _object;
        Method  _method;
    };

    /**
    *   使用该类完成对象的声明使用
    */
    template<typename R,typename A1,typename A2,typename A3,typename A4, typename A5,typename A6>
    class Delegate6
    {
        typedef IDelegate6<R,A1,A2,A3,A4,A5,A6>     TIDelegate6;
    public:
        Delegate6()
        {
            memset(this,0,sizeof(Delegate6));
        }
        Delegate6(const Delegate6& right)
        {
            memcpy(_realObject,right._realObject,sizeof(_realObject));
            if (right._delegate)
            {
                _delegate   =   (TIDelegate6*)(_realObject);
            }
            else
            {
                _delegate   =   0;
            }
        }
        Delegate6&  operator= (const Delegate6& right)
        {
            memcpy(_realObject,right._realObject,sizeof(_realObject));
            if (right._delegate)
            {
                _delegate   =   (TIDelegate6*)(_realObject);
            }
            return  *this;
        }

        /**
        *   通过参数设置代理函数
        */
        template<typename T>
        void    bind(T* object, R (T::*method)( A1,A2,A3,A4,A5,A6 ) )
        {
            if (object != 0 && method != 0)
            {
                /**
                *   在给定的内存上进行构造，而不是使用堆内存
                */
                assert(sizeof(IDelegate6Imp<T,R,A1,A2,A3,A4,A5,A6>(object, method)) <= sizeof(_realObject));
                _delegate   =   new(_realObject) IDelegate6Imp<T,R,A1,A2,A3,A4,A5,A6>(object, method);
            }
            else
            {
                _delegate   =   0;
            }
        }
        /**
        *   调用代理函数
        */
        R operator()( A1 a1,A2 a2,A3 a3,A4 a4,A5 a5,A6 a6)
        {
            return  _delegate->invoke( a1,a2,a3,a4,a5,a6);
        }
    private:
        TIDelegate6*_delegate;
        char        _realObject[32];
    };

    //////////////////////////////////////////////////////////////////////////
    //! 7 argument
    //////////////////////////////////////////////////////////////////////////
    template<typename R,typename A1,typename A2,typename A3,typename A4,typename A5,typename A6,typename A7>
    class IDelegate7
    {
    public:
        virtual R   invoke(A1,A2,A3,A4,A5,A6,A7) =   0;
    };

    template<typename T,typename R,typename A1,typename A2,typename A3,typename A4,typename A5,typename A6,typename A7>
    class IDelegate7Imp :public IDelegate7<R,A1,A2,A3,A4,A5,A6,A7>
    {
    public:
        typedef R   (T::*Method)(A1,A2,A3,A4,A5,A6,A7);

        IDelegate7Imp(T* object,Method method)
        {
            _object =   object;
            _method =   method;
        }
        virtual R   invoke(A1 a1,A2 a2,A3 a3,A4 a4,A5 a5,A6 a6,A7 a7)
        {
            return  (_object->*_method)(a1,a2,a3,a4,a5,a6,a7);
        }
    protected:
        T*      _object;
        Method  _method;
    };

    /**
    *   使用该类完成对象的声明使用
    */
    template<typename R,typename A1,typename A2,typename A3,typename A4, typename A5,typename A6,typename A7>
    class Delegate7
    {
        typedef IDelegate7<R,A1,A2,A3,A4,A5,A6,A7>     TIDelegate7;
    public:
        Delegate7()
        {
            memset(this,0,sizeof(Delegate7));
        }
        Delegate7(const Delegate7& right)
        {
            memcpy(_realObject,right._realObject,sizeof(_realObject));
            if (right._delegate)
            {
                _delegate   =   (TIDelegate7*)(_realObject);
            }
            else
            {
                _delegate   =   0;
            }
        }
        Delegate7&  operator= (const Delegate7& right)
        {
            memcpy(_realObject,right._realObject,sizeof(_realObject));
            if (right._delegate)
            {
                _delegate   =   (TIDelegate7*)(_realObject);
            }
            return  *this;
        }
        /**
        *   通过参数设置代理函数
        */
        template<typename T>
        void    bind(T* object, R (T::*method)( A1,A2,A3,A4,A5,A6,A7 ) )
        {
            if (object != 0 && method != 0)
            {
                /**
                *   在给定的内存上进行构造，而不是使用堆内存
                */
                assert(sizeof(IDelegate7Imp<T,R,A1,A2,A3,A4,A5,A6,A7>(object, method)) <= sizeof(_realObject));
                _delegate   =   new(_realObject) IDelegate7Imp<T,R,A1,A2,A3,A4,A5,A6,A7>(object, method);
            }
            else
            {
                _delegate   =   0;
            }
        }
        /**
        *   调用代理函数
        */
        R operator()( A1 a1,A2 a2,A3 a3,A4 a4,A5 a5,A6 a6,A7 a7)
        {
            return  _delegate->invoke( a1,a2,a3,a4,a5,a6,a7);
        }
    private:
        TIDelegate7*_delegate;
        char        _realObject[32];
    };

    //////////////////////////////////////////////////////////////////////////
    //! 8 argument
    //////////////////////////////////////////////////////////////////////////
    template<typename R,typename A1,typename A2,typename A3,typename A4,typename A5,typename A6,typename A7,typename A8>
    class IDelegate8
    {
    public:
        virtual R   invoke(A1,A2,A3,A4,A5,A6,A7,A8) =   0;
    };

    template<typename T,typename R,typename A1,typename A2,typename A3,typename A4,typename A5,typename A6,typename A7,typename A8>
    class IDelegate8Imp :public IDelegate8<R,A1,A2,A3,A4,A5,A6,A7,A8>
    {
    public:
        typedef R   (T::*Method)(A1,A2,A3,A4,A5,A6,A7,A8);

        IDelegate8Imp(T* object,Method method)
        {
            _object =   object;
            _method =   method;
        }
        virtual R   invoke(A1 a1,A2 a2,A3 a3,A4 a4,A5 a5,A6 a6,A7 a7,A8 a8)
        {
            return  (_object->*_method)(a1,a2,a3,a4,a5,a6,a7,a8);
        }
    protected:
        T*      _object;
        Method  _method;
    };

    /**
    *   使用该类完成对象的声明使用
    */
    template<typename R,typename A1,typename A2,typename A3,typename A4, typename A5,typename A6,typename A7,typename A8>
    class Delegate8
    {
        typedef IDelegate8<R,A1,A2,A3,A4,A5,A6,A7,A8>     TIDelegate8;
    public:
        Delegate8()
        {
            memset(this,0,sizeof(Delegate8));
        }
       
        Delegate8(const Delegate8& right)
        {
            memcpy(_realObject,right._realObject,sizeof(_realObject));
            if (right._delegate)
            {
                _delegate   =   (TIDelegate8*)(_realObject);
            }
            else
            {
                _delegate   =   0;
            }
        }
        Delegate8&  operator= (const Delegate8& right)
        {
            memcpy(_realObject,right._realObject,sizeof(_realObject));
            if (right._delegate)
            {
                _delegate   =   (TIDelegate8*)(_realObject);
            }
            return  *this;
        }
        /**
        *   通过参数设置代理函数
        */
        template<typename T>
        void    bind(T* object, R (T::*method)( A1,A2,A3,A4,A5,A6,A7,A8 ) )
        {
            if (object != 0 && method != 0)
            {
                /**
                *   在给定的内存上进行构造，而不是使用堆内存
                */
                assert(sizeof(IDelegate8Imp<T,R,A1,A2,A3,A4,A5,A6,A7,A8>(object, method)) <= sizeof(_realObject));
                _delegate   =   new(_realObject) IDelegate8Imp<T,R,A1,A2,A3,A4,A5,A6,A7,A8>(object, method);
            }
            else
            {
                _delegate   =   0;
            }
        }
        /**
        *   调用代理函数
        */
        R operator()( A1 a1,A2 a2,A3 a3,A4 a4,A5 a5,A6 a6,A7 a7,A8 a8)
        {
            return  _delegate->invoke( a1,a2,a3,a4,a5,a6,a7,a8);
        }
    private:
        TIDelegate8*_delegate;
        char        _realObject[32];
    };

    //////////////////////////////////////////////////////////////////////////
    //! 9 argument
    //////////////////////////////////////////////////////////////////////////
    template<typename R,typename A1,typename A2,typename A3,typename A4,typename A5,typename A6,typename A7,typename A8,typename A9>
    class IDelegate9
    {
    public:
        virtual R   invoke(A1,A2,A3,A4,A5,A6,A7,A8,A9) =   0;
    };

    template<typename T,typename R,typename A1,typename A2,typename A3,typename A4,typename A5,typename A6,typename A7,typename A8,typename A9>
    class IDelegate9Imp :public IDelegate9<R,A1,A2,A3,A4,A5,A6,A7,A8,A9>
    {
    public:
        typedef R   (T::*Method)(A1,A2,A3,A4,A5,A6,A7,A8,A9);

        IDelegate9Imp(T* object,Method method)
        {
            _object =   object;
            _method =   method;
        }
        virtual R   invoke(A1 a1,A2 a2,A3 a3,A4 a4,A5 a5,A6 a6,A7 a7,A8 a8,A9 a9)
        {
            return  (_object->*_method)(a1,a2,a3,a4,a5,a6,a7,a8,a9);
        }
    protected:
        T*      _object;
        Method  _method;
    };

    /**
    *   使用该类完成对象的声明使用
    */
    template<typename R,typename A1,typename A2,typename A3,typename A4, typename A5,typename A6,typename A7,typename A8,typename A9>
    class Delegate9
    {
        typedef IDelegate9<R,A1,A2,A3,A4,A5,A6,A7,A8,A9>     TIDelegate9;
    public:
        Delegate9()
        {
            memset(this,0,sizeof(Delegate9));
        }
        Delegate9(const Delegate9& right)
        {
            memcpy(_realObject,right._realObject,sizeof(_realObject));
            if (right._delegate)
            {
                _delegate   =   (TIDelegate9*)(_realObject);
            }
            else
            {
                _delegate   =   0;
            }
        }
        Delegate9&  operator= (const Delegate9& right)
        {
            memcpy(_realObject,right._realObject,sizeof(_realObject));
            if (right._delegate)
            {
                _delegate   =   (TIDelegate9*)(_realObject);
            }
            return  *this;
        }
        /**
        *   通过参数设置代理函数
        */
        template<typename T>
        void    bind(T* object, R (T::*method)( A1,A2,A3,A4,A5,A6,A7,A8,A9 ) )
        {
            if (object != 0 && method != 0)
            {
                /**
                *   在给定的内存上进行构造，而不是使用堆内存
                */
                assert(sizeof(IDelegate9Imp<T,R,A1,A2,A3,A4,A5,A6,A7,A8,A9>(object, method)) <= sizeof(_realObject));
                _delegate   =   new(_realObject) IDelegate9Imp<T,R,A1,A2,A3,A4,A5,A6,A7,A8,A9>(object, method);
            }
            else
            {
                _delegate   =   0;
            }
        }
        /**
        *   调用代理函数
        */
        R operator()( A1 a1,A2 a2,A3 a3,A4 a4,A5 a5,A6 a6,A7 a7,A8 a8,A9 a9)
        {
            return  _delegate->invoke( a1,a2,a3,a4,a5,a6,a7,a8,a9);
        }
    private:
        TIDelegate9*_delegate;
        char        _realObject[32];
    };
}