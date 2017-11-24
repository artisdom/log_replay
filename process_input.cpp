bool m_bPressedKeys[256];  // array of pressed keys current frame
bool m_bPressedKeysPrev[256]; // previous frame
bool m_bKeyLock[256];
float m_KeyDelayTimer(0.25f);
float m_KeyRepeatTimer(0.01f);


GetINput()
{
    UCHAR pKeyBuffer[ 256 ];
    ZeroMemory( pKeyBuffer, sizeof( UCHAR ) * 256 );
    GetKeyboardState(pKeyBuffer); // Change this for non-Windows platforms
    memcpy(&m_bPressedKeysPrev, &m_bPressedKeys, sizeof(bool)*256);
    if (!(pKeyBuffer[i] & 0xF0))
    {
        m_bKeyLock[i] = false;
        m_bPressedKeys[i] = false;
    }
    else
    {
        m_bPressedKeys[i] = !(m_bKeyLock[i]);
    }
}


// You can use arrays OR accessors like this
bool KeyDown(const unsigned int& key)   const   { return ( m_bPressedKeys[key])&&(!m_bPressedKeysPrev[key]); }
bool KeyStillDown(const unsigned int& key) const { return ( m_bPressedKeys[key])&&( m_bPressedKeysPrev[key]); }
bool KeyUp(const unsigned int& key) const       { return (!m_bPressedKeys[key])&&( m_bPressedKeysPrev[key]); }
bool KeyStillUp(const unsigned int& key) const  { return (!m_bPressedKeys[key])&&(!m_bPressedKeysPrev[key]); }

// OR something more complicated:
bool IsKeyPressed( const int& key ) // returns true only once per press
{
    if ( KeyDown(key) )  // Simple check
    {
        LockKey(key); // Prevent key from pressing next frames until released
        return true;
    }
    return false;
}

bool IsKeyPressedRepeatable( const int& key, const float& dt )
// "Tetris-style moving"
// returns true once, then after `KeyDelayTimer` seconds and repeats
// every `KeyDelayInterval` seconds (or your time units).
// dt - is a delta time between game frames
{
    if (KeyStillDown(key))
    {
        m_KeyDelayTimer += dt;
        if (m_KeyDelayTimer >= m_KeyDelayInterval)
        {
            m_KeyRepeatTimer += dt;
            if ( m_KeyRepeatTimer >= m_KeyRepeatInterval )
            {
                m_KeyRepeatTimer -= m_KeyRepeatInterval;
                return true;
            }
        }

    }
    if (KeyDown(key))
    {
        return true;
    }
    if (KeyUp(key))
    {
        m_KeyDelayTimer = 0.0f;
    }
    return false;
}




if ( m_bPressedKeys['w'] ) // Just move player continuously every frame while key is pressed
{
    m_Player.Move( m_Player.GetSpeed() * dt, 0 ); // dx, dy
}

if ( m_bPressedKeys[VK_ESCAPE] ) // Go to menu once per press
{
    LockKey( VK_ESCAPE );
    m_Game->ChangeState(MenuState);
}

if ( IsKeyPressedOnce(VK_Space)) // Jump once per Spacebar press
{                                // Same as above, but cleaner
    m_Player->Jump();
}

if ( IsKeyPressedRepeatable(VK_LEFT, dt) ) // Move figure in Tetris.
{                                          // Second tick is delayed and
    m_Tetris->MoveFigureLeft();            // then repeating discretely
}
