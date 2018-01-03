#pragma once

#include "class/CELLNode.hpp"

namespace   CELL
{
    class   CELLBullet :public CELLNode
    {
    public:
        /// 速度
        float       _attrSpeed;
        /// 作用的最远距离
        float       _attrMaxDistance;
        /// 威力    半径
        float       _attrPowerRadius;
        /// 伤害值
        float       _attrHurt;
        /// 飞行方向
        float3      _dir;
        /// 初始化位置
        float3      _posSrc;
        /// 使用的文理
        Texture2dId _texture;
        /// 是否死亡
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