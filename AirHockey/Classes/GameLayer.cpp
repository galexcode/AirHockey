#include "GameLayer.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* GameLayer::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    GameLayer*  layer = GameLayer::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
	//Init Game Elements
	_player1Score = 0;
    _player2Score = 0;
    
    
	//get screen size
	_screenSize = CCDirector::sharedDirector()->getWinSize();
	
    
	//1. add court image
    CCSprite * court = CCSprite::create("court.png");
    court->setPosition(ccp(_screenSize.width * 0.5, _screenSize.height * 0.5));
    this->addChild(court);
    
    
	//2. add players
	_player1 =  GameSprite::gameSpriteWithFile("mallet.png");
	_player1->setPosition(ccp(_screenSize.width * 0.5, _player1->radius() * 2));
    this->addChild(_player1, 0, kPlayer1Tag);
    
    _player2 =  GameSprite::gameSpriteWithFile("mallet.png");
	_player2->setPosition(ccp(_screenSize.width * 0.5, _screenSize.height - _player1->radius() * 2));
    this->addChild(_player2, 0, kPlayer2Tag);
	
	_players = CCArray::create(_player1, _player2, NULL);
    _players->retain();
	
    
    
    
	//3. add puck
	_ball = GameSprite::gameSpriteWithFile("puck.png");
    _ball->setPosition(ccp(_screenSize.width * 0.5, _screenSize.height * 0.5 - 2 * _ball->radius()));
    this->addChild(_ball);
    
    
	//4. add score display
    _player1ScoreLabel = CCLabelTTF::create("0", "Arial", 60);
    _player1ScoreLabel->setPosition(ccp(_screenSize.width - 60, _screenSize.height * 0.5 - 80));
    _player1ScoreLabel->setRotation(90);
    this->addChild(_player1ScoreLabel);
	
    _player2ScoreLabel = CCLabelTTF::create("0", "Arial", 60);
    _player2ScoreLabel->setPosition(ccp(_screenSize.width - 60, _screenSize.height * 0.5 + 80));
    _player2ScoreLabel->setRotation(90);
    this->addChild(_player2ScoreLabel);
    
    
	//listen for touches
    this->setTouchEnabled(true);
    
    //create main loop
    this->schedule(schedule_selector(GameLayer::update));
    
    return true;

}


////////////////////////
// Touches
///////////////////////

void GameLayer::ccTouchesBegan(CCSet* pTouches, CCEvent* event)
{

	CCSetIterator i;
	CCTouch* touch;
    CCPoint tap;
    GameSprite * player;
    
	//loop through all beginning touches
    for( i = pTouches->begin(); i != pTouches->end(); i++) {
		touch = (CCTouch*) (*i);
		
        if(touch) {
            //get location in OpenGL view
    		tap = touch->getLocation();
            
 			//loop through players and check to see if touch is landing on one of them
        	for (int p = 0; p < 2; p++)
            {
        		player = (GameSprite *) _players->objectAtIndex(p);
                
                if (player->boundingBox().containsPoint(tap))
                {
                	//store player's touch
                	player->setTouch(touch);
				}
                
                //CCLog("begin touch %d", p);
                //CCLOG("player xpos: %f", player->getPositionX());
        	}
            
            
		}
	}

}

