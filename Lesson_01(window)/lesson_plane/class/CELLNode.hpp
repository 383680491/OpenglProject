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
        CELL::quatr         _quat;  // ����Ԫ�ر�ʾ��ת�Ⱦ���죬 �����Ԫ�ؿ��Ի�ԭΪ����
        CELL::float3        _scale;
        CELL::float3        _pos;
        CELL::float2        _size;
        CELL::matrix4       _local; //����
        CELL::aabbr         _aabb;  //��Χ��
        unsigned            _flag;  //���32������
    public:
        CELLNode(void* user = 0)
        {
            _pos            =   CELL::float3(0,0,0);
            _scale          =   CELL::float3(1,1,1);
            _quat           =   CELL::angleAxis(float(0),float3(0,0,1));  //��ת
            _local          =   CELL::makeTransform(float3(0,0,0),_scale,_quat); //ͨ�����������γɱ��ؾ���
            setSize(CELL::float2(8,8));
            _flag           =   FLAG_UPDATE;
        }
        virtual ~CELLNode()
        {
        }
        /**
        *   ����λ��
        */
        void    setPosition(const float3& pos)
        {
            _pos    =   pos;
            _flag   |=  FLAG_UPDATE;
        }
        /**
        *   ��ȡλ��
        */
        const float3&    getPosition() const
        {
            return  _pos;
        }
        /**
        *   �������ű���
        */
        void    setScale(const float3& scale)
        {
            _scale  =   scale;
            _flag   |=  FLAG_UPDATE;
        }
        /**
        *   ��ȡ���ű���
        */
        const float3&    getScale() const
        {
            return  _scale;
        }
        /**
        *   ������ת�Ƕ�
        */
        void    setQuat(const quatr& quat)
        {
            _quat   =   quat;
            _flag   |=  FLAG_UPDATE;
        }
        /**
        *   ��ȡ��ת����
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
            _aabb.setExtents(minPos,maxPos); //���ð�Χ�� ���ĵ�һ����(0,0)ʹ�������Χ�� ����ת��ʱ���Χ��ʼ�պ�����ϵƽ��
            _aabb.transform(_local);         //λ����ͨ������������
            _flag   |=  FLAG_UPDATE;
        }
        CELL::float2    getSize() const
        {
            return  _size;
        }
       
        /**
        *   ��ȡ�ڵ�ľֲ�����
        */
        const matrix4&  getMatrix() const
        {
            return  _local;
        }
        /**
        *   ���ø��±�־Ϊ
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
        *   �����Ƿ�ɼ�
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
        *   �Ƿ�ɼ�
        */
        bool    getVisible() const
        {
            return  (_flag & FLAG_VISIBLE) ? true : false;
        }
        /**
        *   ���ñ�־Ϊ
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
        *   ��ȡ��־λ
        */
        unsigned    getFlag() const
        {
            return  _flag;
        }
        /**
        *   ���¾���
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