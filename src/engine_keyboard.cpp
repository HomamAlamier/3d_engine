#include <engine_keyboard.h>

NAMESPACE(sage3d)


KeyboardKeyData::KeyboardKeyData(const std::vector<KeyboardKeys>& keys)
    : _control(false)
    , _shift(false)
    , _alt(false)
{
    for(KeyboardKeys key : keys)
    {
        if (key == KEY_LEFT_CONTROL || key == KEY_RIGHT_CONTROL)
        {
            _control = true;
        }
        else if (key == KEY_LEFT_SHIFT || key == KEY_RIGHT_SHIFT)
        {
            _shift = true;
        }
        else if (key == KEY_LEFT_ALT || key == KEY_RIGHT_ALT)
        {
            _alt = true;
        }
        else
        {
            _keys.push_back(key);
        }
    }
    _hasValues = (_keys.size() > 0);
}

bool KeyboardKeyData::hasKey(KeyboardKeys key)
{
    for(KeyboardKeys k : _keys)
        if (key == k)
            return true;
    return false;
}


NAMESPACE_END
