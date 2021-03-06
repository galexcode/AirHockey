//
//  GameSprite.h
//  AirHockey
//
//  Created by siddharth shekar on 5/12/13.
//
//

#ifndef __GAMESPRITE_H__
#define __GAMESPRITE_H__

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;

class GameSprite: public CCSprite
{
public:
    
    CC_SYNTHESIZE(CCPoint, _nextPosition, NextPosition);
    CC_SYNTHESIZE(CCPoint, _vector, Vector);
    CC_SYNTHESIZE(CCTouch* , _touch, Touch);
    
    GameSprite(void);
    ~GameSprite(void);
    
    static GameSprite* gameSpriteWithFile(const char* pszFileName);
    
    virtual void setPosition(const CCPoint& pos);
    
    float radius();

};








#endif /* defined(__AirHockey__GameSprite__) */
