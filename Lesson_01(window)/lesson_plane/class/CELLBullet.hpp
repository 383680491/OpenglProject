#pragma once

#include "class/CELLNode.hpp"

namespace   CELL
{
    class   CELLBullet :public CELLNode
    {
    public:
        /// �ٶ�
        float       _attrSpeed;
        /// ���õ���Զ����
        float       _attrMaxDistance;
        /// ����    �뾶
        float       _attrPowerRadius;
        /// �˺�ֵ
        float       _attrHurt;
        /// ���з���
        float3      _dir;
        /// ��ʼ��λ��
        float3      _posSrc;
        /// ʹ�õ�����
        Texture2dId _texture;
        /// �Ƿ�����
        bool        _isDead;
    public:
        CELLBullet()
        {
            _attrSpeed          =   100;
            _attrMaxDistance    =   200;
            _attrPowerRadius    =   20;
            _attrHurt           =   10;
        }
        virtual ~CELLBullet()
        {}
        virtual bool    isDead()
        {
            return  distance(_posSrc,_pos) >= _attrMaxDistance;
        }

        virtual void    setPosition(const float3& pos)
        {
            _posSrc =   pos;
            CELLNode::setPosition(pos);
        }

        virtual void    setMaxDistance(float dist)
        {
            _attrMaxDistance    =   dist;
        }

        virtual void    setSpeed(float speeed)
        {
            _attrSpeed  =   speeed;
        }

        virtual void    setDir(const float3& dir)
        {
            _dir    =   dir;
        }

        virtual void    update(float elasped,bool force)
        {
            _pos    +=  _dir * _attrSpeed * elasped;
            CELLNode::update(elasped,force);
        }

    };
}