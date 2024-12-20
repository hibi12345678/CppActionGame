//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#pragma once
#include "Actor.h"


///////////////////////////////////////////////////////////////////////////////
//ReactActor class
///////////////////////////////////////////////////////////////////////////////
class ReactActor : public Actor
{
public:
    //=========================================================================
    // public methods.
    //=========================================================================
    //コンストラクタ
    ReactActor(Game* game);

    //Update
    void UpdateActor(float deltaTime) override;

private:
    //=========================================================================
    // private variables.
    //=========================================================================
    float mLifeSpan; //表示時間
};