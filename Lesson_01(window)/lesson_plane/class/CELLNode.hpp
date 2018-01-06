#pragma once

#include "class/CELLMath.hpp"

namespace   CELL
{
    class   CELLNode
    {
    public:
        enum    FLAG
        {
            FLAG_UPDATE     =   1<<0,
            FLAG_VISIBLE    =   1<<1,
        };
    public:
        CELL::quatr         _quat;  // 用四元素表示旋转比矩阵快， 最后四元素可以还原为矩阵
        CELL::float3        _scale;
        CELL::float3        _pos;
        CELL::float2        _size;
        CELL::matrix4       _local; //矩阵
        CELL::aabbr         _aabb;  //包围盒
        unsigned            _flag;  //最多32个属性
    public:
        CELLNode(void* user = 0)
        {
            _pos            =   CELL::float3(0,0,0);
            _scale          =   CELL::float3(1,1,1);
            _quat           =   CELL::angleAxis(float(0),float3(0,0,1));  //旋转
            _local          =   CELL::makeTransform(float3(0,0,0),_scale,_quat); //通过三个参数形成本地矩阵
            setSize(CELL::float2(8,8));
            _flag           =   FLAG_UPDATE;
        }
        virtual ~CELLNode()
        {
        }
        /**
        *   设置位置
        */
        void    setPosition(const float3& pos)
        {
            _pos    =   pos;
            _flag   |=  FLAG_UPDATE;
        }
        /**
        *   获取位置
        */
        const float3&    getPosition() const
        {
            return  _pos;
        }
        /**
        *   设置缩放比例
        */
        void    setScale(const float3& scale)
        {
            _scale  =   scale;
            _flag   |=  FLAG_UPDATE;
        }
        /**
        *   获取缩放比例
        */
        const float3&    getScale() const
        {
            return  _scale;
        }
        /**
        *   设置旋转角度
        */
        void    setQuat(const quatr& quat)
        {
            _quat   =   quat;
            _flag   |=  FLAG_UPDATE;
        }
        /**
        *   获取旋转数据
        */
        const quatr&getQuat() const 
        {
            return  _quat;
        }


        const aabbr&getAABB() const 
        {
            return  _aabb;
        }
        void    setSize(const CELL::float2& size)
        {
            _size   =   size;
            CELL::float3    minPos  =   -CELL::float3(size.x * 0.5f,size.y * 0.5f,0);
            CELL::float3    maxPos  =   CELL::float3(size.x * 0.5f,size.y * 0.5f,0);
            _aabb.setExtents(minPos,maxPos); //设置包围盒 中心点一定是(0,0)使用轴向包围盒 即旋转的时候包围盒始终和坐标系平行
            _aabb.transform(_local);         //位移是通过矩阵来做的
            _flag   |=  FLAG_UPDATE;
        }
        CELL::float2    getSize() const
        {
            return  _size;
        }
       
        /**
        *   获取节点的局部矩阵
        */
        const matrix4&  getMatrix() const
        {
            return  _local;
        }
        /**
        *   设置更新标志为
        */
        void    setUpdate(bool flag = true)
        {
            if (flag)
            {
                _flag   |=  FLAG_UPDATE;
            }
            else
            {
                _flag   &=  ~FLAG_UPDATE;
            }
            
        }
        bool    needUpdate()
        {
            return  _flag & FLAG_UPDATE;
        }
        /**
        *   设置是否可见
        */
        void    setVisible(bool flag)
        {
            if (flag)
            {
                _flag   |=  FLAG_VISIBLE;
            }
            else
            {
                _flag   &=  ~FLAG_VISIBLE;
            }
        }
        /**
        *   是否可见
        */
        bool    getVisible() const
        {
            return  (_flag & FLAG_VISIBLE) ? true : false;
        }
        /**
        *   设置标志为
        */
        void    setFlag(unsigned flag,bool merge = true)
        {
            if (merge)
            {
                _flag   |=  flag;
            }
            else
            {
                _flag   =   flag;
            }
        }
        /**
        *   获取标志位
        */
        unsigned    getFlag() const
        {
            return  _flag;
        }
        /**
        *   更新矩阵
        */
        void        update(float elasped,bool force)
        {
            if (force)
            {
                _local  =   makeTransform(_pos,_scale,_quat);
                CELL::float3    minPos  =   -CELL::float3(_size.x * 0.5f,_size.y * 0.5f,0);
                CELL::float3    maxPos  =   CELL::float3(_size.x * 0.5f,_size.y * 0.5f,0);
                _aabb.setExtents(minPos,maxPos);
                _aabb.transform(_local);
            }
            else if (_flag & FLAG_UPDATE)
            {
                _local  =   makeTransform(_pos,_scale,_quat);
                CELL::float3    minPos  =   -CELL::float3(_size.x * 0.5f,_size.y * 0.5f,0);
                CELL::float3    maxPos  =   CELL::float3(_size.x * 0.5f,_size.y * 0.5f,0);
                _aabb.setExtents(minPos,maxPos);
                _aabb.transform(_local);
            }
        }
       
    };
}