void GameLayer::ccTouchesMoved(CCSet* pTouches, CCEvent* event)
{
    
    CCSetIterator i;
	CCTouch* touch;
    CCPoint tap;
    GameSprite * player;
    
    //loop through all moving touches
	for( i = pTouches->begin(); i != pTouches->end(); i++) {
		touch = (CCTouch*) (*i);
		
		if(touch) {
			
            tap = touch->getLocation();
			
			for (int p = 0; p < _players->count(); p++)
            {
        		
        		player = (GameSprite *) _players->objectAtIndex(p);
        		//if touch belongs to player
        		if (player->getTouch() != NULL && player->getTouch() == touch) {
                    
                    CCPoint nextPosition = tap;
                    
                    //keep player inside screen
                    if (nextPosition.x < player->radius())
                        nextPosition.x = player->radius();
                    if (nextPosition.x > _screenSize.width - player->radius())
                        nextPosition.x = _screenSize.width - player->radius();
                    if (nextPosition.y < player->radius())
                        nextPosition.y  = player->radius();
                    if (nextPosition.y > _screenSize.height - player->radius())
                        nextPosition.y = _screenSize.height - player->radius();
                    
                    //keep player inside its court
                    if (player->getPositionY() < _screenSize.height * 0.5f) {
                        if (nextPosition.y > _screenSize.height * 0.5 - player->radius()) {
                            nextPosition.y = _screenSize.height * 0.5 - player->radius();
                        }
                    } else {
                        if (nextPosition.y < _screenSize.height * 0.5 + player->radius()) {
                            nextPosition.y = _screenSize.height * 0.5 + player->radius();
                        }
                    }
                    
                    player->setNextPosition(nextPosition);
                    player->setVector(ccp(tap.x - player->getPositionX(), tap.y - player->getPositionY()));
                    
                    
                    //tap is the lastest position
                    //get position is the previous position that player was in
                    CCLog("moved player %d", p);
                    CCLOG("next xpos: %f", nextPosition.x);
                    CCLOG("player xpos: %f", player->getPositionX());
				}
        	}
		}
	}

}

void GameLayer::ccTouchesEnded(CCSet* pTouches, CCEvent* event)
{
    
	CCSetIterator i;
	CCTouch* touch;
    GameSprite * player;
    
    //loop through all ending touches
	for( i = pTouches->begin(); i != pTouches->end(); i++)
    {
		touch = (CCTouch*) (*i);
		
		if(touch)
        {
			for (int p = 0; p < _players->count(); p++)
            {
        		player = (GameSprite *) _players->objectAtIndex(p);
        		if (player->getTouch() != NULL && player->getTouch() == touch)
                {
					//if touch ending belongs to this player, clear it
					player->setTouch(NULL);
                    player->setVector(CCPointZero);
				}
        	}
		}
	}
    
}

