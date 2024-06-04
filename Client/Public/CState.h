// CState.h

#pragma once


class AnimationState
{
public:
        struct ANIMATIONS
        {
            enum  AMANDA
            {
                ANIM_IDLE,
                ANIM_RUN,
                ANIM_ATTACK,
                ANIM_DASH,
                ANIM_END
            };
            enum  MATILDA
            {
                ANIM_IDLE,
                ANIM_RUN,
                ANIM_END

            };
            enum MONSTER
            {
                ANIM_IDLE,
                ANIM_END,
            };
        };
       
public:
        ANIMATIONS Animations;
};