void GameLayer::update(float dt)
{
	//update puck
    CCPoint ballNextPosition = _ball->getNextPosition();
    CCPoint ballVector = _ball->getVector();
    
    //reduce the vector magnitude by 0.98 every update
    ballVector = ccpMult(ballVector, 0.98f);//0.98
    
    
    //ball velocity
    ballNextPosition.x += ballVector.x;
    ballNextPosition.y += ballVector.y;
    
    CCLOG("next ball xpos, pos : %f, %f", ballNextPosition.x, ballNextPosition.y);
    CCLOG("ball x,y  vector: %f, %f", ballVector.x, ballVector.y);    
    
	//test for puck and players collision
	float squared_radii = pow(_player1->radius() + _ball->radius(), 2);
    
    GameSprite * player;
    CCPoint playerNextPosition;
    CCPoint playerVector;
	for (int p = 0; p < _players->count(); p++)
    {
		
		player = (GameSprite *) _players->objectAtIndex(p);
		playerNextPosition = player->getNextPosition();
        playerVector = player->getVector();
        
		float diffx = ballNextPosition.x - player->getPositionX();
		float diffy = ballNextPosition.y - player->getPositionY();
		
		float distance1 = pow(diffx, 2) + pow(diffy, 2);
		float distance2 = pow(_ball->getPositionX() - playerNextPosition.x, 2) + pow(_ball->getPositionY() - playerNextPosition.y, 2);
		
		if (distance1 <= squared_radii || distance2 <= squared_radii)
        {
            
          	float mag_ball = pow(ballVector.x, 2) + pow(ballVector.y, 2);
			float mag_player = pow (playerVector.x, 2) + pow (playerVector.y, 2);
          	
			float force = sqrt(mag_ball + mag_player);
			float angle = atan2(diffy, diffx);
			
			ballVector.x = force * cos(angle);
			ballVector.y = force * sin(angle);
			
            //purpose of this so set the ball at the edge of player immediatly after collision
            //the additon of the force component is to avoid contact again,
            //otherwise will be too close to be treated as collision again.
            ballNextPosition.x = playerNextPosition.x + (player->radius() + _ball->radius())* cos(angle) + force * cos(angle);
            ballNextPosition.y = playerNextPosition.y + (player->radius() + _ball->radius())* sin(angle) + force * sin(angle);
            
            //SimpleAudioEngine::sharedEngine()->playEffect("hit.wav");
		}
	}
    
	//check collision of ball and sides
	if (ballNextPosition.x < _ball->radius())
    {
		ballNextPosition.x = _ball->radius();
        ballVector.x *= -0.8f;
        //SimpleAudioEngine::sharedEngine()->playEffect("hit.wav");
	}
	
	if (ballNextPosition.x > _screenSize.width - _ball->radius())
    {
		ballNextPosition.x = _screenSize.width - _ball->radius();
		ballVector.x *= -0.8f;
        //SimpleAudioEngine::sharedEngine()->playEffect("hit.wav");
	}
    //ball and top of the court
	if (ballNextPosition.y > _screenSize.height - _ball->radius())
    {
		if (_ball->getPosition().x < _screenSize.width * 0.5f - GOAL_WIDTH * 0.5f ||
			_ball->getPosition().x > _screenSize.width * 0.5f + GOAL_WIDTH * 0.5f)
        {
			ballNextPosition.y = _screenSize.height - _ball->radius();
			ballVector.y *= -0.8f;
            //SimpleAudioEngine::sharedEngine()->playEffect("hit.wav");
		}
	}
	//ball and bottom of the court
	if (ballNextPosition.y < _ball->radius() )
    {
		if (_ball->getPosition().x < _screenSize.width * 0.5f - GOAL_WIDTH * 0.5f ||
			_ball->getPosition().x > _screenSize.width * 0.5f + GOAL_WIDTH * 0.5f)
        {
			ballNextPosition.y = _ball->radius();
			ballVector.y *= -0.8f;
            //SimpleAudioEngine::sharedEngine()->playEffect("hit.wav");
		}
	}
    
    //update ball information if passed through the posts
    _ball->setVector(ballVector);
    _ball->setNextPosition(ballNextPosition);
    
    
    //check for goals
    if(ballNextPosition.y < -_ball->radius() * 2)
        this->playerScore(2);
    if(ballNextPosition.y > _screenSize.height + _ball->radius()*2)
        this->playerScore(1);
    
    //setting the next position
    _player1->setPosition(_player1->getNextPosition());
    _player2->setPosition(_player2->getNextPosition());
    _ball->setPosition(_ball->getNextPosition());
    
}

void GameLayer::playerScore(int player)
{
    _ball->setVector(ccp(0, 0));
    
    char score_buffer[10];
    
    if(player==1)
    {
        _player1Score++;
        sprintf(score_buffer, "%i",_player1Score);
        _player1ScoreLabel ->setString(score_buffer);
        _ball->setNextPosition(ccp(_screenSize.width * 0.5,
                                   _screenSize.height * 0.5 + 2 * _ball->radius()));
    }
    else
    {
        _player2Score++;
        sprintf(score_buffer,"%i", _player2Score);
        _player2ScoreLabel->setString(score_buffer);
        _ball->setNextPosition(ccp(_screenSize.width * 0.5,
                                   _screenSize.height * 0.5 - 2 * _ball->radius()));
    }
    
    _player1->setPosition(ccp(_screenSize.width * 0.5,
                              _player1->radius() * 2));
    _player2->setPosition(ccp(_screenSize.width * 0.5,
                              _screenSize.height - _player1->radius() * 2));
    _player1->setTouch(NULL);
    _player2->setTouch(NULL);
    
}




GameLayer::~GameLayer()
{
    CC_SAFE_RELEASE(_players);
}

